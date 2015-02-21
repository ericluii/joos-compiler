// TYPE_CHECKING
/* TypeChecking:
 * sm() is protected in p.A
 */
public class Main extends p.A {
    public Main() {}

    public static int test() {
	p.A.sm();
	Main.sm();
	//sm(); // Joos 2
	return 123;
    }
}
