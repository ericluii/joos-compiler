// DISAMBIGUATION
public class J1_fieldInOwnInit {

    public int f = 117 + (f = 4) + 2;

    public J1_fieldInOwnInit () {}

    public static int test() {
        return new J1_fieldInOwnInit().f;
    }

}
