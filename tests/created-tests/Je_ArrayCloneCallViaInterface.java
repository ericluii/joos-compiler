// Should fail beacuse clone() cannot be found in
// java.lang.Cloneable

public class Je_ArrayCloneCallViaInterface {
    public Je_ArrayCloneCallViaInterface() {
        java.lang.Cloneable x = new int[2];
        x.clone();
    }
}
