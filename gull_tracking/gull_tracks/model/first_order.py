import numpy as np
# import pandas
# from scipy import stats
# from scipy.integrate import quad
# from haversine import haversine, Unit
# from pathlib import Path
# from random import sample
# import json

# from numpy.random import random 

class model:
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
        total_prob = np.sum(self.prob_mat[:,track_j])
        initial_prob = self.prob_mat[obj_i, track_j]

        for obj in range(self.dim):
            if obj == obj_i:
                self.prob_mat[obj, : ] = np.ones(self.dim, dtype=float) * ((1-prob) / (self.dim-1))
                self.prob_mat[obj, track_j] = prob

            else:
                if total_prob == initial_prob:
                    return
                else:
                    curr_prob = self.prob_mat[obj, track_j]
                    new_prob = curr_prob * (1 - prob) / (total_prob - initial_prob)
                    diff = curr_prob - new_prob

                    sum_of_row = np.sum(self.prob_mat[obj, :]) - curr_prob
                    self.prob_mat[obj, :] = self.prob_mat[obj, :] * (1 + (diff / sum_of_row))
                    self.prob_mat[obj, track_j] = new_prob

    def inference(self):
        print("INFERENCE")
        for obj in range(self.dim):
            arr = np.copy(self.prob_mat[obj, :])

            lim = 5 if 5 < self.dim else self.dim

            # Find the indices in the 1D array
            listOfCordinates = arr.argsort()[-lim:].tolist()

            listOfCordinates.reverse()

            print("OBJECT: " + str(obj))
            print("LIKELY TRACKS:")
            for x in listOfCordinates:
                print(str(x) +  ", with PROBABILITY: " + str(arr[x]) )
            print("\n")

# model = model(3)
# model.update(0, 1, 0.5)
# model.update(0, 2, 0.2)
# model.update(0, 1, 0.9)
# model.update(2, 1, 0.1)
# model.inference()

# model = model(3)
# model.update(0, 1, 0.5)
# model.update(0, 2, 0.2)
# model.update(0, 1, 0.9)
# model.update(2, 1, 0.1)
# print(model.prob_mat)
# print("ALAAL")
# model.observation(1, 1, 1)
# print(model.prob_mat)
# # print("PWPWPWPW")
# # print(model.prob_mat[0,:,1,:])
# # print("ALAAL")
# # print(model.prob_mat[1,:,0,:])
# # print("ALAAL")
# # print(model.prob_mat[1,:,1,:])
# model.inference()