#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "utils.h"

#define NUM_LINES 100
#define LOWER_BOUND 0
#define UPPER_BOUND 1000000000000

int main(int argc, char *argv[]) {

    int tot;
    if (argc == 2) {
        //tot = 2 * pow(atoi(argv[1]), 2);
        tot = atoi(argv[1]);
    }
    else if (argc == 1)
        tot = NUM_LINES;
    else
        error(1,"usage: random_input <num lines>","");

    FILE *fp = fopen("inputfile", "w+");
    if (fp == NULL)
        error(1,"random_input: could not open file","");

    int i;
    struct seed sd;
    unsigned long long ri;
    for (i = 0; i < tot; i++) {
        ri = JLKISS64(&sd) % UPPER_BOUND; /* range [0, 10^12 -1] */
        ri = ri + 1;
        fprintf(fp,"%llu\n", ri);
    }

    fclose(fp);
    long long lg;
    printf("size of lg: %zu\n", sizeof(lg));
    printf("RM %d\n", RAND_MAX);
    return 0;
}

