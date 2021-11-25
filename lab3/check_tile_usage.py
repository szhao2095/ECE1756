import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np

filenames = ["checker_simple", "checker_shared", "checker_modifiedcostfunction", "checker_modifiedcostfunction_withsharing"]

plot_data = {}
for filename in filenames:
    temp = []
    for x in range(10):
        temp.append(0)
    plot_data[filename] = temp

for filename in filenames:
    current_file_data = []
    
    data_fd = open(filename, 'r')
    data_section = False
    
    for line in data_fd.readlines():
        segments = line.strip().split()
        
        if (len(segments) > 0):
            if (segments[0] == "Circuit"):
                data_section = True
                continue
            if (segments[0] == "Geometric"):
                data_section = False
                continue
        if (not data_section):
            continue
        
        blocks = float(segments[4])
        tiles = float(segments[5])
        usage_percentage = blocks/tiles * 100
        
        if usage_percentage >= 0 and usage_percentage < 10:
            plot_data[filename][0] += 1
        elif usage_percentage >= 10 and usage_percentage < 20:
            plot_data[filename][1] += 1
        elif usage_percentage >= 20 and usage_percentage < 30:
            plot_data[filename][2] += 1
        elif usage_percentage >= 30 and usage_percentage < 40:
            plot_data[filename][3] += 1
        elif usage_percentage >= 40 and usage_percentage < 50:
            plot_data[filename][4] += 1
        elif usage_percentage >= 50 and usage_percentage < 60:
            plot_data[filename][5] += 1
        elif usage_percentage >= 60 and usage_percentage < 70:
            plot_data[filename][6] += 1
        elif usage_percentage >= 70 and usage_percentage < 80:
            plot_data[filename][7] += 1
        elif usage_percentage >= 80 and usage_percentage < 90:
            plot_data[filename][8] += 1
        elif usage_percentage >= 90 and usage_percentage <= 100:
            plot_data[filename][9] += 1
        else:
            print("Warning: usage_percentage does not fit within buckets: {}", usage_percentage)
            

X = np.arange(10)
fig = plt.figure(dpi=100, figsize=(14, 7))
ax = fig.add_axes([0.1,0.1,0.8,0.8])
plt.style.use('ggplot')

ax.bar(X - 0.30, plot_data["checker_simple"], width = 0.2, label="Simple")
ax.bar(X - 0.10, plot_data["checker_shared"], width = 0.2, label="Shared counts")
ax.bar(X + 0.10, plot_data["checker_modifiedcostfunction"], width = 0.2, label="Modified cost function")
ax.bar(X + 0.3, plot_data["checker_modifiedcostfunction_withsharing"], width = 0.2, label="Modified cost function with sharing")

plt.legend(loc="upper right")
plt.title('Mapper: Single Physical Ram Type - Tile Usage')
plt.xlabel('Buckets')
plt.ylabel('Counts')
plt.xticks(np.arange(10), ['0~10%', '10~20%', '20~30%', '30~40%', '40~50%', '50~60%', '60~70%', '70~80%', '80~90%', '90~100%'])
plt.tick_params(axis='both', which='major', labelsize=10)
plt.show()
plt.savefig("single_ram_tile_usage.png")
                
            
            
            