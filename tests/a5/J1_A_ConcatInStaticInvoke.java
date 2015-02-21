// CODE_GENERATION,TYPE_CHECKING

public class J1_A_ConcatInStaticInvoke {
	public J1_A_ConcatInStaticInvoke() {}
	
	public static int method(String str) {
		return str.length();
	}
	
	public static int test() {
		boolean z = true;
		byte b = (byte)0;
		char c = '1';
		short s = (short)2;
		int i = 3;
		Object o = null;
		return J1_A_ConcatInStaticInvoke.method("" + z + b + c + s + i + o + null) + 107;
	}
}
