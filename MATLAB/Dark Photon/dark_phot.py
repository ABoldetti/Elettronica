import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os

os.chdir(os.path.dirname(__file__))
# Read data from CSV file
data_arr = pd.read_csv('dark_phot.csv' , delimiter=',', header=None)
hist_arr = pd.read_csv('hist_dark_phot.csv' , delimiter=',', header=None)

data_arr = data_arr.to_numpy()
hist_arr = hist_arr.to_numpy()[0]

tunc = 5
fig,ax = plt.subplots(ncols=1, nrows=2)
plt.suptitle('Photodiode 3.4V')
for i in data_arr:
    data = np.concatenate((i[-2:] , i[:-2]))
    ax[0].plot(np.arange(0, tunc, 1), data[:tunc] , alpha=0.2 , color='#1f77b4')

ax[1].hist(hist_arr , bins=200)

ax[0].set_xlabel('Time [ns]')
ax[0].set_ylabel('Amplitude [V]')
ax[1].set_xlabel('Amplitude [V]')

plt.subplots_adjust( hspace=0.6)
plt.savefig('Elettronica/MATLAB/dark_phot.pdf', format='pdf', bbox_inches='tight')


