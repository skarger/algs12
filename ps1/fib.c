/*
 * Problem 2: Three Fibonacci computation methods
 * A macro, DO_MOD, controls whether the methods return the real Fibonacci 
 * numbers or the Fibonacci numbers modulo 2^16.
 * Edit macro value and recompile to switch.
 */

#include <stdio.h>
#include <time.h>

// change to DO_MOD 0 to return unmodifed Fibonacci numbers
#define DO_MOD 1

#if DO_MOD
#define MODULO % 65536
#endif

#ifndef MODULO
#define MODULO 
#endif

int fib_rec(int n);
int fib_iter(int n);
int fib_mat(int n);

/* helper methods */
void raise_fib_matrix(int n, int prodm [2][2]);
void matrix_mult_2x2(int m1 [2][2], int m2 [2][2], int prodm [2][2]);


int main() {
    printf("Fibonacci Demonstration\n\n");
    printf("Warning: no input validation\n\n");

    printf("Which computation method?\n");
    printf("Recursive, Iterative, or Matrix (r|i|m): ");
    char ch;
    scanf("%c", &ch);

    printf("Enter N: ");
    int n;
    scanf("%i", &n);
    
    clock_t start, end;
    double cpu_time_used;
    
    printf("\n");
    if (ch == 'r') {
        start = clock();
        printf("%i", fib_rec(n));
        end = clock();
    }
    else if (ch == 'i') {
        start = clock();
        printf("%i", fib_iter(n));
        end = clock();
    }
    else if (ch == 'm') {
        start = clock();
        printf("%i", fib_mat(n));
        end = clock();
    }
    else {
        printf("Invalid option. Exiting.");
        return 1;
    }

    printf("\n\n");
    printf("clock cycles: %f\n", (double) (end - start));
    printf("CLOCKS_PER_SEC: %d\n", (int) CLOCKS_PER_SEC);
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("cpu time used in seconds : %f \n\n", cpu_time_used);

    return 0;
}

/* 
 * fib_rec - recursively compute the nth Fibonacci number
 */
int fib_rec(int n) {
    if (n == 0 || n == 1)
        return n;
    return (fib_rec(n-1) + fib_rec(n-2)) MODULO;
}

/*
 * fib_iter - iteratively compute the nth Fibonacci number with an array
 */
int fib_iter(int n) {
    int arr[n];
    arr[0] = 0;
    arr[1] = 1;
    int i;
    for (i = 2; i <= n; i++) {
        arr[i] = (arr[i-1] + arr[i-2]) MODULO;
    }
    return arr[n];
}

/*
 * fib_mat - compute nth Fibonacci number using matrix multiplication
 * Note: this function is the same when calculating modulo 2^16.
 * The only change is to matrix_mult_2x2, below.
 */
int fib_mat(int n)  {

    if (n == 0 || n == 1)
        return n;

    // product of fibonacci matrix raised to nth power
    int prodm [2][2];
    raise_fib_matrix(n, prodm);

    // fn = (prodm[0][0] * 0) + (prodm[0][1] * 1);
    int fn = prodm[0][1];
    // do not need to calculate fn+1
    // int fn+1 = (prodm[1][0] * 0) + (prodm[1][1] * 1);
    return fn;
}

/* helper methods for fib_mat */

/* 
 * raise_fib_matrix
 * raise fibonacci matrix to nth power
 * put result in prodm
 */
void raise_fib_matrix(int n, int prodm [2][2]) {
    int fibm [2][2];
    fibm[0][0] = 0;
    fibm[0][1] = 1;
    fibm[1][0] = 1;
    fibm[1][1] = 1;

    /* first populate result with the fibonacci matrix to the 1st power */
    prodm[0][0] = 0;
    prodm[0][1] = 1;
    prodm[1][0] = 1;
    prodm[1][1] = 1;

    /* populate result with fibonacci matrix to the nth power */
    int i;
    for (i = 1; i < n; i++) // does not run when n == 1
        matrix_mult_2x2(fibm, prodm, prodm);

    return;
}

/*
 * matrix_mult_2x2
 * multiply two dimensional arrays m1 and m2 
 * caller must provide prodm in which to store the product
 */
void matrix_mult_2x2(int m1 [2][2], int m2 [2][2], int prodm [2][2]) {
    int a00, a01, a10, a11;
    
    a00 = ( (m1[0][0] * m2[0][0]) + (m1[0][1] * m2[1][0]) ) MODULO;
    a01 = ( (m1[0][0] * m2[0][1]) + (m1[0][1] * m2[1][1]) ) MODULO;
    a10 = ( (m1[1][0] * m2[0][0]) + (m1[1][1] * m2[1][0]) ) MODULO;
    a11 = ( (m1[1][0] * m2[0][1]) + (m1[1][1] * m2[1][1]) ) MODULO;

    prodm[0][0] = a00;
    prodm[0][1] = a01;
    prodm[1][0] = a10;
    prodm[1][1] = a11;

    return;
}

