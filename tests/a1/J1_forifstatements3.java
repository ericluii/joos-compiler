// PARSER_WEEDER
public class J1_forifstatements3 {
    
    public J1_forifstatements3() {}

    public static int test() {
	int i = 42;
	for (i=0; i<10; i=i+1) 
	    if (i==0)
		if (i!=0) 
		    return 42; 
	    else // must bind to closest if 
		return 123;
	return 42;
    }
    
}
