// CODE_GENERATION
public class J1_stringconcat {

    public String s;

    public J1_stringconcat (String str) {
	this.s = str+str+str;
    }

    public static int test() {
	String str = ""+1+"111";
	if (Integer.parseInt("4"+"2")==42)
	    str = str+"22";
	else 
	    str = str+"33";
	int i=0;
	while (i<10) {
	    i = i+1;
	    str = str+"444";
	}
	for (int j=0; j<10; j=j+1) {
	    str = str+"55"+"66"+str;
	}
	if (new J1_stringconcat(str).s.equals((Object)(str+str+str)))
	    return 123;
	else
	    return 0;
    }

}
