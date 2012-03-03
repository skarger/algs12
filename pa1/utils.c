
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "utils.h"

/*
 * random_float
 * return a random float in the interval [a, b]
 * caller must set random seed
 */
float random_float(float a, float b) {
    if (b < a)
        return 0;
    /* make random number in [0,1] */
    float ran_fraction = (1.0 * random()) / RAND_MAX;
    /* scale it to be in interval [a,b] */
    return (a + (b - a) * ran_fraction);
}

/*
 * create_float_array
 * input: n (positive integer)
 * create a float array of size n
 */
float *create_float_array(int n) {
    if (n < 1)
        return NULL;
    float *fp = malloc(n * sizeof(float));
    if (fp == NULL)
        printf("create_float_array: cannot malloc float array\n");
    return fp; 
}

/*
 * euclidean_distance
 * input: float arrays x and y that have equal length equal to dimension
 * returns: euclidean distance between the points represented by x and y
 */
float euclidean_distance(float *x, float *y, int dimension) {
    float dist2 = 0.0, diff;
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

/*
 * error
 * print an error message and return an error code
 */
void error(int errcd, char *msg1, char *msg2) {
    printf("%s %s",msg1, msg2);
    exit(errcd);
}

/*
int main() {
    float x1[2] = {0.0, 0.0};
    float x2[2] = {1.0, 1.0};

    float d = euclidean_distance(x1, x2, 2);
    printf("sqrt 2: %f\n",sqrt(2));
    printf("ed: %f\n",d);
    return 0;
}
*/
