// JOOS1:TYPE_CHECKING,CONSTRUCTOR_NAME
// JOOS2:TYPE_CHECKING,CONSTRUCTOR_NAME
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - A constructor must have the same name as its enclosing class.
 */
public class Je_6_Constructor_WrongName {

    public Je_6_Constructor_WrongName () {}

    public JE_6_CONSTRUCTOR_WRONGNAME (int i) {}

    public static int test() {
        return 123;
    }

}
