// TYPE_CHECKING
public class Main {
    
    public Main() {}

    public static int test() {
	J1_evalMethodInvocation[] j = new J1_evalMethodInvocation[3];
	j[2] = new J1_evalMethodInvocation();

	return j[2].foo();
    }
}
