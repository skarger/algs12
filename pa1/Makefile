CFLAGS = -Wall
randmst: randmst.o graph.o random_graph.o utils.o
	$(CC) $(CFLAGS) randmst.o graph.o random_graph.o utils.o -o randmst

randmst.o: randmst.c randmst.h graph.h random_graph.h utils.h
	$(CC) $(CFLAGS) -c randmst.c

random_graph: random_graph.o graph.o utils.o
	$(CC) $(CFLAGS) random_graph.o graph.o utils.o -o random_graph

random_graph.o: random_graph.c random_graph.h graph.h utils.h
	$(CC) $(CFLAGS) -c random_graph.c

graph: graph.o utils.o
	$(CC) $(CFLAGS) graph.o utils.o -o graph

graph.o: graph.c graph.h utils.h
	$(CC) $(CFLAGS) -c graph.c

utils: utils.o
	$(CC) $(CFLAGS) utils.o -o utils

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm *.o

