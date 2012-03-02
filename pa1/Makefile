CFLAGS = -Wall
LIBS=-lm

randmst: randmst.o graph.o random_graph.o disjoint_set.o utils.o
	$(CC) $(CFLAGS) $(LIBS) randmst.o graph.o random_graph.o disjoint_set.o utils.o -o randmst

randmst.o: randmst.c randmst.h graph.h random_graph.h utils.h
	$(CC) $(CFLAGS) -c randmst.c

disjoint_set: disjoint_set.o utils.o
	$(CC) $(CFLAGS) $(LIBS) disjoint_set.o utils.o -o disjoint_set

disjoint_set.o: disjoint_set.c utils.h
	$(CC) $(CFLAGS) -c disjoint_set.c

random_graph: random_graph.o graph.o utils.o
	$(CC) $(CFLAGS) $(LIBS) random_graph.o graph.o utils.o -o random_graph

random_graph.o: random_graph.c random_graph.h graph.h utils.h
	$(CC) $(CFLAGS) -c random_graph.c

graph: graph.o utils.o
	$(CC) $(CFLAGS) $(LIBS) graph.o utils.o -o graph

graph.o: graph.c graph.h utils.h
	$(CC) $(CFLAGS) -c graph.c

utils: utils.o
	$(CC) $(CFLAGS) $(LIBS) utils.o -o utils

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm *.o

