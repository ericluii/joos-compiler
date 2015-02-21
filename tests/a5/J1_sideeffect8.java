// CODE_GENERATION
public class J1_sideeffect8 {
    public J1_sideeffect8() {}

    protected int g;
    public static int test() {
	return new J1_sideeffect8().test2();
    }
    public int test2() {
	g=0;
        Integer i=null;
	String s = null;
        boolean b81 = (f(1)==1 || i.intValue()<10);
        boolean b82 = (f(1)==2 || s.charAt(0)>10);
	return f(41);
	
    }

    public int f(int x) {
	g=g+1;
	return x*g;
    }

}
