// CODE_GENERATION
import java.util.LinkedList;

public class Main {
    public Main() {}
    public static int test() {
	LinkedList l1 = new LinkedList();
	l1.add(new Object());
	LinkedList l3 = (LinkedList) l1.clone();
	l1.add(new Object());
	LinkedList l2 = (LinkedList) l1.clone();

	LinkedList l = l2;
	l.add((Object)(l = l3));
	return 100*l.size() + 10*l1.size() + l2.size();
    }
}
