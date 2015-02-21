// CODE_GENERATION
public class J1_sideeffects_array2 {
    public J1_sideeffects_array2() {}
    public static int test() {
	int[] a = new int[10];

	for (int i=0; i<a.length; i=i+1) a[i] = 2*i;
	int i = 0;
	a[(a[i]=i=a[2]-1)+1] = a[a[7] = a[i+1]+1]+1;
	/* i == 0
	 * a[2] == 4
	 * i = 3
	 * a[0] = 3
	 * a[i+1] == a[4] == 8
	 * a[7] = 9
	 * a[9] == 18
	 * a[4] =  19
	 * Resultat: {3,2,4,6,19,10,12,9,16,18}
	 */

	int[] b = new int[10];
	b[0] = 3; b[1] = 2;
	b[2] = 4; b[3] = 6;
	b[4] = 19; b[5] = 10;
	b[6] = 12; b[7] = 9;
	b[8] = 16; b[9] = 18;

	int r=0;
	for (i=0; i<a.length; i=i+1)
	    if (a[i]==b[i])
		r = r - 6;
	    else
		r = r - 1234567;

	for (i=-1; i<a.length-1;) r = r + (i-3) * a[i=i+1];
	System.out.println("r="+r);

	return r;
    }
}
