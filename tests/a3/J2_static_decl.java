// JOOS1: PARSER_WEEDER,JOOS1_STATIC_FIELD_DECLARATION,PARSER_EXCEPTION
// JOOS2: DISAMBIGUATION
public class J2_static_decl {
    public J2_static_decl() {}
    protected static int foo = 123;
    public static int test() {
	return J2_static_decl.foo;
    }
}
