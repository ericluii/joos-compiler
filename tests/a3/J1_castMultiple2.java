// TYPE_CHECKING
public class J1_castMultiple2 {

    public J1_castMultiple2 () {}

    public static int test() {
	J1_castMultiple2 a = new J1_castMultiple2();
	a = (J1_castMultiple2) a;
	a = (J1_castMultiple2) (J1_castMultiple2) a;
	a = (J1_castMultiple2) ((J1_castMultiple2) a);
	a = (J1_castMultiple2) ((J1_castMultiple2) (J1_castMultiple2) (J1_castMultiple2) a);
        return 123;
    }

}
