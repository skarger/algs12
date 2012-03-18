
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
            dimension = Integer.parseInt(args[2]);
        } catch (NumberFormatException e) {
            System.out.println("dimension must be an integer");
            System.exit(2);
        }

        FileReader in = null;               
        in = openInputFile(args[1]);
        Scanner sc = new Scanner(in);

        int mat1[][] = new int[dimension][dimension];
        int mat2[][] = new int[dimension][dimension];

        int check = 0; // to check number of values
        check = check + read_matrix(sc, mat1, dimension);
        check = check + read_matrix(sc, mat2, dimension);
        if (check < 2 * (int) Math.pow(dimension,2) )
            throw new IllegalArgumentException("input file " +
                                    "has too few values");

        sc.close();
        in.close();

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

