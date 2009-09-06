/***
 * @file DirectKinematic.h
 * @brief DK implementation for the Novint Falcon based on paper by R.E. Stamper (http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net)/Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef DIRECTKINEMATIC_H_
#define DIRECTKINEMATIC_H_

#include "falcon/kinematic/stamper/PositionMatrix.h"
#include "falcon/kinematic/stamper/AngularMatrix.h"

#include "falcon/gmtl/gmtl.h"

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{

		class DirectKinematic
		{
		public:
			/**
			 * Constructor
			 *
			 */
			DirectKinematic();

			/**
			 * Destructor
			 *
			 *
			 */
			virtual ~DirectKinematic() {}

			/**
			 * Initializes the lookup tables. Can take a while on some machines.
			 *
			 */
			void initialize();

			/**
			 * Given a set of angles, return the position of the end effector they represent
			 *
			 * @param angle Angle of the legs, in radians
			 *
			 * @return Cartesian position of the end effector, in meters
			 */
			gmtl::Point3f calculate(gmtl::Vec3f &angle);

			const static double POSITION_CENTER[3]; /**< Origin of the workspace (offset from the fixed frame origin */
			const static float POSITION_RANGE; /**< Range of each axis of the workspace */
			const static unsigned int POSITION_MATRIX_DENSITY; /**< Density of the positional lookup table */

		private:

			/**
			 * Used for position approximation.
			 * Given an angle and an approximate position, refine the position by finding the closest position available
			 * in the lookup table and refining it.
			 *
			 * @param angle Angle of the legs to calculate a position for
			 * @param approxPosition Current guess of the approximation
			 *
			 * @return Final result once positional error falls below a certain value
			 */
			gmtl::Point3f newtonRaphsonMethod(gmtl::Vec3f &angle, gmtl::Point3f &approxPosition);

			PositionMatrix m_basePositionMatrix; /**< Position lookup table */
			AngularMatrix m_baseAngularMatrix; /**< Angle lookup table */

			float epsilonAngle;	/**< Error in angle approximation */
			float epsilonPosition; /**< Error in position approximation */
			int iteration;		/**< Iteration counter for newton raphson method */

		};
	}
}

#endif /*DIRECTKINEMATIC_H_*/
