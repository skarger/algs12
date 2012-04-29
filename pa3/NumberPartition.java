/* 
 * test various algorithms to solve the NumberPartition problem
 */
public class NumberPartition {
    private static final int MAX_ITER = 25000;

    private enum SolutionType { STANDARD, PREPARTITION };
    private enum Heuristic { RR, HC, SA };

    public static void main(String[] args) {

        long[] data = NumericDataReader.arrayFromFile("inputfile", 100);
        int numItems = data.length;
    
        String results = "";
        String timeResults = "";
/*
        results = "\\begin{tabular}{l r l r l r l}\n";
        results += " & \\multicolumn{2}{c}{Repeated Random} & ";
        results += "\\multicolumn{2}{c}{Hill Climbing} & ";
        results += "\\multicolumn{2}{c}{Simulated Annealing} \\\\ \\hline\n";
        results += "KK & Standard & PP & Standard & PP & Standard & PP ";
        results += "\\\\ \\hline\n";

        timeResults = new String(results);
*/
        long residue;
        Stopwatch sw = new Stopwatch();
        long tm;

        sw.start();
        residue = KarmarkarKarp.KK(data, numItems);
        sw.stop();
        results += residue;
        tm = sw.getElapsedTime();
        timeResults += tm;
        sw.reset();

        Partition s = new Partition(numItems, data);
        for (Heuristic h : Heuristic.values()) {
            if (h == Heuristic.RR) {
                for (SolutionType st : SolutionType.values()) {
                    sw.start();
                    s = repeatedRandom(s, st);
                    sw.stop();
                    residue = s.residue();
                    results += " & " + residue;
                    tm = sw.getElapsedTime();
                    timeResults += " & "+ tm;
                    sw.reset();
                }
            } else if (h == Heuristic.HC) {
                for (SolutionType st : SolutionType.values()) {
                    sw.start();
                    s = hillClimbing(s, st);
                    sw.stop();
                    residue = s.residue();
                    results += " & " + residue;
                    tm = sw.getElapsedTime();
                    timeResults += " & "+ tm;
                    sw.reset();
                }
            } else {
                for (SolutionType st : SolutionType.values()) {
                    sw.start();
                    s = simulatedAnnealing(s, st);
                    sw.stop();
                    residue = s.residue();
                    results += " & " + residue;
                    tm = sw.getElapsedTime();
                    timeResults += " & "+ tm;
                    sw.reset();
                }
            }
        }
        
        results += " \\\\";
        timeResults += " \\\\ \n";

        System.out.print(results);
        System.out.print(":");
        System.out.print(timeResults);
    }

    public static Partition repeatedRandom(Partition s, SolutionType type) {
        if (type == SolutionType.STANDARD)
            s.initialStandardSolution();
        else 
            s.initialPPSolution();
        
        for (int i = 0; i < MAX_ITER; i++) {
            s.randomSolution();        
            if (s.otherResidue() < s.residue()) {
                s.moveToOtherSolution();
            }
        }
        return s;
    }
  
    public static Partition hillClimbing(Partition s, SolutionType type) {
        if (type == SolutionType.STANDARD)
            s.initialStandardSolution();
        else 
            s.initialPPSolution();
        
        for (int i = 0; i < MAX_ITER; i++) {
            s.createRandomNeighbor();        
            if (s.neighborResidue() < s.residue()) {
                s.moveToNeighbor();
            }
        }
        return s;
    }

    public static Partition simulatedAnnealing(Partition s, SolutionType type) {
        if (type == SolutionType.STANDARD)
            s.initialStandardSolution();
        else 
            s.initialPPSolution();
        
        Partition sDoublePrime = new Partition(s);

        long res, nRes, sdpRes;
        res = s.residue();
        sdpRes = sDoublePrime.residue();

        for (int i = 0; i < MAX_ITER; i++) {
            s.createRandomNeighbor();        
            nRes = s.neighborResidue();
            if (nRes < res) {
                s.moveToNeighbor();
                res = s.residue();
            } else if (Math.random() < coolProb(nRes, res, i+1)) {
                s.moveToNeighbor();
                res = s.residue();
            }

            if (res < sdpRes)
                sDoublePrime = new Partition(s);
                sdpRes = res;
        }

        return s;
    }

    private static double T(int iter) {
        double a = Math.pow(10, 10);
        double b = 0.8;
        double c = (double) iter / 300.0;
        c = Math.floor(c);
        double d = Math.pow(b, c);
        return a * d;
    }

    private static double coolProb(long res1, long res2, int iter) {
        double num = -1 * (double) (res1 - res2);
        double den = (double) T(iter);
        double x = num / den;
        return Math.exp(x);
    }
}

/*
        int count = 0;
        int movedCount = 0;
    count++;
    movedCount++;
    System.out.println("count: " + count + " movedCount: " + movedCount);
*/
