from numpy.core.function_base import linspace
from scipy import stats
from scipy.signal import find_peaks
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from scipy.integrate import quad
import json
from pathlib import Path

# source:    https://stackoverflow.com/questions/49801071/how-can-i-use-skewnorm-to-produce-a-distribution-with-the-specified-skew
#           https://stackoverflow.com/questions/5884768/skew-normal-distribution-in-scipy


def createSkewDist(mean, sd, skew, size):

    # calculate the degrees of freedom 1 required to obtain the specific skewness statistic, derived from simulations
    loglog_slope = -2.211897875506251
    loglog_intercept = 1.002555437670879
    df2 = 500
    df1 = 10**(loglog_slope*np.log10(abs(skew)) + loglog_intercept)

    # sample from F distribution
    fsample = np.sort(stats.f(df1, df2).rvs(size=size))

    # adjust the variance by scaling the distance from each point to the distribution mean by a constant, derived from simulations
    k1_slope = 0.5670830069364579
    k1_intercept = -0.09239985798819927
    k2_slope = 0.5823114978219056
    k2_intercept = -0.11748300123471256

    scaling_slope = abs(skew)*k1_slope + k1_intercept
    scaling_intercept = abs(skew)*k2_slope + k2_intercept

    scale_factor = (sd - scaling_intercept)/scaling_slope
    new_dist = (fsample - np.mean(fsample))*scale_factor + fsample

    # flip the distribution if specified skew is negative
    if skew < 0:
        new_dist = np.mean(new_dist) - new_dist

    # adjust the distribution mean to the specified value
    final_dist = new_dist + (mean - np.mean(new_dist))

    return final_dist


def apply_skew_dist(desired_mean=None, desired_skew=None, desired_sd=None):
    if not (desired_mean and desired_skew and desired_sd):
        params = json.load(open(Path(__file__).parent / './input/skew_input.json'))
        desired_mean = params['mean']
        desired_skew = params['skew']
        desired_sd = params['sd']

    final_dist = createSkewDist(
        mean=desired_mean, sd=desired_sd, skew=desired_skew, size=1000000)

    # inspect the plots & moments, try random sample
    fig, ax = plt.subplots(figsize=(12, 7))
    sns.distplot(final_dist, hist=False, ax=ax, color='green',
                 label='generated distribution')
    ae, loce, scalee = stats.skewnorm.fit(final_dist)

    with open(Path(__file__).parent / './output/skew_dist_params.json', 'w+') as file:
        json.dump({'Desired mean': desired_mean,
                   'Desired sd': desired_sd,
                   'Desired skew': desired_skew,
                   'Estimated a': ae,
                   'Estimated loc': loce,
                   'Estimated scale': scalee}, file)

    x_min = np.min(final_dist)
    x_max = np.max(final_dist)
    x = linspace(x_min, x_max, 1000000)
    fitted_dist = stats.skewnorm.pdf(x, a=ae, loc=loce, scale=scalee)
    ax.plot(x, fitted_dist, 'r-', lw=5, alpha=0.6, label='skewnorm pdf')
    #sns.distplot(np.random.choice(final_dist, size=100), hist=True, ax=ax, color='red', hist_kws={'alpha':.2}, label='sample n=100')
    ax.legend()
    plt.show()
    print('Input mean: ', desired_mean)
    print('Result mean: ', np.mean(final_dist))
    print('Fitted mean: ', np.mean(fitted_dist), '\n')

    print('Input SD: ', desired_sd)
    print('Result SD: ', np.std(final_dist))
    print('Fitted SD: ', np.std(fitted_dist), '\n')

    print('Input skew: ', desired_skew)
    print('Result skew: ', stats.skew(final_dist))
    print('Fitted skew: ', stats.skew(fitted_dist))

apply_skew_dist()