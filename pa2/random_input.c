#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "utils.h"

#define DIMENSION 2 
#define LOWER_BOUND 0
#define UPPER_BOUND 1

int main(int argc, char *argv[]) {
    if (argc != 2)
        error(1,"usage: random_input <dimension>","");

    FILE *fp = fopen("inputfile", "w+");
    if (fp == NULL)
        error(1,"random_input: could not open file","");

    srandom(time(NULL));
    int i;
    int tot = 2 * pow(atoi(argv[1]), 2);
    for (i = 0; i < tot; i++) {
        fprintf(fp,"%d\n",(int) round(random_float(LOWER_BOUND, UPPER_BOUND)) );
    }

    fclose(fp);
    
    return 0;
}

