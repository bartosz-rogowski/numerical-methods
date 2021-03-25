set terminal post enhanced colour solid font 21  

set key spacing 2 samplen 3
set xlabel "x" 
set ylabel "f(x)"
set grid
f(x) = 1./(1.+x**2)
l_width = 5
p_size = 1.3
set samples 200
set style line 1 lt rgb "gray30" lw 6 pt 7 ps p_size
set style line 2 lt rgb "royalblue" lw l_width pt 7 ps p_size*1.2

# PETLA DLA WEZLOW ROWNOODLEGLYCH.
# ZALOZENIE: W pliku zad_1.dat znajduja sie dane w dwoch kolumnach. 
# Pierwsza seria danych: dla n=5, nastepnie dwie puste linie,
# druga seria danych (dla n=10), dwie puste linie, itd.
do for [i = 0:3] {
    n = 5 * (i + 1)
    if (i == 1 || i == 2) { 
        set key top center
    }
    if (i == 3) {
        set key bottom center
    }
    set output "inter_n".n.".eps" 
    plot [-5:5][] f(x) w l ls 1 t "f(x) = ^1/_{(1+x^2)}",\
    "zad_1.dat" i i u 1:2 w l ls 2 t "W_{".n."}(x)"
}

# PETLA DLA WEZLOW -- ZER WIELOMIANOW CZEBYSZEWA.
# Dane w pliku zad_2.dat w czterech seriach danych, analogicznie do 1. pliku.
set key right top
do for [i = 0:3] {
    n = 5 * (i + 1)
    set output "inter_n".n."_Czebyszew.eps" 
    plot [-5:5][] f(x) w l ls 1 t "f(x) = ^1/_{(1+x^2)}",\
    "zad_2.dat" i i u 1:2 w l ls 2 t "W_{".n."}(x)"
}
