
typedef struct edge Edge;

struct edge {
    Vertex *start;
    Vertex *end;
    EdgeWeight *weight;
};

/*
 * kruskal
 * Given a pointer to a graph g determine an MST using Kruskal's algorithm.
 * Returns and array of Edge structures with the |V| - 1 MST edges.
 */
Edge *kruskal(Graph *g);

EdgeWeight get_cost (const Edge *ep);
Vertex *get_start_vertex(Edge *ep);
Vertex *get_end_vertex(Edge *ep);
void destroy_edge_array(Edge *e);

