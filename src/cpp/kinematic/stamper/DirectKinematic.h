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

#include "PositionMatrix.h"
#include "AngularMatrix.h"

#include "gmtl/gmtl.h"

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{

		class DirectKinematic
		{
		public:
			DirectKinematic() {}
			virtual ~DirectKinematic() {}

			void initialize();			
			gmtl::Point3f calculate(gmtl::Vec3f angle);
		private:
			gmtl::Point3f newtonRaphsonMethod(gmtl::Vec3f angle, gmtl::Point3f approxPosition);
	
			PositionMatrix *basePositionMatrix;
			AngularMatrix *baseAngularMatrix;
	
			float epsilonAngle;
			float epsilonPosition;
			int iteration;
		};

	}
}
#endif /*DIRECTKINEMATIC_H_*/
