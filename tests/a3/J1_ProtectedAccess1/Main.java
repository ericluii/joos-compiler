// TYPE_CHECKING
/* TypeChecking:
 * f is protected in p.A
 */
public class Main extends p.A {
    public Main() {}

    public static int test() {
	new Main().access();
	return 123;
    }

    public void access() {
	p.A a = this;
	//a.f = 3; // compile time error
	((Main)a).f = 3;
	f = 4;
    }
}
