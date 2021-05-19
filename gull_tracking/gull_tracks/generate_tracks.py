# Compare distances just to see how many birds are within a given meters threshold.
import pandas
from scipy import stats
from scipy.integrate import quad
from haversine import haversine
from pathlib import Path
from random import sample, shuffle
import json
from numpy.random import random
import time
import os
import re
import geopandas as gpd
from geopandas import GeoDataFrame
import geoplot
from shapely.geometry import Point, LineString
import matplotlib
import matplotlib.pyplot as plt
import randomcolor


class TracksGenerator():
    def __init__(self, df=None, bird_ids=None, filtered_df=None, true_tracks={}, computed_tracks={}, p_max=0.5,
                 prob_fn=None, tracks_history={}, prob_history={}, dist_params={}):
        self.df = df
        self.bird_ids = bird_ids
        self.filtered_df = filtered_df
        self.true_tracks = true_tracks
        self.computed_tracks = computed_tracks
        self.p_max = p_max
        self.prob_fn = prob_fn
        self.tracks_history = tracks_history
        self.prob_history = prob_history
        self.dist_params = dist_params
        self.color_map = {}

    def create_sorted_df(self):
        black_gull_path = Path(__file__).parent / \
            "../data/LBBG_ZEEBRUGGE-gps-2013.csv"
        herring_gull_path = Path(__file__).parent / \
            "../data/HG_OOSTENDE-gps-2013.csv"
        black_gull = pandas.read_csv(black_gull_path.open())
        herring_gull = pandas.read_csv(herring_gull_path.open())

        all_gulls = pandas.concat([black_gull, herring_gull])

        all_gulls['timestamp'] = pandas.to_datetime(all_gulls.timestamp)
        all_gulls_sorted = all_gulls.sort_values(by=['timestamp'])
        all_gulls_sorted['lat_long'] = all_gulls_sorted[[
            'location-lat', 'location-long']].apply(tuple, axis=1)
        self.df = all_gulls_sorted

    def generate_bird_ids(self, limit_bgulls=10, limit_hgulls=5):
        all_bird_ids = self.df['individual-local-identifier'].unique()
        bgull_ids = [id for id in list(
            all_bird_ids) if str(id).startswith('L')]
        hgull_ids = [id for id in list(
            all_bird_ids) if not str(id).startswith('L')]
        self.bird_ids = list(list(sample(bgull_ids, limit_bgulls)) + list(sample(hgull_ids, limit_hgulls)))
        in_bird_ids = self.df['individual-local-identifier'].isin(
            self.bird_ids)
        self.filtered_df = self.df[in_bird_ids]
        self.true_tracks = {}
        for x in range(0, len(self.bird_ids)):
            self.true_tracks[self.bird_ids[x]] = x

    def create_prob_fn(self, params=None):
        if not params:
            params = json.load(open(Path(__file__).parent /
                                    'testing/skew/output/skew_dist_params.json'))
        ae, loce, scalee = params['Estimated a'], 0, params['Estimated scale']
        self.dist_params = {
            'Estimated a': ae,
            'Estimated loc': 0,
            'Estimated scale': scalee
        }
        self.prob_fn = lambda x: stats.skewnorm.pdf(
            x, a=ae, loc=loce, scale=scalee)

    def calc_prob(self, x):
        if not self.prob_fn:
            self.create_prob_fn()
        return self.p_max*(1-quad(self.prob_fn, 0, x)[0])

    def generate_tracks(self):
        self.computed_tracks = self.true_tracks.copy()
        df_dict = self.filtered_df.to_dict('records')

        for index in range(0, len(df_dict)):
            curr_rec = df_dict[index]
            event_id = curr_rec['event-id']
            self.prob_history[event_id] = []
            self.tracks_history[event_id] = []
            curr_id = curr_rec['individual-local-identifier']
            curr_ts = curr_rec['timestamp']
            tracks_to_switch = []
            for j in range(index, -1, -1):
                prev_rec = df_dict[j]
                prev_id = prev_rec['individual-local-identifier']
                if curr_id == prev_id:
                    continue
                prev_ts = prev_rec['timestamp']
                if (curr_ts - prev_ts).total_seconds() > 2:
                    break
                prob_of_switch, switch_tracks = self.if_switch_tracks(
                    df_dict[index], prev_rec)
                self.add_to_prob_history(
                    event_id, curr_id, prev_id, prob_of_switch)
                if switch_tracks:
                    tracks_to_switch.append((prob_of_switch, prev_rec))
            shuffle(tracks_to_switch)
            for prob_of_switch, prev_rec in tracks_to_switch:
                prev_id = prev_rec['individual-local-identifier']
                self.switch_tracks(curr_id, prev_id)
            self.tracks_history[event_id] = self.computed_tracks

    def sample_tracks(self, ratio=0.5):
        new_filtered_df = pandas.DataFrame(columns=self.filtered_df.columns)
        filtered_df_groups = self.filtered_df.groupby(['individual-local-identifier'])
        for group in filtered_df_groups:
            recs_dict = group[1].to_dict('records')
            recs_dict_filtered = sample(recs_dict, int(ratio*len(recs_dict)))
            recs_filtered_df = pandas.DataFrame.from_records(recs_dict_filtered)
            new_filtered_df = pandas.concat([new_filtered_df, recs_filtered_df])
        self.filtered_df = new_filtered_df
        self.computed_tracks={}
        self.tracks_history={}
        self.prob_history={}
        self.dist_params={}

    def if_switch_tracks(self, curr_rec, prev_rec):
        curr_loc = curr_rec['lat_long']
        prev_loc = prev_rec['lat_long']
        dist = haversine(curr_loc, prev_loc, unit='km')
        prob_of_switch = self.calc_prob(dist)
        rand_val = random()
        if rand_val <= prob_of_switch:
            return prob_of_switch, True
        return prob_of_switch, False

    def add_to_prob_history(self, event_id, curr_id, prev_id, prob_of_switch):
        self.prob_history[event_id].append(
            {
                'curr_track': self.computed_tracks[curr_id],
                'prev_track': self.computed_tracks[prev_id],
                'prob_of_switch': prob_of_switch#,
                #'event_id': event_id
            }
        )

    def switch_tracks(self, curr_id, prev_id):
        temp_index = self.computed_tracks[curr_id]
        self.computed_tracks[curr_id] = self.computed_tracks[prev_id]
        self.computed_tracks[prev_id] = temp_index

    def history_to_json(self):
        ts = time.time()
        self.prob_history_to_json(ts)
        self.tracks_history_to_json(ts)

    def prob_history_to_json(self, ts=time.time()):
        with open(Path(__file__).parent /
                  f"../data/prob_history_{str(ts)}.json", 'w+') as file:
            json.dump(self.prob_history, file, indent=4)

    def tracks_history_to_json(self, ts=time.time()):
        with open(Path(__file__).parent /
                  f"../data/tracks_history_{str(ts)}.json", 'w+') as file:
            json.dump(self.tracks_history, file, indent=4)

    def get_most_recent_timestamp():
        timestamps = []
        for dir in os.listdir(Path(__file__).parent /
                              "../data/saved_sessions/"):
            try:
                timestamps.append(float(dir))
            except:
                pass
        if not timestamps:
            return None
        return max(timestamps)

    @staticmethod
    def load_session(ts=get_most_recent_timestamp()):
        if not ts:
            return None
        list_of_dirs = os.listdir(Path(__file__).parent / f"../data/saved_sessions/")
        dir_folder = [dir for dir in list_of_dirs if str(ts) in dir][0]
        tg = None
        print(dir_folder)
        dir = Path(__file__).parent / f"../data/saved_sessions/{str(dir_folder)}/"
        with open(dir / "general_fields.json") as file:
            general_fields = json.load(file)
            tg = TracksGenerator(**general_fields)
            tg.create_sorted_df()
            tg.create_prob_fn(tg.dist_params)
        with open(dir / f"tracks_history_{str(ts)}") as file:
            tg.tracks_history = json.load(file)
        with open(dir / f"prob_history_{str(ts)}") as file:
            tg.prob_history = json.load(file)
        with open(dir / 'filtered_df.csv', ) as file:
            tg.filtered_df = pandas.read_csv(file)
        return tg

    def save_session(self, ts=None):
        if not ts:
            ts = time.time()
        dir = Path(__file__).parent / f"../data/saved_sessions/{str(ts)}/"
        os.mkdir(dir)
        with open(dir / "general_fields.json", 'w+') as file:
            json.dump({
                'bird_ids': self.bird_ids,
                'true_tracks': self.true_tracks,
                'computed_tracks': self.computed_tracks,
                'p_max': self.p_max,
                'dist_params': self.dist_params
            }, file)
        with open(dir / f"tracks_history_{str(ts)}", 'w+') as file:
            json.dump(self.tracks_history, file)
        with open(dir / f"prob_history_{str(ts)}", 'w+') as file:
            json.dump(self.prob_history, file)
        with open(dir / 'filtered_df.csv', 'w+') as file:
            self.filtered_df.to_csv(file)

    @staticmethod
    def run():
        tg = TracksGenerator()
        tg.create_sorted_df()
        tg.generate_bird_ids()
        tg.create_prob_fn()
        tg.generate_tracks()
        tg.history_to_json()
        return tg
    
    def plot_all(tg):
        rand_colors = ['black', 'cornflowerblue', 'peru', 'orangered', 'brown', 'darkred', 'lightcoral', 'lightpink', 'darkorange', 'moccasin', 'gold', 'yellow', 'chartreuse', 'forestgreen', 'lime', 'aquamarine', 'turquoise', 'darkviolet', 'magenta', 'crimson', 'dodgerblue', 'darkslategray', 'fuchsia', 'navy', 'darkturquoise', 'purple', 'cyan', ]
        shuffle(rand_colors)
        index = 0
        for bird_id in tg.bird_ids:
            tg.color_map[bird_id] = rand_colors[index]
            index+=1
        TracksGenerator.plot_reg(tg)
        TracksGenerator.plot_new_tracks(tg)
    def plot(gdf, color_map):
        world = gpd.read_file(gpd.datasets.get_path('naturalearth_lowres'))
        print(type(gdf))
        
        gdf.plot(color=list(color_map.values()),
            ax=world.plot(figsize=(10, 6)))
        
        plt.show()
    def plot_reg(tg):
        df = tg.filtered_df
        geometry = [Point(xy) for xy in zip(df['location-long'], df['location-lat'])]
        gdf = GeoDataFrame(df, geometry=geometry) 
        gdf = gdf.groupby(['individual-local-identifier'])['geometry'].apply(lambda x: LineString(x.tolist()))
        gdf = GeoDataFrame(gdf, geometry='geometry')
        TracksGenerator.plot(gdf, tg.color_map)
        #gdf.plot(ax=world.plot(figsize=(10, 6)), cmap='rainbow')

    def plot_new_tracks(tg):
        reversed_true = {v: k for k, v in tg.true_tracks.items()}
        filtered_df_dict = tg.filtered_df.to_dict('records')
        new_df_dict = []
        change_dict = tg.true_tracks
        for rec in filtered_df_dict:
            event_id = rec['event-id']
            curr_track = tg.tracks_history.get(str(event_id), False)
            if curr_track:
                change_dict = curr_track.copy()
            rec_id = rec['individual-local-identifier']
            new_id = reversed_true[change_dict[rec_id]]
            rec['individual-local-identifier'] = new_id
            new_df_dict.append(rec)
        new_df = pandas.DataFrame.from_records(new_df_dict)
        
        geometry = [Point(xy) for xy in zip(new_df['location-long'], new_df['location-lat'])]
        gdf = GeoDataFrame(new_df, geometry=geometry) 
        gdf = gdf.groupby(['individual-local-identifier'])['geometry'].apply(lambda x: LineString(x.tolist()))
        gdf = GeoDataFrame(gdf, geometry='geometry')
        TracksGenerator.plot(gdf, tg.color_map)
    
tg = TracksGenerator.load_session()
TracksGenerator.plot_all(tg)