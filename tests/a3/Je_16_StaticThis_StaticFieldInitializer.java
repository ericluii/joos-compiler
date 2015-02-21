// JOOS1: PARSER_WEEDER,JOOS1_STATIC_FIELD_DECLARATION,PARSER_EXCEPTION
// JOOS2: TYPE_CHECKING,THIS_IN_STATIC_CONTEXT
// JAVAC:UNKNOWN
/**
 * Parser/weeder:
 * - (Joos 1) No static field declarations allowed.
 * Typecheck:
 * - A this reference (AThisExp) must not occur, explicitly or
 * implicitly, in a static method, an initializer for a static field,
 * or an argument to a super or this constructor invocation.
 */
public class Je_16_StaticThis_StaticFieldInitializer {

    public int i = 100;

    public static int j = 20+i+3;

    public Je_16_StaticThis_StaticFieldInitializer () {}

    public static int test() {
        return 123;
    }

}
