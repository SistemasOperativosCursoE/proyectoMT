CFLAGS = -Wall -I .

.PHONY: all

all: schedsim schedgen schedstats

schedsim: schedsim.o funcionesSchedsim.o 
	gcc schedsim.o funcionesSchedsim.o $(CFLAGS) -o $@

schedsim.o : ./src/schedsim.c
	gcc $(CFLAGS) -c $^

funcionesSchedsim.o : ./src/funcionesSchedsim.c
	gcc $(CFLAGS) -c $^

schedgen: schedgen.o funcionesSchedgen.o 
	gcc schedgen.o funcionesSchedgen.o $(CFLAGS) -o $@

schedgen.o : ./src/schedgen.c
	gcc $(CFLAGS) -c $^

funcionesSchedgen.o : ./src/funcionesSchedgen.c
	gcc $(CFLAGS) -c $^

schedstats: schedstats.o funcionesSchedsim.o
	gcc schedstats.o funcionesSchedsim.o $(CFLAGS) -o $@

schedstats.o : ./src/schedstats.c
	gcc $(CFLAGS) -c $^

clean:
	rm -f *.o schedsim schedgen schedstats schedwaits.dat schednturns.dat schedturns.dat *~
