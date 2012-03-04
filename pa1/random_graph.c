#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "utils.h"
#include "graph.h"
#include "random_graph.h"

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
    EdgeWeight *ep;
    int num_vertices = get_num_vertices(g);
    while(vp != NULL) {
        ep = create_edge_weights(num_vertices);
        set_edge_weights(vp, ep, num_vertices);
        vp = next_vertex(g, vp);
    }
    if (dimension == 0)
        make_interval_edge_weights(g);
    else {
        make_cube_edge_weights(g, dimension);
    }
}


/*
 * make_interval_edge_weights
 * input: g, pointer to an undirected graph
 * action: for each vertex, set edge costs to all other vertices
 *         chooses edge costs from the interval [0,1]
 *         only creates (and sets) costs for the "current" vertex and later 
 *         because the costs to earlier vertices have already been set 
 *         given that it is an undirected graph
 */
void make_interval_edge_weights(Graph *g) {
    EdgeWeight *ep;
    Vertex *vp = get_vertex(g, 0);
    int vertex_idx = 0;
    while(vp != NULL) {
        ep = get_edge_weight(vp, vertex_idx);
        set_edge_weight_value(ep, 0.0);
        ep = next_edge_weight(vp, ep);
        while (ep != NULL) {
            set_edge_weight_value(ep, random_float(0,1));
            ep = next_edge_weight(vp, ep);
        }
        vp = next_vertex(g, vp);
        vertex_idx++;
    }
}

void make_cube_edge_weights(Graph *g, int dim) {
    set_random_coordinates(g, dim);

    if (get_num_vertices(g) < 2)
        return; // no edge_weights to compute

    Vertex *vp, *other_v;

    vp = get_vertex(g, 0);
    while(vp != NULL) {
        // start calculating from the current vertex
        other_v = vp; 
        while (other_v != NULL) {
            set_euclidean_edge_weights(vp, other_v);
            other_v = next_vertex(g, other_v);
        }
        vp = next_vertex(g, vp);
    }
}

void set_euclidean_edge_weights(Vertex *v, Vertex *w) {
    EdgeWeight *ep = get_edge_weight(v, get_index(w));
    float dist = euclidean_distance(get_coordinates(v), get_coordinates(w), 
                                    get_dimension(v));
    set_edge_weight_value(ep, dist);
}


void set_random_coordinates(Graph *g, int dim) {
    Vertex *vp = get_vertex(g, 0);
    float *coordinates;
    int i;
    while(vp != NULL) {
        coordinates = create_float_array(dim);
        for(i = 0; i < dim; i++)
            coordinates[i] = random_float(0,1);
        vp->coord = coordinates;
        vp->dimension = dim;
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
    if (get_num_vertices(g) < 2)
        return; // nothing to copy

    Vertex *vp = get_vertex(g, 1), *prev_vertex;
    int current_v_idx = 1, i;
    EdgeWeight *ep, *prev_ep;
    while(vp != NULL) {
        for(i=0; i < current_v_idx; i++) {
            // the cost from the current vertex to vertex i is equal to 
            // the cost from vertex i to the current vertex
            prev_vertex = get_vertex(g, i);
            prev_ep = get_edge_weight(prev_vertex, current_v_idx); 
            ep = get_edge_weight(vp, i);
            set_edge_weight_value(ep,get_edge_weight_value(prev_ep));
        }
        vp = next_vertex(g, vp);
        current_v_idx++;
    }
}





/*
int main() {
    int i;
    int dim = 0;
    Graph *g = create_random_graph(dim, 3);
    printf("gnv %d\n", get_num_vertices(g));

    copy_symmetric_edge_costs(g);

    EdgeWeight *ep;
    Vertex *vt;
    vt = get_vertex(g, 0);

    float min = 1.0, max = 0.0;
        ep = get_edge_weight(vt, 0);
        while(ep != NULL) {
            if (*ep < min)
                min = *ep;
            if (*ep > max)
                max = *ep;
        //        printf(" %f ",*ep);
            ep = next_edge_weight(vt, ep);
        }
    printf("\n");
    printf("max: %f, min: %f", max, min);
    printf("\n");
    printf("last vertex\n");
    vt = get_vertex(g, 2);
    ep = get_edge_weight(vt, 0);
    while(ep != NULL) {
            if (*ep != 0.0) 
                printf(" %f ",*ep);
            ep = next_edge_weight(vt, ep);
    }

    printf("\n");

    vt = get_vertex(g, 0);
    while(vt != NULL) {
        printf("id: %d", vt->id);
        if (dim > 0) {
            printf(" crd: ");
            for(i = 0; i < dim; i++)
                printf("%f ",vt->coord[i]);
        }
        printf(" wt: ");
        ep = get_edge_weight(vt, 0);
        while(ep != NULL) {
            printf(" %f ",*ep);
            ep = next_edge_weight(vt, ep);
        }
        printf("\n");
        printf("\n");
        vt = next_vertex(g, vt);
   }


    destroy_graph(g);
    return 0;
}
*/
