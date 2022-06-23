public class JavaMain {
    public static void main (String[] args) {
        System.out.println("Ok!");
        new JavaMain().show();
    }

    // native 声明非 java 方法
    public native void show();
    static {
        System.loadLibrary("jni1");
    }
}
