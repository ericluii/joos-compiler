// CODE_GENERATION

public class J1_A_LazyEval {
	public J1_A_LazyEval() {}
	
	public int sideeffect = 1;
	
	public boolean m1() {
		sideeffect = sideeffect-1;
		return false;
	}
	
	public boolean m2() {
		sideeffect = sideeffect*2;
		return false;
	}
	
	public int m() {
		boolean b = m1() || (m2() || true);
		return sideeffect;
	}
	
	public static int test() {
		J1_A_LazyEval j = new J1_A_LazyEval();
		return j.m()+123;
	}
}
		
