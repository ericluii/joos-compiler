// DISAMBIGUATION
public class J1_backwardsFieldRef {
	public int field2 = 100;
	public int field1 = field2+23;
	public J1_backwardsFieldRef() { }
	public static int test() { 
		J1_backwardsFieldRef b = new J1_backwardsFieldRef();
		
		return b.field1;
	}
}
