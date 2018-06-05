import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import sys


print sys.argv[1]
weight = np.fromfile(sys.argv[1], dtype = sys.argv[2])
#weight = np.fromfile(sys.argv[1], dtype = np.int16)	

#weight = np.fromfile("conv_weight0_16x27x1_4.weight")
#print "conv_weight0_16x27x1_4.weight"
print "Miminum = ", weight.min()
print "Maximum = ", weight.max()
print "Near 0 = ", weight[(np.abs(weight - 0)).argmin()]

sns.set(color_codes=True)
sns.distplot(weight)
#plt.show()
print weight[0]




#plt.show()
#sns.distplot(weight, kde=False, rug=True)
#plt.show()
