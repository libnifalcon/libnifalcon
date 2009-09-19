/***
 * @file FalconGeometry.h
 * @brief Constants defining the mechanical measurements of the falcon
 * @author Kevin Ouellet (kouellet@users.sourceforge.net)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 * Many of the following values refer to a diagram from R.E. Stamper Kinematics Paper, page 30
 * Viewable at http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf
 *
 */

#ifndef FALCONGEOMETRY_H_
#define FALCONGEOMETRY_H_

namespace libnifalcon
{
	const static double WHEEL_SLOTS_NUMBER = 320; /*!< Number of transparent slot on the internal encoder wheel */
	const static double SHAFT_DIAMETER = 0.01425; /*!< Diameter of the motor shaft, in meters */
	const static double SMALL_ARM_DIAMETER = 0.110; /*!< Small diameter of the arm in meters */
	const static double THETA_OFFSET_ANGLE = 35; /*!< When encoder value = 0 (approx) */

	const static double a = 0.060; /*!< Distance from leg base to start of knee, in meters */
	const static double b = 0.1025; /*!< Length of shin parallelogram side, in meters */
	const static double c = 0.01443; /*!< Length from shin connection point to end effector center, u component, in meters */
	const static double d = 0.01125; /*!< Length of shin to end effector connection joint, in meters */
	const static double e = d; /*!< Length of knee to shin connection joint, in meters */
	const static double f = -0.025; /*!< Length from shin connection point to end effector center, v component, in meters */
	const static double r = 0.0400; /*!< Distance from fixed frame origin to leg base, u component, in meters */
	const static double s = -0.02309; /*!< Distance from fixed frame origin to leg base, v component, in meters */

	const static double PI = 3.14159265; /*!< PI Constant, to save having to include one */
	const static double OFFSET_ANGLE = (PI/12); /*!< Offset of each axis from the desk plane (15 degrees) */
	const static double phy[] = { PI/2 + OFFSET_ANGLE, -PI/6 + OFFSET_ANGLE, -5*PI/6  + OFFSET_ANGLE}; /*!< Angles of each of the three legs, in radians */
}

#endif /*FALCONGEOMETRY_H_*/
