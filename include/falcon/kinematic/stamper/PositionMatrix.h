/***
 * @file PositionMatrix.h
 * @brief Lookup table generator for discerning positions from angles (workaround to avoid calculating the 32 degree polynomial from Stamper paper)
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
#ifndef POSITIONMATRIX_H_
#define POSITIONMATRIX_H_

#include "falcon/gmtl/gmtl.h"
#include "falcon/kinematic/stamper/AngularMatrix.h"

namespace libnifalcon
{
	namespace StamperKinematicImpl
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
