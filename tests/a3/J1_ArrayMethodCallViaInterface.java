/* This test should pass even though it is an array
 * method call, because the call is made via a reference
 * to an interface. The method toString() comes from
 * java.lang.Object. Please refer to JLS 9.2, point number
 * three regarding the member of interfaces. The point
 * is reflected in the CompilationTable's member 'extendFromObject'
 * 
 * Note that the method toString() since it comes from java.lang.Object
 * refers to a ClassMethodTable, which represents the method toString()
 * of java.lang.Object
 * */

public class J1_ArrayMethodCallViaInterface {
    public J1_ArrayMethodCallViaInterface() {
        java.lang.Cloneable x = new int[2];
        x.toString();
    }
}
