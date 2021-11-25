import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np

# LW = []
# LD = []
# ID = []
# S = []
# P = []
# Type = []
# W = []
# D = []

filenames = ["mapping_simple", "mapping_shared", "mapping_modifiedcostfunction", "mapping_modifiedcostfunction_withsharing"]

all_data = {}

for filename in filenames:
    current_file_data = []
    data_fd = open(filename, 'r')
    for line in data_fd.readlines():
        segments = line.strip().split(" ")
        
        LW_n = float(segments[4])
        LD_n = float(segments[6])
        ID_n = str(segments[0])+"_"+str(segments[8])
        S_n = float(segments[10])
        P_n = float(segments[12])
        Type_n = float(segments[14])
        W_n = float(segments[18])
        D_n = float(segments[20])
        
        # LW.append(LW_n)
        # LD.append(LD_n)
        # ID.append(ID_n)
        # S.append(S_n)
        # P.append(P_n)
        # Type.append(Type_n)
        # W.append(W_n)
        # D.append(D_n)
        
        Total_logic_bits_n = LW_n*LD_n
        Total_used_bits_n = S_n*P_n*W_n*D_n
        
        data = {}
        data["ID"] = ID_n
        data["total_logic"] = Total_logic_bits_n
        data["total_used"] = Total_used_bits_n
        
        current_file_data.append(data)
    all_data[filename] = current_file_data
    
plot_data = {}
for filename in filenames:
    temp = []
    for x in range(10):
        temp.append(0)
    plot_data[filename] = temp
    
for filename in filenames:
    print(filename)
    for count, each_data_point in enumerate(all_data[filename]):
        if (count % 1000 == 0):
            print(count)
        same_id = [x for x in all_data[filename] if x["ID"] == each_data_point["ID"]]
        
        # print(same_id)
        
        all_data_temp = [x for x in all_data[filename] if x not in same_id]
        all_data[filename] = all_data_temp
        
        total_total_logic_bits = 0
        total_used_sanity = each_data_point["total_used"]
        
        for each_same_id_point in same_id:
            total_total_logic_bits += each_same_id_point["total_logic"]
            if each_same_id_point["total_used"] != each_data_point["total_used"]:
                print("Warning: Same ID but different physical ram!!")
            
        
        Percentage_waste_n = (total_used_sanity-total_total_logic_bits)/total_used_sanity * 100
        
        if Percentage_waste_n >= 0 and Percentage_waste_n < 10:
            plot_data[filename][0] += 1
        elif Percentage_waste_n >= 10 and Percentage_waste_n < 20:
            plot_data[filename][1] += 1
        elif Percentage_waste_n >= 20 and Percentage_waste_n < 30:
            plot_data[filename][2] += 1
        elif Percentage_waste_n >= 30 and Percentage_waste_n < 40:
            plot_data[filename][3] += 1
        elif Percentage_waste_n >= 40 and Percentage_waste_n < 50:
            plot_data[filename][4] += 1
        elif Percentage_waste_n >= 50 and Percentage_waste_n < 60:
            plot_data[filename][5] += 1
        elif Percentage_waste_n >= 60 and Percentage_waste_n < 70:
            plot_data[filename][6] += 1
        elif Percentage_waste_n >= 70 and Percentage_waste_n < 80:
            plot_data[filename][7] += 1
        elif Percentage_waste_n >= 80 and Percentage_waste_n < 90:
            plot_data[filename][8] += 1
        elif Percentage_waste_n >= 90 and Percentage_waste_n <= 100:
            plot_data[filename][9] += 1
        else:
            print("Warning: Percentage_waste_n does not fit within buckets: {}", Percentage_waste_n)
        
        #print("Total_logic_bits:{}, Total_used_bits:{}, Percentage_waste_n:{}".format(Total_logic_bits, Total_used_bits, Percentage_waste_n))
    
    
X = np.arange(10)
fig = plt.figure(dpi=100, figsize=(14, 7))
ax = fig.add_axes([0.1,0.1,0.8,0.8])
plt.style.use('ggplot')

ax.bar(X - 0.30, plot_data["mapping_simple"], width = 0.2, label="Simple")
ax.bar(X - 0.10, plot_data["mapping_shared"], width = 0.2, label="Shared")
ax.bar(X + 0.10, plot_data["mapping_modifiedcostfunction"], width = 0.2, label="Modified cost function")
ax.bar(X + 0.3, plot_data["mapping_modifiedcostfunction_withsharing"], width = 0.2, label="Modified cost function with sharing")

plt.legend(loc="upper right")
plt.title('Mapper: Single Physical Ram Type - Physical RAM Usage (Total Required Logical Bits/Total Physical Ram Bits)')
plt.xlabel('Buckets')
plt.ylabel('Counts')
plt.xticks(np.arange(10), ['0~10%', '10~20%', '20~30%', '30~40%', '40~50%', '50~60%', '60~70%', '70~80%', '80~90%', '90~100%'])
plt.tick_params(axis='both', which='major', labelsize=10)
plt.show()
plt.savefig("single_ram_pram_usage.png")
    
    
    