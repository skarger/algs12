
typedef struct graph Graph;
typedef struct adjacency Adjacency;
typedef struct vertex Vertex;
typedef float Edge;

struct graph {
    int num_vertices;
    Vertex *adj;
};

struct vertex {
    int id;
    int explored;
    int num_edges;
    Edge *edges;
    float *coord; // coordinates
};

Graph *create_graph(int num_vertices);
void destroy_graph(Graph *g);

Vertex *create_adjacency(int num_vertices);
void initialize_vertex(Vertex *v);
Vertex *get_vertex(Graph *g, int i);
Vertex *next_vertex(Graph *g, Vertex *current);

Edge *create_edges(int num_edges);
void set_edges(Vertex *v, Edge *edges, int num_edges);
Edge *get_edge(Vertex *v, int i);
Edge get_edge_cost(Vertex *v, int i);
void set_edge_cost(Edge *edge, Edge cost);
Edge *next_edge(Vertex *v, Edge *current);

void fill_edges(Vertex *v);
void zero_self_costs(Graph *g);


