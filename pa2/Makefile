CFLAGS = -Wall
LIBS=-lm

strassen: strassen.o utils.o
	$(CC) $(CFLAGS) $(LIBS) strassen.o utils.o -o strassen

strassen.o: strassen.c utils.h
	$(CC) $(CFLAGS) -c strassen.c

utils: utils.o
	$(CC) $(CFLAGS) $(LIBS) utils.o -o utils

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm *.o

