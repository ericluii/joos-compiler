// TYPE_CHECKING,CODE_GENERATION
public class J1_nestedcast {

    public J1_nestedcast () {}

    public static int test() {
	String String = "Hello";
	String=(String)String+(String)((String)" "+(String)"World");
	return String.length()+112;
    }

}
