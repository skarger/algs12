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
    v->num_edges = 0;
    v->edges = NULL;
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

Edge *create_edges(int num_edges) {
    // create array of edge costs
    Edge *ep = (Edge*) malloc(num_edges * sizeof(Edge));
    if (ep == NULL) {
        error(1,"create_edges: cannot malloc edges\n","");
    }
    return ep;
}

void set_edges(Vertex *v, Edge *edges, int num_edges) {
    if (v->edges != NULL)
        free(v->edges); // out with the old
    v->edges = edges; // in with the new
    v->num_edges = num_edges;
}

Edge *get_edge(Vertex *v, int i) {
    if (i < 0 || i > (v->num_edges-1)) {
        error(1,"get_edge: invalid edge index\n","");
    }
    return &(v->edges[i]);
}

Edge get_edge_cost(Vertex *v, int i) {
    if (i < 0 || i > (v->num_edges-1)) {
        error(1,"get_edge_cost: invalid edge index\n","");
    }
    return v->edges[i];
}

void set_edge_cost(Edge *edge, Edge cost) {
    *edge = cost;
}

Edge *next_edge(Vertex *v, Edge *current) {
    if (current < v->edges || current >= (v->edges + v->num_edges)) {
        error(1,"next_edge: invalid current pointer\n","");
    }
    current++;
    if (current == (v->edges + v->num_edges))
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

int get_num_edges(Vertex *v) {
    return v->num_edges;
}

float *get_coordinates(Vertex *v) {
    return v->coord;
}

void destroy_graph(Graph *g) {
    int i;
    for (i = 0; i < g->num_vertices; i++) { 
        if (g->adj[i].coord != NULL)
            free(g->adj[i].coord);
        free(g->adj[i].edges); // edge arrays
    }
    free(g->adj);
    free(g);
}

void fill_edges(Vertex *v) {
    Edge *p = get_edge(v, 0);
    while (p != NULL) {
        set_edge_cost(p, 0.5);
        p = next_edge(v, p);
    }
}

void zero_self_costs(Graph *g) {
    int i;
    Vertex *vp;
    Edge *ep;
    for(i = 0; i < g->num_vertices; i++) {
        vp = get_vertex(g, i);
        ep = get_edge(vp, i);
        set_edge_cost(ep, 0);
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
        Edge *ep = create_edges(10);
        set_edges(vp, ep, 10);
    }

    for (i = 0; i < 10; i++) {
        fill_edges(get_vertex(g, i));
    }

    zero_self_costs(g);
    printf("\n");
    int j;
    Vertex *vt;
    for (j=0; j < 10; j++) {
        vt = get_vertex(g, j);
        for (i = 0; i < 10; i++) {
                printf("ew: %f ",(get_edge_cost(vt, i)));
        }
        printf("\n");
    }

    Edge *new_edge = (Edge *) malloc(10 * sizeof(Edge));
    Edge edgearr[10] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};

    for (i = 0; i < 10; i++) {
        new_edge[i] = edgearr[i];
        printf("%f\n", (new_edge[i]));
    }

    vt = get_vertex(g, 9);
    set_edges(vt, new_edge, 10);

    zero_self_costs(g);

    Edge *ep;
    vt = get_vertex(g, 0);
    while(vt != NULL) {
        ep = get_edge(vt, 0);
        while(ep != NULL) {
                printf("ew: %f ",*ep);
                ep = next_edge(vt, ep);
        }
        printf("\n");
        vt = next_vertex(g, vt);
    }
    // should error:
    //Vertex *myv = get_vertex(g, 0);
    //get_edge_cost(myv, 10);

    destroy_graph(g);

    return 0;
}
*/
