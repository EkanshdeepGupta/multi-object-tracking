from generate_tracks import TracksGenerator
import numpy as np
import os
from pathlib import Path
import json
import random

import sys
sys.path.append('./model')
import model.first_order as first_order
import model.second_order as second_order


def get_total_probability(tg, size=15):
    prob_history = tg.prob_history
    myModel = first_order.model(size) #change first_order to second_order
    count = 0
    for event_id in prob_history:
        for hist in prob_history[event_id]:
            #print(count)
            count += 1
            myModel.update(hist["curr_track"], hist["prev_track"], hist["prob_of_switch"])

            rand = random.random()

            if rand < 0.001:
                true_tracks_items = list(tg.true_tracks.items())
                obj_i, track_j = random.choice(true_tracks_items)
                #print(f'{str(obj_i)}, {str(track_j)}')
                myModel.observation(track_j, track_j, 0.7) #where obj_i is a randomly chosen gull, and track_j is its actual track at this moment.


    myModel.inference()

random.seed(0)
tg = TracksGenerator.load_session()
get_total_probability(tg)