// REACHABILITY
import java.util.Random;

public class Main {

    public Random random = new Random();

    public Main () {}

    public void m() {
	if (random.nextBoolean()) {
	    System.out.println("then branch completes normally!");
	}
	else {
	    return;
	}
    }

    public void m1() {
	if (random.nextBoolean()) {
	    return;
	}
	else {
	    return;
	}
    }

    public static int test() {
	Main j = new Main();
        j.m();
	j.m1();
        return 123;
    }

}
