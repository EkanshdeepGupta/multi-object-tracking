#plot map of birds. Currently trying to figure out labelling.
import numpy as np
import matplotlib.pyplot as plt
import pandas
import dateutil.parser
from shapely.geometry import Point
import geopandas as gpd
from geopandas import GeoDataFrame
from haversine import haversine, Unit
def dist_diff(df):
     df['m_dist'] = haversine(df, df.shift(), unit = 'm')
     df['m_dist'].fillna(0 ,inplace = True)
     return df
black_gull = pandas.read_csv('/Users/suhashoysala/Downloads/gull_tracking/LBBG_ZEEBRUGGE-gps-2013.csv')
herring_gull = pandas.read_csv('/Users/suhashoysala/Downloads/gull_tracking/HG_OOSTENDE-gps-2013.csv')

all_gulls = pandas.concat([black_gull, herring_gull])

all_gulls['timestamp'] = pandas.to_datetime(all_gulls.timestamp)
all_gulls_sorted = all_gulls.sort_values(by=['timestamp'])

geometry = [Point(xy) for xy in zip(all_gulls['location-long'], all_gulls['location-lat'])]

gdf = GeoDataFrame(all_gulls, geometry=geometry) 

world = gpd.read_file(gpd.datasets.get_path('naturalearth_lowres'))

gdf.plot(ax=world.plot(figsize=(10, 6)), marker='o', color='red', markersize=15)

plt.show()

#Below is grouping birds by timestamp.
"""
black_gull_groups = black_gull.groupby(['individual-local-identifier'])

black_gull_groups_dict = {}
for id, frame in black_gull_groups:
    frame['timestamp'] = list(map(dateutil.parser.parse, frame['timestamp']))
    black_gull_groups_dict[id] = frame

for id, frame in black_gull_groups:
    print(frame['timestamp'].iloc[0])
"""