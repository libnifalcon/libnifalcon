//Original code by Kevin Ouellet
//kouellet@users.sourceforge.net
#ifndef DIRECTKINEMATIC_H_
#define DIRECTKINEMATIC_H_

#include "PositionMatrix.h"
#include "AngularMatrix.h"

#include "gmtl/gmtl.h"

namespace libnifalcon
{
	namespace stamper
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
