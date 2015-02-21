// TYPE_CHECKING
public class J1_referencecasts {
    public String x;
    public J1_referencecasts(String x) { this.x = x; }
    public static int test() {
      Object obj = (J1_referencecasts)new J1_referencecasts("123");
      obj = (J1_referencecasts)obj;
      obj = (Integer)new Integer(Integer.parseInt(((J1_referencecasts)obj).x));
      return ((Integer)obj).intValue();
  }
}
