// DISAMBIGUATION
/* Disambiguation:
 * The initializer for a non-static field must not refer by simple name to
 * itself or a non-static field declared later in the same class, except as the
 * direct left-hand side of an assignment.
 */
public class J1_5_ForwardReference_ArrayLength{

  public int[] a = new int[123];
  public int x = a.length; // this is a backward reference
  
  public J1_5_ForwardReference_ArrayLength(){}
  
  public static int test() {
      J1_5_ForwardReference_ArrayLength j = new J1_5_ForwardReference_ArrayLength();
      return j.x;
  }
}
