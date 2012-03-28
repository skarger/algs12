/*
 * MatrixReference - data type used for recursive matrix multiplication
 * Combines a reference to a Matrix object, the dimension of the sub-matrix to
 * be multiplied, and the starting row and column of that sub-matrix.
 *
 * Fields are public for convenience.
 */
public class MatrixReference {
	Matrix matrix;
	int dimension;
	int startRow;
	int startCol;
	
	public MatrixReference(Matrix m, int d, int sr, int sc) {
		matrix = m;
		dimension = d;
		startRow = sr;
		startCol = sc;
	}
}