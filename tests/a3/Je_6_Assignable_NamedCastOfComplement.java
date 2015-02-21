// JOOS1:PARSER_WEEDER,PARSER_EXCEPTION,SYNTAX_ERROR,TYPE_CHECKING,INVALID_CAST,UNOP_TYPE
// JOOS2:PARSER_WEEDER,PARSER_EXCEPTION,SYNTAX_ERROR,TYPE_CHECKING,INVALID_CAST,UNOP_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Cast of complement to named type not allowed
 * - Complement operator cannot be applied to int.
 */
public class Je_6_Assignable_NamedCastOfComplement{

    public Je_6_Assignable_NamedCastOfComplement(){}

    public static int test(){
	int Je_6_Assignable_NamedCastOfComplement = 256;
	int a = 133;
	int result = (Je_6_Assignable_NamedCastOfComplement)!a;
	return result;
    }

}
