
import java.util.*;

public class Partition {
    private final long[] data;        // the original set of numbers
    private final int numItems;       // the number of numbers

    private int[] solution;           // +1,-1 values or pre-partition indices
    private int[] otherSolution;      // used for comparing residues

    private long[] ppData;            // mutable pre-partition of the numbers
    private long[] otherPPData;       // used for comparing residues

    private int iNeighbor, jNeighbor; // indices changed to obtain neighbor
    private long sumA1, sumA2;        // sums of the respective subsets

    private enum SolutionType { UNDEFINED, STANDARD, PREPARTITION };
    private SolutionType solType;     // flag for standard or pre-partition


    /*
     * Partition - create a partition object with the passed data values
     *
     * In this assignment numItems will always equal the length of data.
     * It is parametrized because potentially we would solve the
     * Number Partion problem on a sub-range of data.
     */
    public Partition(int numItems, long[] data) {
        if (numItems > data.length) {
            throw new IllegalArgumentException("Partition: " + 
                                     "numItems > data length");
        }
        if (numItems != data.length) {
            System.out.println("Partition: warning: numItems != data length");
        }

        this.numItems = numItems;
        this.data = data;
        solution = new int[numItems];
        otherSolution = new int[numItems];
        ppData = new long[numItems];
        otherPPData = new long[numItems];
        solType = SolutionType.UNDEFINED;
    }

    /*
     * initialStandardSolution - set this solution to a random partition
     * composed of +1 and -1 values.
     * Designed to be used when initially creating or re-setting a Partition.
     */
    public void initialStandardSolution() {
        solType = SolutionType.STANDARD;
        randomStandardSolution();
        moveToOtherSolution();
        sumSubsets();
    }

    /*
     * randomStandardSolution - generate solution composed of +1 and -1 values.
     */
    public void randomStandardSolution() {
        plusMinusPartition(otherSolution);
    }

    /*
     * moveToOtherSolution - set solution to be the values that
     * have been generated by randomStandardSolution
     */
    public void moveToOtherSolution() {
        if (solType == SolutionType.UNDEFINED) {
            throw new RuntimeException
                ("moveToOtherSolution: solution undefined");
        }

        System.arraycopy(otherSolution, 0, solution, 0, this.numItems);

        if (solType == SolutionType.PREPARTITION)
            System.arraycopy(otherPPData, 0, ppData, 0, this.numItems);
    }

    /*
     * initialPPSolution - set this solution using a random pre-partition
     */
    public void initialPPSolution() {
        solType = SolutionType.PREPARTITION;
        randomPPSolution();
        moveToOtherSolution();
    }

    /*
     * randomPPSolution - generate random solution by pre-partitioning
     */
    public void randomPPSolution() {
        ppIndices(otherSolution);
        applyPP(otherPPData, otherSolution);
    }

    /*
     * residue - return the residue resulting from this solution
     */
    public long residue() {
        if (solType == SolutionType.STANDARD)
            return Math.abs(sumA1 + sumA2);
        else if (solType == SolutionType.PREPARTITION)
            return KarmarkarKarp.KK(this.ppData, this.numItems);
        else
            throw new RuntimeException ("residue: solution undefined");
    }

    /*
     * createRandomNeighbor - create a neighbor to this solution
     */
    public void createRandomNeighbor() {
        if (solType == SolutionType.STANDARD)
            randomStandardNeighbor(); 
        else if (solType == SolutionType.PREPARTITION)
            randomPPNeighbor(); 
        else
            throw new RuntimeException ("createRandomNeighbor: solution undefined");
    }

    /*
     * neighborResidue - return the residue resulting from this solution's
     * neighbor, which must be already created by createRandomNeighbor
     */
    public long neighborResidue() {
        if (solType == SolutionType.STANDARD)
            return standardNeighborResidue();
        else if (solType == SolutionType.PREPARTITION)
            return ppNeighborResidue();
        else
            throw new RuntimeException ("neighborResidue: solution undefined");
    }

    /*
     * moveToNeighbor - move the current solution to the neighbor that was 
     * created by createRandomNeighbor
     */
    public void moveToNeighbor() {
        if (solType == SolutionType.STANDARD) {
            int i = iNeighbor, j = jNeighbor;
            // need to update sums before updating the solution value itself
            updateSums(i, solution[i], otherSolution[i]);
            solution[i] = otherSolution[i];

            updateSums(j, solution[j], otherSolution[j]);
            solution[j] = otherSolution[j];
        } else if (solType == SolutionType.PREPARTITION) {
            //
        } else
            throw new RuntimeException ("moveToNeighbor: solution undefined");
    }

    /*
     * randomStandardNeighbor - generate a random neigbor to a standard soln.
     * does not update solution array
     */
    private void randomStandardNeighbor() {
        // pick two indices
        int i = randomRange(numItems);
        int j;
        do {
            j = randomRange(numItems);
        } while (i == j);

        iNeighbor = i; jNeighbor = j;
        // switch i, maybe switch j
        otherSolution[i] = -1 * solution[i];
        otherSolution[j] = randomPosNeg1() * solution[j];
    }

    /*
     * standardNeighborResidue - calculate the residue resulting from the 
     * (already created) neighbor to this solution
     */
    private long standardNeighborResidue() {
            long tempSum1 = sumA1;
            long tempSum2 = sumA2;

            int idx = iNeighbor;
            int oldVal = solution[idx];
            int newVal = otherSolution[idx];
            long dlt = delta(idx, oldVal, newVal);
            tempSum1 = tempSum1 + dlt;
            tempSum2 = tempSum2 + dlt;

            idx = jNeighbor;
            oldVal = solution[idx];
            newVal = otherSolution[idx];
            dlt = delta(idx, oldVal, newVal);
            tempSum1 = tempSum1 + dlt;
            tempSum2 = tempSum2 + dlt;

            return Math.abs(tempSum1 + tempSum2);
    }

    /*
     * delta - given an index, its original value and a new value, 
     * return the signed amount by which sums should change
     */
    private long delta(int idx, int oldVal, int newVal) {
        if (oldVal == -1 && newVal == 1)
            return data[idx];
        else if (oldVal == 1 && newVal == -1)
            return (-1 * data[idx]);
        else
            return 0;
    }

    /*
     * updateSums - update this solution's two subset sum values
     */
    private void updateSums(int idx, int oldVal, int newVal) {
        long dlt = delta(idx, oldVal, newVal);
        sumA1 += dlt;
        sumA2 += dlt;
    }

    private void randomPPNeighbor() {
        return;
    }

    private long ppNeighborResidue() {
        return 0;
    }

    /*
     * plusMinusPartition - create a partition composed of +1 and -1 assignments
     * populates the passed array
     */
    private void plusMinusPartition(int[] arr) {
        for (int i = 0; i < this.numItems; i++) {
            arr[i] = randomPosNeg1();
        }
    }

    /*
     * sumSubsets - add up the positive values into sumA1 and
     * the negative values into sumA2
     * only makes sense to call when solution is +/- 1 values
     */
    private void sumSubsets() {
        sumA1 = sumA2 = 0;
        for (int i = 0; i < this.numItems; i++) {
            if (solution[i] == 1)
                sumA1 += data[i];
            else
                sumA2 -= data[i];
        }
    }

    /*
     * applyPP - apply pre-partition indexes to create a pre-partitioned set 
     * of numbers based on the original set.
     * resets and populates passed dest array
     */
    private void applyPP(long[] dest, int[] part) {
        for (int i = 0; i < this.numItems; i++) {
            dest[i] = 0;
        }

        for (int j = 0; j < this.numItems; j++) {
            dest[part[j]] = dest[part[j]] + data[j];
        }
    }

    /*
     * ppIndices - create pre-partition indices composed of [0 - n-1] values
     * populates passed array
     */
    private void ppIndices(int[] arr) {
        for (int i = 0; i < this.numItems; i++) {
            arr[i] = randomRange(this.numItems);
        }
    }

    /*
     * randomPosNeg1 - return 1 or -1 with equal probability
     */
    private static int randomPosNeg1() {
        int val = (int) ((Math.random() * 2));
        if (val == 0) val = -1;
        return val;
    }

    /*
     * randomRange - return random value in [0, n-1]
     */
    private static int randomRange(int n) {
        int val = (int) ((Math.random() * n));
        return val;
    }


    public static void main(String[] args) {
        long[] arr = {10, 8, 7, 6, 5};

        Partition p = new Partition(5, arr);


        p.initialStandardSolution();
        for (int i = 0; i < p.numItems; i++) {
            System.out.println(p.solution[i] + " " + p.data[i]);
        }
        System.out.println("sumA1 " + p.sumA1 + " sumA2 " + p.sumA2);
        System.out.println("residue: " + p.residue());

        p.createRandomNeighbor();
        System.out.println("iN " + p.iNeighbor + " jN " + p.jNeighbor);
        System.out.println("neighborResidue: " + p.neighborResidue());

        System.out.println("After moving to neighbor...");
        p.moveToNeighbor();
        for (int i = 0; i < p.numItems; i++) {
            System.out.println(p.solution[i] + " " + p.data[i]);
        }
        System.out.println("residue: " + p.residue());
/*
        System.out.println("---");
        p.randomPPSolution();
        for (int i = 0; i < p.numItems; i++) {
            System.out.println(p.data[i] + " " + p.solution[i] + " " + p.ppData[i]);
        }
        System.out.println("residue: " + p.residue());
        System.out.println("---");

        int [] ts = {0, 1, 1, 3, 4};
        p.solution = ts;
        p.applyPP();
        for (int i = 0; i < p.numItems; i++) {
            System.out.println(p.data[i] + " " + p.solution[i] + " " + p.ppData[i]);
        }
*/

    }
}
