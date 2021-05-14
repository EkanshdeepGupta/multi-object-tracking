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


class TracksGenerator():
    def __init__(self):
        self.df = None
        self.bird_ids = None
        self.filtered_df = None
        self.true_tracks = {}
        self.computed_tracks = {}
        self.p_max = 1
        self.prob_fn = None
        self.tracks_history = []
        self.prob_history = []

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

    def generate_bird_ids(self, limit_bgulls=10):
        all_bird_ids = self.df['individual-local-identifier'].unique()
        bgull_ids = [id for id in list(
            all_bird_ids) if str(id).startswith('L')]
        hgull_ids = [id for id in list(
            all_bird_ids) if str(id).startswith('H')]
        self.bird_ids = list(list(sample(bgull_ids, limit_bgulls)) + hgull_ids)
        in_bird_ids = self.df['individual-local-identifier'].isin(self.bird_ids)
        self.filtered_df = self.df[in_bird_ids]
        self.true_tracks = {}
        for x in range(0, len(self.bird_ids)):
            self.true_tracks[self.bird_ids[x]] = x

    def create_prob_fn(self):
        params = json.load(open(Path(__file__).parent /
                                '../skew/output/skew_dist_params.json'))
        ae, loce, scalee = params['Estimated a'], 0, params['Estimated scale']
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
                prob_of_switch = self.if_switch_tracks(df_dict[index], prev_rec)
                if prob_of_switch:
                    tracks_to_switch.append((prob_of_switch, prev_rec))
            shuffle(tracks_to_switch)
            for prob_of_switch, prev_rec in tracks_to_switch:
                prev_id = prev_rec['individual-local-identifier']
                self.switch_tracks(curr_id, prev_id, prob_of_switch)
                switch_identifier = f'{curr_rec["event-id"]}-{prev_rec["event-id"]}'
                self.tracks_history.append([
                    switch_identifier, self.computed_tracks])

    def if_switch_tracks(self, curr_rec, prev_rec):
        curr_loc = curr_rec['lat_long']
        prev_loc = prev_rec['lat_long']
        dist = haversine(curr_loc, prev_loc, unit='km')
        prob_of_switch = self.calc_prob(dist)
        rand_val = random()
        if rand_val <= prob_of_switch:
            return prob_of_switch
        return None
    
    def switch_tracks(self, curr_id, prev_id, prob_of_switch):
        self.prob_history.append(
            {
                'curr_track': self.computed_tracks[curr_id],
                'prev_track': self.computed_tracks[prev_id],
                'prob_of_switch': prob_of_switch
            }
        )
        temp_index = self.computed_tracks[curr_id]
        self.computed_tracks[curr_id] = self.computed_tracks[prev_id]
        self.computed_tracks[prev_id] = temp_index
    
    def history_to_json(self):
        ts = time.time()
        self.prob_history_to_json(ts)
        self.tracks_history_to_json(ts)
    
    def prob_history_to_json(self, ts=time.time()):
        with open(Path(__file__).parent / \
            f"../data/prob_history_{str(ts)}.json", 'w+') as file:
            json.dump(self.prob_history, file, indent=4)
    
    def tracks_history_to_json(self, ts=time.time()):
        with open(Path(__file__).parent / \
            f"../data/tracks_history_{str(ts)}.json", 'w+') as file:
            json.dump(self.tracks_history, file, indent=4)

    @staticmethod
    def run():
        tg = TracksGenerator()
        tg.create_sorted_df()
        tg.generate_bird_ids()
        tg.create_prob_fn()
        tg.generate_tracks()
        return tg

#tg = TracksGenerator.run()