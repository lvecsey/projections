set terminal png enhanced size 1680,1050
set output 'output_projections.png'
set style fill solid
set boxwidth 0.9
set yrange [0:]
set xtics format ""
set grid ytics
set xlabel "Days"
set ylabel "Revenue with (Costs, Margin) as Profit"

plot "output_projections.dat" using 4 title "Revenue Remaining" with boxes lt rgb "#40FF66", "output_projections.dat" using 3 title "Margin" with boxes lt rgb "#60FF00", "output_projections.dat" using 2:xticlabels(1) title "Costs" with boxes lt rgb "#102030"


     
     
     
