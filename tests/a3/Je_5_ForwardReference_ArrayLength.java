// JOOS1:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
// JOOS2:DISAMBIGUATION,ILLEGAL_FORWARD_FIELD_REFERENCE
// JAVAC:UNKNOWN
// 
/* Disambiguation:
 * The initializer for a non-static field must not refer by simple name to
 * itself or a non-static field declared later in the same class, except as the
 * direct left-hand side of an assignment.
 */
public class Je_5_ForwardReference_ArrayLength{

  public int x = a.length; // this is an illegal forward reference
  public int[] a = new int[123];

  public Je_5_ForwardReference_ArrayLength(){}

  public static int test(){
    Je_5_ForwardReference_ArrayLength j = new Je_5_ForwardReference_ArrayLength();
    return j.x;
  }
}
