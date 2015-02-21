// JOOS1: PARSER_WEEDER,JOOS1_STATIC_FIELD_DECLARATION,PARSER_EXCEPTION
// JOOS2: DISAMBIGUATION
/* Disambiguation:
 * (Joos 2) The initializer for a static field must not refer by simple name to
 * itself or a static field declared later in the same class, except as the
 * direct left-hand side of an assignment.
 */
public class J2_5_ForwardReference_StaticField {
  public static int c1 = (J2_5_ForwardReference_StaticField.j=1)+2; // this is not illegal since j is direct 
                                  // left-hand side of an assignment
  public static int j;
  
  public J2_5_ForwardReference_StaticField() {}
  
  public static int test() {
      return J2_5_ForwardReference_StaticField.c1+J2_5_ForwardReference_StaticField.j+119;
  }
}
