/**
 * Typecheck:
 * - A non-void method cannot return void
 */
public class Je_6_Return_Void_NonVoid {

    public Je_6_Return_Void_NonVoid () {}

    public int m() {
		return;
    }

    public static int test() {
        return 123;
    }

}
