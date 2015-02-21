// DISAMBIGUATION
public class J1_FieldRestrictionDuringInit {

    public J1_FieldRestrictionDuringInit () {}

    public int j = 1;
    public int i = j;

    public static int test() {
        return 123;
    }

}
