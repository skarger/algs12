CFLAGS = -Wall
LIBS=-lm
JC = javac

default: kk
	#$(JC) *.java

kk: kk.o utils.o
	$(CC) $(CFLAGS) $(LIBS) kk.o utils.o -o kk

kk.o: kk.c utils.h
	$(CC) $(CFLAGS) -c kk.c

random_input: random_input.o utils.o
	$(CC) $(CFLAGS) $(LIBS) random_input.o utils.o -o random_input

random_input.o: random_input.c utils.h
	$(CC) $(CFLAGS) -c random_input.c

utils: utils.o
	$(CC) $(CFLAGS) $(LIBS) utils.o -o utils

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c


clean: default
	rm *.o 


