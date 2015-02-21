// DISAMBIGUATION,CODE_GENERATION
public class J1_fieldinit_forward_ref2 {

    public int i = 121 + (j=2);
    public int j = 0;

    public J1_fieldinit_forward_ref2 () {}

    public static int test() {
        return new J1_fieldinit_forward_ref2().i;
    }

}
