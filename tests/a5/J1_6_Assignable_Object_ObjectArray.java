// TYPE_CHECKING,CODE_GENERATION
/**
 * Typecheck:
 * - Type A[] is not assignable to type A.
 */
public class J1_6_Assignable_Object_ObjectArray {

    public J1_6_Assignable_Object_ObjectArray () {}

    public static int test() {
	Object j = new Object[123];
	return ((Object[])j).length;
    }

}
