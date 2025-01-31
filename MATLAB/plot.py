import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os

os.chdir(os.path.dirname(__file__))

hist_dark_phot = pd.read_csv('Dark Photon/hist_dark_phot.csv', delimiter=',', header=None)
hist_phot = pd.read_csv('Presa-mia Micro/hist_phot_3.4V.csv', delimiter=',', header=None)

hist_dark_phot = hist_dark_phot.to_numpy()[0]
hist_phot = hist_phot.to_numpy()[0]

bin_d , edg_d = np.histogram(hist_dark_phot , bins=200)
bin_p , edg_p = np.histogram(hist_phot , bins=200)

bin = bin_p-(bin_d/20)
plt.hist(edg_p[:-1], bins=edg_p, weights=bin)
plt.show()

