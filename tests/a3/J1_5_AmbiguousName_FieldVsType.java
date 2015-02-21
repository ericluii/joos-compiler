// DISAMBIGUATION

public class J1_5_AmbiguousName_FieldVsType {
	public Integer J1_5_AmbiguousName_FieldVsType = new Integer(123);
	
	public J1_5_AmbiguousName_FieldVsType() {}
	
	public int method() {
		return J1_5_AmbiguousName_FieldVsType.intValue(); // <- should disambiguate to the field and not the type.
	}
	
	public static int intValue() {
		return 42;
	}
	
	public static int test() {
		J1_5_AmbiguousName_FieldVsType j = new J1_5_AmbiguousName_FieldVsType();
		return j.method();
	}
}
