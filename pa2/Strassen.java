
import java.io.*;
import java.util.*;

public class Strassen {

    private static int DEFAULT_DIM = 8;

    /** 
     * 
     */ 
    public static void main(String[] args) throws IOException {

        int cutoff = Integer.parseInt(args[0]);
        if (cutoff > 0)
            Matrix.setCutoff(cutoff);

        int dimension = 0;
        try {
            dimension = Integer.parseInt(args[1]);
        } catch (NumberFormatException e) {
            System.out.println("dimension must be an integer: " + args[1]);
            System.exit(2);
        }

        // calculations if dimension != 2^k

        FileReader in = null;               
        in = openInputFile(args[2]);
        Scanner sc = new Scanner(in);

        int data1[][] = new int[dimension][dimension];
        int data2[][] = new int[dimension][dimension];
        int data3[][] = new int[dimension][dimension];
        int data4[][] = new int[dimension][dimension];

        int check = 0; // to check number of values
        check = check + read_matrix(sc, data1, dimension);
        check = check + read_matrix(sc, data2, dimension);
        if (check < 2 * (int) Math.pow(dimension,2) )
            throw new IllegalArgumentException("input file " +
                                    "has too few values");

        sc.close();
        in.close();

        Matrix m1 = new Matrix(dimension, dimension, data1);
        Matrix m2 = new Matrix(dimension, dimension, data2);

        Matrix m3 = new Matrix(dimension, dimension, data3);
        m3 = m1.strassen(m2);
        m3.showDiagonal();

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

