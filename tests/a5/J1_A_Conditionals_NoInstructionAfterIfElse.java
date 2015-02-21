// CODE_GENERATION
public class J1_A_Conditionals_NoInstructionAfterIfElse{
    
    public J1_A_Conditionals_NoInstructionAfterIfElse(){}

    public static int test(){
	J1_A_Conditionals_NoInstructionAfterIfElse a = new J1_A_Conditionals_NoInstructionAfterIfElse();
	if (a.foo()){
	    return 123;
	}
	else return 0;
    }

    public boolean foo(){
	if (true) {
	    return true;
	}
	else {
	    return false;
	}
    }

}

