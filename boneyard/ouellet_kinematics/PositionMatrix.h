/***
 * @file PositionMatrix.h
 * @brief Lookup table generator for discerning positions from angles (workaround to avoid calculating the 32 degree polynomial from Stamper paper)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net)/Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef POSITIONMATRIX_H_
#define POSITIONMATRIX_H_

#include "stdint.h"
#include "falcon/gmtl/gmtl.h"
#include "falcon/core/FalconLogger.h"
#include "falcon/kinematic/stamper/AngularMatrix.h"

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{

		class PositionMatrix
		{
		public:
			PositionMatrix(const double* center, const float& range, const unsigned int density);
			virtual ~PositionMatrix();
			void populate(AngularMatrix& angularMatrix);
			float getRange();
		private:
			double m_matrixCenter[3];
			float m_matrixRange;
			float m_matrixDelta;
			int m_matrixDensity;
			DECLARE_LOGGER();
		};
	}
}

#endif /*POSITIONMATRIX_H_*/
