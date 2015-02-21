// JOOS1:TYPE_CHECKING,INSTANTIATE_INTERFACE
// JOOS2:TYPE_CHECKING,INSTANTIATE_INTERFACE
// JAVAC:UNKNOWN
// 
/**
 * Typecheck: 
 * - The type in a class instance creation expression must
 *   be a non-abstract class.  
 */
public class Je_6_InstantiateInterface {

    public Je_6_InstantiateInterface() {
	java.io.Serializable s = new java.io.Serializable();
    }
}
