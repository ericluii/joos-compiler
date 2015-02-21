// CODE_GENERATION
public class J1_A_FieldInitialization_NonConstant_Before {
	public int b = this.a; // b = 0
	public int a = new Integer(22).intValue(); // a = 22

	public J1_A_FieldInitialization_NonConstant_Before() {}
	
	public static int test() {
		J1_A_FieldInitialization_NonConstant_Before j = new J1_A_FieldInitialization_NonConstant_Before();
		return j.a + j.b + 101;
	}
}
