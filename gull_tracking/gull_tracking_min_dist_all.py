#Compare distances just to see how many birds are within a given meters threshold.
import numpy as np
import matplotlib.pyplot as plt
import pandas
import dateutil.parser
from shapely.geometry import Point
import geopandas as gpd
from geopandas import GeoDataFrame
from haversine import haversine, Unit
from pathlib import Path

def dist_diff(df):
     df['m_dist'] = haversine(df, df.shift(), unit = 'm')
     df['m_dist'].fillna(0 ,inplace = True)
     return df
black_gull_path = Path(__file__).parent / "./data/LBBG_ZEEBRUGGE-gps-2013.csv"
herring_gull_path = Path(__file__).parent / "./data/HG_OOSTENDE-gps-2013.csv"
black_gull = pandas.read_csv(black_gull_path.open())
herring_gull = pandas.read_csv(herring_gull_path.open())

all_gulls = pandas.concat([black_gull, herring_gull])

all_gulls['timestamp'] = pandas.to_datetime(black_gull.timestamp)
all_gulls_sorted = all_gulls.sort_values(by=['timestamp'])
all_gulls_sorted['lat_long'] = all_gulls_sorted[['location-lat', 'location-long']].apply(tuple, axis=1)
ags_dict = all_gulls_sorted.to_dict('records')

min_dists = []
index = 0
prev_id = ''
prev_j = 0
for index in range(len(ags_dict)):
    dists = []
    for j in range(index+1, len(ags_dict)):
        if ags_dict[index]['individual-local-identifier'] == prev_id:
            j = prev_j
        if (ags_dict[index]['timestamp'] - ags_dict[j]['timestamp']).total_seconds() > 1:
            break
        if ags_dict[index]['individual-local-identifier']!= ags_dict[j]['individual-local-identifier']:
            if abs((ags_dict[index]['timestamp'] - ags_dict[j]['timestamp']).total_seconds()) <= 1:
                dists.append(haversine(ags_dict[index]['lat_long'], ags_dict[j]['lat_long'], unit = 'm'))
            else:
                break
    prev_id = ags_dict[index]['individual-local-identifier']
    prev_j = j
    print(f'Prev id was {str(prev_id)}, prev j was {str(j)}')
    print(f'At index {str(index)}')
    if dists:
        min_dists.append(min(dists))

print(min(min_dists))