set terminal post enhanced colour solid font 20  

# # # # SKRYPT GENERUJE WYKRESY NA PODSTAWIE TRZECH PLIKOW: U.dat, U_hist.dat, T_hist.dat

# #     PLIK U.dat zawiera dwie kolumny danych: x_i, x_{i+1} dla rozkladu jednorodnego U(0,1).
#         - w 1. serii danych: dla a=123, c=1, m=2^{15}.
#         - w 2. serii danych: dla a=69069, c=1, m=2^{32}.

# #     PLIK U_hist.dat zawiera dwie kolumny dla rozkladu jednorodnego: wspolrzedna srodka j-tego podprzedzialu, n_j/N.
#         - w 1. serii danych: dla a=123, c=1, m=2^{15}.
#         - w 2. serii danych: dla a=69069, c=1, m=2^{32}.

# #     PLIK T_hist.dat zawiera trzy kolumny dla rozkladu trojkatnego: wspolrzedna srodka j-tego podprzedzialu, n_j/N, p_j.


# # 1. wykres: zaleznosc x_{i+1}(x_i) dla 1. przypadku U(0,1).
reset
set xtics out
set ytics out
set size square
set output "U1r.eps"
set xlabel "x_i"
set ylabel "x_{i+1}"
set key outside top horizontal
plot "U.dat" i 0 u 1:2 pt 7 ps 0.2 lc rgb "blue" t "a=123, c=1, m=2^{15}"

# # 2. wykres: zaleznosc x_{i+1}(x_i) dla 2. przypadku U(0,1).
set output "U2r.eps"
plot "U.dat" i 1 u 1:2 pt 7 ps 0.2 lc rgb "dark-spring-green" t "a=69069, c=1, m=2^{32}"


# # 3. wykres: histogram dla U(0,1).
reset
set style line 1 lt 0 lw 2 lc rgb "gray30"
set style line 2 lw 3 lc rgb "red"
k_max = 12.
x_max = 1.
x_min = 0.
delta(x1, x2) = (x2-x1)/k_max
a = 1./k_max/2.
b = 0.02

set output "Ur_hist.eps" 
set key outside top horizontal spacing 1.5 samplen 1
set ylabel "^{n_j}/_N"
set xlabel "x"
set format x "%.2g"
set xtics out font ",16" offset 0, 0.5 0, 1/12., 1
do for [t=1:11] {
  set arrow t from 2*t*a, graph 0 to 2*t*a, graph 1 nohead ls 1
}
plot [0:1][0:] "U_hist.dat" i 0 u ($1-b/2.):2:(b) w boxes fs pattern 3 lc 3 t "a=123, c=1, m=2^{15}", \
  "" i 1 u ($1+b/2.):2:(b) w boxes fs pattern 7 lc rgb "dark-spring-green" t "a=69069, c=1, m=2^{32}", \
  delta(x_min, x_max) ls 2 t "R. teoretyczny: f(x)=^1/_{12}"


# # 4. wykres: histogram dla rozkladu trojkatnego.
set output "T_hist.eps" 
set key samplen 4
k_max=10
delta(x1, x2) = (x2-x1)/k_max
mu = 4.0
Delta = 3.0
x_min = mu-Delta
x_max = mu+Delta
a = (x_max-x_min)/k_max/2.
set samples 400

do for [t=1:9] {
  set arrow t from x_min+2*t*a, graph 0 to x_min+2*t*a, graph 1 nohead lt 0 lw 2 lc rgb "gray30"
}
unset arrow 10
unset arrow 11
set xtics out font ",20" offset 0, 0.5 x_min, delta(x_min, x_max), x_max
plot [x_min:x_max][0:] "T_hist.dat" i 0 u 1:2 w boxes lc rgb "blue" fs pattern 1 t "n_j/N", \
  "" i 0 u 1:3 w boxes lc rgb "red" lw 3 fs fill empty t "p_j",

