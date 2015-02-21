// JOOS1:DISAMBIGUATION,VARIABLE_NOT_FOUND
// JOOS2:DISAMBIGUATION,VARIABLE_NOT_FOUND
// JAVAC:UNKNOWN
// 
/**
 * Disambiguation:
 * - If none of the disambiguation rules apply, then an error message
 * must be produced.
 */
public class Je_5_AmbiguousName_NoDeclaration {

    public Je_5_AmbiguousName_NoDeclaration() {}

    public static int test() {	
	int x = nowhere;
	return 123;}

}
