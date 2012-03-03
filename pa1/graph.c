#include <stdlib.h>
#include <stdio.h>
#include "randmst.h"
#include "graph.h"
#include "utils.h"



Graph *create_graph(int num_vertices) {
    Graph *g = malloc(sizeof(Graph));
    if (g == NULL) {
        error(1,"create_graph: cannot malloc Graph\n","");
    }
    g->num_vertices = num_vertices;
    if ((g->adj = create_adjacency(num_vertices)) == NULL) {
        return NULL;
    }
    return g;
}

Vertex *create_adjacency(int num_vertices) {
    // array of vertices
    Vertex *vp = (Vertex*) malloc(num_vertices * sizeof(Vertex));
    if (vp == NULL) {
        error(1,"create_adjacency: cannot malloc vertex array\n","");
    }

    int i;
    for (i = 0; i < num_vertices; i++) {
        initialize_vertex(&vp[i]);
        vp[i].id = i;     // give each vertex an id
    }

    return vp;
}

void initialize_vertex(Vertex *v) {
    v->id = -1;
    v->explored = 0;
    v->num_edge_weights = 0;
    v->edge_weights = NULL;
    v->dimension = 0;
    v->coord = NULL;
}

Vertex *get_vertex(Graph *g, int i) {
    if (i < 0 || i > (g->num_vertices-1)) {
        error(1,"get_vertex: invalid vertex index\n","");
    }
    return &(g->adj[i]);
}

Vertex *next_vertex(Graph *g, Vertex *current) {
    if (current < g->adj || current >= g->adj + g->num_vertices) {
        error(1,"next_vertex: invalid current pointer\n","");
    }
    current++;
    if (current == g->adj + g->num_vertices)
        return NULL;
    else
        return current;
}

EdgeWeight *create_edge_weights(int num_edge_weights) {
    // create array of edge costs
    EdgeWeight *ep = (EdgeWeight*) malloc(num_edge_weights*sizeof(EdgeWeight));
    if (ep == NULL) {
        error(1,"create_edge_weights: cannot malloc edge_weights\n","");
    }
    return ep;
}

void set_edge_weights(Vertex *v, EdgeWeight *edge_weights, int num_edge_weights){
    if (v->edge_weights != NULL)
        free(v->edge_weights); // out with the old
    v->edge_weights = edge_weights; // in with the new
    v->num_edge_weights = num_edge_weights;
}

EdgeWeight *get_edge_weight(Vertex *v, int i) {
    if (i < 0 || i > (v->num_edge_weights-1)) {
        error(1,"get_edge_weight: invalid edge weight index\n","");
    }
    return &(v->edge_weights[i]);
}

EdgeWeight get_edge_weight_value(EdgeWeight *ep) {
    return *ep;
}

void set_edge_weight_value(EdgeWeight *edge_weight, EdgeWeight cost) {
    *edge_weight = cost;
}

EdgeWeight *next_edge_weight(Vertex *v, EdgeWeight *current) {
    if (current < v->edge_weights || 
        current >= (v->edge_weights + v->num_edge_weights)) {
        error(1,"next_edge_weight: invalid current pointer\n","");
    }
    current++;
    if (current == (v->edge_weights + v->num_edge_weights))
        return NULL;
    else
        return current;
}

int get_num_vertices(Graph *g) {
    return g->num_vertices;
}

int get_dimension(Vertex *v) {
    return v->dimension;
}

int get_index(Vertex *v) {
    return v->id;
}

int get_num_edge_weights(Vertex *v) {
    return v->num_edge_weights;
}

float *get_coordinates(Vertex *v) {
    return v->coord;
}

void destroy_graph(Graph *g) {
    int i;
    for (i = 0; i < g->num_vertices; i++) { 
        if (g->adj[i].coord != NULL)
            free(g->adj[i].coord);
        free(g->adj[i].edge_weights); // edge arrays
    }
    free(g->adj);
    free(g);
}

void fill_edge_weights(Vertex *v) {
    EdgeWeight *p = get_edge_weight(v, 0);
    while (p != NULL) {
        set_edge_weight_value(p, 0.5);
        p = next_edge_weight(v, p);
    }
}

void zero_self_costs(Graph *g) {
    int i;
    Vertex *vp;
    EdgeWeight *ep;
    for(i = 0; i < g->num_vertices; i++) {
        vp = get_vertex(g, i);
        ep = get_edge_weight(vp, i);
        set_edge_weight_value(ep, 0);
    }
}

/*
int main() {
    Graph *g = create_graph(10);
    if (g == NULL)
        printf("g is null\n");
    else {
        printf("g created\n");
        printf("g nv = %d\n", g->num_vertices);
    }

    int i;
    for (i = 0; i < 10; i++) {
        Vertex *vp = get_vertex(g, i);
        EdgeWeight *ep = create_edge_weights(10);
        set_edge_weights(vp, ep, 10);
    }

    for (i = 0; i < 10; i++) {
        fill_edge_weights(get_vertex(g, i));
    }

    zero_self_costs(g);
    printf("\n");
    int j;
    Vertex *vt;
    vt = get_vertex(g, 0);
    EdgeWeight *ep;
    while(vt != NULL) {
        ep = get_edge_weight(vt, 0);
        while(ep != NULL) {
                printf("ew: %f ",*ep);
                ep = next_edge_weight(vt, ep);
        }
        vt = next_vertex(g, vt);
        printf("\n");
    }

    EdgeWeight *new_edge_weight = (EdgeWeight *) malloc(10*sizeof(EdgeWeight));
    EdgeWeight edgearr[10] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0,10.0};

    for (i = 0; i < 10; i++) {
        new_edge_weight[i] = edgearr[i];
        printf("%f\n", (new_edge_weight[i]));
    }

    vt = get_vertex(g, 9);
    set_edge_weights(vt, new_edge_weight, 10);

    zero_self_costs(g);

    vt = get_vertex(g, 0);
    while(vt != NULL) {
        ep = get_edge_weight(vt, 0);
        while(ep != NULL) {
                printf("ew: %f ",*ep);
                ep = next_edge_weight(vt, ep);
        }
        printf("\n");
        vt = next_vertex(g, vt);
    }

    destroy_graph(g);

    return 0;
}
*/
