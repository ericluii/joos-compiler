// CODE_GENERATION
public class J1_concatInMethods {

    public J1_concatInMethods() {}

    public String foo(String s) { 
	String h ="#"; 
	return s+h; 
    } 

    public static int test() { 
	J1_concatInMethods j = new J1_concatInMethods();
	String s=""; 
	s = "st1"+"st2"+"st2"+j.foo("st"+3+(j.foo("4+5+32")))+j.foo("34"); 
	return s.length() + 100; 
    } 
}
