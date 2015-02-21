//DISAMBIGUATION

public class J1_5_AmbiguousName_LocalVsField_SameLine {
	public int a = 123;
	
	public J1_5_AmbiguousName_LocalVsField_SameLine() {}
	
	public int method() {
		int b = a; int a = 0;
		return b;
	}
	
	public static int test() {
		J1_5_AmbiguousName_LocalVsField_SameLine j = new J1_5_AmbiguousName_LocalVsField_SameLine();
		return j.a;
	}
}
