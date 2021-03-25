set terminal post enhanced colour solid font 21 
set encoding iso_8859_2

set key spacing 2 samplen 3
set xlabel "x" 
set grid
x_min = -4.
x_max = 4.
sigma = (x_max-x_min)/16.0
x0 = 2.0
f(x) = sin(14.*pi*x/(x_max-x_min)) * ( exp(-(x-x0)**2/(2.*sigma**2)) + exp(-(x+x0)**2/(2.*sigma**2))  )
m(sr)=10+sr*20
l_width = 5
p_size = 1.3

set style line 1 lt rgb "gray30" lw 6 pt 7 ps p_size
set style line 2 lt rgb "red" lw l_width pt 7 ps p_size
set samples 500

# WYKRES SIEDMIU WIELOMIANOW GRAMA.
# ZALOZENIE: W pliku Gram.dat jest 8 kolumn z danymi
#   uzupelnionymi tak, jak podano na UPeL-u (ad. 3)

set output "Gram.eps"
set ylabel "{/Symbol j}_j(x)"
set key top center outside horizontal
plot [][-1.05:1.05] for [j=2:8] "Gram.dat" u 1:j t "{/Symbol j}_".(j-2)."(x)" w l lw 5


set yrange [-1.5:1.5]
set ylabel "f(x)"
set key inside

# WYKRESY APROKSYMACJI -- PETLA DLA TRZECH ROZNYCH PARAMETROW m
# ZALOZENIA:
#    1. Plik pkt.dat zawiera dane o wezlach w dwoch kolumnach: x_i, y_i.
#    2. Plik approx.dat zawiera wyniki wszystkich trzech aproksymacji
#       w dwoch kolumnach: x_k, F(x_k):
#        - w pierwszej serii danych dla m = 10,
#        - po dwoch pustych liniach druga seria danych: dla m = 30,
#        - po dwoch pustych liniach trzecia seria danych: dla m = 50.

do for [sr=0:2]{
  set output "approx_".m(sr).".eps"
  plot f(x) ls 1 t "f(x)", "pkt.dat" pt 7 t "f_{szum}(x)",\
    "approx.dat" i sr w l ls 2 t "F(x), m=".m(sr).""
}

