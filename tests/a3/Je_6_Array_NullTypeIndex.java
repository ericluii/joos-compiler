// JOOS1:TYPE_CHECKING,NON_NUMERIC_ARRAY_SIZE
// JOOS2:TYPE_CHECKING,NON_NUMERIC_ARRAY_SIZE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Array index must have numeric type
 */
public class Je_6_Array_NullTypeIndex {

    public Je_6_Array_NullTypeIndex () {}

    public static int test() {
	Object[] os = new Object[null];
        return 123;
    }

}
