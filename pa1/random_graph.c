#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "graph.h"


Graph *create_random_graph(int dim, int num_vertices);

void make_cube_edges(Graph *g, int dim);
void set_random_coordinates(Graph *g, int dim);
void make_interval_edges(Graph *g);
float random_float(float a, float b);
void load_graph(Graph *g, int dimension);
void copy_symmetric_edge_costs(Graph *g);

Graph *create_random_graph(int dim, int num_vertices) {
    Graph *g = create_graph(num_vertices);
    if (dim == 0 || dim == 2 || dim == 3 || dim == 4) {
        load_graph(g, dim);
    } else {
        error(1,"create_random_graph: invalid dimension - try 0, 2, 3, 4\n","");
    }
    return g;
}

void load_graph(Graph *g, int dimension) {
    Vertex *vp = get_vertex(g, 0);
    Edge *ep;
    while(vp != NULL) {
        ep = create_edges(g->num_vertices);
        set_edges(vp, ep, g->num_vertices);
        vp = next_vertex(g, vp);
    }
    if (dimension == 0)
        make_interval_edges(g);
    else {
        make_cube_edges(g, dimension);
    }
}


/*
 * make_interval_edges
 * input: g, pointer to an undirected graph
 * action: for each vertex, set edge costs to all other vertices
 *         chooses edge costs from the interval [0,1]
 *         only creates (and sets) costs for the "current" vertex and later 
 *         because the costs to earlier vertices have already been set 
 *         given that it is an undirected graph
 */
void make_interval_edges(Graph *g) {
    srandom(time(NULL));
    Edge *ep;
    Vertex *vp = get_vertex(g, 0);
    int vertex_idx = 0;
    while(vp != NULL) {
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

void make_cube_edges(Graph *g, int dim) {
    set_random_coordinates(g, dim);
    Edge *ep;
    Vertex *vp = get_vertex(g, 0);
    int vertex_idx = 0;
    while(vp != NULL) {

        vp = next_vertex(g, vp);
    }
}


void set_random_coordinates(Graph *g, int dim) {
    srandom(time(NULL));
    Vertex *vp = get_vertex(g, 0);
    float *coordinates;
    int i;
    while(vp != NULL) {
        coordinates = create_float_array(dim);
        for(i = 0; i < dim; i++)
            coordinates[i] = random_float(0,1);
        vp->coord = coordinates;
        vp = next_vertex(g, vp);
    }
}

/*
 * copy_symmetric_edge_costs
 * input: g, pointer to an undirected graph that has an adjacency matrix 
 * with only the upper right triangle populated.
 * action: copy over the edge costs as illustrated below:
 *
 *       0  1  2            0  1  2
 *       -------            -------
 *   0 | 0  x  y        0 | 0  x  y
 *   1 | -  0  z   ->   1 | x  0  z
 *   2 | -  -  0        2 | y  z  0
 */
void copy_symmetric_edge_costs(Graph *g) {
    if (g->num_vertices < 2)
        return; // nothing to copy

    Vertex *vp = get_vertex(g, 1), *prev_vertex;
    int current_v_idx = 1, i;
    Edge *ep;
    while(vp != NULL) {
        for(i=0; i < current_v_idx; i++) {
            // the cost from the current vertex to vertex i is equal to 
            // the cost from vertex i to the current vertex
            ep = get_edge(vp, i);
            prev_vertex = get_vertex(g, i);
            set_edge_cost(ep,get_edge_cost(prev_vertex, current_v_idx));
        }
        vp = next_vertex(g, vp);
        current_v_idx++;
    }
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

int main() {
    int i;
    int dim = 0;
    Graph *g = create_random_graph(dim, 2);
    printf("gnv %d\n", g->num_vertices);

/*
    copy_symmetric_edge_costs(g);
*/
    Edge *ep;
    Vertex *vt;
    vt = get_vertex(g, 0);

/*
    float min = 1.0, max = 0.0;
        ep = get_edge(vt, 0);
        while(ep != NULL) {
            if (*ep < min)
                min = *ep;
            if (*ep > max)
                max = *ep;
        //        printf(" %f ",*ep);
            ep = next_edge(vt, ep);
        }
    printf("\n");
    printf("max: %f, min: %f", max, min);
    printf("\n");
    printf("last vertex\n");
    vt = get_vertex(g, 32677);
        ep = get_edge(vt, 0);
        while(ep != NULL) {
                if (*ep != 0.0) 
                    printf(" %f ",*ep);
                ep = next_edge(vt, ep);
        }
*/

    while(vt != NULL) {
        printf("id: %d", vt->id);
        printf(" coord: ");
        for(i = 0; i < dim; i++)
            printf("%f ",vt->coord[i]);
/*
        ep = get_edge(vt, 0);
        while(ep != NULL) {
            printf(" %f ",*ep);
            ep = next_edge(vt, ep);
        }
*/
        printf("\n");
        printf("\n");
        vt = next_vertex(g, vt);
    }


    destroy_graph(g);
    return 0;
}
