#include <stdio.h>
#include <math.h>

void stooge_sort( int arr [], int lower, int upper );
void swap( int arr [], int i, int j );

int main () {
    //int len = 3;
    //int my_arr [] = {3, 2, 1};
    //int len = 0;
    //int my_arr [] = {};
    //int len = 1;
    //int my_arr [] = {55};
    //int len = 2;
    //int my_arr [] = {1, 55};
    //int len = 3;
    //int my_arr [] = {1, 3, 2};
    //int len = 4;
    //int my_arr [] = {80, 1, 3, 2};
    //int len = 5;
    //int my_arr [] = {0, 80, 1, 3, 2};
    //int len = 6;
    //int my_arr [] = {5, 6, 80, 1, 3, 2};
    //int len = 7;
    //int my_arr [] = {5, 6, 80, 79, 1, 3, 2};
    //int len = 8;
    //int my_arr [] = {5, 6, 80, 79, 78, 1, 3, 2};
    int len = 9;
    int my_arr [] = {90, 5, 6, 80, 77, 79, 78, 1, 3};

    int i;
    for (i = 0; i < len; i++)
        printf("%d ", my_arr[i]);

    printf("\n\n");
    stooge_sort(my_arr, 0, len-1);

    for (i = 0; i < len; i++)
        printf("%d ", my_arr[i]);
    
    return 0;
}

void stooge_sort( int arr [], int lower, int upper ) {
    if ( upper < lower )
        return; 

    if ( lower == upper ) // one item, nothing to sort
        return;  

    if ( (upper - lower) == 1 ) {  // two items, sort them
        if ( arr[lower] > arr[upper] )
            swap( arr, lower, upper);
        return;
    }

    // looking at 3+ items

    // calculate cutoff points
    double third = (upper - lower) / 3.0;
    int onethird = (int) floor(lower + third);
    int twothirds = (int) ceil(lower + (2 * third));

    // handle special cases to avoid infinite recursion
    if (onethird == lower) onethird++;
    if (twothirds == upper) twothirds--;

    stooge_sort( arr, lower, twothirds );
    stooge_sort( arr, onethird, upper );
    stooge_sort( arr, lower, twothirds );

    return;
}

void swap( int arr [], int i, int j ) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}
