// CODE_GENERATION
public class J1_sim_xor {
    public J1_sim_xor() {}

    public static int test() {
	J1_sim_xor obj = new J1_sim_xor();
	String s = "kFF\nSE_X\nHKYO\nKXO\nHOFEDM\n^E\n_Y"+(char)11;
	System.out.println(obj.crypt(s, 42));

	String s2 = obj.crypt(s, 6*9);

	
	if (s2.equals((Object)"]pp<esin<~}oy<}ny<~ypsr{<hs<io="))
	    return 123;
	else return 0;
    }

    public String crypt(String org, int key) {
	String sb = "";
	for (int i=0; i<org.length(); i=i+1)
	    sb = sb + ((char)(sim_xor((int)org.charAt(i), key)));
	return sb;
    }


    public int sim_xor(int x, int y) {
	if (x==0 && y==0) return 0;
	boolean bit0 = (x%2 + y%2 == 1);
	int result = sim_xor(x/2, y/2)*2;
	if (bit0) result = result+1;
	return result;
    }
}
