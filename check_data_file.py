import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import sys
import pandas as pd
import seaborn as sns
f = []

print ("python check_data_file.py cd_out_weight_files cd_out_qweight_files cd_out_input_files cd_out_qinput_files prediction_cube.bin golden_prediction_cube.bin")

def get_order_array(data):
    s = np.abs(data)
    s = s + 0.0000000001
    s = np.log10(s)
    np.nan_to_num(s)
    s = np.trunc(s)
    np.nan_to_num(s)
    return s

weight_files = open(sys.argv[1], "r")
qweight_files = open(sys.argv[2], "r")
input_files = open(sys.argv[3], "r")
qinput_files = open(sys.argv[4], "r")
out_files = open(sys.argv[5], "r")
qout_files = open(sys.argv[6], "r")
prd_bin = str.strip(sys.argv[7])
gd_prd_bin = str.strip(sys.argv[8])


w_df = pd.DataFrame(columns = ["order"])
qw_df = pd.DataFrame(columns = ["order"])
in_df = pd.DataFrame(columns = ["order"])
qin_df = pd.DataFrame(columns = ["order"])
out_df = pd.DataFrame(columns = ["order"])
qout_df = pd.DataFrame(columns = ["order"])
prd_df = pd.DataFrame(columns = ["order"])
gd_prd_df = pd.DataFrame(columns = ["order"])
for index, line in enumerate (weight_files):
    print line
    if line.find("#") >= 0:
        continue
    data = np.fromfile(str.strip(line), dtype = np.float32)
    print "Max = ", data.max()
    print "Mim = ", data.min()
    print "Near 0 = ", data[np.abs(data - 0).argmin()]
    ten_dis = get_order_array(data)
    sns.set(style="darkgrid")
    df = pd.DataFrame(ten_dis, columns = ["order"])
    w_df = w_df.append(df)    


for index, line in enumerate (qweight_files):
    print line
    if line.find("#") >= 0:
        continue
    data = np.fromfile(str.strip(line), dtype = np.int16)
    print "Max = ", data.max()
    print "Mim = ", data.min()
    print "Near 0 = ", data[np.abs(data - 0).argmin()]
    data = data + 1
    ten_dis = get_order_array(data)
    sns.set(style="darkgrid")
    df = pd.DataFrame(ten_dis, columns = ["order"])
    qw_df = qw_df.append(df)    
    
for index, line in enumerate (input_files):
    print line
    if line.find("#") >= 0:
        continue
    data = np.fromfile(str.strip(line), dtype = np.float32)
    print "Max = ", data.max()
    print "Mim = ", data.min()
    print "Near 0 = ", data[np.abs(data - 0).argmin()]
    ten_dis = get_order_array(data)
    sns.set(style="darkgrid")
    df = pd.DataFrame(ten_dis, columns = ["order"])
    in_df = in_df.append(df)



for index, line in enumerate (qinput_files):
    print line
    if line.find("#") >= 0:
        continue
    data = np.fromfile(str.strip(line), dtype = np.int16)
    print "Max = ", data.max()
    print "Mim = ", data.min()
    print "Near 0 = ", data[np.abs(data - 0).argmin()]
    ten_dis = get_order_array(data)
    sns.set(style="darkgrid")
    df = pd.DataFrame(ten_dis, columns = ["order"])
    qin_df = qin_df.append(df)


for index, line in enumerate (out_files):
    print line
    if line.find("#") >= 0:
        continue
    data = np.fromfile(str.strip(line), dtype = np.int32)
    print "Max = ", data.max()
    print "Mim = ", data.min()
    print "Near 0 = ", data[np.abs(data - 0).argmin()]
    ten_dis = get_order_array(data)
    sns.set(style="darkgrid")
    df = pd.DataFrame(ten_dis, columns = ["order"])
    out_df = out_df.append(df)



for index, line in enumerate (qout_files):
    print line
    if line.find("#") >= 0:
        continue
    data = np.fromfile(str.strip(line), dtype = np.int16)
    print "Max = ", data.max()
    print "Mim = ", data.min()
    print "Near 0 = ", data[np.abs(data - 0).argmin()]
    ten_dis = get_order_array(data)
    sns.set(style="darkgrid")
    df = pd.DataFrame(ten_dis, columns = ["order"])
    qout_df = qout_df.append(df)



print prd_bin
data = np.fromfile(prd_bin, dtype = np.float32)
print "Max = ", data.max()
print "Mim = ", data.min()
print "Near 0 = ", data[np.abs(data - 0).argmin()]
ten_dis = get_order_array(data)
sns.set(style="darkgrid")
df = pd.DataFrame(ten_dis, columns = ["order"])
prd_df = prd_df.append(df)

print gd_prd_bin
data = np.fromfile(gd_prd_bin, dtype = np.float32)
print "Max = ", data.max()
print "Mim = ", data.min()
print "Near 0 = ", data[np.abs(data - 0).argmin()]
ten_dis = get_order_array(data)
sns.set(style="darkgrid")
df = pd.DataFrame(ten_dis, columns = ["order"])
gd_prd_df = gd_prd_df.append(df)




w_figure = plt.figure()
ax = sns.countplot(x = "order", data=w_df)
w_figure.canvas.set_window_title("Weights")


qw_figure = plt.figure()
ax = sns.countplot(x = "order", data=qw_df)
qw_figure.canvas.set_window_title("quan_Weights")


in_figure = plt.figure()
ax = sns.countplot(x = "order", data=in_df)
in_figure.canvas.set_window_title("inputs")


qin_figure = plt.figure()
ax = sns.countplot(x = "order", data=qin_df)
qin_figure.canvas.set_window_title("quan_inputs")


out_figure = plt.figure()
ax = sns.countplot(x = "order", data=out_df)
out_figure.canvas.set_window_title("outs")


qout_figure = plt.figure()
ax = sns.countplot(x = "order", data=qout_df)
qout_figure.canvas.set_window_title("quan_outs")


prd_figure = plt.figure()
ax = sns.countplot(x = "order", data=prd_df)
prd_figure.canvas.set_window_title("final_predict_cube")


gd_prd_figure = plt.figure()
ax = sns.countplot(x = "order", data=gd_prd_df)
gd_prd_figure.canvas.set_window_title("gorden_predict_cube")

plt.show()
#raw_input()
plt.close("all")
