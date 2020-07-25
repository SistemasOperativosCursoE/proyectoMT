CFLAGS = -Wall -I .

.PHONY: all

all: schedsim schedgen schedstats

schedsim: schedsim.o funcionesSchedsim.o 
	gcc schedsim.o funcionesSchedsim.o $(CFLAGS) -o $@

schedsim.o : ./schedsim.c
	gcc $(CFLAGS) -c $^

funcionesSchedsim.o : ./funcionesSchedsim.c
	gcc $(CFLAGS) -c $^

schedgen: schedgen.o funcionesSchedgen.o 
	gcc schedgen.o funcionesSchedgen.o $(CFLAGS) -o $@

schedgen.o : ./schedgen.c
	gcc $(CFLAGS) -c $^

funcionesSchedgen.o : ./funcionesSchedgen.c
	gcc $(CFLAGS) -c $^

schedstats: schedstats.o funcionesSchedsim.o funcionesSchedstats.o
	gcc schedstats.o funcionesSchedsim.o funcionesSchedstats.o $(CFLAGS) -o $@

schedstats.o : ./schedstats.c
	gcc $(CFLAGS) -c $^

funcionesSchedstats.o : ./funcionesSchedstats.c
	gcc $(CFLAGS) -c $^

clean:
	rm -f *.o schedsim schedgen schedstats schedwaits.dat schednturns.dat schedturns.dat *~
