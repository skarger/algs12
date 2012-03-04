#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "graph.h"
#include "random_graph.h"
#include "disjoint_set.h"
#include "kruskal.h"

typedef struct edge_list EdgeList;

/* internal structures */
struct edge_list {
    Edge *edges;
    int num_edges;
};


/* internal function prototypes */
Disjoint_Set *create_graph_disjoint_set(Graph *g);
int union_if_necessary(Edge *ep, Disjoint_Set *ds);

int compare_edge(const void *p, const void *q);


EdgeList *make_graph_edge_list(Graph *g);
int insert_edges_for_vertex(Graph *g, Vertex *v, Edge *arr, int start_idx);
EdgeWeight *get_edge_after_self(Vertex *v, int vertex_idx);
EdgeList *create_edge_list(int size);
Edge *get_edges(EdgeList *el);
int get_num_edges(EdgeList *el);
void set_num_edges(EdgeList *el, int n);
void destroy_edge_list(EdgeList *el);

Edge *create_edge_array(int num_edges);

void copy_edge(Edge *src, Edge *dest);
void populate_edge(Edge *e, Vertex *v, Vertex *w, EdgeWeight *ewp);

EdgeWeight k(int num_vertices, int dimension);

/* function definitions */

/*
 * kruskal
 * Given a pointer to a graph g determine an MST using Kruskal's algorithm.
 * Returns and array of Edge structures with the |V| - 1 MST edges.
 */
Edge *kruskal(Graph *g) {
    EdgeList *el = make_graph_edge_list(g);
    qsort(get_edges(el), get_num_edges(el), sizeof(Edge), compare_edge);

    Disjoint_Set *ds = create_graph_disjoint_set(g);    
    
    int mst_size = get_num_vertices(g) - 1;
    Edge *x = malloc(mst_size * sizeof(Edge));
    if (x == NULL)
        error(1,"kruskal: cannot malloc Edge pointers\n","");

    Edge *edges = get_edges(el);
    Edge *one_edge;
    int i, j = 0;
    for (i = 0; i < get_num_edges(el); i++) {
        one_edge = &edges[i];
        if (union_if_necessary(one_edge, ds)) {
            copy_edge(one_edge, &x[j++]);
        }
    }

    destroy_edge_list(el);
    return x;
}

/*
 * union_if_necessary
 * Given a pointer to an edge, checks if the edge's two vertices are in 
 * the same set of the Disjoint_Set structure pointed to by ds.
 * Returns: 1 if the vertices were unioned, 0 otherwise.
 * Note: Calls find on the items of ds associated with the edge vertices.
 */
int union_if_necessary(Edge *ep, Disjoint_Set *ds) {
        Vertex *u = get_start_vertex(ep);
        Vertex *v = get_end_vertex(ep);
        int u_idx = get_index(u);
        int v_idx = get_index(v);
        DSItem *u_item = get_item_by_index(ds, u_idx);
        DSItem *v_item = get_item_by_index(ds, v_idx);

        if ( find(u_item) != find(v_item) ) {
            union_ds(ds, u_item, v_item);
            return 1;
        }
        return 0;
}

Disjoint_Set *create_graph_disjoint_set(Graph *g) {
    Disjoint_Set *ds = create_disjoint_set(get_num_vertices(g));
    DSItem *cur = get_items(ds);
    int i;
    for (i = 0; i < get_num_vertices(g); i++) {
        makeset(cur);
        cur++;
    }
    return ds;
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

EdgeWeight get_cost(const Edge *ep) {
    return get_edge_weight_value(ep->weight);
}

EdgeList *create_edge_list(int size) {
    EdgeList *el = malloc(sizeof(EdgeList));
    if (el == NULL)
        error(1,"create_edge_list: could not malloc EdgeList","");

    el->edges = create_edge_array(size);
    el->num_edges = size;
    return el;
}

Vertex *get_start_vertex(Edge *ep) {
    return ep->start;
}

Vertex *get_end_vertex(Edge *ep) {
    return ep->end;
}

Edge *create_edge_array(int num_edges) {
    Edge *edges = malloc(num_edges * sizeof(Edge));
    if (edges == NULL)
        error(1,"create_edge: could not malloc Edge array","");
    return edges;
}

void copy_edge(Edge *src, Edge *dest) {
    populate_edge(dest, src->start, src->end, src->weight);
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

void set_num_edges(EdgeList *el, int n) {
    el->num_edges = n;
}

EdgeList *make_graph_edge_list(Graph *g) {
    int max_edges = triangular_number(get_num_vertices(g) - 1);
    EdgeList *el = create_edge_list(max_edges);
    Edge *edges = get_edges(el);
    int edges_idx = 0;

    Vertex *vp = get_vertex(g, 0);
    while (vp != NULL) {
        edges_idx = insert_edges_for_vertex(g, vp, edges, edges_idx);
        vp = next_vertex(g, vp);
    }

    set_num_edges(el, edges_idx);
    return el;
}


void destroy_edge_list(EdgeList *el) {
    destroy_edge_array(el->edges);
    free(el);
}

void destroy_edge_array(Edge *e) {
    free(e);
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
    int max_cost = k(get_num_vertices(g), get_dimension(v));

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
 * return the maximum weight expected of any edge in an MST for
 * graph with size num_vertices and dimension 0, 2, 3, 4.
 */
EdgeWeight k(int num_vertices, int dimension) {
    return 2.0;
/*
this function is incomplete so it just returns 2.0

below are expected values for the distance between two points 
randomly chosen from the entities from this assignment
from http://mathworld.wolfram.com/HypercubeLinePicking.html

this function should also be dependent on the number of vertices
*/

    EdgeWeight exp0 = 0.3333333;
    EdgeWeight exp2 = 0.5214054;
    EdgeWeight exp3 = 0.6617072;
    EdgeWeight exp4 = 0.7776657;
    switch (dimension) {
        case 0:
            return exp0;
            break;
        case 2:
            return exp2;
            break;
        case 3:
            return exp3;
            break;
        case 4:
            return exp4;
            break;
        default:
            error(1,"k: invalid dimension","");
            return 1;
            break;
    }
}

/*
int main() {
    int dim = 0, num_v = 5;
    int i;
    int big_graph = (num_v > 10);

    Graph *g = create_random_graph(dim, num_v);
    Vertex *vp = get_vertex(g, 0);
    EdgeWeight *ewp;

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

    Edge *mst = kruskal(g); //make_graph_edge_list(g);

    if (!big_graph) {
        printf("Edges in MST\n");
        printf("u v cost\n");
        for (i = 0; i < num_v-1; i++) {
            printf("%d ",get_index(get_start_vertex(&mst[i])));
            printf("%d ",get_index(get_end_vertex(&mst[i])));
            printf("%f \n", get_cost(&mst[i]));
        }
    }

    destroy_edge_array(mst);
    destroy_graph(g);

    return 0;
}
*/
