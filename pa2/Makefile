CFLAGS = -Wall
LIBS=-lm
JC = javac

default: strassen 
	$(JC) *.java

strassen: strassen.o utils.o
	$(CC) $(CFLAGS) $(LIBS) strassen.o utils.o -o strassen

strassen.o: strassen.c utils.h
	$(CC) $(CFLAGS) -c strassen.c

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


