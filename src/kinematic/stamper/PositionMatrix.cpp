/***
 * @file PositionMatrix.cpp
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
#include "falcon/kinematic/stamper/PositionMatrix.h"
#include "falcon/kinematic/stamper/AngularMatrixLookup.h"

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
				m_matrixCenter[i] = center[i];
		}

		PositionMatrix::~PositionMatrix()
		{
			LOG_INFO("Destruction...");
		}

		void PositionMatrix::populate(AngularMatrix& angularMatrix)
		{
			LOG_INFO("Populating position matrix for stamper kinematics...");
			
			for (int i=-(m_matrixDensity/2); i<=(m_matrixDensity/2); i++)
				for (int j=-(m_matrixDensity/2); j<=(m_matrixDensity/2); j++)
					for (int k=-(m_matrixDensity/2); k<=(m_matrixDensity/2); k++)
					{
						angularMatrix.setPosition(gmtl::Vec3f(m_matrixCenter[0]+(i*m_matrixDelta),m_matrixCenter[1]+(j*m_matrixDelta),m_matrixCenter[2]+(k*m_matrixDelta)));
					}
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
			angularMatrix.copyLookupTable(stamper_lookup_matrix);
		}

		float PositionMatrix::getRange()
		{			
			return m_matrixRange;
		}

	}
}
