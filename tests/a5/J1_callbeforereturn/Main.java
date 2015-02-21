// CODE_GENERATION
/*
 *	Tests istore-iload-call-return.
 */
import java.util.*;

public class Main {
    public Main() {}
    public static int test() {
	int r = 0;
	if (Main.virt("Demo")==4) r=r+1;
	if (Main.intf((List)new LinkedList())==0) r=r+1;
	if (Main.stat_ref("Skoda")==5) r=r+1;
	if (Main.stat_int(10)==100) r=r+1;
	return r + 119;
    }


    public static int virt(String s)
    {String s2=s; return s2.length();}

    public static int stat_ref(String s)
    {String s2=s; return Main.stat_ref2(s2);}
    public static int stat_ref2(String s) {return s.length();}

    public static int stat_int(int x)
    {int x2=x; return Main.stat_int2(x2);}
    public static int stat_int2(int x) {return x*x;}

    public static int intf(List l)
    {List l2 = l; return l2.size();}
}
