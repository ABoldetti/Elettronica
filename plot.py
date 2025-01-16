import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('Elettronica/MATLAB/hist_phot_3.4V.csv' , header=None)

plt.hist( df.to_numpy().T , 100)
plt.show()