// DISAMBIGUATION

public class J1_5_AmbiguousName_LocalVsType {
	public int foo;
	
	public J1_5_AmbiguousName_LocalVsType() {}
	
	public static int test() {
		J1_5_AmbiguousName_LocalVsType J1_5_AmbiguousName_LocalVsType = new J1_5_AmbiguousName_LocalVsType();
		J1_5_AmbiguousName_LocalVsType.foo = 123;
		return J1_5_AmbiguousName_LocalVsType.foo;
	}
}
