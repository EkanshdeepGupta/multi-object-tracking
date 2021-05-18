#plot map of birds. Currently trying to figure out labelling.
import numpy as np
import matplotlib.pyplot as plt
import pandas
from shapely.geometry import Point
import geopandas as gpd
from geopandas import GeoDataFrame
from haversine import haversine, Unit
def dist_diff(df):
     df['m_dist'] = haversine(df, df.shift(), unit = 'm')
     df['m_dist'].fillna(0 ,inplace = True)
     return df
black_gull = pandas.read_csv(r'C:\Users\14hoy\git\multi-object-tracking\gull_tracking\data\LBBG_ZEEBRUGGE-gps-2013.csv')
herring_gull = pandas.read_csv(r'C:\Users\14hoy\git\multi-object-tracking\gull_tracking\data\HG_OOSTENDE-gps-2013.csv')

all_gulls = pandas.concat([black_gull, herring_gull])

all_gulls['timestamp'] = pandas.to_datetime(all_gulls.timestamp)
all_gulls_sorted = all_gulls.sort_values(by=['timestamp'])

geometry = [Point(xy) for xy in zip(all_gulls['location-long'], all_gulls['location-lat'])]

gdf = GeoDataFrame(all_gulls, geometry=geometry) 

gdf = gdf.groupby(gdf['individual-local-identifier'])

world = gpd.read_file(gpd.datasets.get_path('naturalearth_lowres'))

gdf.plot(ax=world.plot(figsize=(10, 6)), marker='+', markersize=15)

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