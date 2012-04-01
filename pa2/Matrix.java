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
    private static int STRASSEN_N0 = 655;   // threshold for conventional mult
    private final int M;             // number of rows
    private final int N;             // number of columns
    private final int[][] data;      

    /* shorthand for the intermediate calculations in Strassen's alg */
    private enum SubMat {
        A, B, C, D, E, F, G, H
    }

    private enum PreSumDiff {
        // M = minus, P = plus, so FMH means F minus H, etc.
        FMH, APB, CPD, GME, APD, EPH, BMD, GPH, AMC, EPF
    }

    private enum PiProduct {
        P1, P2, P3, P4, P5, P6, P7
    }

    private enum PostSum {
        AEPBG, AFPBH, CEPDG, CFPDH
    }


    /* methods */
    public static void setCutoff(int cu) {
        STRASSEN_N0 = cu;
    }

    public static int getCutoff() {
        return STRASSEN_N0;
    }

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
        int dim = mM3.rowDim();

        if (dim < STRASSEN_N0) {
            _multiply(M1, mM1, M2, mM2, M3, mM3);
            return;
        }

        // zero out the storage from the previous run
        strg.clear(dim);

        // compute coordinates for the sub-operations
        Hashtable<Enum,MxMap> maps = computeMaps(mM1, mM2, mM3);

        // compute the first 10 sums/differences
        for (PreSumDiff psd : PreSumDiff.values())
            computeSumDiff(M1, M2, dim, maps, strg, psd);

        // compute P1 - P7
        for (PiProduct pp : PiProduct.values())
            computePiProduct(M1, M2, dim, maps, strg, pp);
        
        // compute final 4 sums on M3
        for (PostSum ps : PostSum.values())
            computePostSum(M3, dim, maps, strg, ps);

    }

    private void computeSumDiff(Matrix M1, Matrix M2, int dim, Hashtable maps,
                                  StrassenStorage strg, PreSumDiff psd) {
        SubMat A = SubMat.A;
        SubMat B = SubMat.B;
        SubMat C = SubMat.C;
        SubMat D = SubMat.D;
        SubMat E = SubMat.E;
        SubMat F = SubMat.F;
        SubMat G = SubMat.G;
        SubMat H = SubMat.H;

        PreSumDiff FMH = PreSumDiff.FMH;
        PreSumDiff APB = PreSumDiff.APB;
        PreSumDiff CPD = PreSumDiff.CPD;
        PreSumDiff GME = PreSumDiff.GME;
        PreSumDiff APD = PreSumDiff.APD;
        PreSumDiff EPH = PreSumDiff.EPH;
        PreSumDiff BMD = PreSumDiff.BMD;
        PreSumDiff GPH = PreSumDiff.GPH;
        PreSumDiff AMC = PreSumDiff.AMC;
        PreSumDiff EPF = PreSumDiff.EPF;

        switch(psd) {
            case FMH:
                _minus(M2, (MxMap) maps.get(F), M2, (MxMap) maps.get(H),
                     (Matrix) strg.getStorage(dim, FMH), (MxMap) maps.get(FMH));
                break;
            case APB:
                _plus(M1, (MxMap) maps.get(A), M1, (MxMap) maps.get(B),
                     (Matrix) strg.getStorage(dim, APB), (MxMap) maps.get(APB));
                break; 
            case CPD:
                _plus(M1, (MxMap) maps.get(C), M1, (MxMap) maps.get(D),
                     (Matrix) strg.getStorage(dim, CPD), (MxMap) maps.get(CPD));
                break; 
            case GME:
                _minus(M2, (MxMap) maps.get(G), M2, (MxMap) maps.get(E),
                     (Matrix) strg.getStorage(dim, GME), (MxMap) maps.get(GME));
                break;
            case APD:
                _plus(M1, (MxMap) maps.get(A), M1, (MxMap) maps.get(D),
                     (Matrix) strg.getStorage(dim, APD), (MxMap) maps.get(APD));
                break; 
            case EPH:
                _plus(M2, (MxMap) maps.get(E), M2, (MxMap) maps.get(H),
                     (Matrix) strg.getStorage(dim, EPH), (MxMap) maps.get(EPH));
                break;
            case BMD:
                _minus(M1, (MxMap) maps.get(B), M1, (MxMap) maps.get(D),
                     (Matrix) strg.getStorage(dim, BMD), (MxMap) maps.get(BMD));
                break; 
            case GPH:
                _plus(M2, (MxMap) maps.get(G), M2, (MxMap) maps.get(H),
                     (Matrix) strg.getStorage(dim, GPH), (MxMap) maps.get(GPH));
                break;
            case AMC:
                _minus(M1, (MxMap) maps.get(A), M1, (MxMap) maps.get(C),
                     (Matrix) strg.getStorage(dim, AMC), (MxMap) maps.get(AMC));
                break;
            case EPF:
                _plus(M2, (MxMap) maps.get(E), M2, (MxMap) maps.get(F),
                     (Matrix) strg.getStorage(dim, EPF), (MxMap) maps.get(EPF));
                break;
            default:
                throw new RuntimeException("computeSumDiff: "+psd+" invalid");
        }

    }

    // makes recursive _strassen calls
    private void computePiProduct(Matrix M1, Matrix M2, int dim, Hashtable maps,
                                  StrassenStorage strg, PiProduct pp) {
        SubMat A = SubMat.A;
        SubMat B = SubMat.B;
        SubMat C = SubMat.C;
        SubMat D = SubMat.D;
        SubMat E = SubMat.E;
        SubMat F = SubMat.F;
        SubMat G = SubMat.G;
        SubMat H = SubMat.H;

        PreSumDiff FMH = PreSumDiff.FMH;
        PreSumDiff APB = PreSumDiff.APB;
        PreSumDiff CPD = PreSumDiff.CPD;
        PreSumDiff GME = PreSumDiff.GME;
        PreSumDiff APD = PreSumDiff.APD;
        PreSumDiff EPH = PreSumDiff.EPH;
        PreSumDiff BMD = PreSumDiff.BMD;
        PreSumDiff GPH = PreSumDiff.GPH;
        PreSumDiff AMC = PreSumDiff.AMC;
        PreSumDiff EPF = PreSumDiff.EPF;

        PiProduct P1 = PiProduct.P1;
        PiProduct P2 = PiProduct.P2;
        PiProduct P3 = PiProduct.P3;
        PiProduct P4 = PiProduct.P4;
        PiProduct P5 = PiProduct.P5;
        PiProduct P6 = PiProduct.P6;
        PiProduct P7 = PiProduct.P7;

        switch (pp) {
            case P1:
                _strassen(M1, (MxMap) maps.get(A),
                          (Matrix) strg.getStorage(dim, FMH),
                          (MxMap) maps.get(FMH),
                          (Matrix) strg.getStorage(dim,P1),
                          (MxMap) maps.get(P1), strg);
                break;
            case P2:
                _strassen((Matrix) strg.getStorage(dim, APB),
                          (MxMap) maps.get(APB),
                          M2, (MxMap) maps.get(H),
                          (Matrix) strg.getStorage(dim, P2),
                          (MxMap) maps.get(P2), strg);
                break;
            case P3:
                _strassen((Matrix) strg.getStorage(dim, CPD),
                          (MxMap) maps.get(CPD),
                          M2, (MxMap) maps.get(E),
                          (Matrix) strg.getStorage(dim, P3),
                          (MxMap) maps.get(P3), strg);
                break;
            case P4:
                _strassen(M1, (MxMap) maps.get(D),
                          (Matrix) strg.getStorage(dim, GME),
                          (MxMap) maps.get(GME),
                          (Matrix) strg.getStorage(dim,P4),
                          (MxMap) maps.get(P4), strg);
                break;
            case P5:
                _strassen((Matrix) strg.getStorage(dim, APD),
                          (MxMap) maps.get(APD),
                          (Matrix) strg.getStorage(dim, EPH),
                          (MxMap) maps.get(EPH),
                          (Matrix) strg.getStorage(dim, P5),
                          (MxMap) maps.get(P5), strg);
                break;
            case P6:
                _strassen((Matrix) strg.getStorage(dim, BMD),
                          (MxMap) maps.get(BMD),
                          (Matrix) strg.getStorage(dim, GPH),
                          (MxMap) maps.get(GPH),
                          (Matrix) strg.getStorage(dim, P6),
                          (MxMap) maps.get(P6), strg);
                break;
            case P7:
                _strassen((Matrix) strg.getStorage(dim, AMC),
                          (MxMap) maps.get(AMC),
                          (Matrix) strg.getStorage(dim, EPF),
                          (MxMap) maps.get(EPF),
                          (Matrix) strg.getStorage(dim, P7),
                          (MxMap) maps.get(P7), strg);
                break;
            default:
                throw new RuntimeException("computePiProduct: "+pp+" invalid");
        }
    }

    private void computePostSum(Matrix M3, int dim, Hashtable maps,
                                StrassenStorage strg, PostSum ps) {
        PiProduct P1 = PiProduct.P1;
        PiProduct P2 = PiProduct.P2;
        PiProduct P3 = PiProduct.P3;
        PiProduct P4 = PiProduct.P4;
        PiProduct P5 = PiProduct.P5;
        PiProduct P6 = PiProduct.P6;
        PiProduct P7 = PiProduct.P7;

        PostSum AEPBG = PostSum.AEPBG;
        PostSum AFPBH = PostSum.AFPBH;
        PostSum CEPDG = PostSum.CEPDG;
        PostSum CFPDH = PostSum.CFPDH;

        switch (ps) {
            case AEPBG:
                _plus(M3, (MxMap) maps.get(AEPBG),
                      (Matrix) strg.getStorage(dim, P5), (MxMap) maps.get(P5),
                      M3, (MxMap) maps.get(AEPBG));
                _plus(M3, (MxMap) maps.get(AEPBG),
                      (Matrix) strg.getStorage(dim, P4), (MxMap) maps.get(P4),
                      M3, (MxMap) maps.get(AEPBG));
                _minus(M3, (MxMap) maps.get(AEPBG),
                      (Matrix) strg.getStorage(dim, P2), (MxMap) maps.get(P2),
                      M3, (MxMap) maps.get(AEPBG));
                _plus(M3, (MxMap) maps.get(AEPBG),
                      (Matrix) strg.getStorage(dim, P6), (MxMap) maps.get(P6),
                      M3, (MxMap) maps.get(AEPBG));
                break;
            case AFPBH:
                _plus(M3, (MxMap) maps.get(AFPBH),
                      (Matrix) strg.getStorage(dim, P1), (MxMap) maps.get(P1),
                      M3, (MxMap) maps.get(AFPBH));
                _plus(M3, (MxMap) maps.get(AFPBH),
                      (Matrix) strg.getStorage(dim, P2), (MxMap) maps.get(P2),
                      M3, (MxMap) maps.get(AFPBH));
                break;
            case CEPDG:
                _plus(M3, (MxMap) maps.get(CEPDG),
                      (Matrix) strg.getStorage(dim, P3), (MxMap) maps.get(P3),
                      M3, (MxMap) maps.get(CEPDG));
                _plus(M3, (MxMap) maps.get(CEPDG),
                      (Matrix) strg.getStorage(dim, P4), (MxMap) maps.get(P4),
                      M3, (MxMap) maps.get(CEPDG));
                break;
            case CFPDH:
                _plus(M3, (MxMap) maps.get(CFPDH),
                      (Matrix) strg.getStorage(dim, P5), (MxMap) maps.get(P5),
                      M3, (MxMap) maps.get(CFPDH));
                _plus(M3, (MxMap) maps.get(CFPDH),
                      (Matrix) strg.getStorage(dim, P1), (MxMap) maps.get(P1),
                      M3, (MxMap) maps.get(CFPDH));
                _minus(M3, (MxMap) maps.get(CFPDH),
                      (Matrix) strg.getStorage(dim, P3), (MxMap) maps.get(P3),
                      M3, (MxMap) maps.get(CFPDH));
                _minus(M3, (MxMap) maps.get(CFPDH),
                      (Matrix) strg.getStorage(dim, P7), (MxMap) maps.get(P7),
                      M3, (MxMap) maps.get(CFPDH));
                break;
            default:
                throw new RuntimeException("computePostSum: "+ps+" invalid");
        }
    }

    private Hashtable<Enum,MxMap> computeMaps(MxMap mM1, MxMap mM2, MxMap mM3) {
        int d = mM1.rowDim(); // assume that dimensions are equal
        int d_2 = d/2;

        // 8 sub-matrices of the two being multiplied
        MxMap mA = new MxMap(d_2, d_2, mM1.startRow(), mM1.startCol());
        MxMap mB = new MxMap(d_2, d_2, mM1.startRow(), mM1.startCol() + d_2);
        MxMap mC = new MxMap(d_2, d_2, mM1.startRow() + d_2, mM1.startCol());
        MxMap mD = new MxMap(d_2, d_2,
                              mM1.startRow() + d_2, mM1.startCol() + d_2);
        MxMap mE = new MxMap(d_2, d_2, mM2.startRow(), mM2.startCol());
        MxMap mF = new MxMap(d_2, d_2, mM2.startRow(), mM2.startCol() + d_2);
        MxMap mG = new MxMap(d_2, d_2, mM2.startRow() + d_2, mM2.startCol());
        MxMap mH = new MxMap(d_2, d_2,
                              mM2.startRow() + d_2, mM2.startCol() + d_2);

        // The 10 first sum/differences and 7 Pi Products have their own storage
        MxMap mF10 = new MxMap(d_2, d_2, 0, 0);
        MxMap mPi = new MxMap(d_2, d_2, 0, 0);

        // 4 post sums are on the destination matrix
        MxMap mAEPBG = new MxMap(d_2,d_2, mM3.startRow(), mM3.startCol());
        MxMap mAFPBH = new MxMap(d_2,d_2, mM3.startRow(), mM3.startCol() + d_2);
        MxMap mCEPDG = new MxMap(d_2,d_2, mM3.startRow() + d_2, mM3.startCol());
        MxMap mCFPDH = new MxMap(d_2,d_2, 
                           mM3.startRow() + d_2, mM3.startCol() + d_2);

        // load the maps into a Hashtable for the caller to use
        Hashtable<Enum,MxMap> maps = new Hashtable<Enum,MxMap>();

        maps.put(SubMat.A, mA);
        maps.put(SubMat.B, mB);
        maps.put(SubMat.C, mC);
        maps.put(SubMat.D, mD);
        maps.put(SubMat.E, mE);
        maps.put(SubMat.F, mF);
        maps.put(SubMat.G, mG);
        maps.put(SubMat.H, mH);

        for (PreSumDiff psd : PreSumDiff.values())
            maps.put(psd, mF10);

        for (PiProduct pp : PiProduct.values())
            maps.put(pp, mPi);

        maps.put(PostSum.AEPBG, mAEPBG);
        maps.put(PostSum.AFPBH, mAFPBH);
        maps.put(PostSum.CEPDG, mCEPDG);
        maps.put(PostSum.CFPDH, mCFPDH);
        
        return maps;
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

        /* set storage for given dimension to zeroes */
        public void clear(int dim) {
            int dim_2 = dim/2;
            Matrix m;
            for (PiProduct pp : PiProduct.values()) {
                m = getStorage(dim, pp);
                for (int i = 0; i < dim_2; i++)
                    for (int j = 0; j < dim_2; j++)
                        m.data[i][j] = 0;
            }
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

        int ard = mA.rowDim();
        int acd = mA.colDim();
        int brd = mB.rowDim();

        if (acd != brd)
            throw new RuntimeException("Illegal matrix dimensions");

        int asr = mA.startRow();
        int asc = mA.startCol();
        int bsr = mB.startRow();
        int bsc = mB.startCol();
        int csr = mC.startRow();
        int csc = mC.startCol();

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

        int asr = mA.startRow();
        int asc = mA.startCol();
        int bsr = mB.startRow();
        int bsc = mB.startCol();
        int csr = mC.startRow();
        int csc = mC.startCol();

        for (int i = 0; i < mA.rowDim(); i++)
            for (int j = 0; j < mA.colDim(); j++) {
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

        int asr = mA.startRow();
        int asc = mA.startCol();
        int bsr = mB.startRow();
        int bsc = mB.startCol();
        int csr = mC.startRow();
        int csc = mC.startCol();

        for (int i = 0; i < mA.rowDim(); i++)
            for (int j = 0; j < mA.colDim(); j++) {
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

    public void showDiagonal() {
        for (int i = 0; i < M; i++)
            System.out.printf("%d \n", data[i][i]);
        System.out.println(); // trailing newline
    }

    // return logarithm base 2 of input
    private static double log2(double x) {
        return ( Math.log(x) / Math.log(2) );
    }




    /* testing */
    public void createStrassenStorage() {
        StrassenStorage strg = new StrassenStorage(1024);
        Matrix m1 = strg.getStorage(1024, PreSumDiff.FMH);
        Matrix m2 = strg.getStorage(512, PiProduct.P5);
    }

    public void testComputeMaps() {
		MxMap mM1 = new MxMap(8, 8, 0, 0);
		MxMap mM2 = new MxMap(8, 8, 0, 0);
		MxMap mM3 = new MxMap(8, 8, 0, 0);
        Hashtable<Enum,MxMap> maps = computeMaps(mM1, mM2, mM3);
        for(SubMat sm : SubMat.values())
            System.out.println(sm + " " + maps.get(sm));
        for(PiProduct pp : PiProduct.values())
            System.out.println(pp + " " + maps.get(pp));
        for(PreSumDiff psd : PreSumDiff.values())
            System.out.println(psd + " " + maps.get(psd));
        for(PostSum ps : PostSum.values())
            System.out.println(ps + " " + maps.get(ps));
    }

    public void testComputation() {
        int dim = 16;
//        int[][] d1 = {{1,0,1,0},{0,1,0,1},{1,0,1,0},{0,1,0,1}};
//        int[][] d2 = {{1,0,1,0},{0,1,0,1},{1,0,1,0},{0,1,0,1}};
//        int[][] d1 = {{1,0},{0,1},{1,0},{0,1}};
//        int[][] d2 = {{1,0},{0,1},{1,0},{0,1}};
//        int[][] d1 = {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};
//        int[][] d2 = {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};
//        int[][] d1 = {{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1}};
//        int[][] d2 = {{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1}};
//        int[][] d1 = {{1,0,1,0,1,0,1,0},{0,1,0,1,0,1,0,1},{1,0,1,0,1,0,1,0},{0,1,0,1,0,1,0,1},{1,0,1,0,1,0,1,0},{0,1,0,1,0,1,0,1},{1,0,1,0,1,0,1,0},{0,1,0,1,0,1,0,1}};
//        int[][] d2 = {{1,0,1,0,1,0,1,0},{0,1,0,1,0,1,0,1},{1,0,1,0,1,0,1,0},{0,1,0,1,0,1,0,1},{1,0,1,0,1,0,1,0},{0,1,0,1,0,1,0,1},{1,0,1,0,1,0,1,0},{0,1,0,1,0,1,0,1}};

        int[][] d1 = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};
        int[][] d2 = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

        int[][] d3 = new int[dim][dim];

        Matrix M1 = new Matrix(dim,dim,d1);
        Matrix M2 = new Matrix(dim,dim,d2);
        Matrix M3 = new Matrix(dim,dim,d3);
        StrassenStorage strg = new StrassenStorage(dim);
		MxMap mM1 = new MxMap(dim, dim, 0, 0);
		MxMap mM2 = new MxMap(dim, dim, 0, 0);
		MxMap mM3 = new MxMap(dim, dim, 0, 0);
        Hashtable<Enum,MxMap> maps = computeMaps(mM1, mM2, mM3);

        _strassen(M1, mM1, M2, mM2, M3, mM3, strg);
        M1.show();
        System.out.println();
        M2.show();
        System.out.println();
        M3.show();
    }

    public static void main(String args[]) {
        Matrix m1 = new Matrix(2,2);
//        m1.createStrassenStorage();
//        m1.testComputeMaps();
        m1.testComputation();
    }
}

