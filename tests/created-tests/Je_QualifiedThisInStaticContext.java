public class Je_QualifiedThisInStaticContext {
    public Je_QualifiedThisInStaticContext() {}
    public static void someMethod() {
        Je_QualifiedThisInStaticContext t = Je_QualifiedThisInStaticContext.this;
    }
}
