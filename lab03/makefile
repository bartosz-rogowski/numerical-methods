CC=gcc
CFLAGS= -O2 -lm -lgsl -lgslcblas
DEPS=
OBJS=main.o
OUT=main
OUTPUT=out.dat

$(OUT): $(OBJS)
	$(CC)  -o $@  $^ $(CFLAGS)
	
.c.o:
	$(CC) -c -o $@ $< $(CFLAGS) 
	
.PHONY: clean	
clean:
	rm -f *o
	rm -f *~
	rm -f $(OUT)
	rm -f $(OUTPUT)
	
run:
	./main

output:
	@touch $(OUTPUT)
	./main > $(OUTPUT)