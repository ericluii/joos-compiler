// JOOS1: PARSER_WEEDER,JOOS1_STATIC_FIELD_DECLARATION,PARSER_EXCEPTION
// JOOS2: DISAMBIGUATION,TYPE_CHECKING
public class J2_staticField2 {
        public static int staticField = 23;

        public J2_staticField2() { }
        public static int test() {
                return 100 + J2_staticField2.staticField;
        }
}
