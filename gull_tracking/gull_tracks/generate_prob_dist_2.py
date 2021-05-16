import numpy as np
import os
from pathlib import Path
import json
import random

import sys
sys.path.append('./model')
import first_order
import second_order


def get_prob_history():
    for path, currentDirectory, files in os.walk(Path(__file__).parent /
                                                 "../data/"):
        print(files)
        for file in files:
            if file.startswith("prob_history"):
                return open(Path(__file__).parent /
                            f"../data/{file}")

def get_total_probability(size=15):
    prob_history = json.load(get_prob_history())
    myModel = first_order.model(size) #change first_order to second_order
    count = 0
    for hist in prob_history:
        print(count)
        count += 1
        myModel.update(hist["curr_track"], hist["prev_track"], hist["prob_of_switch"])

        rand = random.random()

        if rand < 0.01:
            myModel.observation(obj_i, track_j, 0.9) #where obj_i is a randomly chosen gull, and track_j is its actual track at this moment.


    myModel.inference()

get_total_probability()