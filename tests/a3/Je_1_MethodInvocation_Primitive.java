// JOOS1:TYPE_CHECKING,NON_REFERENCE_RECEIVER
// JOOS2:TYPE_CHECKING,NON_REFERENCE_RECEIVER
// JAVAC:UNKNOWN
/**
 * TypeChecking:
 * - Method invocation on primitive types not allowed
 */
public class Je_1_MethodInvocation_Primitive {

    public Je_1_MethodInvocation_Primitive() {}

    public static int test () {
	int a = 42.parseInt("-123");
	return a;
    }
}
