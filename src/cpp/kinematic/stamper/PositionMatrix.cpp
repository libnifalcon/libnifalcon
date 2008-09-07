//Original code by Kevin Ouellet
//kouellet@users.sourceforge.net
#include "PositionMatrix.h"

namespace libnifalcon
{
	namespace stamper
	{

		PositionMatrix::PositionMatrix(gmtl::Vec3f center, float range, int density)
		{
			this->center = center;
			this->range = range;
			this->density = density;
	
			delta = range/((float)density);
		}

		PositionMatrix::~PositionMatrix()
		{
		}

		void PositionMatrix::populate(AngularMatrix *angularMatrix)
		{
			for (int i=-(density/2); i<=(density/2); i++)
				for (int j=-(density/2); j<=(density/2); j++)
					for (int k=-(density/2); k<=(density/2); k++)
					{
						angularMatrix->setPosition(gmtl::Vec3f(center[0]+(i*delta),center[1]+(j*delta),center[2]+(k*delta)));
					}
		}

		float PositionMatrix::getRange()
		{
			return range;
		}

	}
}
