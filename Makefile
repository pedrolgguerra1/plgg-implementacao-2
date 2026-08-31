CC = gcc
CFLAGS = -Wall -Wextra -O2

OBJS = 
all: mandelbrot

mandelbrot: $(OBJS)
	$(CC) $(CFLAGS) -o mandelbrot $(OBJS)

%.o: %.c common.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	