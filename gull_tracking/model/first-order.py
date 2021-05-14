import numpy as np
# import pandas
# from scipy import stats
# from scipy.integrate import quad
# from haversine import haversine, Unit
# from pathlib import Path
# from random import sample
# import json

# from numpy.random import random 

class firstOrder:
    def __init__(self, n): # n number of tracks
        self.dim = n
        self.prob_mat = np.identity(self.dim, dtype=float)

    def update(self, track_i, track_j, prob):
        array_mix = np.identity(self.dim, dtype=float)
        array_mix[track_i, track_j] = prob
        array_mix[track_j, track_i] = prob
        array_mix[track_i, track_i] = 1 - prob
        array_mix[track_j, track_j] = 1 - prob

        self.prob_mat = self.prob_mat @ array_mix

    def observation(self, obj_i, track_j, prob):
        self.prob_mat[obj_i, : ] = np.ones(self.dim, dtype=float) * ((1-prob) / (self.dim-1))
        self.prob_mat[obj_i, track_j] = prob

   

model = firstOrder(2)
model.update(0, 1, 0.5)
model.observation(1, 1, 1)

print(model.prob_mat)
