// TYPE_CHECKING
public class J1_castthis {

    public J1_castthis() {}

    public void m() {
	Object o = (Object)this;
    }

    public static int test() {
	new J1_castthis().m();
	return 123;
    }


}
