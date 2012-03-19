
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

    // Populate C = A * B
    // note that caller must provide destination matrix C
    public static void multiply(Matrix A, Matrix B, Matrix C, int M, int N) {
        if (M != N) throw new RuntimeException("Illegal dimensions.");
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                for (int k = 0; k < N; k++) {
                    // conventional path:
                    // C.data[i][j] += (A.data[i][k] * B.data[k][j]);

                    // superior cache performance path:
                    C.data[i][k] += (A.data[i][j] * B.data[j][k]);
                }
    }

    // does A = B exactly?
    public boolean eq(Matrix B) {
        Matrix A = this;
        if (B.M != A.M || B.N != A.N) 
            throw new RuntimeException("Illegal dimensions.");
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
