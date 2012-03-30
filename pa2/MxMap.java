/*
 * MxMap - data type used for recursive matrix arithmetic 
 * Combines the row and column dimensions of the sub-matrix in question
 * and the starting row and column of that sub-matrix.
 *
 */
public class MxMap {
	private int dimM; // row dimension
    private int dimN; // column dimension
	private int startRow;
	private int startCol;
	
	public MxMap(int dm, int dn, int sr, int sc) {
		dimM = dm;
        dimN = dn;
		startRow = sr;
		startCol = sc;
	}

    public int getRowDim() {
        return dimM;
    }

    public int getColDim() {
        return dimN;
    }

    public int getStartRow() {
        return startRow;
    }

    public int getStartCol() {
        return startCol;
    }
}

