// JOOS1:HIERARCHY,EXTENDS_FINAL_CLASS
// JOOS2:HIERARCHY,EXTENDS_FINAL_CLASS
// JAVAC:UNKNOWN
// 
/**
 * Hierarchy:
 * - A class must not extend a final class (8.1.1.2, 8.1.3, simple
 * constraint 4).
 */
public class Je_5_ClassExtendsInterface extends A {

    public Je_5_ClassExtendsInterface() {}

    public static int test() {
	return 123;
    }

}
