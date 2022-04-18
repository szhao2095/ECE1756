import sys, os, subprocess
import math

def geo_mean(xs):
    return math.exp(math.fsum(math.log(x) for x in xs) / len(xs))


def main():
    all_architecture = []
    file_name = ""
    
    
    ### Q1
    if ("-q1" in sys.argv):
        all_architecture = [
            # {
                # 'filename': 'k6_N10_40nm_length_1.xml',
                # 'name': '1'
            # },
            # {
                # 'filename': 'k6_N10_40nm_length_2.xml',
                # 'name': '2'
            # },
            # {
                # 'filename': 'k6_N10_40nm_length_4.xml',
                # 'name': '4'
            # },
            # {
                # 'filename': 'k6_N10_40nm_length_8.xml',
                # 'name': '8'
            # },
            {
                'filename': 'k6_N10_40nm_length_16.xml',
                'name': '16'
            },
            {
                'filename': 'k6_N10_40nm.xml',
                'name': 'base'
            }
        ]
        file_name = "q1"
    
    ### 2a, 2b
    if ("-q2ab" in sys.argv):
        all_architecture = [
            {
                'filename': 'k6_N10_40nm_clb_0_15.xml',
                'name': 'clb_0_15'
            },
            {
                'filename': 'k6_N10_40nm_clb_0_5.xml',
                'name': 'clb_0_5'
            },
            {
                'filename': 'k6_N10_40nm_clb_1_0.xml',
                'name': 'clb_1_0'
            },
            {
                'filename': 'k6_N10_40nm_io_0_15.xml',
                'name': 'io_0_15'
            },
            {
                'filename': 'k6_N10_40nm_io_0_5.xml',
                'name': 'io_0_5'
            },
            {
                'filename': 'k6_N10_40nm_io_1_0.xml',
                'name': 'io_1_0'
            }
        ]
        file_name = "q2ab"
    
    ### 2c
    if ("-q2c" in sys.argv):
        all_architecture = [
            {
                'filename': 'k6_N10_40nm_clb_0_15_no_crossbar.xml',
                'name': 'clb_0_15_no_crossbar'
            },
            {
                'filename': 'k6_N10_40nm_clb_0_5_no_crossbar.xml',
                'name': 'clb_0_5_no_crossbar'
            },
            {
                'filename': 'k6_N10_40nm_clb_1_0_no_crossbar.xml',
                'name': 'clb_1_0_no_crossbar'
            }
        ]
        file_name = "q2c"
    
    ### 3_res_cap_limit
    if ("-q3_rc_lim" in sys.argv):
        all_architecture = [
            {
                'filename': 'k6_N10_40nm_len4_i.xml',
                'name': 'len4_i'
            },
            {
                'filename': 'k6_N10_40nm_len4_ii.xml',
                'name': 'len4_ii'
            }
        ]
        file_name = "q3_rc_lim"

    ### 3_double_full
    if ("-q3_double_full" in sys.argv):
        all_architecture = [
            {
                'filename': 'k6_N10_40nm_double_2_full_4_full.xml',
                'name': '2_full_4_full'
            },
            {
                'filename': 'k6_N10_40nm_double_2_full_8_full.xml',
                'name': '2_full_8_full'
            },
            {
                'filename': 'k6_N10_40nm_double_4_full_8_full.xml',
                'name': '4_full_8_full'
            }
        ]
        file_name = "q3_double_full"

    ### 3_double_full_rc_lim
    if ("-q3_double_full_rc_lim" in sys.argv):
        all_architecture = [
            {
                'filename': 'k6_N10_40nm_double_2_full_4_full.xml',
                'name': '2_full_4_full'
            },
            {
                'filename': 'k6_N10_40nm_double_2_full_4_full_i.xml',
                'name': '2_full_4_full_i'
            },
            {
                'filename': 'k6_N10_40nm_double_2_full_4_full_ii.xml',
                'name': '2_full_4_full_ii'
            }
        ]
        file_name = "q3_double_full_rc_lim"

    ### q3_double_alt_sb
    if ("-q3_double_alt_sb" in sys.argv):
        all_architecture = [
            {
                'filename': 'k6_N10_40nm_double_2_alt_4_alt.xml',
                'name': '2_alt_4_alt'
            },
            {
                'filename': 'k6_N10_40nm_double_2_alt_8_alt.xml',
                'name': '2_alt_8_alt'
            },
            {
                'filename': 'k6_N10_40nm_double_4_alt_8_alt.xml',
                'name': '4_alt_8_alt'
            }
        ]
        file_name = "q3_double_alt_sb"

    ### q3_double_alt_sb_cb
    if ("-q3_double_alt_sb_cb" in sys.argv):
        all_architecture = [
            {
                'filename': 'k6_N10_40nm_double_2_alt_4_alt.xml',
                'name': '2_alt_4_alt'
            },
            {
                'filename': 'k6_N10_40nm_double_2_alt_8_alt.xml',
                'name': '2_alt_8_alt'
            },
            {
                'filename': 'k6_N10_40nm_double_4_alt_8_alt.xml',
                'name': '4_alt_8_alt'
            }
        ]
        file_name = "q3_double_alt_sb_cb"

    ### 3_double_full_clb_fc_0_5
    if ("-q3_double_full_clb_fc_0_5" in sys.argv):
        all_architecture = [
            {
                'filename': 'k6_N10_40nm_double_2_full_4_full.xml',
                'name': '2_full_4_full'
            },
            {
                'filename': 'k6_N10_40nm_double_2_full_8_full.xml',
                'name': '2_full_8_full'
            },
            {
                'filename': 'k6_N10_40nm_double_4_full_8_full.xml',
                'name': '4_full_8_full'
            }
        ]
        file_name = "q3_double_full_clb_fc_0_5"

    ### q3_double_alt_sb_cb_2
    if ("-q3_double_alt_sb_cb_2" in sys.argv):
        all_architecture = [
            {
                'filename': 'k6_N10_40nm_double_2_alt_4_alt.xml',
                'name': '2_alt_4_alt'
            },
            {
                'filename': 'k6_N10_40nm_double_2_alt_8_alt.xml',
                'name': '2_alt_8_alt'
            },
            {
                'filename': 'k6_N10_40nm_double_4_alt_8_alt.xml',
                'name': '4_alt_8_alt'
            }
        ]
        file_name = "q3_double_alt_sb_cb_2"

    ### q3_double_alt_sb_cb_rc_lim
    if ("-q3_double_alt_sb_cb_rc_lim" in sys.argv):
        all_architecture = [
            {
                'filename': 'k6_N10_40nm_double_2_alt_4_alt.xml',
                'name': '2_alt_4_alt'
            },
            {
                'filename': 'k6_N10_40nm_double_2_alt_4_alt_i.xml',
                'name': '2_alt_4_alt_i'
            },
            {
                'filename': 'k6_N10_40nm_double_2_alt_4_alt_ii.xml',
                'name': '2_alt_4_alt_ii'
            }
        ]
        file_name = "q3_double_alt_sb_cb_rc_lim"

    ### q3_double_alt_sb_cb_2_rc_lim
    if ("-q3_double_alt_sb_cb_2_rc_lim" in sys.argv):
        all_architecture = [
            # {
                # 'filename': 'k6_N10_40nm_double_2_alt_4_alt.xml',
                # 'name': '2_alt_4_alt'
            # },
            {
                'filename': 'k6_N10_40nm_double_2_alt_4_alt_i.xml',
                'name': '2_alt_4_alt_i'
            },
            {
                'filename': 'k6_N10_40nm_double_2_alt_4_alt_ii.xml',
                'name': '2_alt_4_alt_ii'
            }
        ]
        file_name = "q3_double_alt_sb_cb_2_rc_lim"

    all_seeds = [1, 2, 3, 4, 5]

    all_benchmarks = [
        {
            'name':'clma',
            'filename': 'clma.blif'
        },
        {
            'name':'elliptic',
            'filename': 'elliptic.blif'
        },
        {
            'name':'ex1010',
            'filename': 'ex1010.blif'
        },
        {
            'name':'frisc',
            'filename': 'frisc.blif'
        },
        {
            'name':'pdc',
            'filename': 'pdc.blif'
        },
        {
            'name':'s38417',
            'filename': 's38417.blif'
        },
        {
            'name':'s38584.1',
            'filename': 's38584.1.blif'
        },
        {
            'name':'spla',
            'filename': 'spla.blif'
        }
    ]

    table_ljust_10 = 10
    table_ljust_15 = 15
    table_ljust_20 = 20
    table_ljust_30 = 30
    
    all_final_prints = []

    with open('summary_' + file_name, 'w+') as summary_fd:
        for each_arch in all_architecture:   
    
            all_min_channel_width_arch = []
            all_area_per_tile_arch = []
            all_crit_path_delay_arch = []     
            
            with open('details_' + file_name + '_' + each_arch['name'], 'w+') as details_fd:
                table_header_str = '{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}'.format(
                                'Architecture'.ljust(table_ljust_15),
                                'Seed'.ljust(table_ljust_10),
                                'clma_wd'.ljust(table_ljust_10),
                                'clma_area'.ljust(table_ljust_10),
                                'clma_delay'.ljust(table_ljust_15),
                                'elliptic_wd'.ljust(table_ljust_15),
                                'elliptic_area'.ljust(table_ljust_15),
                                'elliptic_delay'.ljust(table_ljust_20),
                                'ex1010_wd'.ljust(table_ljust_15),
                                'ex1010_area'.ljust(table_ljust_15),
                                'ex1010_delay'.ljust(table_ljust_15),
                                'frisc_wd'.ljust(table_ljust_10),
                                'frisc_area'.ljust(table_ljust_10),
                                'frisc_delay'.ljust(table_ljust_15),
                                'pdc_wd'.ljust(table_ljust_10),
                                'pdc_area'.ljust(table_ljust_10),
                                'pdc_delay'.ljust(table_ljust_15),
                                's38417_wd'.ljust(table_ljust_15),
                                's38417_area'.ljust(table_ljust_15),
                                's38417_delay'.ljust(table_ljust_15),
                                's38584.1_wd'.ljust(table_ljust_15),
                                's38584.1_area'.ljust(table_ljust_15),
                                's38584.1_delay'.ljust(table_ljust_15),
                                'spla_wd'.ljust(table_ljust_10),
                                'spla_area'.ljust(table_ljust_10),
                                'spla_delay'.ljust(table_ljust_15),
                                'geomean_wd'.ljust(table_ljust_30),
                                'geomean_area'.ljust(table_ljust_30),
                                'geomean_delay'.ljust(table_ljust_30),
                                )
                #print(table_header_str)
                details_fd.write(table_header_str)
                details_fd.write("\n")
                
                for each_seed in all_seeds:
                
                    all_min_channel_width = []
                    all_area_per_tile = []
                    all_crit_path_delay = []
                    all_data = {}
                
                    for each_benchmark in all_benchmarks:
                        print("Running " + each_arch['name'] + " for " + each_benchmark['name'] + ", seed: " + str(each_seed))

                        os.system('../vtr-verilog-to-routing-8.0.0/vpr/vpr ' + each_arch['filename'] + ' ../a4_benchmarks/' + each_benchmark['filename'] + ' --seed ' + str(each_seed) + ' > temp')

                        min_channel_width = float(subprocess.check_output("grep 'Best routing' temp", shell=True).decode('utf-8').split("of ")[1].split('.')[0].strip())

                        new_channel_width = int(min_channel_width*1.3)
                        if (new_channel_width%2) != 0:
                            new_channel_width += 1
                            
                        print("First run completed, running again with new_channel_width: " + str(new_channel_width) + ", min_channel_width: " + str(min_channel_width))
                            
                        os.system('../vtr-verilog-to-routing-8.0.0/vpr/vpr ' + each_arch['filename'] + ' ../a4_benchmarks/' + each_benchmark['filename'] + ' --seed ' + str(each_seed) + ' --route_chan_width ' + str(new_channel_width) + ' > temp')
                        
                        print("Second run completed for " + each_arch['name'] + "::" + each_benchmark['name'] + "::" + str(each_seed))

                        area_per_tile = float(subprocess.check_output("grep 'Total routing area' temp", shell=True).decode('utf-8').split("tile: ")[1].strip())

                        crit_path_delay = float(subprocess.check_output("grep 'Final critical path' temp", shell=True).decode('utf-8').split("path: ")[1].split(' ns,')[0].strip())

                        data = {}
                        data[each_benchmark['name'] + "_wd"] = min_channel_width
                        data[each_benchmark['name'] + "_area"] = area_per_tile
                        data[each_benchmark['name'] + "_delay"] = crit_path_delay
                        all_data[each_benchmark['name']] = data
                        
                        all_min_channel_width.append(min_channel_width)
                        all_area_per_tile.append(area_per_tile)
                        all_crit_path_delay.append(crit_path_delay)
                            
                        if each_benchmark['name'] == "clma":
                            os.system('rm -f ./clma.net')
                            os.system('rm -f ./clma.place')
                            os.system('rm -f ./clma.route')
                        
                        if each_benchmark['name'] == "elliptic":
                            os.system('rm -f ./elliptic.net')
                            os.system('rm -f ./elliptic.place')
                            os.system('rm -f ./elliptic.route')
                            
                        if each_benchmark['name'] == "ex1010":
                            os.system('rm -f ./ex1010.net')
                            os.system('rm -f ./ex1010.place')
                            os.system('rm -f ./ex1010.route')
                            
                        if each_benchmark['name'] == "frisc":
                            os.system('rm -f ./frisc.net')
                            os.system('rm -f ./frisc.place')
                            os.system('rm -f ./frisc.route')
                            
                        if each_benchmark['name'] == "pdc":
                            os.system('rm -f ./pdc.net')
                            os.system('rm -f ./pdc.place')
                            os.system('rm -f ./pdc.route')
                            
                        if each_benchmark['name'] == "s38417":
                            os.system('rm -f ./s38417.net')
                            os.system('rm -f ./s38417.place')
                            os.system('rm -f ./s38417.route')
                            
                        if each_benchmark['name'] == "s38584.1":
                            os.system('rm -f ./s38584.1.net')
                            os.system('rm -f ./s38584.1.place')
                            os.system('rm -f ./s38584.1.route')
                            
                        if each_benchmark['name'] == "spla":
                            os.system('rm -f ./spla.net')
                            os.system('rm -f ./spla.place')
                            os.system('rm -f ./spla.route')
                    
                        os.system('rm -f ./*.rpt')
                        os.system('rm -f temp')
                        os.system('rm -f vpr_stdout.log')

                    geomean_wd = geo_mean(all_min_channel_width)
                    geomean_area = geo_mean(all_area_per_tile)
                    geomean_delay = geo_mean(all_crit_path_delay)
                    
                    all_min_channel_width_arch.append(geomean_wd)
                    all_area_per_tile_arch.append(geomean_area)
                    all_crit_path_delay_arch.append(geomean_delay)

                    table_entry_str = '{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}'.format(
                                    each_arch['name'].ljust(table_ljust_15),
                                    str(each_seed).ljust(table_ljust_10),
                                    str(all_data['clma']['clma_wd']).ljust(table_ljust_10),
                                    str(all_data['clma']['clma_area']).ljust(table_ljust_10),
                                    str(all_data['clma']['clma_delay']).ljust(table_ljust_15),
                                    str(all_data['elliptic']['elliptic_wd']).ljust(table_ljust_15),
                                    str(all_data['elliptic']['elliptic_area']).ljust(table_ljust_15),
                                    str(all_data['elliptic']['elliptic_delay']).ljust(table_ljust_20),
                                    str(all_data['ex1010']['ex1010_wd']).ljust(table_ljust_15),
                                    str(all_data['ex1010']['ex1010_area']).ljust(table_ljust_15),
                                    str(all_data['ex1010']['ex1010_delay']).ljust(table_ljust_15),
                                    str(all_data['frisc']['frisc_wd']).ljust(table_ljust_10),
                                    str(all_data['frisc']['frisc_area']).ljust(table_ljust_10),
                                    str(all_data['frisc']['frisc_delay']).ljust(table_ljust_15),
                                    str(all_data['pdc']['pdc_wd']).ljust(table_ljust_10),
                                    str(all_data['pdc']['pdc_area']).ljust(table_ljust_10),
                                    str(all_data['pdc']['pdc_delay']).ljust(table_ljust_15),
                                    str(all_data['s38417']['s38417_wd']).ljust(table_ljust_15),
                                    str(all_data['s38417']['s38417_area']).ljust(table_ljust_15),
                                    str(all_data['s38417']['s38417_delay']).ljust(table_ljust_15),
                                    str(all_data['s38584.1']['s38584.1_wd']).ljust(table_ljust_15),
                                    str(all_data['s38584.1']['s38584.1_area']).ljust(table_ljust_15),
                                    str(all_data['s38584.1']['s38584.1_delay']).ljust(table_ljust_15),
                                    str(all_data['spla']['spla_wd']).ljust(table_ljust_10),
                                    str(all_data['spla']['spla_area']).ljust(table_ljust_10),
                                    str(all_data['spla']['spla_delay']).ljust(table_ljust_15),
                                    str(geomean_wd).ljust(table_ljust_30),
                                    str(geomean_area).ljust(table_ljust_30),
                                    str(geomean_delay).ljust(table_ljust_30),
                                    )
                    #print(table_entry_str)
                    details_fd.write(table_entry_str)
                    details_fd.write("\n")
                
                geo_mean_area = geo_mean(all_area_per_tile_arch)
                geo_mean_delay = geo_mean(all_crit_path_delay_arch)
                final_print = "Arch: " + each_arch['name'] + ", Min channel width geomean: " + str(geo_mean(all_min_channel_width_arch)) + ", Area per tile geomean: " + str(geo_mean_area) + ", Critical path delay geomean: " + str(geo_mean_delay) + ", Area-delay product: " + str(geo_mean_area*geo_mean_delay) + "\n"
                
                details_fd.write("\n")
                details_fd.write(final_print)
                details_fd.write("\n")
                    
                #print(final_print)
                summary_fd.write(final_print)
                summary_fd.write("\n")
            
if __name__ == "__main__":
    main()




