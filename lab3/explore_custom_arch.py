import os

ljust_lutram_percentage = 15
ljust_lb_per_lutram = 15
ljust_size = 12
ljust_width = 12
ljust_ratio = 12
ljust_geomean = 20

all_tests = []

tests_lutram_50 = []
# Section 1
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 16, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})  # Best
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 16, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
# Stratix-IV
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 16, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})

tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})

tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 32, 'ratio_2': 150})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 32, 'ratio_2': 150})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 64, 'ratio_2': 150})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 64, 'ratio_2': 150})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 128, 'ratio_2': 150})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 128, 'ratio_2': 150})

# Section 2 // Best from section 1 but vary BRAM1 & BRAM2 ratio
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 5, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 100})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 5, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 150})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 5, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 200})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 5, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 250})

tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 100})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 150})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 200})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 250})

tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 100})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 150})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 200})
tests_lutram_50.append({'lb_per_lutram': 2, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 250})

tests_lutram_33 = []
# Section 1 // Repeat tests_lutram_50's section 1 with 33%
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300}) # Best
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 16, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 16, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 16, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})

tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})

tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 32, 'ratio_2': 150})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 32, 'ratio_2': 150})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 64, 'ratio_2': 150})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 64, 'ratio_2': 150})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 128, 'ratio_2': 150})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 128, 'ratio_2': 150})

# Section 2 // Best from section 1 but vary various
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})

tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 100})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 150}) # Best
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 200})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 250})

tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 100})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 150})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 200})
tests_lutram_33.append({'lb_per_lutram': 3, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 250})

tests_lutram_25 = []
# Section 1 // Repeat tests_lutram_50's section 1 with 25%
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300}) # Best
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 16, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 16, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 16, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})

tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})

tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 32, 'ratio_2': 150})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 32, 'ratio_2': 150})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 64, 'ratio_2': 150})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 64, 'ratio_2': 150})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 128, 'ratio_2': 150})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 128, 'ratio_2': 150})

# Section 2 // Best from section 1 but vary various
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})

tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 100})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 150}) # Best
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 200})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 250})

tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 100})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 150})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 200})
tests_lutram_25.append({'lb_per_lutram': 4, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 250})

tests_lutram_10 = []
# Section 1 // Repeat tests_lutram_50's section 1 with 10%
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 16, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 16, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300}) # Best
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 16, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})

tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 32, 'ratio_2': 300})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 300})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 300})

tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 32, 'ratio_2': 150})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 32, 'ratio_2': 150})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 64, 'ratio_2': 150})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 64, 'ratio_2': 150})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 32, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 128, 'ratio_2': 150})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 32, 'max_width_1': 64, 'ratio_1': 10, 'max_bits_K_2': 64, 'max_width_2': 128, 'ratio_2': 150})

# Section 2 // Best from section 1 but vary various
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 100})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 150}) # Best
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 200})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 250})

tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 100})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 150})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 200})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 10, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 250})

tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 100})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 150})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 200})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 128, 'ratio_2': 250})

tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 100})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 150})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 200})
tests_lutram_10.append({'lb_per_lutram': 10, 'max_bits_K_1': 8, 'max_width_1': 32, 'ratio_1': 25, 'max_bits_K_2': 128, 'max_width_2': 64, 'ratio_2': 250})

all_tests.append(tests_lutram_50)
all_tests.append(tests_lutram_33)
all_tests.append(tests_lutram_25)
all_tests.append(tests_lutram_10)

all_final_prints = []

with open('explore_details_custom', 'w+') as explore_details_fd:
    for each_test_set in all_tests:
        
        lowest_area = 10e10
        lowest_area_test_case = None

        table_header_str = '{}{}{}{}{}{}{}{}{}'.format(
            'LUTRAM %'.ljust(ljust_lutram_percentage),
            'LBs / LUTRAM'.ljust(ljust_lb_per_lutram),
            'BRAM_Size_1'.ljust(ljust_size),
            'Max Width_1'.ljust(ljust_width),
            'LBs/BRAM_1'.ljust(ljust_ratio),
            'BRAM_Size_2'.ljust(ljust_size),
            'Max_Width_2'.ljust(ljust_width),
            'LBs/BRAM_2'.ljust(ljust_ratio),
            'Geomean Area (MWTA)'.ljust(ljust_geomean),
            )
        print(table_header_str)
        explore_details_fd.write(table_header_str)
        explore_details_fd.write("\n")

        for each_test in each_test_set:
                
            run_mapper_cmd = './mapper 4 ' + str(each_test['lb_per_lutram']) + ' ' + str(each_test['max_bits_K_1']) + ' ' + str(each_test['max_width_1']) + ' ' + str(each_test['ratio_1']) + ' ' + str(each_test['max_bits_K_2']) + ' ' + str(each_test['max_width_2']) + ' ' + str(each_test['ratio_2']) + ' > temp'
            run_checker_cmd = './checker -t -l ' + str(each_test['lb_per_lutram']-1) + ' 1 -b ' + str(each_test['max_bits_K_1']*1024) + ' ' + str(each_test['max_width_1']) + ' ' + str(each_test['ratio_1']) + ' 1 -b ' + str(each_test['max_bits_K_2']*1024) + ' ' + str(each_test['max_width_2']) + ' ' + str(each_test['ratio_2']) + ' 1 logical_rams.txt logic_block_count.txt temp > temp2'
            
            os.system(run_mapper_cmd)
            os.system(run_checker_cmd)
            
            geomean_area = ''
            with open('temp2', 'r') as f:
                lines = f.read().splitlines()
                geomean_area = lines[-1].split()[-1]
            
            lutram_percentage = 1.0/float(each_test['lb_per_lutram'])*100
            size_str_1 = str(each_test['max_bits_K_1']) + "K"
            size_str_2 = str(each_test['max_bits_K_2']) + "K"
            table_entry_str = '{}{}{}{}{}{}{}{}{}'.format(
                str(lutram_percentage).ljust(ljust_lutram_percentage),
                str(each_test['lb_per_lutram']).ljust(ljust_lb_per_lutram),
                size_str_1.ljust(ljust_size),
                str(each_test['max_width_1']).ljust(ljust_width),
                str(each_test['ratio_1']).ljust(ljust_ratio),
                size_str_2.ljust(ljust_size),
                str(each_test['max_width_2']).ljust(ljust_width),
                str(each_test['ratio_2']).ljust(ljust_ratio),
                geomean_area.ljust(ljust_geomean),
                )
            print(table_entry_str)
            explore_details_fd.write(table_entry_str)
            explore_details_fd.write("\n")
            
            if float(geomean_area) < lowest_area:
                lowest_area = float(geomean_area)
                lowest_area_test_case = each_test
        
        final_print = 'Best test case: LUTRAM: ' + str(1.0/float(lowest_area_test_case['lb_per_lutram'])*100) + '%, LBs/LUTRAM: ' + str(lowest_area_test_case['lb_per_lutram']) + ', Size_1: ' + str(lowest_area_test_case['max_bits_K_1']) + "K" + ', Width_1: ' + str(lowest_area_test_case['max_width_1']) + ', Ratio_1: ' + str(lowest_area_test_case['ratio_1']) + ', Size_2: ' + str(lowest_area_test_case['max_bits_K_2']) + "K" + ', Width_2: ' + str(lowest_area_test_case['max_width_2']) + ', Ratio_2: ' + str(lowest_area_test_case['ratio_2']) + ', With geomean area: ' + str(lowest_area)
        all_final_prints.append(final_print)
        
        print("")
        explore_details_fd.write("\n")
            

with open('explore_summary_custom', 'w+') as explore_summary_fd:
    for each_final_print in all_final_prints:
        print(each_final_print)
        explore_summary_fd.write(each_final_print)
        explore_summary_fd.write("\n")
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            