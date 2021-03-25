CC=gcc
CFLAGS= -O2 -lm -lgsl -lgslcblas
DEPS=
OBJS=main.o
OUT=main
GNUPLOT=gnuplot_13.sh

$(OUT): $(OBJS)
	$(CC)  -o $@  $^ $(CFLAGS)
	
.c.o:
	$(CC) -c -o $@ $< $(CFLAGS) 
	
.PHONY: clean	
clean:
	rm -f *o
	rm -f *~
	rm -f $(OUT)
	
run:
	./main

mem:
	valgrind ./main

graph:
	gnuplot $(GNUPLOT)
	rm out.dat

erase:
	rm -f *o
	rm -f *~
	rm -f $(OUT)
	rm *dat
	rm *eps