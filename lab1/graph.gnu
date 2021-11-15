set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 600, 400 
set output 'graph.png'
set key bmargin center horizontal Right noreverse enhanced autotitle box lt black linewidth 1.000 dashtype solid
set style increment default
set style data lines
set title "ECE1756 Lab1: Exact vs. Approximated Exponential Function" 
set xrange [ 0 : 4 ] noreverse nowriteback
set x2range [ * : * ] noreverse writeback
set yrange [ * : * ] noreverse writeback
set y2range [ * : * ] noreverse writeback
set zrange [ * : * ] noreverse writeback
set cbrange [ * : * ] noreverse writeback
set rrange [ * : * ] noreverse writeback
## Last datafile plotted: "using.dat"
plot 'exp.txt' using 1:2 title "exp(x)" with lines lw 2 lc rgb "blue", 'testcase.txt' using 1:2 t "Hardware Output" with points ps 1 lw 2 lc rgb "black"
