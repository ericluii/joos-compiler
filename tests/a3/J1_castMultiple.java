// TYPE_CHECKING
public class J1_castMultiple {

    public J1_castMultiple () {}

    public static int test() {
	int i = 123;
	i = (int) i;
	i = (int) (int) i;
	i = (int) ((int) i);
        return (int) ((int) (int) (int)i);
    }

}
