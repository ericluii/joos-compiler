// DISAMBIGUATION

public class J1_5_AmbiguousName_FieldVsType_Initializer {
	public Integer J1_5_AmbiguousName_FieldVsType_Initializer = new Integer(123);
	
	public int field = J1_5_AmbiguousName_FieldVsType_Initializer.intValue(); // <- should disambiguate to the field and not the type.
	
	public J1_5_AmbiguousName_FieldVsType_Initializer() {}
	
	public static int intValue() {
		return 42;
	}
	
	public static int test() {
		J1_5_AmbiguousName_FieldVsType_Initializer j = new J1_5_AmbiguousName_FieldVsType_Initializer();
		return j.field;
	}
}
