import java.io.Serializable;

public class J1_ValidCastings {
    public J1_ValidCastings() {
        /* All of these are valid castings (and also
         * instanceof). Please review JLS 5.2 and A3 specs */

        Cloneable s = (Cloneable) new int[2];
        Object o = (Cloneable) new Object[3];
        Serializable s2 = (Serializable) new Serializable[3];
        Serializable s3 = (Serializable) null;
    }
}
