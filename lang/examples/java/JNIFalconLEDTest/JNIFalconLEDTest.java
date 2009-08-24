class JNIFalconLEDTest
{
    static {
        System.loadLibrary("JNIFalcon");
    }
    
    public static void main(String argv[])
    {
        JNIFalconLED l = new JNIFalconLED();
        l.runDemo();
    }
}
