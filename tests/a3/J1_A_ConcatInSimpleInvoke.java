// CODE_GENERATION,TYPE_CHECKING

public class J1_A_ConcatInSimpleInvoke {
	public J1_A_ConcatInSimpleInvoke() {}
	
	public int method1(String str) {
		return str.length();
	}
	
	public int method2() {
		boolean z = true;
		byte b = (byte)0;
		char c = '1';
		short s = (short)2;
		int i = 3;
		Object o = null;
		return method1("" + z + b + c + s + i + o + null) + 107;
	}
	
	public static int test() {
		J1_A_ConcatInSimpleInvoke j = new J1_A_ConcatInSimpleInvoke();
		return j.method2();
	}
}
