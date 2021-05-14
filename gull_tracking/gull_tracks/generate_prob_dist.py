import numpy as np
import os
from pathlib import Path
import json

def get_prob_history():
    for path, currentDirectory, files in os.walk(Path(__file__).parent /
                                                 "../data/"):
        print(files)
        for file in files:
            if file.startswith("prob_history"):
                return open(Path(__file__).parent /
                            f"../data/{file}")


def get_prob_matrix(hist, size=15):
    curr_track, prev_track, prob_of_switch = hist["curr_track"], hist["prev_track"], hist["prob_of_switch"]
    mat = np.identity(size)
    mat[curr_track][prev_track], mat[prev_track][curr_track] = [prob_of_switch]*2
    mat[curr_track][curr_track], mat[prev_track][prev_track] = [1-prob_of_switch]*2
    return mat

def get_total_probability(size=15):
    prob_history = json.load(get_prob_history())
    mat = np.identity(size)
    for hist in prob_history:
        prob_matrix = get_prob_matrix(hist)
        mat = np.matmul(prob_matrix, mat)
    return mat

get_total_probability()