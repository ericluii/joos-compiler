// DISAMBIGUATION
public class J1_localDeclAccess {
	public J1_localDeclAccess() { }
	public static int test() { 
		int i = 23;
		{
			int j = 100;
			return i+j;
		}
	}
}
