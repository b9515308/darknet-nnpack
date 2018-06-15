import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import sys
import pandas as pd
import seaborn as sns

def get_order_array(data):
    s = np.abs(data)
    s = np.log10(s)
    np.nan_to_num(s)
    s = np.floor(s)
    return s

file = open(sys.argv[1], "r")

for line in file:
    print line
    data = np.fromfile(str.strip(line), dtype = np.float32)
    print "Max = ", data.max()
    print "Mim = ", data.min()
    print "Near 0 = ", data[np.abs(data - 0).argmin()]
    ten_dis = get_order_array(data)
    sns.set(style="darkgrid")
    df = pd.DataFrame(ten_dis, columns = ["order"])
    ax = sns.countplot(x = "order", data=df)
    plt.show()

#print sys.argv[1]
#weight = np.fromfile(sys.argv[1], dtype = sys.argv[2])
#weight = np.fromfile(sys.argv[1], dtype = np.int16)	

#weight = np.fromfile("conv_weight0_16x27x1_4.weight")
#print "conv_weight0_16x27x1_4.weight"
#print "Miminum = ", weight.min()
#print "Maximum = ", weight.max()
#print "Near 0 = ", weight[(np.abs(weight - 0)).argmin()]

#sns.set(color_codes=True)
#sns.distplot(weight)
#plt.show()
#print weight[0]




#plt.show()
#sns.distplot(weight, kde=False, rug=True)
#plt.show()
