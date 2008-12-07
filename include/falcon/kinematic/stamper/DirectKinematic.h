/***
 * @file DirectKinematic.h
 * @brief DK implementation for the Novint Falcon based on paper by R.E. Stamper (http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net)
 * @version $Id$
 * @copyright (c) 2007-2008 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * $HeadURL$
 * 
 * Project info at http://libnifalcon.sourceforge.net/ 
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
			DirectKinematic();
			virtual ~DirectKinematic() {}

			void initialize();			
			gmtl::Point3f calculate(gmtl::Vec3f &angle);
		private:
			gmtl::Point3f newtonRaphsonMethod(gmtl::Vec3f &angle, gmtl::Point3f &approxPosition);
	
			PositionMatrix m_basePositionMatrix;
			AngularMatrix m_baseAngularMatrix;
	
			float epsilonAngle;
			float epsilonPosition;
			int iteration;

			const static float POSITION_RANGE = 0.200;
			const static uint32_t POSITION_MATRIX_DENSITY = 64;
		};

	}
}
#endif /*DIRECTKINEMATIC_H_*/
