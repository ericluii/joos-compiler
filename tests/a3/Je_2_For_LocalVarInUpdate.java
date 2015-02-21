// JOOS1:DISAMBIGUATION,VARIABLE_NOT_FOUND
// JOOS2:DISAMBIGUATION,VARIABLE_NOT_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Environments:
 * - The update clause of a for loop is not in the scope of a local
 * variable declared in the body of the loop.
 */
public class Je_2_For_LocalVarInUpdate {

    public Je_2_For_LocalVarInUpdate() {}

    public static int test() {
	for (int i=0; i<41; i=tmp+1) {
	    int tmp=i;
	}
	return 123;
    }
}
