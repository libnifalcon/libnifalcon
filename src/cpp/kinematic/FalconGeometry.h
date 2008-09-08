//Original code by Kevin Ouellet
//kouellet@users.sourceforge.net
#ifndef FALCONGEOMETRY_H_
#define FALCONGEOMETRY_H_

namespace libnifalcon
{
	const static int WHEEL_SLOTS_NUMBER = 320; // Number of transparent slot on the encoder wheel, 
	const static int SHAFT_DIAMETER = 14.25f; // Diameter of the motor shaft
	const static int SMALL_ARM_DIAMETER = 110.0f; // Small diameter of the arm
	const static int THETA_OFFSET_ANGLE = 35; // When encoder value = 0 (approx)

	const static float a = 60;
	const static float b = 102.5;
	const static float c = 14;
	const static float d = 11.25;
	const static float e = d;
	const static float f = -25;
	const static float s = -27.330909;
	const static float r = 37.233758;
	
	// degrees = {90, -30, 150} -> radians = {PI/2,-PI/6,-5*PI/6}
	// There's an offset of 15 degrees per axis
	const static double PI = 3.14159265;
	const static double OFFSET_ANGLE = (PI/12);
	const static float phy[] = { PI/2 + OFFSET_ANGLE, -PI/6 + OFFSET_ANGLE, -5*PI/6  + OFFSET_ANGLE};
}

#endif /*FALCONGEOMETRY_H_*/
