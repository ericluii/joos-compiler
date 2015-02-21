// CODE_GENERATION
public class J1_sideeffects_array {
    public J1_sideeffects_array() {}
    public static int test() {
	int[] a = new int[3];
	int i=2;
	a[i=i-1] = (a[i=i-1]=1)+1;
	i=2;
	a[i] = 2*(a[i=i-1])-(a[i=i-1]);
	return 100*a[0] + 10*a[1] + a[2];
    }
}
