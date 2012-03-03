#include <stdlib.h>

#include "graph.h"
#include "random_graph.h"

int kruskal(Graph *g) {

    return 0;
}

int compare_edge(void *edge1, void *edge2) {
    float ew1 = get_cost(edge1);
    float ew2 = get_cost(edge2);
    if (ew1 < ew2)
        return -1;
    if (ew1 > ew2)
        return 1;
    return 0;
}


qsort(edges, n_edges, sizeof(Edge)

