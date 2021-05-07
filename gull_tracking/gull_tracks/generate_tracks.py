# Compare distances just to see how many birds are within a given meters threshold.
import numpy as np
import matplotlib.pyplot as plt
import pandas
import dateutil.parser
from shapely.geometry import Point
import geopandas as gpd
from geopandas import GeoDataFrame
from haversine import haversine, Unit
from pathlib import Path
from random import sample

class TracksGenerator():
    def __init__(self):
        self.df = None
        self.bird_ids = None
        self.filtered_df = None
        self.true_tracks = None

    def create_sorted_df(self):
        black_gull_path = Path(__file__).parent / \
            "../data/LBBG_ZEEBRUGGE-gps-2013.csv"
        herring_gull_path = Path(__file__).parent / \
            "../data/HG_OOSTENDE-gps-2013.csv"
        black_gull = pandas.read_csv(black_gull_path.open())
        herring_gull = pandas.read_csv(herring_gull_path.open())

        all_gulls = pandas.concat([black_gull, herring_gull])

        all_gulls['timestamp'] = pandas.to_datetime(black_gull.timestamp)
        all_gulls_sorted = all_gulls.sort_values(by=['timestamp'])
        all_gulls_sorted['lat_long'] = all_gulls_sorted[[
            'location-lat', 'location-long']].apply(tuple, axis=1)
        self.df = all_gulls_sorted

    def generate_bird_ids(self, limit_bgulls=10):
        all_bird_ids = self.df['individual-local-identifier'].unique()
        bgull_ids = [id for id in list(all_bird_ids) if str(id).startswith('L')]
        hgull_ids = [id for id in list(all_bird_ids) if str(id).startswith('H')]
        self.bird_ids = list(sample(bgull_ids, limit_bgulls)) + hgull_ids
        in_bird_ids = self.df['individual-local-identifier'] in self.bird_ids
        self.filtered_df = self.df[in_bird_ids]
        self.true_tracks = {}
        for x in range(1, len(self.bird_ids)+1):
            self.true_tracks[x] = self.bird_ids[x]
        
    def generate_tracks(self):
        for row in self.filtered_df:
            pass