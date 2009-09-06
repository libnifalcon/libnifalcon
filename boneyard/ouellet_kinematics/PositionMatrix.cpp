/***
 * @file PositionMatrix.cpp
 * @brief Lookup table generator for discerning positions from angles (workaround to avoid calculating the 32 degree polynomial from Stamper paper)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net) / Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#include "falcon/kinematic/stamper/PositionMatrix.h"

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{

		PositionMatrix::PositionMatrix(const double* center, const float& range, const unsigned int density) :
			m_matrixRange(range),
			m_matrixDensity(density),
			m_matrixDelta(range/(float)density),
			INIT_LOGGER("PositionMatrix")
		{
			LOG_INFO("Construction...");
			for(int i = 0; i < 3; ++i)
			{
				m_matrixCenter[i] = center[i];
			}
		}

		PositionMatrix::~PositionMatrix()
		{
			LOG_INFO("Destruction...");
		}

		void PositionMatrix::populate(AngularMatrix& angularMatrix)
		{
			/*
			std::cout << "Trying to populate!" << std::endl;
			LOG_INFO("Populating position matrix for stamper kinematics...");

			for (int i=-(m_matrixDensity/2); i<=(m_matrixDensity/2); i++)
				for (int j=-(m_matrixDensity/2); j<=(m_matrixDensity/2); j++)
					for (int k=-(m_matrixDensity/2); k<=(m_matrixDensity/2); k++)
					{
						angularMatrix.setPosition(gmtl::Vec3f(m_matrixCenter[0]+(i*m_matrixDelta),m_matrixCenter[1]+(j*m_matrixDelta),m_matrixCenter[2]+(k*m_matrixDelta)));
					}
					std::cout << "Populated!" << std::endl;
			*/
			/*
			std::cout << "{";
			for (int i=0; i<16; i++)
			{
				std::cout << "{";
				for (int j=0; j<16; j++)
				{
					std::cout << "{";
					for (int k=0; k<16; k++)
					{

						gmtl::Vec3f v = angularMatrix.getVec(i, j, k);
						std::cout << "{" << v[0] << "," << v[1] << "," << v[2] << "},";
					}
					std::cout << "},";
				}
				std::cout << "},";
			}
			std::cout << "}" << std::endl;
			*/
			angularMatrix.copyLookupTable();
		}

		float PositionMatrix::getRange()
		{
			return m_matrixRange;
		}

	}
}
