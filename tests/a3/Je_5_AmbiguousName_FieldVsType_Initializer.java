//JOOS1:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
//JOOS2:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
//JAVAC:UNKNOWN

public class Je_5_AmbiguousName_FieldVsType_Initializer {
	public int field = Je_5_AmbiguousName_FieldVsType_Initializer.intValue(); // <- should disambiguate to the field and not the type, thus causing an error.
	public Integer Je_5_AmbiguousName_FieldVsType_Initializer = new Integer(42);
	
	public Je_5_AmbiguousName_FieldVsType_Initializer() {}
	
	public static int intValue() {
		return 123;
	}
	
	public static int test() {
		Je_5_AmbiguousName_FieldVsType_Initializer j = new Je_5_AmbiguousName_FieldVsType_Initializer();
		return j.field;
	}
}
