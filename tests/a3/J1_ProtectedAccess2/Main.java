// TYPE_CHECKING
/* TypeChecking:
 * m() is protected in p.A
 */
public class Main extends p.A {
    public Main() {}

    public static int test() {
	new Main().access();
	return 123;
    }

    public void access() {
	p.A a = this;
	//a.m(); // compile time error
	((Main)a).m();
	m();
    }
}
