
typedef struct graph Graph;
typedef struct adjacency Adjacency;
typedef struct vertex Vertex;
typedef float EdgeWeight;

struct graph {
    int num_vertices;
    Vertex *adj;
};

struct vertex {
    int id;
    int explored;
    int num_edge_weights;
    EdgeWeight *edge_weights;
    int dimension; // number of coordinates
    float *coord;  // coordinate values
};

Graph *create_graph(int num_vertices);
void destroy_graph(Graph *g);

Vertex *create_adjacency(int num_vertices);
void initialize_vertex(Vertex *v);
Vertex *get_vertex(Graph *g, int i);
Vertex *next_vertex(Graph *g, Vertex *current);

EdgeWeight *create_edge_weights(int num_edge_weights);
void set_edge_weights(Vertex *v, EdgeWeight *edge_weights,int num_edge_weights);
EdgeWeight *get_edge_weight(Vertex *v, int i);
EdgeWeight get_edge_weight_value(EdgeWeight *ep);
void set_edge_weight_value(EdgeWeight *edge_weight, EdgeWeight cost);
EdgeWeight *next_edge_weight(Vertex *v, EdgeWeight *current);

int get_num_vertices(Graph *g);
int get_dimension(Vertex *v);
int get_index(Vertex *v);
int get_num_edge_weights(Vertex *v);
float *get_coordinates(Vertex *v);

void fill_edge_weights(Vertex *v);
void zero_self_costs(Graph *g);


