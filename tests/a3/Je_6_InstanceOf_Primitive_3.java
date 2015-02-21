// JOOS1:TYPE_CHECKING,INVALID_INSTANCEOF
// JOOS2:TYPE_CHECKING,INVALID_INSTANCEOF
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Expression clause of instanceof must have reference type
 */
public class Je_6_InstanceOf_Primitive_3 {

    public Je_6_InstanceOf_Primitive_3() {}

    public static int test() { 
	return 123; 
    }

    public boolean foo(int i) {
	return i instanceof Object;
    }
}
