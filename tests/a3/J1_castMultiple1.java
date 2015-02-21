// TYPE_CHECKING
public class J1_castMultiple1 {

    public J1_castMultiple1 () {}

    public static int test() {
	J1_castMultiple1[] a = new J1_castMultiple1[3];
	a = (J1_castMultiple1[]) a;
	a = (J1_castMultiple1[]) (J1_castMultiple1[]) a;
	a = (J1_castMultiple1[]) ((J1_castMultiple1[]) a);
	a = (J1_castMultiple1[]) ((J1_castMultiple1[]) (J1_castMultiple1[]) (J1_castMultiple1[]) a);
        return 123;
    }

}
