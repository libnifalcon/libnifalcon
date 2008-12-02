/***
 * @file FalconGeometry.h
 * @brief Constants defining the mechanical measurements of the falcon
 * @author Kevin Ouellet (kouellet@users.sourceforge.net)
 * @version $Id$
 * @copyright (c) 2007-2008 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * $HeadURL$
 * 
 * Project info at http://libnifalcon.sourceforge.net/ 
 *
 * Many of the following values refer to a diagram from R.E. Stamper Kinematics Paper, page 30  
 * Viewable at http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf
 *
 */

#ifndef FALCONGEOMETRY_H_
#define FALCONGEOMETRY_H_

namespace libnifalcon
{
	const static float WHEEL_SLOTS_NUMBER = 320; /*!< Number of transparent slot on the internal encoder wheel */
	const static float SHAFT_DIAMETER = 0.01425f; /*!< Diameter of the motor shaft, in meters */
	const static float SMALL_ARM_DIAMETER = 0.110f; /*!< Small diameter of the arm in meters */
	const static float THETA_OFFSET_ANGLE = 35; /*!< When encoder value = 0 (approx) */

	const static float a = 0.060; /*!< Distance from leg base to start of knee, in meters */
	const static float b = 0.1025; /*!< Length of shin parallelogram side, in meters */
	const static float c = 0.014; /*!< Length from shin connection point to end effector center, u component, in meters */
	const static float d = .01125; /*!< Length of shin to end effector connection joint, in meters */
	const static float e = d; /*!< Length of knee to shin connection joint, in meters */
	const static float f = -.025; /*!< Length from shin connection point to end effector center, v component, in meters */
	const static float r = .037233758; /*!< Distance from fixed frame origin to leg base, u component, in meters */
	const static float s = -.027330909; /*!< Distance from fixed frame origin to leg base, v component, in meters */
	
	const static double PI = 3.14159265; /*!< PI Constant, to save having to include one */
	const static double OFFSET_ANGLE = (PI/12); /*!< Offset of each axis from the desk plane (15 degrees) */
	const static float phy[] = { PI/2 + OFFSET_ANGLE, -PI/6 + OFFSET_ANGLE, -5*PI/6  + OFFSET_ANGLE}; /*!< Angles of each of the three legs, in radians */
}

#endif /*FALCONGEOMETRY_H_*/
