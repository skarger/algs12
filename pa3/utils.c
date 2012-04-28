
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "utils.h"

/*
 * random_double
 * return a random double in the interval [a, b]
 * caller must set random seed
 */
double random_double(double a, double b) {
    if (b < a)
        return 0;
    /* make random number in [0,1] */
    double ran_fraction = (1.0 * random()) / RAND_MAX;
    /* scale it to be in interval [a,b] */
    return (a + (b - a) * ran_fraction);
}

/*
 * create_double_array
 * input: n (positive integer)
 * create a double array of size n
 */
double *create_double_array(int n) {
    if (n < 1)
        return NULL;
    double *fp = malloc(n * sizeof(double));
    if (fp == NULL)
        printf("create_double_array: cannot malloc double array\n");
    return fp; 
}

/*
 * euclidean_distance
 * input: double arrays x and y that have equal length equal to dimension
 * returns: euclidean distance between the points represented by x and y
 */
double euclidean_distance(double *x, double *y, int dimension) {
    double dist2 = 0.0, diff;
    int i;
    for (i = 0; i < dimension; i++) {
        diff = x[i] - y[i];
        dist2 = dist2 + (diff * diff);
    }
    return sqrt(dist2);
}

/*
 * triangular_number
 * return the sum of the integers from 1 to n (the nth triangular number)
 */
int triangular_number(int n) {
    return (n * (n+1)) / 2;
}

/* devrand and KISS RNG built taken (and modified) from 
 * http://www.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
 */
unsigned int devrand(void)
{
    int fn;
    unsigned int r;
    fn = open("/dev/urandom", O_RDONLY);
    if (fn == -1)
        exit(-1); /* Failed! */
    if (read(fn, &r, 4) != 4)
        exit(-1); /* Failed! */
    close(fn);
    return r;
}


void init_KISS(struct seed *sd)
{
    sd->x = devrand();
    sd->x = (sd->x)<<32;
    sd->x = sd->x | devrand(); /* make x 64 bits */
    while (!(sd->y = devrand())); { /* y must not be zero! */
        sd->z1 = devrand();
        sd->z2 = devrand();
    }
    sd->y = (sd->y)<<32; sd->y = sd->y | devrand();

    /* We don’t really need to set c as well but let's anyway */
    /* NOTE: offset c by 1 to avoid z=c=0 */
    sd->c1 = devrand() % 698769068 + 1; /* Should be less than 698769069 */
    sd->c2 = devrand() % 698769068 + 1; /* Should be less than 698769069 */
}

unsigned long long JLKISS64(struct seed *sd)
{ 
    static unsigned long long x,y; /* Seed variables */
    static unsigned int z1, c1, z2, c2; /* Seed variables */
    static int called = 0;
    if (called == 0) {
        x = sd->x, y = sd->y;
        z1 = sd->z1, c1 = sd->c1, c2 = sd->c2, z2 = sd->z2;
    }
    called++;

    unsigned long long t;

    x = 1490024343005336237ULL * x + 123456789;
    y ^= y << 21; y ^= y >> 17; y ^= y << 30; /* Do not set y=0! */
    t = 4294584393ULL * z1 + c1; c1 = t >> 32; z1 = t;
    t = 4246477509ULL * z2 + c2; c2 = t >> 32; z2 = t;
    return x + y + z1 + ((unsigned long long)z2 << 32); /* Return 64-bit result */
}

/*
 * error
 * print an error message and return an error code
 */
void error(int errcd, char *msg1, char *msg2) {
    printf("%s %s\n",msg1, msg2);
    exit(errcd);
}

/*
int main() {
    double x1[2] = {0.0, 0.0};
    double x2[2] = {1.0, 1.0};

    double d = euclidean_distance(x1, x2, 2);
    printf("sqrt 2: %f\n",sqrt(2));
    printf("ed: %f\n",d);

    struct seed sd;
    init_KISS(&sd);
    unsigned long long rn;
    rn = JLKISS64(&sd);
    printf("rn: %llu\n",rn);

    return 0;
}
*/
