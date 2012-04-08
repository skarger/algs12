import java.math.BigInteger;

public class RSATest {

    public static void main(String args[]) {
/*
        BigInteger n = new BigInteger("46947848749720430529628739081");
        BigInteger e = new BigInteger("37267486263679235062064536973");
        String binx = "100011111010011110110110010101000001" +
                      "101101110010101000001100001110111001000001000001";
        BigInteger x = new BigInteger(binx, 2);

        //System.out.println(x.toString(2));
        //System.out.println(x);

        BigInteger eofx = x.modPow(e, n);        
        System.out.println(eofx);
*/

        BigInteger n = new BigInteger("294409");
        BigInteger n_1 = n.subtract(new BigInteger("1"));
        BigInteger a = new BigInteger("2");
        //BigInteger a = new BigInteger("7");
        BigInteger ax;
        BigInteger ax_1;
        BigInteger u = new BigInteger("36801");
        BigInteger two = new BigInteger("2");
        BigInteger one = new BigInteger("1");
        BigInteger negone = new BigInteger("-1");
	while (u.compareTo(n_1) < 0) {
		ax_1 = a.modPow(u,n);	
		System.out.println("u "+ u);
		u = u.multiply(two);
		System.out.println("u "+ u);
		ax = a.modPow(u,n);	
		System.out.println("ax_1 " + ax_1 + " ax " + ax);
		if ((!ax_1.equals(one) && !ax_1.equals(negone)) && ax.equals(one) ) {
			System.out.println("composite: ");
			System.out.println("ax_1 " + ax_1 + " ax " + ax);
		}
	}


    }
}
