#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "disjoint_set.h"

/*
int main() {
    // run some tests 
    int i;
    int num = 10;
    Disjoint_Set *ds = create_disjoint_set(num);
    DSItem *cur = get_items(ds);
    printf("ns: %d\n",get_num_sets(ds));
    for (i=0; i < 10; i++) {
    	makeset(cur);
    	printf("%d cur rank: %d cur: %p cur parent: %p\n",i, get_rank(cur),cur, find(cur));
    	cur++;
    }

    printf("\n");
    cur = get_items(ds);    
    for (i=0; i < 10; i = i+2) {
    	union_ds(ds, cur, cur+1);
    	printf("%d cur rank: %d cur: %p cur parent: %p\n",i, get_rank(cur),cur, get_parent(cur));
    	printf("%d c+1 rank: %d c+1: %p c+1 parent: %p\n",i+1, get_rank(cur+1),cur +1, find(cur +1));
    	cur += 2;
    }  
    printf("ns: %d \n",get_num_sets(ds));
    
    printf("\n");
    cur = get_items(ds);
    for (i=1; i <= 10; i = i+4) {
    	union_ds(ds, cur, cur+2);
    	cur += 3;
    }    
    cur = get_items(ds);	    
    for (i=0; i < 10; i++) {
    	printf("%d cur rank: %d cur: %p cur parent: %p\n",i, get_rank(cur),cur,find(cur));
    	cur++;
    }       
    printf("ns: %d \n",get_num_sets(ds));
    
    printf("\n");
    cur = get_items(ds);
    for (i=1; i <= 10; i = i+10) {
    	union_ds(ds, cur, cur+6);
    	cur += 7;
    }    
    cur = get_items(ds);	    
    for (i=0; i < 10; i++) {
    	printf("%d cur rank: %d cur: %p cur parent: %p\n",i, get_rank(cur),cur,find(cur));
    	cur++;
    }       
    printf("ns: %d \n",get_num_sets(ds));	    

    return 0;
}
*/


/* the primary disjoint set functions */
void makeset(DSItem *x) {
    set_parent(x, x);
    set_rank(x, 0);
}

DSItem *find(DSItem *x) {
    DSItem *p = get_parent(x);
    if (x != p) {
        set_parent(x, find(p));
    }
    return get_parent(x);
}

DSItem *link(Disjoint_Set *ds, DSItem *x, DSItem *y) {
	//printf("x %p y %p\n",x,y);
    int rx = get_rank(x), ry = get_rank(y);
    if (rx > ry) {
        DSItem *temp = x;
        x = y;
        y = temp;
    }

    if (rx == ry)
        set_rank(y, ry+1);
	
	decrement_num_sets(ds);
	
    set_parent(x, y);
    return y;
}

void union_ds(Disjoint_Set *ds, DSItem *x, DSItem *y) {
	if (x == y)
		return;
	if (get_parent(x) == get_parent(y))
		return;
    link(ds, find(x),find(y));
}


/* other functions */
Disjoint_Set *create_disjoint_set(int num_items) {
    Disjoint_Set *ds = malloc(sizeof(Disjoint_Set));
    if (ds == NULL) {
        error(1,"create_disjoint_set: cannot malloc Disjoint_Set","");
    }
    ds->items = create_items(num_items);
    ds->num_sets = num_items;
    return ds;
}

int get_num_sets(Disjoint_Set *ds) {
	return ds->num_sets;
}

DSItem *create_items(int num_items) {
    DSItem *items = malloc(num_items * sizeof(DSItem));
    if (items == NULL) {
        error(1,"create_items: cannot malloc items","");
    }
    return items;
}

DSItem *get_items(Disjoint_Set *ds) {
	return ds->items;
}

void increment_num_sets(Disjoint_Set *ds) {
	ds->num_sets++;
}

void decrement_num_sets(Disjoint_Set *ds) {
	ds->num_sets--;
}

void set_parent(DSItem *x, DSItem *p) {
    x->p = p;
}

DSItem *get_parent(DSItem *x) {
    return x->p;
}

DSItem *get_item_by_index(Disjoint_Set *ds, int idx) {
    DSItem *items = get_items(ds);
    return &items[idx];
}

void set_rank(DSItem *x, int rank) {
    x->rank = rank;
}

int get_rank(DSItem *x) {
    return x->rank;
}
