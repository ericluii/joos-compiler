// JOOS1: PARSER_WEEDER,JOOS1_INTERFACE,PARSER_EXCEPTION
// JOOS2: TYPE_CHECKING
// JAVAC:

public class Main implements J2_interface {

    public Main() {}

    public static int test() {
	J2_interface obj = (J2_interface)new Main();
	return 123;
    }

} 
