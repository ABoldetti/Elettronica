import numpy as np
import matplotlib.pyplot as plt
import scipy as sp

def line( x , a , b):
    return x*a + b


data=np.loadtxt('/Users/ibolde/coding/Lab_Elect/Termic_photons.csv' , delimiter=',' , dtype=str)

data = np.transpose(data[1::])

data = np.array(data , dtype = float)
print(data)

print(data[0] , data[2])
freq = data[0]/data[1]

print(freq)

#plt.scatter( data[2] , data[1])
plt.scatter( data[2],freq)
plt.show()

