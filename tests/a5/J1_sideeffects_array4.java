// CODE_GENERATION
public class J1_sideeffects_array4 {
    public J1_sideeffects_array4() {}
    public static int test() {
	int[] a = new int[100]; 
	int[] b = new int[100];
	
	for (int i=0; i<100; i=i+1) a[i] = (3*i)%100;
	for (int i=0; i<100; i=i+1) b[i] = 99-i;
	
	int[] x = a;

	x[(x=b)[x[(x=a)[x[(x=b)[30]+1]+2]+4]+8]+16] = 123;
	// 30 b> 69 +> 70 a> 10 +> 12 a> 36 +> 40 b> 59 +> 67 b> 32 +> 48

	return a[48];
   }
}
