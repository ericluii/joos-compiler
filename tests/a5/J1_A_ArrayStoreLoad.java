// CODE_GENERATION

public class J1_A_ArrayStoreLoad {
	public J1_A_ArrayStoreLoad() {}
	
	public static int test() {
		boolean[] z = new boolean[3];
		byte[] b = new byte[4];
		short[] s = new short[5];
		char[] c = new char[6];
		int[] i = new int[7];
		String[] o = new String[8];
		
		z[1] = true;
		b[2] = (byte)42;
		s[3] = (short)12345;
		c[4] = '%';
		i[5] = 123;
		o[6] = "foobar";
		
		if (!z[1]) {
			return 1;
		}
		if (b[2] != 42) {
			return 2;
		}
		if (s[3] != 12345) {
			return 3;
		}
		if (c[4] != '%') {
			return 4;
		}
		if (!o[6].equals((Object)"foobar")) {
			return 5;
		}
		return i[5];
	}
}
