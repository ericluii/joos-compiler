// CODE_GENERATION

public class J1_A_String_ByteShortCharInt {
	public J1_A_String_ByteShortCharInt() {}
	
	public static int test() {
		int result = 0;

		byte b0 = (byte)-128;
		byte b1 = (byte)127;
		if (String.valueOf((int)b0).equals((Object)(""+b0))) result = result + 1;
		if (String.valueOf((int)b1).equals((Object)(""+b1))) result = result + 2;
		
		short s0 = (short)-32768;
		short s1 = (short)32767;
		if (String.valueOf((int)s0).equals((Object)(""+s0))) result = result + 4;
		if (String.valueOf((int)s1).equals((Object)(""+s1))) result = result + 8;

		char c0 = (char)0;
		char c1 = (char)65535;
		if (String.valueOf(c0).equals((Object)(""+c0))) result = result + 16;
		if (String.valueOf(c1).equals((Object)(""+c1))) result = result + 32;

		int i0 = (int)-2147483648;
		int i1 = (int)2147483647;
		if (String.valueOf(i0).equals((Object)(""+i0))) result = result + 64;
		if (String.valueOf(i1).equals((Object)(""+i1))) result = result + 128;

		if (result == 255) return 123;
		return result;
	}
}
