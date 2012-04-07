import java.math.BigInteger;

public class RSATest {

    public static void main(String args[]) {
        BigInteger n = new BigInteger("46947848749720430529628739081");
        BigInteger e = new BigInteger("37267486263679235062064536973");
        String binx = "100011111010011110110110010101000001" +
                      "101101110010101000001100001110111001000001000001";
        BigInteger x = new BigInteger(binx, 2);

        //System.out.println(x.toString(2));
        //System.out.println(x);

        BigInteger eofx = x.modPow(e, n);        
        System.out.println(eofx);
    }
}
