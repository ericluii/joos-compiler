// CODE_GENERATION

public class J1_A_LazyEagerAndOr {
	
	public J1_A_LazyEagerAndOr() {}
	
	public int sum = 3;
	
	public boolean sideeffect(boolean bar, int value) {
		if (bar) {
			sum = sum + value;
		} else {
			sum = sum - value;
		}
		return bar;
	}
	
	public static int test() {
		J1_A_LazyEagerAndOr j = new J1_A_LazyEagerAndOr();
		boolean b0 = j.sideeffect(false,10) & j.sideeffect(false,20);
		boolean b1 = j.sideeffect(false,30) && j.sideeffect(false,40);
		boolean b2 = j.sideeffect(true,50) | j.sideeffect(true,60);
		boolean b3 = j.sideeffect(true,70) || j.sideeffect(true,80);
		return j.sum;
	}
}
