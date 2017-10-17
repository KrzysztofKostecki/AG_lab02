set term png size 1200,720
set out 'result.png'
set logscale x
set xlabel "Wielkosc populacji"
set ylabel "Liczba schematów"
set yrange [0:850]
set ytics add ("3^6" 729)
plot 'data.dat' w l t "Liczba reprezentowanych schematów", 729 w l t "Max liczba schematów"