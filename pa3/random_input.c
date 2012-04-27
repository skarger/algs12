#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "utils.h"

#define NUM_LINES 100
#define LOWER_BOUND 0
#define UPPER_BOUND 10000

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

    srandom(time(NULL));
    int i;
    for (i = 0; i < tot; i++) {
        fprintf(fp,"%d\n",(int) round(random_double(LOWER_BOUND,UPPER_BOUND)) );
    }

    fclose(fp);
    long long lg;
    printf("size of lg: %lld\n", sizeof(lg));
    return 0;
}

