// TYPE_CHECKING,CODE_GENERATION
public class J1_typecheck_array {

    public J1_typecheck_array () {}

    public static int test() {
	// array creation
        byte[] bs = new byte[(byte)1];
	short[] ss = new short[(short)1];
	char[] cs = new char['z'];
	int[] is = new int[123];
	String[] strs = new String[1];
	Object[] objs = strs;

	// array assignment
	bs[(byte)0] = (byte)1;
	ss[(short)0] = (short)1;
	ss[(short)0] = (byte)1;
	cs['a'] = 'a';	
	is[0] = 1;
	is[0] = (byte)1;
	is[0] = (short)1;
	is[0] = (char)1;
	strs[0] = "flim";
	objs[0] = "flam";
	    
	// array lookup
	byte b = bs[(byte)0];
	short s = bs[(short)0];
	int i = bs[0];
	int j = is['a'];
	String str = strs[0];
	Object obj1 = strs[0];

        return 123;
    }

}
