#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "graph.h"


Graph *create_random_graph(int dim, int num_vertices);

void set_cube_edges(Graph *g, int dim);
void make_interval_edges(Graph *g);
float random_float(float a, float b);
void load_graph(Graph *g, int dimension);

Graph *create_random_graph(int dim, int num_vertices) {
    Graph *g = create_graph(num_vertices);
    if (dim == 0 || dim == 2 || dim == 3 || dim == 4) {
        load_graph(g, dim);
    } else {
        return NULL;
    }
    return g;
}

void load_graph(Graph *g, int dimension) {
    Vertex *vp = get_vertex(g, 0);
    Edge *ep;
    int vertex_idx = 0;
    while(vp != NULL) {
        //printf("vidx %d | ", vertex_idx);
        ep = create_edges(g->num_vertices);
        set_edges(vp, ep, g->num_vertices);
        vp = next_vertex(g, vp);
        vertex_idx++;
    }
    if (dimension == 0)
        make_interval_edges(g);
/*
    else {
        // cube
    }
*/
}

void make_interval_edges(Graph *g) {
    srandom(time(NULL));
    Edge *ep;
    Vertex *vp = get_vertex(g, 0);
    int vertex_idx = 0;
    while(vp != NULL) {
        // fill in edge costs starting from current vertex
        // if we are working with undirected graphs then 
        // the edge costs to earlier vertices are already in their edge lists
        // later we can fill in those costs using the earlier vertices
        ep = get_edge(vp, vertex_idx);
        set_edge_cost(ep, 0.0);
        ep = next_edge(vp, ep);
        while (ep != NULL) {
            set_edge_cost(ep, random_float(0,1));
            ep = next_edge(vp, ep);
        }
        vp = next_vertex(g, vp);
        vertex_idx++;
    }
}


/*
// build up edge list
f(Graph *g, Edge *edge_list) {
    
        while(ep != NULL) 
    v = get_vertex(i)
    for j = ei+1 to last edge
        x = cost_func(v, j)
        set_edge_cost(x, j)
        get_vertex(j)
        set_edge_cost(x, i) for j
}
*/


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

int main() {
    srandom(time(NULL));
    int i;
    for (i = 0; i < 10; i++) {
        printf("%f\n",random_float(0,1));
    }

    //printf("RAND_MAX %d\n", RAND_MAX);
    //printf("random max %f\n", pow(2,31) - 1);

    Graph *g = create_random_graph(0, 16384);    
    printf("gnv %d\n", g->num_vertices);

    destroy_graph(g);
/*
    Edge *ep;
    Vertex *vt;
    vt = get_vertex(g, 0);
    while(vt != NULL) {
        printf("id: %d", vt->id);
        ep = get_edge(vt, 0);
        while(ep != NULL) {
                printf(" %f ",*ep);
                ep = next_edge(vt, ep);
        }
        printf("\n");
        printf("\n");
        vt = next_vertex(g, vt);
    }
*/

    return 0;
}
