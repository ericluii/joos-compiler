// JOOS1: PARSER_WEEDER,JOOS1_THIS_CALL,PARSER_EXCEPTION
// JOOS2: TYPE_CHECKING,CODE_GENERATION
import java.io.Serializable;

public class J2_exactMatchConstructor3 implements Serializable {
    public int a;
        
    protected J2_exactMatchConstructor3(Object o) {
        a = 123;
    }
    
    protected J2_exactMatchConstructor3(Serializable o) {
        a = 2;
    }
    
    protected J2_exactMatchConstructor3(J2_exactMatchConstructor3 o) {
        a = 1;
    }

    public J2_exactMatchConstructor3() {
    }

    public static int test() {
        return new J2_exactMatchConstructor3(new Object()).a;
    }
}
