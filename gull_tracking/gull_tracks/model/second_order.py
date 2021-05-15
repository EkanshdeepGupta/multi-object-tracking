import numpy as np
# import pandas
# from scipy import stats
# from scipy.integrate import quad
# from haversine import haversine, Unit
# from pathlib import Path
# from random import sample
# import json

# from numpy.random import random 


def switch(i,j,k):
    if k == i:
        return j
    elif k == j:
        return i
    else:
        return k

class model:
    def __init__(self, n): # n number of tracks
        self.dim = n
        self.prob_mat = np.zeros((self.dim, self.dim, self.dim, self.dim), dtype=float)

        for i in range(self.dim):
            for j in range(self.dim):
                self.prob_mat[i,i,j,j] = 1

    def update(self, i, j, p): #track_i and track_j exchange with probability p
        new_mat = np.zeros((self.dim, self.dim, self.dim, self.dim), dtype=float)
        for obj_1 in range(self.dim):
            for track_1 in range(self.dim):
                for obj_2 in range(self.dim):
                    for track_2 in range(self.dim):
                        new_mat[obj_1, track_1, obj_2, track_2] = (1-p) * self.prob_mat[obj_1, track_1, obj_2, track_2] + p * self.prob_mat[obj_1, switch(i,j,track_1), obj_2, switch(i,j,track_2)]
        
        self.prob_mat = new_mat                        
        
    def observation(self, i, j, prob): #obj_i on track_j with probability prob
        pass

        new_mat = np.copy(self.prob_mat)

        for obj in range(self.dim):
            new_mat[i,:,obj,:] = self.rebalanceR(j, prob, self.prob_mat[i,:,obj,:])
            new_mat[obj,:,i,:] = self.rebalanceC(j, prob, self.prob_mat[obj,:,i,:])





        # for obj in range(self.dim):
        #     for track in range(self.dim):

        
        # self.prob_mat[obj_i, : ] = np.ones(self.dim, dtype=float) * ((1-prob) / (self.dim-1))
        # self.prob_mat[obj_i, track_j] = prob

    def inference(self): # returns the most likely permutation
        pass

    def rebalanceR(self, j, prob, mat):
        mat1 = np.copy(mat)
        curr_prob = np.sum(mat[j])

        if curr_prob != 1 and curr_prob != 0:
            mat1[j] = mat[j] * (prob / curr_prob)

            for i in range(self.dim):
                if i != j:
                    mat1[j] = mat[j] * ((1 - prob)/(1 - curr_prob))

        return mat1   


    def rebalanceC(self, j, prob, mat):
        mat1 = np.copy(mat)
        curr_prob = np.sum(mat[:,j])

        if curr_prob != 1 and curr_prob != 0:
            mat1[:,j] = mat[:,j] * (prob / curr_prob)

            for i in range(self.dim):
                if i != j:
                    mat1[:,j] = mat[:,j] * ((1 - prob)/(1 - curr_prob))

        return mat1   

# model = model(3)
# model.update(0, 1, 0.5)
# model.update(0, 2, 0.2)
# model.update(0, 1, 0.9)
# model.update(2, 1, 0.1)
# print(model.prob_mat)
# print("ALAAL")
# model.observation(1, 1, 1)
# print(model.prob_mat)
# print("PWPWPWPW")
# print(model.prob_mat[0,:,1,:])
# print("ALAAL")
# print(model.prob_mat[1,:,0,:])
# print("ALAAL")
# print(model.prob_mat[1,:,1,:])