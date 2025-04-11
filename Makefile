CC = gcc
CFLAGS = -Wall -O2 -pthread -lm -static
OBJ = main.o sorting_algorithms.o utils.o
DEPS = sorting_algorithms.h utils.h

all: sortbench

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sortbench: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o sortbench

