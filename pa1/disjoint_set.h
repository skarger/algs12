
typedef struct disjoint_set Disjoint_Set;
typedef struct ds_item DSItem;

struct disjoint_set {
    DSItem *items;
    int num_sets;
};

struct ds_item {
    DSItem *p;
    int rank;
};

/* function prototypes */
Disjoint_Set *create_disjoint_set(int num_items);
int get_num_sets(Disjoint_Set *ds);

void makeset(DSItem *x);
DSItem *find(DSItem *x);
DSItem *link(Disjoint_Set *ds, DSItem *x, DSItem *y);
void union_ds(Disjoint_Set *ds, DSItem *x, DSItem *y);

DSItem *create_items(int num_items);
DSItem *get_items(Disjoint_Set *ds);
void increment_num_sets(Disjoint_Set *ds);
void decrement_num_sets(Disjoint_Set *ds);

void set_parent(DSItem *x, DSItem *p);
DSItem *get_parent(DSItem *x);
void set_rank(DSItem *x, int rank);
int get_rank(DSItem *x);

