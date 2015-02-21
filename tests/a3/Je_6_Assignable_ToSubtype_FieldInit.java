// JOOS1:TYPE_CHECKING,ASSIGN_TYPE
// JOOS2:TYPE_CHECKING,ASSIGN_TYPE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck:
 * - Type Object is not assignable to type String.
 */
public class Je_6_Assignable_ToSubtype_FieldInit {

    public String s = new Object();
    
    public Je_6_Assignable_ToSubtype_FieldInit () {}

    public static int test() {
        return 123;
    }

}
