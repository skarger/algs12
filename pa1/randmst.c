#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"
#include "graph.h"
#include "random_graph.h"
#include "randmst.h"

int main(int argc, char * argv[]) {
    /* input validation */
    if (argc != 5)
        error(1,"usage: randmst 0 numpoints numtrials dimension\n", "");

    // note: atoi returns 0 if input is not an integer
    int flag = atoi(argv[1]);
    int numpoints = atoi(argv[2]);
    int numtrials = atoi(argv[3]);
    int dim = atoi(argv[4]);

    if (!(dim == 0 || dim == 2 || dim == 3 || dim == 4)) {
        error(2,"randmst: dimension must be 0, 2, 3, or 4\n","");
    }

    if (flag != 0)
        printf("flag\n");


    Graph *g = create_random_graph(dim, numpoints);
    double weight[numtrials]; // storage for MST weight

    /* compute MST and weight */

    int i;
    for (i = 0; i < numtrials; i++) {
        // g = ...
        // compute MST and weight
        weight[i] = 1;
    }


    /* report results */
    double avg = 0.0;
    for (i = 0; i < numtrials; i++) {
        avg = avg + weight[i];
    }
    avg = avg / numtrials;

    // output: average numpoints numtrials dimension
    printf("%f %d %d %d\n", avg, numpoints, numtrials, dim);

    return 0;
}

