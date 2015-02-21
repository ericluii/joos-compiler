// TYPE_CHECKING
public class J1_ClosestMatchMultiplePath1
{
    public J1_ClosestMatchMultiplePath1() {}

    public static int test() {
	return new J1_ClosestMatchMultiplePath1().m("a", "b");
    }

    public int m(String a, String b) { return 123; }
    public int m(Object a, String b) { return 122; }
    public int m(String a, Object b) { return 121; }
    public int m(Object a, Object b) { return 120; }
}
