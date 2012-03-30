
import java.io.*;
import java.util.*;

public class Strassen {

    private static int DEFAULT_DIM = 8;

    /** 
     * 
     */ 
    public static void main(String[] args) throws IOException {

        int dimension = 0;
        try {
            dimension = Integer.parseInt(args[1]);
        } catch (NumberFormatException e) {
            System.out.println("dimension must be an integer: " + args[1]);
            System.exit(2);
        }

        FileReader in = null;               
        in = openInputFile(args[2]);
        Scanner sc = new Scanner(in);

        int mat1[][] = new int[dimension][dimension];
        int mat2[][] = new int[dimension][dimension];
        int mat3[][] = new int[dimension][dimension];
        int mat4[][] = new int[dimension][dimension];

        int check = 0; // to check number of values
        check = check + read_matrix(sc, mat1, dimension);
        check = check + read_matrix(sc, mat2, dimension);
        if (check < 2 * (int) Math.pow(dimension,2) )
            throw new IllegalArgumentException("input file " +
                                    "has too few values");

        sc.close();
        in.close();

        Matrix m1 = new Matrix(dimension, dimension, mat1);
        Matrix m2 = new Matrix(dimension, dimension, mat2);
        m1.show();
        System.out.println();
        m2.show();

        System.out.println();
        Matrix m3 = new Matrix(dimension, dimension, mat3);
/*
		MatrixReference mr1 = new MatrixReference(m1, dimension, 0, 0);
		MatrixReference mr2 = new MatrixReference(m2, dimension, 0, 0);
		MatrixReference mr3 = new MatrixReference(m3, dimension, 0, 0);
*/

		MxMap mr1 = new MxMap(4, 4, 0, 0);
		MxMap mr2 = new MxMap(4, 4, 0, 0);
		MxMap mr3 = new MxMap(4, 4, 0, 0);

        Stopwatch sw = new Stopwatch();
        long tm_mult, tm_mult_con;

        sw.start();
        Matrix._multiply(m1, mr1, m2, mr2, m3);
        sw.stop();
        tm_mult = sw.getElapsedTime();

        m3.show();

        
/*
        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < mat1.length; j++)
                System.out.print(mat1[i][j] + " ");
            System.out.println();
        }
        System.out.println();
        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < mat1.length; j++)
                System.out.print(mat2[i][j] + " ");
            System.out.println();
        }
        System.out.println();
*/
    }

    private static int read_matrix(Scanner sc, int[][] mat, int dimension) {
        int i, j, cnt = 0;
        for (i = 0; i < dimension; i++) {
            j = 0;
            while(sc.hasNextInt() && j < dimension) {
                mat[i][j++] = sc.nextInt();
                cnt++;
            }
        }
        return cnt;
    }

    /**
     * openInputFile - create a FileReader object for filename
     */
    private static FileReader openInputFile(String filename) {
        FileReader in = null;
        try {
            in = new FileReader(filename);
            return in;
        } catch (FileNotFoundException e) {
            System.out.println("Can't open file " + filename);
            System.exit(1);
        }
        return in;
    }

}

