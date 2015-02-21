// TYPE_CHECKING,CODE_GENERATION
public class J1_typecheck_expstm {

    public J1_typecheck_expstm () {}

    public static int test() {
	new Object();
	new Object().toString();
	boolean b = (true == false);
	int i = 7;
	char c = 'q';
	String s = "abcdef"+"xys";
	Object o = s;
	int[] is = new int[5];
	Object[] os = new String[6];
	os[3] = "flimflam";
	String[] ss = (String[])os;
	ss[3].length();
        return 123;
    }

}
