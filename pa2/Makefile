CFLAGS = -Wall
LIBS=-lm
JC = javac

default: strassen 
	$(JC) *.java

strassen: strassen.o utils.o
	$(CC) $(CFLAGS) $(LIBS) strassen.o utils.o -o strassen

strassen.o: strassen.c utils.h
	$(CC) $(CFLAGS) -c strassen.c

utils: utils.o
	$(CC) $(CFLAGS) $(LIBS) utils.o -o utils

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c


clean: default
	rm *.o 


