// CODE_GENERATION
/* CodeGeneration:
 * Code Templates should preserve semantics of the program.
 */

public class J1_A_Complement_SideEffect{

    public J1_A_Complement_SideEffect(){}

    public int count = 122;

    public boolean incCount(){
	count = count + 1;
	return true;
    }
    
    public static int test() {
	J1_A_Complement_SideEffect c = new J1_A_Complement_SideEffect();
	boolean b = (!c.incCount()); // incCount most be called exactly one time
	return c.count;
    }
}
