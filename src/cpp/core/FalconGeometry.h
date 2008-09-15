//! Falcon Geometry Constants
/*!
  Measurements for use with Novint Falcon Kinematics Models
  
  Many of the following values refer to diagram from R.E. Stamper Kinematics Paper, page 30
  
  Viewable at http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf

  Original code and all measurements by Kevin Ouellet
  
  kouellet@users.sourceforge.net
*/


#ifndef FALCONGEOMETRY_H_
#define FALCONGEOMETRY_H_

namespace libnifalcon
{
	const static int WHEEL_SLOTS_NUMBER = 320; /*!< Number of transparent slot on the internal encoder wheel */
	const static int SHAFT_DIAMETER = 14.25f; /*!< Diameter of the motor shaft, in mm */
	const static int SMALL_ARM_DIAMETER = 110.0f; /*!< Small diameter of the arm in mm */
	const static int THETA_OFFSET_ANGLE = 35; /*!< When encoder value = 0 (approx) */

	const static float a = 60; /*!< Distance from leg base to start of knee, in mm */
	const static float b = 102.5; /*!< Length of shin parallelogram side, in mm */
	const static float c = 14; /*!< Length from shin connection point to end effector center, u component, in mm */
	const static float d = 11.25; /*!< Length of shin to end effector connection joint, in mm */
	const static float e = d; /*!< Length of knee to shin connection joint, in mm */
	const static float f = -25; /*!< Length from shin connection point to end effector center, v component, in mm */
	const static float r = 37.233758; /*!< Distance from fixed frame origin to leg base, u component, in mm */
	const static float s = -27.330909; /*!< Distance from fixed frame origin to leg base, v component, in mm */
	
	const static double PI = 3.14159265; /*!< PI Constant, to save having to include one */
	const static double OFFSET_ANGLE = (PI/12); /*!< Offset of each axis from the desk plane (15 degrees) */
	const static float phy[] = { PI/2 + OFFSET_ANGLE, -PI/6 + OFFSET_ANGLE, -5*PI/6  + OFFSET_ANGLE}; /*!< Angles of each of the three legs, in radians */
}

#endif /*FALCONGEOMETRY_H_*/
