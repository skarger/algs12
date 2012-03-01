#include <stdlib.h>

#include "utils.h"

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

void makeset(DSItem *x);
DSItem *find(DSItem *x);
DSItem *link(DSItem *x, DSItem *y);
void union_ds(DSItem *x, DSItem *y);

void set_parent(DSItem *x, DSItem *p);
DSItem *get_parent(DSItem *x);
void set_rank(DSItem *x, int rank);
int get_rank(DSItem *x);


int main() {
    /* run some tests */
    return 0;
}

Disjoint_Set *create_disjoint_set(int num_items) {
    Disjoint_Set *ds = malloc(num_items * sizeof(DSItem));
    if (ds == NULL) {
        error(1,"create_disjoint_set: cannot malloc items","");
    }
    return ds;
}

void makeset(DSItem *x) {
    set_parent(x, x);
    set_rank(x, 0);
}

void set_parent(DSItem *x, DSItem *p) {
    x->p = p;
}

DSItem *get_parent(DSItem *x) {
    return x->p;
}

void set_rank(DSItem *x, int rank) {
    x->rank = rank;
}

int get_rank(DSItem *x) {
    return x->rank;
}

DSItem *find(DSItem *x) {
    DSItem *p = get_parent(x);
    if (x != p) {
        set_parent(x, find(p));
    }
    return get_parent(x);
}

DSItem *link(DSItem *x, DSItem *y) {
    int rx = get_rank(x), ry = get_rank(y);
    if (rx > ry) {
        DSItem *temp = x;
        x = y;
        y = temp;
    }

    if (rx == ry)
        set_rank(y, ry+1);

    set_parent(x, y);
    return y;
}

void union_ds(DSItem *x, DSItem *y) {
    link(find(x),find(y));
}
