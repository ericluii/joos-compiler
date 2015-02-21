// TYPE_CHECKING
public class J1_length_field_not_array{

    public int length = 42;

    public J1_length_field_not_array(){
	length = 123;
    }

    public static int test(){
	J1_length_field_not_array tmp = new J1_length_field_not_array();
	return tmp.length;
    }

}
