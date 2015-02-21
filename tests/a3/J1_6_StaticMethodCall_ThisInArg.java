// TYPE_CHECKING

public class J1_6_StaticMethodCall_ThisInArg {
	public J1_6_StaticMethodCall_ThisInArg() {}
	
	public int foo = 123;
	
	public int instanceMethod() {
		return J1_6_StaticMethodCall_ThisInArg.staticMethod(foo);
	}
	
	public static int staticMethod(int foo) {
		return foo;
	}
	
	public static int test() {
		J1_6_StaticMethodCall_ThisInArg j = new J1_6_StaticMethodCall_ThisInArg();
		return j.instanceMethod();
	}
}
