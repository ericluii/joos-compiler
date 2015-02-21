// TYPE_CHECKING
public class J1_constructoroverloading {
    public int x = 0;
    public J1_constructoroverloading() {
	this.x = 23;
    }
    public J1_constructoroverloading(int x) {
	this.x = x;
    }
    public static int test() {
	J1_constructoroverloading obj1 = new J1_constructoroverloading();
	J1_constructoroverloading obj2 = new J1_constructoroverloading(100);
	return obj1.x + obj2.x;
    }
}
