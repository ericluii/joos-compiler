//PARSER_WEEDER
//JOOS1:TYPE_CHECKING,NON_BOOLEAN_CONDITION
//JOOS2:TYPE_CHECKING,NON_BOOLEAN_CONDITION
//JAVAC:UNKNOWN

public class Je_6_For_NullInCondition {
	public Je_6_For_NullInCondition() {
		for (;null;) {}
	}
	
	public static int test() {
		return 123;
	}
}
