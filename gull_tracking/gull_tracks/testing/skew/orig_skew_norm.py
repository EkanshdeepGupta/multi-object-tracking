from matplotlib import pyplot as plt
import numpy as np
from scipy.stats import skewnorm
data = skewnorm.rvs(3, loc=500, scale=500, size=1000).astype(np.int)
params = skewnorm.fit(data, 10, loc=500, scale=500)
ax = plt.axes()
x = np.linspace(0, 2000, 50000)
skewnorm.pdf(x, *params)
true_dist = 1/max(skewnorm.pdf(x, *params))*skewnorm.pdf(x, *params)
ax.plot(x, 1/max(skewnorm.pdf(x, *params))*skewnorm.pdf(x, *params), label='approximated skewnorm')
