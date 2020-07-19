CFLAGS = -Wall -I .

.PHONY: all

all: schedsim

schedsim: schedsim.o funcionesSchedsim.o 
	gcc schedsim.o funcionesSchedsim.o $(CFLAGS) -o $@

schedsim.o : ./src/schedsim.c
	gcc $(CFLAGS) -c $^

funcionesSchedsim.o : ./src/funcionesSchedsim.c
	gcc $(CFLAGS) -c $^

clean:
	rm -f *.o schedsim *~
