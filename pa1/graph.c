#include <stdlib.h>
#include <stdio.h>
#include "randmst.h"
#include "graph.h"

Graph *create_graph(int num_vertices);
Adjacency *create_adjacency(int num_vertices);
Vertex *create_vertex(int num_edges);
void destroy_graph(Graph *g);
void fill_edges(Vertex *v, int num_edges);
Vertex *get_vertex(Graph *g, int n);

void error(int errcd, char *msg1, char *msg2) {
    printf("%s %s",msg1, msg2);
    exit(errcd);
}

struct graph {
    int num_vertices;
    int num_edges;
    Adjacency *adj;
};

struct adjacency {
    Vertex **adj_mat;
};

struct vertex {
    int id;
    int explored;
    float *edges;
};

struct edge {
    Vertex *start;
    Vertex * end;
    double cost;
};

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

Adjacency *create_adjacency(int num_vertices) {
    Adjacency *a = malloc(sizeof(Adjacency));
    if (a == NULL) {
        error(1,"create_adjacency: cannot malloc Adjacency\n","");
    }
    // array of vertices
    Vertex **v_arr = (Vertex**) malloc(num_vertices * sizeof(Vertex*));
    if (v_arr == NULL) {
        error(1,"create_adjacency: cannot malloc vertex array\n","");
    }

    int i;
    for (i = 0; i < num_vertices; i++) {
        v_arr[i] = create_vertex(num_vertices);
        v_arr[i]->id = i;     // give each vertex an id
    }

    a->adj_mat = v_arr;
    return a;
}

Vertex *create_vertex(int num_edges) {
    Vertex *v = (Vertex*) malloc(sizeof(Vertex));
    if (v == NULL) {
        error(1,"create_vertex: cannot malloc Vertex","");
    }
    float *e_arr = (float*) malloc(num_edges * sizeof(float*));
    if (e_arr == NULL) {
        error(1,"create_vertex: cannot malloc edges\n","");
    }
                                   
    v->edges = e_arr;
    v->explored = 0;
    return v;
}

void fill_edges(Vertex *v, int num_edges) {
    int i;
    for (i = 0; i < num_edges; i++) {
        v->edges[i] = 0.5;
        // debug        printf("i = %d, ve = %f, adrve = %p\n", i, v->edges[i], &v->edges[i]);
    }
}

Vertex *get_vertex(Graph *g, int n) {
    // debug    printf("vid: %d vadr: %p\n",g->adj->adj_mat[n]->id, g->adj->adj_mat[n]);
    return g->adj->adj_mat[n];
}

void destroy_graph(Graph *g) {
    int i;
    for (i = 0; i < g->num_vertices; i++) {
        free(g->adj->adj_mat[i]->edges);
    }
    free(g->adj->adj_mat);
    free(g->adj);
    free(g);
}
    
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
        fill_edges(get_vertex(g, i), 10);
    }
    for (i = 0; i < 10; i++) {
        printf("ew: %f ",(g->adj->adj_mat[i]->edges[0]));
    }
    printf("\n");
    int j;
    for (j=0; j < 10; j++) {
        for (i = 0; i < 10; i++) {
                printf("ew: %f ",(g->adj->adj_mat[j]->edges[i]));
        }
        printf("\n");
    }

    destroy_graph(g);

    return 0;
}


