//Original code by Kevin Ouellet
//kouellet@users.sourceforge.net
#ifndef POSITIONMATRIX_H_
#define POSITIONMATRIX_H_

#include "gmtl/gmtl.h"
#include "AngularMatrix.h"

namespace libnifalcon
{
	namespace stamper
	{

		class PositionMatrix
		{
		public:
			PositionMatrix(gmtl::Vec3f center, float range, int density);
			virtual ~PositionMatrix();
			void populate(AngularMatrix *angularMatrix);
			float getRange();
		private:
			gmtl::Vec3f center;
			float range;
			float delta;
			int density;
		};
	}
}

#endif /*POSITIONMATRIX_H_*/
