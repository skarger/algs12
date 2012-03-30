
/*
* Class representing a matrix.
* Includes a subset of typical matrix operations.
*
* This Matrix class uses or draws inspiration from 
* various parts of the Matrix class defined here:
* http://introcs.cs.princeton.edu/java/95linear/Matrix.java.html
*/
public class Matrix {
    private final int M;             // number of rows
    private final int N;             // number of columns
    private final int[][] data;      

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

    // return C = A * B
    public Matrix multiply(Matrix B) {
        int[][] data = new int[this.M][B.N];
        Matrix C = new Matrix(this.M, B.N, data);
        MxMap mA = new MxMap(this.M, this.N, 0, 0);
        MxMap mB = new MxMap(B.M, B.N, 0, 0);
        _multiply(this, mA, B, mB, C);
        return C;
    }

    // helper method to populate C = A * B
    // note that caller must provide destination storage and
    // maps telling what parts of matrices to multiply
    public static void _multiply(Matrix A, MxMap mA, Matrix B, MxMap mB,
                                    Matrix C) {
        int mard = mA.getRowDim();
        int macd = mA.getColDim();
        int mbrd = mB.getRowDim();

        if (macd != mbrd)
            throw new RuntimeException("Illegal matrix dimensions");

        int asr = mA.getStartRow();
        int asc = mA.getStartCol();
        int bsr = mB.getStartRow();
        int bsc = mB.getStartCol();
        for (int i = 0; i < mard; i++)
            for (int j = 0; j < mbrd; j++)
                for (int k = 0; k < macd; k++) {
                    // superior cache performance path:
                    C.data[i][k] +=
                    (A.data[asr + i][asc + j] * B.data[bsr + j][bsc + k]);
                }
    }

    // return C = A + B
    public Matrix plus(Matrix B) {
        Matrix A = this;
        if (B.M != A.M || B.N != A.N)
            throw new RuntimeException("Illegal matrix dimensions.");
        Matrix C = new Matrix(M, N);
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                C.data[i][j] = A.data[i][j] + B.data[i][j];
        return C;
    }


    // return C = A - B
    public Matrix minus(Matrix B) {
        Matrix A = this;
        if (B.M != A.M || B.N != A.N)
            throw new RuntimeException("Illegal matrix dimensions.");
        Matrix C = new Matrix(M, N);
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                C.data[i][j] = A.data[i][j] - B.data[i][j];
        return C;
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
    


}
