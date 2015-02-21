// JOOS1:DISAMBIGUATION,TYPE_CHECKING,NON_REFERENCE_RECEIVER
// JOOS2:DISAMBIGUATION,TYPE_CHECKING,NON_REFERENCE_RECEIVER
// JAVAC:UNKNOWN
// 
/**
 * Disambiguation/TypeChecking:
 * - This testcase tests the scope of a local variable.
 * (It should fail in the typechecking phase)
 */
public class Je_5_AmbiguousInvoke_LocalInOwnInitializer{

    public Integer a = new Integer(123);

    public Je_5_AmbiguousInvoke_LocalInOwnInitializer(){
    }

    public int foo(){
	int a = a.intValue(); // a refers to the local variable and is thus of type int
	return a;
    }

    public static int test(){
	return new Je_5_AmbiguousInvoke_LocalInOwnInitializer().foo();
    }
}
