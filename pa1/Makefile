CFLAGS = -Wall
randmst: randmst.o graph.o
	$(CC) $(CFLAGS) randmst.o graph.o -o randmst

randmst.o: randmst.c randmst.h graph.h
	$(CC) $(CFLAGS) -c randmst.c

random_graph: random_graph.o graph.o graph.h
	$(CC) $(CFLAGS) random_graph.o graph.o -o random_graph

random_graph.o: random_graph.c
	$(CC) $(CFLAGS) -c random_graph.c

graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c

clean:
	rm *.o

