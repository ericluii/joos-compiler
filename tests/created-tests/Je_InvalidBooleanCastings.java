public class Je_InvalidBooleanCastings {
    // Boolean casts can only be on
    // boolean types, since booleans can only
    // be converted by identity conversion
    // JLS 5.1.1 - Identity conversion
    // JLS 5.5 - Primitive casts
    public Je_InvalidBooleanCastings() {
        int c = 2;
        char d = 'h';
        short x = (short) 1;
        byte y = (byte) 127;
        boolean b = (boolean) c;
        boolean b1 = (boolean) d;
        boolean b2 = (boolean) x;
        boolean b3 = (boolean) y;
    }
}
