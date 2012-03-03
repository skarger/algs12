
Graph *create_random_graph(int dim, int num_vertices);
void make_cube_edge_weights(Graph *g, int dim);
void set_euclidean_edge_weights(Vertex *v, Vertex *w);
void set_random_coordinates(Graph *g, int dim);
void make_interval_edge_weights(Graph *g);
void load_graph(Graph *g, int dimension);
void copy_symmetric_edge_costs(Graph *g);

