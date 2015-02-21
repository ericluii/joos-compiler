// TYPE_CHECKING
public class J1_castarrayaccess {

    public J1_castarrayaccess() {}

    public static int test() {
	String[] s = new String[5];
	Object o = (Object)s[1];
	return 123;
    }


}
