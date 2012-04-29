CFLAGS = -Wall -std=gnu99
LIBS=-lm
JC = javac

default: kk NumberPartition

kk: kk.o utils.o KarmarkarKarp
	$(CC) $(CFLAGS) $(LIBS) kk.o utils.o -o kk

kk.o: kk.c utils.h
	$(CC) $(CFLAGS) -c kk.c

KarmarkarKarp: KarmarkarKarp.class

KarmarkarKarp.class: KarmarkarKarp.java NumericDataReader.class
	$(JC) KarmarkarKarp.java

Partition: Partition.class

Partition.class: Partition.java KarmarkarKarp.class
	$(JC) Partition.java

NumberPartition: NumberPartition.class

NumberPartition.class: NumberPartition.java Partition.class Stopwatch.class
	$(JC) NumberPartition.java

NumericDataReader.class: NumericDataReader.java
	$(JC) NumericDataReader.java

Stopwatch.class: Stopwatch.java
	$(JC) Stopwatch.java
    

random_input: random_input.o utils.o
	$(CC) $(CFLAGS) $(LIBS) random_input.o utils.o -o random_input

random_input.o: random_input.c utils.h
	$(CC) $(CFLAGS) -c random_input.c

utils: utils.o
	$(CC) $(CFLAGS) $(LIBS) utils.o -o utils

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c


clean:
	rm *.o 

