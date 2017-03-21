CC = gcc
F77 = gfortran
CFLAGS = -Wall -ansi -pedantic -O3 -lm
OUT = c_ep1 f_ep1

all: $(OUT)

clean: 
	rm -f $(OUT)

c_ep1: c_ep1.c
	$(CC) c_ep1.c $(CFLAGS) -o c_ep1

f_ep1: f_ep1.f
	$(F77) f_ep1.f -Wall -o f_ep1
	
