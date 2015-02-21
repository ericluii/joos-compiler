// TYPE_CHECKING,CODE_GENERATION
public class J1_typecheck_plus {

    public J1_typecheck_plus () {}

    public static int test() {

	// numeric
	int i = 0;
	i = 1+1;
	i = (byte)1+(byte)1;
	i = (short)1+(short)1;
	i = (char)1+(char)1;
	i = (byte)1+(short)1;
	i = (short)1+(char)1;

	// String
	String s = null;
	s = "int"+1;
	s = "byte"+(byte)1;
	s = "short"+(short)1;
	s = "char"+(char)1;
	s = "String"+"flam";
	s = "Object"+new Object();
	s = "null"+null;
	s = "array"+new int[5];

        return 123;
    }

}
