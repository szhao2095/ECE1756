import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np

file_1_str = "checker_mapper_1"
file_2_str = "checker_custom_arch_best"

file_1_data = []
file_2_data = []

plot_data = []

with open(file_1_str, 'r') as current_fd:
    data_section = False
    for line in current_fd.readlines():
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
        
        file_1_data.append(float(segments[6]))

with open(file_2_str, 'r') as current_fd:
    data_section = False
    for line in current_fd.readlines():
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
        
        file_2_data.append(float(segments[6]))
        
plot_data = np.subtract(file_2_data, file_1_data)

X = np.arange(69)
fig = plt.figure(dpi=100, figsize=(14, 7))
ax = fig.add_axes([0.1,0.1,0.8,0.8])
ax.bar(X, plot_data, width = 0.5, label="Data")
plt.legend(loc="upper right")
plt.title('Area difference: ' + file_2_str + " - " + file_1_str)
plt.xlabel('Circuit No.')
plt.ylabel('Area')
plt.xticks(np.arange(0, 70, 5))
plt.tick_params(axis='both', which='major', labelsize=10)
plt.show()
plt.savefig("area_difference.png")