// JOOS1:TYPE_CHECKING,UNOP_TYPE
// JOOS2:TYPE_CHECKING,UNOP_TYPE
// JAVAC:UNKNOWN
/* TypeChecking:
 * Complement must be on boolean types.
 */
public class Je_1_Complement_OfIntLiteral{

    public Je_1_Complement_OfIntLiteral(){}

    public static int test(){
	return !123;
    }
}
