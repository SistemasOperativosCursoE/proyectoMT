
CFLAGS = -Wall -I .
main_c= ./src/schedsim.c
funciones_c= ./src/funcionesSchedsim.c
funciones_h= ./include/funcionesSchedsim.h

.PHONY: all

all: schedsim

schedsim: schedsim.o funcionesSchedsim.o 
	gcc schedsim.o funcionesSchedsim.o $(CFLAGS) -o $@

schedsim.o : $(main_c)
	gcc $(CFLAGS) -c $^

funcionesSchedsim.o : $(funciones_c)
	gcc $(CFLAGS) -c $^

clean:
	rm -f *.o schedsim *~
