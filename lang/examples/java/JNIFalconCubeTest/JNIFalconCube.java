import java.util.*;
import java.lang.Math.*;
class JNIFalconCube
{
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
        FalconVec3d pos_fv = new FalconVec3d();
        FalconVec3d force_fv = new FalconVec3d();

        double pos[] = {0,0,0};
        double force[] = {0,0,0};

        double cornerA[] = {-.030, -.030, .095};
        double cornerB[] = {.030, .030, .155};
        int stiffness = 1000;

        int axis;
            

        while(true) {
            if(!fdd.runIOLoop(1 | 2)) {                
                continue;
            }
            pos_fv = fdd.getPosition();
            pos[0] = pos_fv.getX();
            pos[1] = pos_fv.getY();
            pos[2] = pos_fv.getZ();

            int outside = 3;            
            int closest = -1;
            double dist = 10000;
            for(axis = 0; axis < 3; axis++) {                
                force[axis] = 0;
                if(pos[axis] > cornerA[axis] && pos[axis] < cornerB[axis]) {
                    double dA = pos[axis]-cornerA[axis];
                    double dB = pos[axis]-cornerB[axis];
                    if(Math.abs(dA) < Math.abs(dist)) {
                        dist = dA;
                        closest = axis;
                    }
                    if (Math.abs(dB) < Math.abs(dist)) {
                        dist = dB;
                        closest = axis;
                    }
                    outside--;
                }
            }
            if(closest > -1 && outside == 0) {                
                force[closest] = -stiffness*dist;
            }

            force_fv.setX(force[0]);
            force_fv.setY(force[1]);
            force_fv.setZ(force[2]);
            fdd.setForces(force_fv);
        }
    }
}
            
