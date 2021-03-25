set terminal post enhanced colour solid font 25  
reset

# # SKRYPT GENERUJE WYKRESY DLA JEDNEGO PLIKU: out.dat Z DWIEMA KOLUMNAMI: n, blad bezwzgledny |c - c_a|.
#          - w 1. serii danych: wyniki dla 1. funkcji.
#          - w 2. serii danych (po dwoch pustych liniach): wyniki dla 2. funkcji z kwadratura Hermite'a.
#          - w 3. serii danych: wyniki dla 2. funkcji z kwadratura Legendre'a.
#          - w 4. serii danych: wyniki dla 3. funkcji.

set style line 2 lt rgb "red" lw 5 pt 7 ps 1.3
set style line 3 lt rgb "dark-spring-green" lw 5 pt 7 ps 1.3

# Wykres funkcji g_2(x) (2. zadanie)
g2(x) = log(x) * exp(-x**2)
set samples 500
set output "g2.eps"
set xlabel "x"
set ylabel "g_2(x)"
set grid
set key right bottom spacing 2
plot [0.01:2.5] g2(x) ls 3 t "g_2(x) = ln(x)e^{-x^2}"

# 1. zadanie: wykres bledu.
set key right top
set output "c1_err.eps"
set xlabel "n"
set ylabel "|c_1 - c_{1,a}|"
plot "out.dat" i 0 u 1:2 w lp ls 2 t "|c_1 - c_{1,a}|"

# 2. zadanie: wykres bledu dla obu kwadratur.
set output "c2_err.eps"
set ylabel "|c_2 - c_{2,a}|"
plot "out.dat" i 1 u 1:2 w lp ls 2 t "|c_2 - c_{2,a}|, Hermite", \
""  i 2 u 1:2 w lp ls 3 t "|c_2 - c_{2,a}|, Legendre"

# 3. zadanie: wykres bledu.
set output "c3_err.eps"
set ylabel "|c_3 - c_{3,a}|"
plot [] "out.dat" i 3 u 1:2 w lp ls 2 t "|c_3 - c_{3,a}|"
