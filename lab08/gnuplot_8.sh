set terminal post enhanced colour solid font 21 

set key spacing 2 samplen 3
set samples 200
set xlabel "x" 
set ylabel "f(x)"
set grid
f1(x) = 1./(1.+x**2)
f2(x) = cos(2.*x)
l_width = 5
p_size = 1.3

set style line 1 lt rgb "gray30" lw 6 pt 7 ps p_size
set style line 2 lt rgb "royalblue" lw l_width pt 7 ps p_size*1.2

array n[3]; n[1] = 5; n[2] = 8; n[3] = 21

# PETLA DLA FUNKCJI f1.
# ZALOZENIE: W pliku f1.dat znajduja sie dane w dwoch kolumnach (x oraz s(x)). 
# Pierwsza seria danych: dla n=5, nastepnie dwie puste linie,
# druga seria danych (dla n=8), dwie puste linie, itd.

do for [i=0:2]{
    set output "f1_n".n[i+1].".eps" 
    plot [-5:5][] f1(x) w l ls 1 t "f_1(x) = ^1/_{(1+x^2)}",\
      "f1.dat" i i u 1:2 w l ls 2 t "s(x), n = ".n[i+1].""
}

# PETLA DLA FUNKCJI f2.
# ZALOZENIE: W pliku f2.dat znajduja sie dane w dwoch kolumnach (x oraz s(x)). 
# Pierwsza seria danych: dla n=5, nastepnie dwie puste linie,
# druga seria danych (dla n=8), dwie puste linie, itd.

set ytics 0.5
set key right top horizontal outside
do for [i=0:2]{
    set output "f2_n".n[i+1].".eps" 
    plot [-5:5][] f2(x) w l ls 1 t "f_2(x) = cos(2x)",\
      "f2.dat" i i u 1:2 w l ls 2 t "s(x), n = ".n[i+1].""
}

# WYKRES DRUGICH POCHODNYCH
# ZALOZENIE: W pliku pochodne.dat znajduja sie dane w trzech kolumnach: 
# 1. kolumna: xw[i]
# 2. kolumna: m[i]
# 3. kolumna: iloraz roznicowy (13) obliczony w punkcie xw[i] -- podpisany jako "numeryczny"
# (10 wierszy, poniewaz n = 10)

fpp(x) = (6.*x*x-2.)/( (1.+x**2) )**3
set output "pochodne.eps"
set ylabel "f_1''(x)"
set key right bottom vertical inside
plot [-5:5] fpp(x) w l ls 1 t "f_1''(x) analityczne",\
  "pochodne.dat" u 1:2 pt 7 lc rgb "blue" t "m_i",\
  "" u 1:3 pt 7 lc rgb "red" t "f_1''(x_i) numeryczne" 




