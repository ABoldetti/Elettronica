import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os

os.chdir(os.path.dirname(__file__))

hist_dark_phot = pd.read_csv('Dark Photon/hist_dark_phot.csv', delimiter=',', header=None)
hist_phot = pd.read_csv('Presa-mia Micro/hist_phot_3.4V.csv', delimiter=',', header=None)
hist_dark_phot2 = pd.read_csv('dati_buio.csv', delimiter=',', header=None)


hist_dark_phot = hist_dark_phot.to_numpy()[0]
hist_phot = hist_phot.to_numpy()[0]
hist_dark_phot2 = hist_dark_phot2.to_numpy()

bin_d , edg_d = np.histogram(hist_dark_phot , bins=200 , density=False)
bin , edg = np.histogram(hist_phot , bins=200 , density=False)



plt.hist(hist_phot, bins=200 , color = 'g' , alpha = 0.5 , density=False , label='Fotoni')
plt.hist(hist_dark_phot2, bins=200 , color = 'r' , alpha = 0.5 , density=False , label='Buio 34V')
plt.hist(hist_dark_phot, bins=200 , color = 'b' , alpha = 0.5 , density=False , label='Buio 36V')
plt.title('Paragone dark count e a regime')
plt.xlabel('Voltaggio [V]')
plt.ylabel('counts')
plt.legend()
plt.savefig('Paragone_dark_count.pdf' , format='pdf')
plt.show()


arr = [ 0 , 10 , 22 , 35 , 47 , 59 , 71 , 85]

V_value =[ (edg[i] + edg[i+1])/2 for i in arr]
V_err = [ np.std([edg[i] , edg[i+1]]) for i in arr]
V_pred = [ V_value[0] * (i+1) for i in range(len(V_value))]

plt.title('Voltaggio fotoni')
plt.errorbar( np.arange(1 , len(V_value)+1), V_value , yerr=V_err , color='r' , fmt='o' , label='Dati sperimentali')
plt.scatter( np.arange(1 ,len(V_value)+1), V_pred , color='b' , label='Valori attesi')
plt.xlabel('Numero fotoni')
plt.ylabel('Voltaggio [V]')
plt.legend()
plt.savefig('Voltaggio_fotoni.pdf' , format='pdf') 
plt.show()