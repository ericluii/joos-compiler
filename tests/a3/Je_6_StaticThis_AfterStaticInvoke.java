// JOOS1:TYPE_CHECKING,THIS_IN_STATIC_CONTEXT
// JOOS2:TYPE_CHECKING,THIS_IN_STATIC_CONTEXT
// JAVAC:UNKNOWN
// 
public class Je_6_StaticThis_AfterStaticInvoke{

    public int a = 42;

    public Je_6_StaticThis_AfterStaticInvoke(){
    }

    public static int foo(){
	return 123;
    }

    public static int test(){
	int a = Je_6_StaticThis_AfterStaticInvoke.foo();
	return this.a;
    }
}
