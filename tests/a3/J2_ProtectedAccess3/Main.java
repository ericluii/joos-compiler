// JOOS1: PARSER_WEEDER,JOOS1_STATIC_FIELD_DECLARATION,PARSER_EXCEPTION
// JOOS2: TYPE_CHECKING
public class Main extends p.A {
    public Main() {}

    public static int test() {
	p.A.sf = 3;
	Main.sf = 3;
	//sf = 4;
	return 123;
    }
}
