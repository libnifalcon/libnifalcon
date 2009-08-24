class JNIFalconCubeTest
{
    static {
        System.loadLibrary("JNIFalcon");
    }
    
    public static void main(String argv[])
    {
        JNIFalconCube l = new JNIFalconCube();
        l.runDemo();
    }
}
