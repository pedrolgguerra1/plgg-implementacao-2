CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lm -lpthread

OBJS = main.o common.o mandelbrot_serial.o mandelbrot_pthreads1.o mandelbrot_pthreads2.o

all: mandelbrot

mandelbrot: $(OBJS)
	$(CC) $(CFLAGS) -o mandelbrot $(OBJS) $(LDFLAGS)

%.o: %.c common.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o mandelbrot mandelbrot_plgg_*.pgm times.txt

.PHONY: all clean