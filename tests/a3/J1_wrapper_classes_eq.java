// TYPE_CHECKING
public class J1_wrapper_classes_eq{

    public J1_wrapper_classes_eq() {}

    public static int test(){
	Integer ri = new Integer(4);
	Object i = ri;
	Character rc = new Character('c');
	Object c = rc;
	Short rs = new Short((short)42);
	Object s = rs;
	Byte rb = new Byte((byte)11);
	Object b = rb;

	if (ri == i && rc == c && rs == s && rb == b) {
	    return 123;
	}

	return 42;
    }

}
