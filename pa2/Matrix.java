import java.util.Hashtable;

/*
* Class representing a matrix.
* Includes a subset of typical matrix operations and Strassen's algorithm.
*
* This Matrix class uses or draws inspiration from 
* various parts of the Matrix class defined here:
* http://introcs.cs.princeton.edu/java/95linear/Matrix.java.html
*/
public class Matrix {
    int STRASSEN_N0 = 128;          // threshold for conventional mult
    private final int M;             // number of rows
    private final int N;             // number of columns
    private final int[][] data;      

    /* shorthand for the intermediate calculations in Strassen's alg */
    private enum PreSumDiff {
        // M = minus, P = plus, so FMH means F minus H, etc.
        FMH, APB, CPD, GME, APD, EPH, BMD, GPH, AMC, EPF
    }

    private enum PiProduct {
        P1, P2, P3, P4, P5, P6, P7
    }

    private enum PostSumDiff {
        AEPBG, AFPBH, CEPDG, CFPDH
    }


    /* class methods */

    // create matrix, no data
    public Matrix(int M, int N) {
        this.M = M;
        this.N = N;
        data = null;
    }

    // create matrix, refer to existing data
    public Matrix(int M, int N, int[][] data) {
        this.M = M;
        this.N = N;
        this.data = data;
    }

    // OO wrapper for strassen
    // compute C = this * B using Strassen's algorithm
    public Matrix strassen(Matrix B) {
        // only allow square matrices having dimension a power of 2
        Matrix A = this;
        if (A.M != A.N || A.M != B.M || A.N != B.N)
            throw new RuntimeException("strassen: must use square matrices " +
                                       "of equal dimension");
        double logdim = log2( (double) A.M);
        if (logdim != (int) logdim)
            throw new RuntimeException("strassen: dimension must be a " +
                                       "power of 2");

        // create matrix to hold product
        int[][] data = new int[A.M][A.N];
        Matrix C = new Matrix(A.M, A.N, data);

        // create initial maps telling _strassen where to multiply
        MxMap mA = new MxMap(A.M, A.N, 0, 0);
        MxMap mB = new MxMap(B.M, B.N, 0, 0);
        MxMap mC = new MxMap(A.M, A.N, 0, 0);

        // create *all* storage used for intermediate, recursive calculations
        StrassenStorage strg = new StrassenStorage(A.M);

        _strassen(A, mA, B, mB, C, mC, strg);

        return C;
    }

    private void _strassen(Matrix M1, MxMap mM1, Matrix M2, MxMap mM2, 
                           Matrix M3, MxMap mM3, StrassenStorage strg) {
        if (mM3.getRowDim() < STRASSEN_N0) {
            _multiply(M1, mM1, M2, mM2, M3, mM3);
            return;
        }

        // 
    }

    /* 
     * StrassenStorage - data type to store Strassen calculations
     * Consists of a Hashtable of Hashtables
     * The outer HT has keys for each dimension from N to N0
     * Its values are the inner HTs. A given inner HT has keys for 17 
     * intermediate calculations, and values of matrices for storing 
     * those calculations.
     */
    private class StrassenStorage {
        Hashtable<Integer,Hashtable> htStorage;

        public StrassenStorage(int dim) {
            htStorage = new Hashtable<Integer,Hashtable>();
            // dimension i requires blocks of storage with dimension i/2
            // the final set of storage is for dimension N0, so has dim N0 / 2
            for (int i = dim; i >= STRASSEN_N0; i /= 2) {
                Hashtable<Enum,Matrix> htDimension = 
                                        new Hashtable<Enum,Matrix>();
                int sub_i = i/2;

                // for the current dimension need 17 matrices
                // 10 for the initial sums and differences
                for (PreSumDiff psd : PreSumDiff.values()) {
                    int[][] data = new int[sub_i][sub_i];
                    Matrix m = new Matrix(sub_i, sub_i, data);
                    htDimension.put(psd, m);
                }

                // 7 for the products Pi
                for (PiProduct pp : PiProduct.values()) {
                    int[][] data = new int[sub_i][sub_i];
                    Matrix m = new Matrix(sub_i, sub_i, data);
                    htDimension.put(pp, m);
                }

                htStorage.put(i, htDimension);
            }
        }

        /* get a reference to the matrix for a given calcuation and dimension */
        public Matrix getStorage(int dim, PreSumDiff psd) {
            return (Matrix) ((Hashtable) htStorage.get(dim)).get(psd);
        }

        public Matrix getStorage(int dim, PiProduct pp) {
            return (Matrix) ((Hashtable) htStorage.get(dim)).get(pp);
        }
    }

    // return C = A * B
    public Matrix multiply(Matrix B) {
        int[][] data = new int[this.M][B.N];
        Matrix C = new Matrix(this.M, B.N, data);
        MxMap mA = new MxMap(this.M, this.N, 0, 0);
        MxMap mB = new MxMap(B.M, B.N, 0, 0);
        MxMap mC = new MxMap(this.M, B.N, 0, 0);
        _multiply(this, mA, B, mB, C, mC);
        return C;
    }

    // populate C = A * B
    // note that caller must provide destination storage and
    // maps telling what parts of matrices to multiply
    public static void _multiply(Matrix A, MxMap mA, Matrix B, MxMap mB,
                                 Matrix C, MxMap mC) {

        int ard = mA.getRowDim();
        int acd = mA.getColDim();
        int brd = mB.getRowDim();

        if (acd != brd)
            throw new RuntimeException("Illegal matrix dimensions");

        int asr = mA.getStartRow();
        int asc = mA.getStartCol();
        int bsr = mB.getStartRow();
        int bsc = mB.getStartCol();
        int csr = mC.getStartRow();
        int csc = mC.getStartCol();

        for (int i = 0; i < ard; i++)
            for (int j = 0; j < brd; j++)
                for (int k = 0; k < acd; k++) {
                    // superior cache performance path:
                    C.data[csr + i][csc + k] +=
                    (A.data[asr + i][asc + j] * B.data[bsr + j][bsc + k]);
                }
    }

    // return C = A + B
    public Matrix plus(Matrix B) {
        Matrix A = this;
        if (B.M != A.M || B.N != A.N)
            throw new RuntimeException("Illegal matrix dimensions.");
        int[][] data = new int[A.M][A.N];
        Matrix C = new Matrix(M, N, data);
        MxMap mA = new MxMap(this.M, this.N, 0, 0);
        MxMap mB = new MxMap(B.M, B.N, 0, 0);
        MxMap mC = new MxMap(this.M, B.N, 0, 0);
        _plus(A, mA, B, mB, C, mC);
        return C;
    }

    public static void _plus(Matrix A, MxMap mA, Matrix B, MxMap mB,
                             Matrix C, MxMap mC) {

        int asr = mA.getStartRow();
        int asc = mA.getStartCol();
        int bsr = mB.getStartRow();
        int bsc = mB.getStartCol();
        int csr = mC.getStartRow();
        int csc = mC.getStartCol();

        for (int i = 0; i < mA.getRowDim(); i++)
            for (int j = 0; j < mA.getColDim(); j++) {
                C.data[csr + i][csc + j] =
                A.data[asr + i][asc + j] +
                B.data[bsr + i][bsc + j];
            }
    }


    // return C = A - B
    public Matrix minus(Matrix B) {
        Matrix A = this;
        if (B.M != A.M || B.N != A.N)
            throw new RuntimeException("Illegal matrix dimensions.");
        int[][] data = new int[A.M][A.N];
        Matrix C = new Matrix(M, N, data);
        MxMap mA = new MxMap(this.M, this.N, 0, 0);
        MxMap mB = new MxMap(B.M, B.N, 0, 0);
        MxMap mC = new MxMap(this.M, B.N, 0, 0);
        _minus(A, mA, B, mB, C, mC);
        return C;
    }

    public static void _minus(Matrix A, MxMap mA, Matrix B, MxMap mB,
                            Matrix C, MxMap mC) {

        int asr = mA.getStartRow();
        int asc = mA.getStartCol();
        int bsr = mB.getStartRow();
        int bsc = mB.getStartCol();
        int csr = mC.getStartRow();
        int csc = mC.getStartCol();

        for (int i = 0; i < mA.getRowDim(); i++)
            for (int j = 0; j < mA.getColDim(); j++) {
                C.data[csr + i][csc + j] =
                A.data[asr + i][asc + j] -
                B.data[bsr + i][bsc + j];
            }
    }

    // does A = B exactly?
    public boolean eq(Matrix B) {
        Matrix A = this;
        if (B.M != A.M || B.N != A.N) 
            throw new RuntimeException("Illegal matrix dimensions.");
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                if (A.data[i][j] != B.data[i][j]) return false;
        return true;
    }

    // print matrix to standard output
    public void show() {
        if (M > 16) // don't print large matrices
            return;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) 
                System.out.printf("%d ", data[i][j]);
            System.out.println();
        }
    }
    

    // return logarithm base 2 of input
    private static double log2(double x) {
        return ( Math.log(x) / Math.log(2) );
    }

/* dimension shorthand
        int ard = mA.getRowDim();
        int acd = mA.getColDim();
        int brd = mB.getRowDim();
        int bcd = mB.getColDim();
        int crd = mC.getRowDim();
        int ccd = mC.getColDim();
*/

/* start point shorthand
        int asr = mA.getStartRow();
        int asc = mA.getStartCol();
        int bsr = mB.getStartRow();
        int bsc = mB.getStartCol();
        int csr = mC.getStartRow();
        int csc = mC.getStartCol();
*/

/* testing */
    public void createStrassenStorage() {
        StrassenStorage strg = new StrassenStorage(1024);
        Matrix m1 = strg.getStorage(1024, PreSumDiff.FMH);
        Matrix m2 = strg.getStorage(512, PiProduct.P5);
    }

    public static void main(String args[]) {
        Matrix m1 = new Matrix(4,4);
        m1.createStrassenStorage();
    }
}

