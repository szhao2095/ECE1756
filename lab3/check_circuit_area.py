import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np

filenames = ["checker_simple", "checker_shared", "checker_modifiedcostfunction", "checker_modifiedcostfunction_withsharing"]

X = np.arange(69)
plt.style.use('ggplot')
    
for filename in filenames:
    data_fd = open(filename, 'r')
    data_section = False
    
    plot_data = []
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
        
        plot_data.append(float(segments[6]))
        
            

    fig = plt.figure(dpi=100, figsize=(14, 7))
    ax = fig.add_axes([0.1,0.1,0.8,0.8])

    plot_label = "Simple"
    if (filename == "checker_shared"):
        plot_label = "Shared"
    elif (filename == "checker_modifiedcostfunction"):
        plot_label = "Modified cost function" 
    elif (filename == "checker_modifiedcostfunction_withsharing"):
        plot_label = "Modified cost function with sharing"

    ax.bar(X, plot_data, width = 0.5, label=plot_label)
    ax.set_ylim([0, 1.5e9])

    plt.legend(loc="upper right")
    plt.title('Mapper: Single Physical Ram Type - Circuit Area (' + plot_label+')')
    plt.xlabel('Circuit No.')
    plt.ylabel('Area')
    plt.xticks(np.arange(0, 70, 5))
    plt.tick_params(axis='both', which='major', labelsize=10)
    plt.show()
    plt.savefig("single_ram_circuit_area_"+filename+".png")
                
            
            
            