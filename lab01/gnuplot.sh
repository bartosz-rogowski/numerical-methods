set terminal post enhanced colour solid font 20  # wybor formatu, w jakim chcemy utworzyc wynikowy rysunek

set output "x_t.eps" # nazwa pliku wynikowego
set title "Wychylenie x(t)" # tytul wykresu
set xlabel "t" # etykieta osi OX
set ylabel "x(t)" # etykieta osi OY
set grid # wlaczenie widoczności siatki pomocniczej
set samples 500
plot "out.dat" w p t "x(t), dt = 0.1", cos(x) t "cos(t)" # rysujemy out.dat i cosinusa
# plot - polecenie rysowania pliku o podanej nazwie "out.dat"
# w p == with points
# t "dt = 0.1" == title "dt = 0.1"