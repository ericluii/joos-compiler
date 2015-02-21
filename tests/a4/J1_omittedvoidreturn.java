// REACHABILITY
public class J1_omittedvoidreturn {
    public J1_omittedvoidreturn() {}
    public int y;
    public void m() {
	this.y = 17;
	// return;
    }
    public static int test() {
	J1_omittedvoidreturn obj = new J1_omittedvoidreturn();
	obj.m();
	return obj.y+106;
    }
}
