import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

# Read data from CSV file
data = pd.read_csv('Elettronica/MATLAB/data.csv')
print(data.size)

data = data.to_numpy().T

plt.plot(np.arange(len(data[0])), data[0])

plt.show()

# for i in data.columns:
#     plt.plot(data[i])
# plt.show()

