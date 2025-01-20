import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv('Elettronica/MATLAB/hist_phot_3.4V.csv' , header=None)

plt.hist( df.to_numpy().T , 100)
plt.show()
plt.clf()

df1 = pd.read_csv('Elettronica/MATLAB/phot_3.4V.csv' , header=None)

df1 = df1.to_numpy()
for i in df1:
    plt.plot( np.linspace(0, 1, len(i)), i)
#plt.show()