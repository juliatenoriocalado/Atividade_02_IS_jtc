CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -fopenmp -pthread

TARGET = mandelbrot

all: $(TARGET)

$(TARGET): mandelbrot.c
	$(CC) $(CFLAGS) mandelbrot.c -o $(TARGET) 

clean:
	rm -f $(TARGET) *.o