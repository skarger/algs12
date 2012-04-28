
struct seed {
    unsigned long long x,y; /* Seed variables */
    unsigned int z1, c1, z2, c2; /* Seed variables */
};

double random_double(double a, double b);
double *create_double_array(int n);
void error(int errcd, char *msg1, char *msg2);
double euclidean_distance(double *x, double *y, int dimension);
int triangular_number(int n);

/* devrand and KISS RNG built taken (and modified) from 
 * http://www.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
 */
unsigned int devrand(void);
void init_KISS(struct seed *sd);
unsigned long long JLKISS64(struct seed *sd);

