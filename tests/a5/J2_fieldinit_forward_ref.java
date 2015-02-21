// JOOS1: PARSER_WEEDER,JOOS1_STATIC_FIELD_DECLARATION,PARSER_EXCEPTION
// JOOS2: DISAMBIGUATION,CODE_GENERATION
public class J2_fieldinit_forward_ref {

    public static int i = 1+J2_fieldinit_forward_ref.j + J2_fieldinit_forward_ref.i;
    public static int j = 17;

    public J2_fieldinit_forward_ref () {}

    public static int test() {
        return 122+J2_fieldinit_forward_ref.i;
    }

}
