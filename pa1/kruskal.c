#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "graph.h"
#include "random_graph.h"

typedef struct edge Edge;
typedef struct edge_list EdgeList;

struct edge {
    Vertex *start;
    Vertex *end;
    EdgeWeight *weight;
};

struct edge_list {
    Edge *edges;
    int num_edges;
};

EdgeWeight get_cost (const Edge *ep);
int compare_edge(const void *p, const void *q);
Edge *create_edge_array(int num_edges);
void populate_edge(Edge *e, Vertex *v, Vertex *w, EdgeWeight *ewp);
EdgeList *make_graph_edge_list(Graph *g);
Edge *get_edges(EdgeList *el);
int get_num_edges(EdgeList *el);
EdgeWeight *get_edge_after_self(Vertex *v, int vertex_idx);
int insert_edges_for_vertex(Graph *g, Vertex *v, Edge *arr, int start_idx);
EdgeWeight k(int n);

EdgeList * kruskal(Graph *g);

EdgeList *kruskal(Graph *g) {
    EdgeList *el = make_graph_edge_list(g);
    qsort(get_edges(el), get_num_edges(el), sizeof(Edge), compare_edge);
    return el;
}

int compare_edge(const void *p, const void *q) {
    const Edge *edge1 = p;
    const Edge *edge2 = q;

    const EdgeWeight ew1 = get_cost(edge1);
    const EdgeWeight ew2 = get_cost(edge2);

    if (ew1 < ew2)
        return -1;
    if (ew1 > ew2)
        return 1;
    return 0;
}

EdgeWeight get_cost (const Edge *ep) {
    return get_edge_weight_value(ep->weight);
}

Edge *create_edge_array(int num_edges) {
    Edge *edges = malloc(num_edges * sizeof(Edge));
    if (edges == NULL)
        error(1,"create_edge: could not malloc Edge array","");
    return edges;
}

void populate_edge(Edge *e, Vertex *v, Vertex *w, EdgeWeight *ewp) {
    e->start = v;
    e->end = w;
    e->weight = ewp;
}

Edge *get_edges(EdgeList *el) {
    return el->edges;
}

int get_num_edges(EdgeList *el) {
    return el->num_edges;
}

EdgeList *make_graph_edge_list(Graph *g) {
    EdgeList *el = malloc(sizeof(EdgeList));
    if (el == NULL)
        error(1,"make_graph_edge_list: could not malloc EdgeList","");

    int max_edges = triangular_number(get_num_vertices(g) - 1);
    Edge *edges = create_edge_array(max_edges);
    int edges_idx = 0;

    Vertex *vp = get_vertex(g, 0);
    while (vp != NULL) {
        edges_idx = insert_edges_for_vertex(g, vp, edges, edges_idx);
        vp = next_vertex(g, vp);
    }

    el->edges = edges;
    el->num_edges = edges_idx;
    return el;
}

void destroy_edge_list(EdgeList *el) {
    free(el->edges);
    free(el);
}

EdgeWeight *get_edge_after_self(Vertex *v, int vertex_idx) {
    // start at the current vertex's cost to itself, and advance 1
    EdgeWeight *ewp = get_edge_weight(v, vertex_idx); 
    ewp = next_edge_weight(v, ewp); 
    return ewp;
}

int insert_edges_for_vertex(Graph *g, Vertex *v, Edge *arr, int start_idx) {
    /* prep for this specific vertex */
    int cur_v_id = get_index(v);
    int to_v_id = cur_v_id + 1;
    EdgeWeight *ewp = get_edge_after_self(v, cur_v_id);
    int max_cost = k(get_dimension(v));

    int edges_idx = start_idx; // where to start filling the array

    while (ewp != NULL) {
        if (get_edge_weight_value(ewp) <= max_cost) {
            // add Edge data to edges array
            populate_edge(&arr[edges_idx], v, get_vertex(g,to_v_id), ewp);
            edges_idx++;
        }
        ewp = next_edge_weight(v, ewp);
        to_v_id++;
    }

    return edges_idx;
}

/*
 * k
 * return the maximum weight expected of any edge in a
 * mimimum spanning tree with dimension 0, 2, 3, 4.
 */
EdgeWeight k(int n) {
    return 2.0;
}

int main() {
    int dim = 4, num_v = 6;
    int i;
    int big_graph = (num_v > 32);

    Graph *g = create_random_graph(dim, num_v);
    Vertex *vp = get_vertex(g, 0);
    int cur_v_id = 0, to_v_id;
    EdgeWeight *ewp;

/*
    Edge *edges = create_edge_array( triangular_number(num_v-1) );
    int edges_idx = 0;
    while (vp != NULL) {
        // start at the current vertex's cost to itself, and advance 1
        ewp = get_edge_weight(vp, cur_v_id); 
        ewp = next_edge_weight(vp, ewp); 
        to_v_id = cur_v_id + 1;

        while (ewp != NULL) {
            // make Edge
            populate_edge(&edges[edges_idx], vp, get_vertex(g,to_v_id), ewp);
            edges_idx++;
            ewp = next_edge_weight(vp, ewp);
            to_v_id++;
        }

        vp = next_vertex(g, vp);
        cur_v_id++;
    }
*/

    EdgeList *edge_l = kruskal(g); //make_graph_edge_list(g);
    Edge *edges = get_edges(edge_l);

    if (!big_graph) {
        vp = get_vertex(g,0);
        while(vp != NULL) {
            printf("id: %d", vp->id);
            //if (dim > 0) {
            if (1 == 0) {
                printf(" crd: ");
                for(i = 0; i < dim; i++)
                    printf("%f ",vp->coord[i]);
            }
            printf(" wt: ");
            ewp = get_edge_weight(vp, 0);
            while(ewp != NULL) {
                printf(" %f ",*ewp);
                ewp = next_edge_weight(vp, ewp);
            }
            printf("\n");
            printf("\n");
            vp = next_vertex(g, vp);
        }
    }

    int tnd = triangular_number(num_v-1);
    printf("tnd %d\n", tnd);
    qsort(edges, get_num_edges(edge_l), sizeof(Edge), compare_edge);
    printf("edges sorted\n");

    if (!big_graph) {
        for (i = 0; i < tnd; i++) {
            printf("%d %f\n", i, get_cost(&edges[i]));
        }
    }

    destroy_edge_list(edge_l);
    destroy_graph(g);

    return 0;
}

