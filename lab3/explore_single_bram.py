import os

ljust_lutram_option = 15
ljust_size = 15
ljust_width = 15
ljust_ratio = 15
ljust_geomean = 45

all_tests = []

tests_1k = []
tests_1k.append({'max_bits_K': 1, 'max_width': 2, 'ratio': 10})
tests_1k.append({'max_bits_K': 1, 'max_width': 4, 'ratio': 10})
tests_1k.append({'max_bits_K': 1, 'max_width': 8, 'ratio': 10})
tests_1k.append({'max_bits_K': 1, 'max_width': 16, 'ratio': 10})
tests_1k.append({'max_bits_K': 1, 'max_width': 32, 'ratio': 10})
tests_1k.append({'max_bits_K': 1, 'max_width': 64, 'ratio': 10})
tests_1k.append({'max_bits_K': 1, 'max_width': 128, 'ratio': 10})
tests_1k.append({'max_bits_K': 1, 'max_width': 2, 'ratio': 25})
tests_1k.append({'max_bits_K': 1, 'max_width': 4, 'ratio': 25})
tests_1k.append({'max_bits_K': 1, 'max_width': 8, 'ratio': 25})
tests_1k.append({'max_bits_K': 1, 'max_width': 16, 'ratio': 25})
tests_1k.append({'max_bits_K': 1, 'max_width': 32, 'ratio': 25})
tests_1k.append({'max_bits_K': 1, 'max_width': 64, 'ratio': 25})
tests_1k.append({'max_bits_K': 1, 'max_width': 128, 'ratio': 25})
tests_1k.append({'max_bits_K': 1, 'max_width': 2, 'ratio': 50})
tests_1k.append({'max_bits_K': 1, 'max_width': 4, 'ratio': 50})
tests_1k.append({'max_bits_K': 1, 'max_width': 8, 'ratio': 50})
tests_1k.append({'max_bits_K': 1, 'max_width': 16, 'ratio': 50})
tests_1k.append({'max_bits_K': 1, 'max_width': 32, 'ratio': 50})
tests_1k.append({'max_bits_K': 1, 'max_width': 64, 'ratio': 50})
tests_1k.append({'max_bits_K': 1, 'max_width': 128, 'ratio': 50})
tests_1k.append({'max_bits_K': 1, 'max_width': 2, 'ratio': 100})
tests_1k.append({'max_bits_K': 1, 'max_width': 4, 'ratio': 100})
tests_1k.append({'max_bits_K': 1, 'max_width': 8, 'ratio': 100})
tests_1k.append({'max_bits_K': 1, 'max_width': 16, 'ratio': 100})
tests_1k.append({'max_bits_K': 1, 'max_width': 32, 'ratio': 100})
tests_1k.append({'max_bits_K': 1, 'max_width': 64, 'ratio': 100})
tests_1k.append({'max_bits_K': 1, 'max_width': 128, 'ratio': 100})


tests_2k = []
tests_2k.append({'max_bits_K': 2, 'max_width': 2, 'ratio': 10})
tests_2k.append({'max_bits_K': 2, 'max_width': 4, 'ratio': 10})
tests_2k.append({'max_bits_K': 2, 'max_width': 8, 'ratio': 10})
tests_2k.append({'max_bits_K': 2, 'max_width': 16, 'ratio': 10})
tests_2k.append({'max_bits_K': 2, 'max_width': 32, 'ratio': 10})
tests_2k.append({'max_bits_K': 2, 'max_width': 64, 'ratio': 10})
tests_2k.append({'max_bits_K': 2, 'max_width': 128, 'ratio': 10})
tests_2k.append({'max_bits_K': 2, 'max_width': 2, 'ratio': 25})
tests_2k.append({'max_bits_K': 2, 'max_width': 4, 'ratio': 25})
tests_2k.append({'max_bits_K': 2, 'max_width': 8, 'ratio': 25})
tests_2k.append({'max_bits_K': 2, 'max_width': 16, 'ratio': 25})
tests_2k.append({'max_bits_K': 2, 'max_width': 32, 'ratio': 25})
tests_2k.append({'max_bits_K': 2, 'max_width': 64, 'ratio': 25})
tests_2k.append({'max_bits_K': 2, 'max_width': 128, 'ratio': 25})
tests_2k.append({'max_bits_K': 2, 'max_width': 2, 'ratio': 50})
tests_2k.append({'max_bits_K': 2, 'max_width': 4, 'ratio': 50})
tests_2k.append({'max_bits_K': 2, 'max_width': 8, 'ratio': 50})
tests_2k.append({'max_bits_K': 2, 'max_width': 16, 'ratio': 50})
tests_2k.append({'max_bits_K': 2, 'max_width': 32, 'ratio': 50})
tests_2k.append({'max_bits_K': 2, 'max_width': 64, 'ratio': 50})
tests_2k.append({'max_bits_K': 2, 'max_width': 128, 'ratio': 50})
tests_2k.append({'max_bits_K': 2, 'max_width': 2, 'ratio': 100})
tests_2k.append({'max_bits_K': 2, 'max_width': 4, 'ratio': 100})
tests_2k.append({'max_bits_K': 2, 'max_width': 8, 'ratio': 100})
tests_2k.append({'max_bits_K': 2, 'max_width': 16, 'ratio': 100})
tests_2k.append({'max_bits_K': 2, 'max_width': 32, 'ratio': 100})
tests_2k.append({'max_bits_K': 2, 'max_width': 64, 'ratio': 100})
tests_2k.append({'max_bits_K': 2, 'max_width': 128, 'ratio': 100})

tests_4k = []
tests_4k.append({'max_bits_K': 4, 'max_width': 2, 'ratio': 10})
tests_4k.append({'max_bits_K': 4, 'max_width': 4, 'ratio': 10})
tests_4k.append({'max_bits_K': 4, 'max_width': 8, 'ratio': 10})
tests_4k.append({'max_bits_K': 4, 'max_width': 16, 'ratio': 10})
tests_4k.append({'max_bits_K': 4, 'max_width': 32, 'ratio': 10})
tests_4k.append({'max_bits_K': 4, 'max_width': 64, 'ratio': 10})
tests_4k.append({'max_bits_K': 4, 'max_width': 128, 'ratio': 10})
tests_4k.append({'max_bits_K': 4, 'max_width': 2, 'ratio': 25})
tests_4k.append({'max_bits_K': 4, 'max_width': 4, 'ratio': 25})
tests_4k.append({'max_bits_K': 4, 'max_width': 8, 'ratio': 25})
tests_4k.append({'max_bits_K': 4, 'max_width': 16, 'ratio': 25})
tests_4k.append({'max_bits_K': 4, 'max_width': 32, 'ratio': 25})
tests_4k.append({'max_bits_K': 4, 'max_width': 64, 'ratio': 25})
tests_4k.append({'max_bits_K': 4, 'max_width': 128, 'ratio': 25})
tests_4k.append({'max_bits_K': 4, 'max_width': 2, 'ratio': 50})
tests_4k.append({'max_bits_K': 4, 'max_width': 4, 'ratio': 50})
tests_4k.append({'max_bits_K': 4, 'max_width': 8, 'ratio': 50})
tests_4k.append({'max_bits_K': 4, 'max_width': 16, 'ratio': 50})
tests_4k.append({'max_bits_K': 4, 'max_width': 32, 'ratio': 50})
tests_4k.append({'max_bits_K': 4, 'max_width': 64, 'ratio': 50})
tests_4k.append({'max_bits_K': 4, 'max_width': 128, 'ratio': 50})
tests_4k.append({'max_bits_K': 4, 'max_width': 2, 'ratio': 100})
tests_4k.append({'max_bits_K': 4, 'max_width': 4, 'ratio': 100})
tests_4k.append({'max_bits_K': 4, 'max_width': 8, 'ratio': 100})
tests_4k.append({'max_bits_K': 4, 'max_width': 16, 'ratio': 100})
tests_4k.append({'max_bits_K': 4, 'max_width': 32, 'ratio': 100})
tests_4k.append({'max_bits_K': 4, 'max_width': 64, 'ratio': 100})
tests_4k.append({'max_bits_K': 4, 'max_width': 128, 'ratio': 100})

tests_8k = []
tests_8k.append({'max_bits_K': 8, 'max_width': 2, 'ratio': 10})
tests_8k.append({'max_bits_K': 8, 'max_width': 4, 'ratio': 10})
tests_8k.append({'max_bits_K': 8, 'max_width': 8, 'ratio': 10})
tests_8k.append({'max_bits_K': 8, 'max_width': 16, 'ratio': 10})
tests_8k.append({'max_bits_K': 8, 'max_width': 32, 'ratio': 10})
tests_8k.append({'max_bits_K': 8, 'max_width': 64, 'ratio': 10})
tests_8k.append({'max_bits_K': 8, 'max_width': 128, 'ratio': 10})
tests_8k.append({'max_bits_K': 8, 'max_width': 2, 'ratio': 25})
tests_8k.append({'max_bits_K': 8, 'max_width': 4, 'ratio': 25})
tests_8k.append({'max_bits_K': 8, 'max_width': 8, 'ratio': 25})
tests_8k.append({'max_bits_K': 8, 'max_width': 16, 'ratio': 25})
tests_8k.append({'max_bits_K': 8, 'max_width': 32, 'ratio': 25})
tests_8k.append({'max_bits_K': 8, 'max_width': 64, 'ratio': 25})
tests_8k.append({'max_bits_K': 8, 'max_width': 128, 'ratio': 25})
tests_8k.append({'max_bits_K': 8, 'max_width': 2, 'ratio': 50})
tests_8k.append({'max_bits_K': 8, 'max_width': 4, 'ratio': 50})
tests_8k.append({'max_bits_K': 8, 'max_width': 8, 'ratio': 50})
tests_8k.append({'max_bits_K': 8, 'max_width': 16, 'ratio': 50})
tests_8k.append({'max_bits_K': 8, 'max_width': 32, 'ratio': 50})
tests_8k.append({'max_bits_K': 8, 'max_width': 64, 'ratio': 50})
tests_8k.append({'max_bits_K': 8, 'max_width': 128, 'ratio': 50})
tests_8k.append({'max_bits_K': 8, 'max_width': 2, 'ratio': 100})
tests_8k.append({'max_bits_K': 8, 'max_width': 4, 'ratio': 100})
tests_8k.append({'max_bits_K': 8, 'max_width': 8, 'ratio': 100})
tests_8k.append({'max_bits_K': 8, 'max_width': 16, 'ratio': 100})
tests_8k.append({'max_bits_K': 8, 'max_width': 32, 'ratio': 100})
tests_8k.append({'max_bits_K': 8, 'max_width': 64, 'ratio': 100})
tests_8k.append({'max_bits_K': 8, 'max_width': 128, 'ratio': 100})

tests_16k = []
tests_16k.append({'max_bits_K': 16, 'max_width': 2, 'ratio': 10})
tests_16k.append({'max_bits_K': 16, 'max_width': 4, 'ratio': 10})
tests_16k.append({'max_bits_K': 16, 'max_width': 8, 'ratio': 10})
tests_16k.append({'max_bits_K': 16, 'max_width': 16, 'ratio': 10})
tests_16k.append({'max_bits_K': 16, 'max_width': 32, 'ratio': 10})
tests_16k.append({'max_bits_K': 16, 'max_width': 64, 'ratio': 10})
tests_16k.append({'max_bits_K': 16, 'max_width': 128, 'ratio': 10})
tests_16k.append({'max_bits_K': 16, 'max_width': 2, 'ratio': 25})
tests_16k.append({'max_bits_K': 16, 'max_width': 4, 'ratio': 25})
tests_16k.append({'max_bits_K': 16, 'max_width': 8, 'ratio': 25})
tests_16k.append({'max_bits_K': 16, 'max_width': 16, 'ratio': 25})
tests_16k.append({'max_bits_K': 16, 'max_width': 32, 'ratio': 25})
tests_16k.append({'max_bits_K': 16, 'max_width': 64, 'ratio': 25})
tests_16k.append({'max_bits_K': 16, 'max_width': 128, 'ratio': 25})
tests_16k.append({'max_bits_K': 16, 'max_width': 2, 'ratio': 50})
tests_16k.append({'max_bits_K': 16, 'max_width': 4, 'ratio': 50})
tests_16k.append({'max_bits_K': 16, 'max_width': 8, 'ratio': 50})
tests_16k.append({'max_bits_K': 16, 'max_width': 16, 'ratio': 50})
tests_16k.append({'max_bits_K': 16, 'max_width': 32, 'ratio': 50})
tests_16k.append({'max_bits_K': 16, 'max_width': 64, 'ratio': 50})
tests_16k.append({'max_bits_K': 16, 'max_width': 128, 'ratio': 50})
tests_16k.append({'max_bits_K': 16, 'max_width': 2, 'ratio': 100})
tests_16k.append({'max_bits_K': 16, 'max_width': 4, 'ratio': 100})
tests_16k.append({'max_bits_K': 16, 'max_width': 8, 'ratio': 100})
tests_16k.append({'max_bits_K': 16, 'max_width': 16, 'ratio': 100})
tests_16k.append({'max_bits_K': 16, 'max_width': 32, 'ratio': 100})
tests_16k.append({'max_bits_K': 16, 'max_width': 64, 'ratio': 100})
tests_16k.append({'max_bits_K': 16, 'max_width': 128, 'ratio': 100})

tests_32k = []
tests_32k.append({'max_bits_K': 32, 'max_width': 2, 'ratio': 10})
tests_32k.append({'max_bits_K': 32, 'max_width': 4, 'ratio': 10})
tests_32k.append({'max_bits_K': 32, 'max_width': 8, 'ratio': 10})
tests_32k.append({'max_bits_K': 32, 'max_width': 16, 'ratio': 10})
tests_32k.append({'max_bits_K': 32, 'max_width': 32, 'ratio': 10})
tests_32k.append({'max_bits_K': 32, 'max_width': 64, 'ratio': 10})
tests_32k.append({'max_bits_K': 32, 'max_width': 128, 'ratio': 10})
tests_32k.append({'max_bits_K': 32, 'max_width': 2, 'ratio': 25})
tests_32k.append({'max_bits_K': 32, 'max_width': 4, 'ratio': 25})
tests_32k.append({'max_bits_K': 32, 'max_width': 8, 'ratio': 25})
tests_32k.append({'max_bits_K': 32, 'max_width': 16, 'ratio': 25})
tests_32k.append({'max_bits_K': 32, 'max_width': 32, 'ratio': 25})
tests_32k.append({'max_bits_K': 32, 'max_width': 64, 'ratio': 25})
tests_32k.append({'max_bits_K': 32, 'max_width': 128, 'ratio': 25})
tests_32k.append({'max_bits_K': 32, 'max_width': 2, 'ratio': 50})
tests_32k.append({'max_bits_K': 32, 'max_width': 4, 'ratio': 50})
tests_32k.append({'max_bits_K': 32, 'max_width': 8, 'ratio': 50})
tests_32k.append({'max_bits_K': 32, 'max_width': 16, 'ratio': 50})
tests_32k.append({'max_bits_K': 32, 'max_width': 32, 'ratio': 50})
tests_32k.append({'max_bits_K': 32, 'max_width': 64, 'ratio': 50})
tests_32k.append({'max_bits_K': 32, 'max_width': 128, 'ratio': 50})
tests_32k.append({'max_bits_K': 32, 'max_width': 2, 'ratio': 100})
tests_32k.append({'max_bits_K': 32, 'max_width': 4, 'ratio': 100})
tests_32k.append({'max_bits_K': 32, 'max_width': 8, 'ratio': 100})
tests_32k.append({'max_bits_K': 32, 'max_width': 16, 'ratio': 100})
tests_32k.append({'max_bits_K': 32, 'max_width': 32, 'ratio': 100})
tests_32k.append({'max_bits_K': 32, 'max_width': 64, 'ratio': 100})
tests_32k.append({'max_bits_K': 32, 'max_width': 128, 'ratio': 100})
tests_32k.append({'max_bits_K': 32, 'max_width': 2, 'ratio': 150})
tests_32k.append({'max_bits_K': 32, 'max_width': 4, 'ratio': 150})
tests_32k.append({'max_bits_K': 32, 'max_width': 8, 'ratio': 150})
tests_32k.append({'max_bits_K': 32, 'max_width': 16, 'ratio': 150})
tests_32k.append({'max_bits_K': 32, 'max_width': 32, 'ratio': 150})
tests_32k.append({'max_bits_K': 32, 'max_width': 64, 'ratio': 150})
tests_32k.append({'max_bits_K': 32, 'max_width': 128, 'ratio': 150})

tests_64k = []
tests_64k.append({'max_bits_K': 64, 'max_width': 2, 'ratio': 10})
tests_64k.append({'max_bits_K': 64, 'max_width': 4, 'ratio': 10})
tests_64k.append({'max_bits_K': 64, 'max_width': 8, 'ratio': 10})
tests_64k.append({'max_bits_K': 64, 'max_width': 16, 'ratio': 10})
tests_64k.append({'max_bits_K': 64, 'max_width': 32, 'ratio': 10})
tests_64k.append({'max_bits_K': 64, 'max_width': 64, 'ratio': 10})
tests_64k.append({'max_bits_K': 64, 'max_width': 128, 'ratio': 10})
tests_64k.append({'max_bits_K': 64, 'max_width': 2, 'ratio': 25})
tests_64k.append({'max_bits_K': 64, 'max_width': 4, 'ratio': 25})
tests_64k.append({'max_bits_K': 64, 'max_width': 8, 'ratio': 25})
tests_64k.append({'max_bits_K': 64, 'max_width': 16, 'ratio': 25})
tests_64k.append({'max_bits_K': 64, 'max_width': 32, 'ratio': 25})
tests_64k.append({'max_bits_K': 64, 'max_width': 64, 'ratio': 25})
tests_64k.append({'max_bits_K': 64, 'max_width': 128, 'ratio': 25})
tests_64k.append({'max_bits_K': 64, 'max_width': 2, 'ratio': 50})
tests_64k.append({'max_bits_K': 64, 'max_width': 4, 'ratio': 50})
tests_64k.append({'max_bits_K': 64, 'max_width': 8, 'ratio': 50})
tests_64k.append({'max_bits_K': 64, 'max_width': 16, 'ratio': 50})
tests_64k.append({'max_bits_K': 64, 'max_width': 32, 'ratio': 50})
tests_64k.append({'max_bits_K': 64, 'max_width': 64, 'ratio': 50})
tests_64k.append({'max_bits_K': 64, 'max_width': 128, 'ratio': 50})
tests_64k.append({'max_bits_K': 64, 'max_width': 2, 'ratio': 100})
tests_64k.append({'max_bits_K': 64, 'max_width': 4, 'ratio': 100})
tests_64k.append({'max_bits_K': 64, 'max_width': 8, 'ratio': 100})
tests_64k.append({'max_bits_K': 64, 'max_width': 16, 'ratio': 100})
tests_64k.append({'max_bits_K': 64, 'max_width': 32, 'ratio': 100})
tests_64k.append({'max_bits_K': 64, 'max_width': 64, 'ratio': 100})
tests_64k.append({'max_bits_K': 64, 'max_width': 128, 'ratio': 100})
tests_64k.append({'max_bits_K': 64, 'max_width': 2, 'ratio': 150})
tests_64k.append({'max_bits_K': 64, 'max_width': 4, 'ratio': 150})
tests_64k.append({'max_bits_K': 64, 'max_width': 8, 'ratio': 150})
tests_64k.append({'max_bits_K': 64, 'max_width': 16, 'ratio': 150})
tests_64k.append({'max_bits_K': 64, 'max_width': 32, 'ratio': 150})
tests_64k.append({'max_bits_K': 64, 'max_width': 64, 'ratio': 150})
tests_64k.append({'max_bits_K': 64, 'max_width': 128, 'ratio': 150})
tests_64k.append({'max_bits_K': 64, 'max_width': 2, 'ratio': 200})
tests_64k.append({'max_bits_K': 64, 'max_width': 4, 'ratio': 200})
tests_64k.append({'max_bits_K': 64, 'max_width': 8, 'ratio': 200})
tests_64k.append({'max_bits_K': 64, 'max_width': 16, 'ratio': 200})
tests_64k.append({'max_bits_K': 64, 'max_width': 32, 'ratio': 200})
tests_64k.append({'max_bits_K': 64, 'max_width': 64, 'ratio': 200})
tests_64k.append({'max_bits_K': 64, 'max_width': 128, 'ratio': 200})

tests_128k = []
tests_128k.append({'max_bits_K': 128, 'max_width': 2, 'ratio': 10})
tests_128k.append({'max_bits_K': 128, 'max_width': 4, 'ratio': 10})
tests_128k.append({'max_bits_K': 128, 'max_width': 8, 'ratio': 10})
tests_128k.append({'max_bits_K': 128, 'max_width': 16, 'ratio': 10})
tests_128k.append({'max_bits_K': 128, 'max_width': 32, 'ratio': 10})
tests_128k.append({'max_bits_K': 128, 'max_width': 64, 'ratio': 10})
tests_128k.append({'max_bits_K': 128, 'max_width': 128, 'ratio': 10})
tests_128k.append({'max_bits_K': 128, 'max_width': 2, 'ratio': 25})
tests_128k.append({'max_bits_K': 128, 'max_width': 4, 'ratio': 25})
tests_128k.append({'max_bits_K': 128, 'max_width': 8, 'ratio': 25})
tests_128k.append({'max_bits_K': 128, 'max_width': 16, 'ratio': 25})
tests_128k.append({'max_bits_K': 128, 'max_width': 32, 'ratio': 25})
tests_128k.append({'max_bits_K': 128, 'max_width': 64, 'ratio': 25})
tests_128k.append({'max_bits_K': 128, 'max_width': 128, 'ratio': 25})
tests_128k.append({'max_bits_K': 128, 'max_width': 2, 'ratio': 50})
tests_128k.append({'max_bits_K': 128, 'max_width': 4, 'ratio': 50})
tests_128k.append({'max_bits_K': 128, 'max_width': 8, 'ratio': 50})
tests_128k.append({'max_bits_K': 128, 'max_width': 16, 'ratio': 50})
tests_128k.append({'max_bits_K': 128, 'max_width': 32, 'ratio': 50})
tests_128k.append({'max_bits_K': 128, 'max_width': 64, 'ratio': 50})
tests_128k.append({'max_bits_K': 128, 'max_width': 128, 'ratio': 50})
tests_128k.append({'max_bits_K': 128, 'max_width': 2, 'ratio': 100})
tests_128k.append({'max_bits_K': 128, 'max_width': 4, 'ratio': 100})
tests_128k.append({'max_bits_K': 128, 'max_width': 8, 'ratio': 100})
tests_128k.append({'max_bits_K': 128, 'max_width': 16, 'ratio': 100})
tests_128k.append({'max_bits_K': 128, 'max_width': 32, 'ratio': 100})
tests_128k.append({'max_bits_K': 128, 'max_width': 64, 'ratio': 100})
tests_128k.append({'max_bits_K': 128, 'max_width': 128, 'ratio': 100})
tests_128k.append({'max_bits_K': 128, 'max_width': 2, 'ratio': 150})
tests_128k.append({'max_bits_K': 128, 'max_width': 4, 'ratio': 150})
tests_128k.append({'max_bits_K': 128, 'max_width': 8, 'ratio': 150})
tests_128k.append({'max_bits_K': 128, 'max_width': 16, 'ratio': 150})
tests_128k.append({'max_bits_K': 128, 'max_width': 32, 'ratio': 150})
tests_128k.append({'max_bits_K': 128, 'max_width': 64, 'ratio': 150})
tests_128k.append({'max_bits_K': 128, 'max_width': 128, 'ratio': 150})
tests_128k.append({'max_bits_K': 128, 'max_width': 2, 'ratio': 200})
tests_128k.append({'max_bits_K': 128, 'max_width': 4, 'ratio': 200})
tests_128k.append({'max_bits_K': 128, 'max_width': 8, 'ratio': 200})
tests_128k.append({'max_bits_K': 128, 'max_width': 16, 'ratio': 200})
tests_128k.append({'max_bits_K': 128, 'max_width': 32, 'ratio': 200})
tests_128k.append({'max_bits_K': 128, 'max_width': 64, 'ratio': 200})
tests_128k.append({'max_bits_K': 128, 'max_width': 128, 'ratio': 200})
tests_128k.append({'max_bits_K': 128, 'max_width': 2, 'ratio': 300})
tests_128k.append({'max_bits_K': 128, 'max_width': 4, 'ratio': 300})
tests_128k.append({'max_bits_K': 128, 'max_width': 8, 'ratio': 300})
tests_128k.append({'max_bits_K': 128, 'max_width': 16, 'ratio': 300})
tests_128k.append({'max_bits_K': 128, 'max_width': 32, 'ratio': 300})
tests_128k.append({'max_bits_K': 128, 'max_width': 64, 'ratio': 300})
tests_128k.append({'max_bits_K': 128, 'max_width': 128, 'ratio': 300})

all_tests.append(tests_1k)
all_tests.append(tests_2k)
all_tests.append(tests_4k)
all_tests.append(tests_8k)
all_tests.append(tests_16k)
all_tests.append(tests_32k)
all_tests.append(tests_64k)
all_tests.append(tests_128k)

all_final_prints = []

with open('explore_details', 'w+') as explore_details_fd:
    for use_lutram in [False, True]:
        for each_test_set in all_tests:
            
            lowest_area = 10e10
            lowest_area_test_case = None

            table_header_str = '{}{}{}{}{}'.format(
                'Use LUTRAM'.ljust(ljust_lutram_option),
                'BRAM Size'.ljust(ljust_size),
                'Max Width'.ljust(ljust_width),
                'LBs/BRAM'.ljust(ljust_ratio),
                'Geomean Area (MWTA)'.ljust(ljust_geomean),
                )
            print(table_header_str)
            explore_details_fd.write(table_header_str)
            explore_details_fd.write("\n")

            for each_test in each_test_set:
                lutram_option = '2'
                if use_lutram:
                    lutram_option = '3'
                    
                run_mapper_cmd = './mapper ' + lutram_option + ' ' + str(each_test['max_bits_K']) + ' ' + str(each_test['max_width']) + ' ' + str(each_test['ratio']) + ' > temp'
                run_checker_cmd = './checker -t -l 1 1 -b ' + str(each_test['max_bits_K']*1024) + ' ' + str(each_test['max_width']) + ' ' + str(each_test['ratio']) + ' 1 logical_rams.txt logic_block_count.txt temp > temp2'
                
                os.system(run_mapper_cmd)
                os.system(run_checker_cmd)
                
                geomean_area = ''
                with open('temp2', 'r') as f:
                    lines = f.read().splitlines()
                    geomean_area = lines[-1].split()[-1]
                
                lutram_str = 'False'
                if use_lutram:
                    lutram_str = 'True'
                size_str = str(each_test['max_bits_K']) + "K"
                table_entry_str = '{}{}{}{}{}'.format(
                    lutram_str.ljust(ljust_lutram_option),
                    size_str.ljust(ljust_size),
                    str(each_test['max_width']).ljust(ljust_width),
                    str(each_test['ratio']).ljust(ljust_ratio),
                    geomean_area.ljust(ljust_geomean),
                    )
                print(table_entry_str)
                explore_details_fd.write(table_entry_str)
                explore_details_fd.write("\n")
                
                if float(geomean_area) < lowest_area:
                    lowest_area = float(geomean_area)
                    lowest_area_test_case = each_test
            
            final_print = 'Best test case: LUTRAM: ' + lutram_str + ', Size:' + str(lowest_area_test_case['max_bits_K']) + "K" + ', Width: ' + str(lowest_area_test_case['max_width']) + ', Ratio: ' + str(lowest_area_test_case['ratio']) + ', With geomean area: ' + str(lowest_area)
            all_final_prints.append(final_print)
            
            print("")
            explore_details_fd.write("\n")
            

with open('explore_summary', 'w+') as explore_summary_fd:
    for each_final_print in all_final_prints:
        print(each_final_print)
        explore_summary_fd.write(each_final_print)
        explore_summary_fd.write("\n")
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            