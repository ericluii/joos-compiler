// CODE_GENERATION

public class J1_A_ArrayBaseInAssignment {
	public J1_A_ArrayBaseInAssignment() {}
	
	public int[] nonstatic_field = new int[6];
	
	public int[] nonstatic_method() {
		return nonstatic_field;
	}
	
	public static int[] static_method(int[] a) {
		return a;
	}
	
	public int method() {
		int[] local = nonstatic_field;
		local[0] = 1; // local
		nonstatic_field[1] = 2; // nonstatic field
		nonstatic_method()[2] = 8; // nonstatic method invoke
		J1_A_ArrayBaseInAssignment.static_method(local)[3] = 16; // static method invoke
		(new int[1])[0] = 0; // new array <- no sideeffects so it cannot be tested
		(local = local)[4] = 32; // assignment
		((int[])local)[5] = 64; // cast
		return local[0]+local[1]+local[2]+local[3]+local[4]+local[5];
	}
	
	public static int test() {
		J1_A_ArrayBaseInAssignment j = new J1_A_ArrayBaseInAssignment();
		return j.method();
	}
}
