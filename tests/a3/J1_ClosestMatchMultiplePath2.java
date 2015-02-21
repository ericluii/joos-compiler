// TYPE_CHECKING
public class J1_ClosestMatchMultiplePath2
{
    public J1_ClosestMatchMultiplePath2() {}

    public static int test() {
	return new J1_ClosestMatchMultiplePath2().m("a", "b");
    }

    public int m(Object a, Object b) { return 120; }
    public int m(String a, Object b) { return 121; }
    public int m(Object a, String b) { return 122; }
    public int m(String a, String b) { return 123; }
}
