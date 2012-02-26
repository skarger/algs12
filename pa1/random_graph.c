#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "graph.h"


Graph *create_random_graph(int dim, int num_vertices);

void set_cube_edges(Graph *g, int dim);
void set_interval_edges(Graph *g, int a, int b);
float random_float(float a, float b);

Graph *create_random_graph(int dim, int num_vertices) {
    Graph *g = create_graph(num_vertices);
    if (dim == 0) {
        set_interval_edges(g, 0, 1);
    } else if (dim == 2 || dim == 3 || dim == 4) {
        set_cube_edges(g, dim);
    } else {
        return NULL;
    }

    return g;
}

void set_interval_edges(Graph *g, int a, int b) {
    int num_v = g->num_vertices;
}

/*
 * random_float
 * return a random float in the interval [a, b]
*/
float random_float(float a, float b) {
    if (b < a)
        return 0;
    /* make random number in [0,1] */
    float ran_fraction = (1.0 * random()) / RAND_MAX;
    /* scale it to be in interval [a,b] */
    return (a + (b - a) * ran_fraction);
}

void set_cube_edges(Graph *g, int dim) {
    printf("sce\n");
}


/*
for 0 to n-1
    v = get_vertex(i)
    for j = ei+1 to last edge
        x = cost_func(v, j)
        set_edge_cost(x, j)
        get_vertex(j)
        set_edge_cost(x, i) for j
*/

int main() {
    srandom(time(NULL));
    int i;
    for (i = 0; i < 10; i++) {
        printf("%f\n",random_float(0,1));
    }

    //printf("RAND_MAX %d\n", RAND_MAX);
    //printf("random max %f\n", pow(2,31) - 1);
    return 0;
}
