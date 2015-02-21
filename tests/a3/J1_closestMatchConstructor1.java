// TYPE_CHECKING
import java.io.Serializable;

public class J1_closestMatchConstructor1 implements Serializable {
    public int a;
    
    public J1_closestMatchConstructor1() {
        a = 4;
    }
    
    protected J1_closestMatchConstructor1(Object o) {
        a = 42;
    }
    
    protected J1_closestMatchConstructor1(J1_closestMatchConstructor1 o) {
        a = 123;
    }

    public static int test() {
        return new J1_closestMatchConstructor1(new J1_closestMatchConstructor1()).a;
    }
}
