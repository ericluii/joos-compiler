// DISAMBIGUATION,CODE_GENERATION
public class J1_sideeffects_obj3 {
    protected J1_sideeffects_obj3 subject = null;

    public J1_sideeffects_obj3(int a, J1_sideeffects_obj3 s) {
	this.a = a;
	subject = s;
    }

    public static int test() {
	J1_sideeffects_obj3 tmp = null;
	J1_sideeffects_obj3 obj =
	    new J1_sideeffects_obj3(1,
		    tmp=new J1_sideeffects_obj3(2,
				(J1_sideeffects_obj3)null));

	obj.foo().a = obj.bar();
	return 4*tmp.a + 5*obj.a + obj.subject.a;
    }


    protected int a = 0;

    public J1_sideeffects_obj3 foo() {
	a = a+2;
	return subject;
    }
    public int bar() {
	subject = new J1_sideeffects_obj3(a+10*subject.a,(J1_sideeffects_obj3)null);
	a = a+5;
	return a+7;
    }
}
