// TYPE_CHECKING
public class J1_instanceof {
  public J1_instanceof() {}
  public static int test() {
    boolean b1 = new J1_instanceof() instanceof J1_instanceof;
    boolean b2 = new String() instanceof String;
    boolean b3 = "" instanceof String;
    if (b1 && b2 && b3) return 123;
    else return 1;
  }
}
