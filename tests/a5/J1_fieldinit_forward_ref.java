// DISAMBIGUATION,CODE_GENERATION
public class J1_fieldinit_forward_ref {

    public int i = 121+this.i+(i=1)+this.i+this.j;
    public int j = 17;

    public J1_fieldinit_forward_ref () {}

    public static int test() {
        return new J1_fieldinit_forward_ref().i;
    }

}
