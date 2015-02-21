// JOOS1: PARSER_WEEDER,JOOS1_STATIC_FIELD_DECLARATION,PARSER_EXCEPTION
// JOOS2: TYPE_CHECKING
/* TypeChecking:
 * 
 * Test for Protected Access
 * 
 * B.B extends A.A
 * C.C extends B.B
 * D.D extends C.C
 * 
 * see C.java
 */
public class Main {
    public Main() {}
    
    public static int test() {
	return 123;
    }
}
