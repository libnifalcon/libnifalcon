import java.util.*;

class JNIFalconLED
{
    int total_time = 25;
    int led_limit = total_time;
    int led_count = 0;
    int led_move = -1;

    public void timeUpdate()
    {
        led_limit = led_limit + led_move;
        if(led_limit == 0 || led_limit == total_time) {                
            led_move = -led_move;
        }
    }

    public void runDemo()
    {
        FalconDeviceBridge fdd = new FalconDeviceBridge();
        System.out.println("Devices Available: " + fdd.getCount());
        if(!fdd.open(0)) {
            System.out.println("Cannot open device, exiting..." + fdd.getCount());
            return;
        }                       
        if(!fdd.loadFirmware()) {
            System.out.println("Cannot load firmware, exiting..." + fdd.getCount());
            return;
        }

        Timer t = new Timer();
        t.scheduleAtFixedRate(new TimerTask() {
                public void run() {
                    timeUpdate();
                }
            }, 0, 100);
        
        while(true) {
            if(!fdd.runIOLoop(1))
                continue;

            led_count = led_count + 1;
            if(led_count == total_time) {
                led_count = 0;
            }
    
            if(led_count > led_limit)
                fdd.setLEDStatus(4);
            else
                fdd.setLEDStatus(0);
        
        }
    }
}
            
