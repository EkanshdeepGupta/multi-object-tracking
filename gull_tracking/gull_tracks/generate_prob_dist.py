import numpy as np
import os
from pathlib import Path
import json

def get_most_recent_timestamp():
        timestamps = []
        for dir in os.listdir(Path(__file__).parent /
                              "../data/saved_sessions/"):
            timestamps.append(float(dir))
        if not timestamps:
            return None
        return max(timestamps)

def get_prob_history(ts=get_most_recent_timestamp()):
    if not ts:
        return None
    dir = Path(__file__).parent / f"../data/saved_sessions/{str(ts)}/"
    return open(dir / f"tracks_history_{str(ts)}")


def get_prob_matrix(hist, size=15):
    curr_track, prev_track, prob_of_switch = hist["curr_track"], hist["prev_track"], hist["prob_of_switch"]
    mat = np.identity(size)
    mat[curr_track][prev_track], mat[prev_track][curr_track] = [prob_of_switch]*2
    mat[curr_track][curr_track], mat[prev_track][prev_track] = [1-prob_of_switch]*2
    return mat

def get_total_probability(size=15):
    prob_history = json.load(get_prob_history())
    mat = np.identity(size)
    for event_id in prob_history:
        for hist in prob_history[event_id]:
            prob_matrix = get_prob_matrix(hist)
            mat = np.matmul(prob_matrix, mat)
    return mat


#get_total_probability()
