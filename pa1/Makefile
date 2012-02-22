CFLAGS = -Wall
randmst: randmst.o graph.o
	$(CC) $(CFLAGS) randmst.o graph.o -o randmst

randmst.o: randmst.c randmst.h graph.h
	$(CC) $(CFLAGS) -c randmst.c

graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c

clean:
	rm *.o