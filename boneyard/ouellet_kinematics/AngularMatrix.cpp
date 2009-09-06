/***
 * @file AngularMatrix.cpp
 * @brief Lookup table to find cartesian coordinates of end effector based on leg angles
 * @author Kevin Ouellet (kouellet@users.sourceforge.net) / Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#include "falcon/kinematic/stamper/AngularMatrix.h"
#include "falcon/kinematic/stamper/StamperUtils.h"
#include "falcon/kinematic/stamper/AngularMatrixLookup.h"

#include "falcon/core/FalconGeometry.h"
#include "falcon/gmtl/gmtl.h"
#include <iostream>

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{

		AngularMatrix::AngularMatrix(const gmtl::Vec3f& center, const float& range, unsigned int size)
		{
			this->center = center;
			this->range = range;
			this->size = size;

			matrix.resize(boost::extents[size][size][size][3]);

			minimum[0] = center[0]-(range/2.0f);
			minimum[1] = center[1]-(range/2.0f);
			minimum[2] = center[2]-(range/2.0f);

			maximum[0] = center[0]+(range/2.0f);
			maximum[1] = center[1]+(range/2.0f);
			maximum[2] = center[2]+(range/2.0f);

			delta = range/size;

			minimumPosition[0] = .100;
			minimumPosition[1] = .100;
			minimumPosition[2] = .250;

			maximumPosition[0] = -.100;
			maximumPosition[1] = -.100;
			maximumPosition[2] = 0.050;

			gmtl::Vec3f zero_vec(0,0,0);

			for (unsigned int i=0; i<size; i++)
				for (unsigned int j=0; j<size; j++)
					for (unsigned int k=0; k<size; k++)
					{
						setVec(i,j,k,zero_vec);
					}
		}

		AngularMatrix::~AngularMatrix()
		{
		}

	    void AngularMatrix::copyLookupTable()
		{
			//memcpy(matrix, lookup, 16*16*16*sizeof(float));
			for (int i=0; i<16; i++)
			{
				for (int j=0; j<16; j++)
				{
					for (int k=0; k<16; k++)
					{
						matrix[i][j][k][0] = stamper_lookup_matrix[i][j][k][0];
						matrix[i][j][k][1] = stamper_lookup_matrix[i][j][k][1];
						matrix[i][j][k][2] = stamper_lookup_matrix[i][j][k][2];
					}
				}
			}
		}

		bool AngularMatrix::setPosition(const gmtl::Point3f& position)
		{

			Angle allAngle = InverseKinematic::calculate(position);

			gmtl::Vec3f angle(allAngle.theta1[0],allAngle.theta1[1],allAngle.theta1[2]);

			//std::cout << position[0] << ":" << position[1] << ":" << position[2] << std::endl;
			//std::cout << angle[0] << ":" << angle[1] << ":" << angle[2] << std::endl;
			if (angle[0]>=minimum[0] && angle[0]<=maximum[0] &&
				angle[1]>=minimum[1] && angle[1]<=maximum[1] &&
				angle[2]>=minimum[2] && angle[2]<=maximum[2])
			{
				int a = (int)((angle[0]-minimum[0])/delta);
				int b = (int)((angle[0]-minimum[0])/delta);
				int c = (int)((angle[0]-minimum[0])/delta);
				if (a<0 || b<0 || c<0 || a>size || b>size || c>size) exit(-1);
				setVec((int)((angle[0]-minimum[0])/delta),
					   (int)((angle[1]-minimum[1])/delta),
					   (int)((angle[2]-minimum[2])/delta),
					   position);
				if (position[0] < minimumPosition[0])
					minimumPosition[0] = position[0];
				if (position[1] < minimumPosition[1])
					minimumPosition[1] = position[1];
				if (position[2] < minimumPosition[2])
					minimumPosition[2] = position[2];

				if (position[0] > maximumPosition[0])
					maximumPosition[0] = position[0];
				if (position[1] > maximumPosition[1])
					maximumPosition[1] = position[1];
				if (position[2] > maximumPosition[2])
					maximumPosition[2] = position[2];

				return true;
			}
			return false;
		}

		bool AngularMatrix::getPosition(const gmtl::Vec3f& angle, gmtl::Point3f &position)
		{
			if (angle[0]>=minimum[0] && angle[0]<=maximum[0] &&
				angle[1]>=minimum[1] && angle[1]<=maximum[1] &&
				angle[2]>=minimum[2] && angle[2]<=maximum[2])
			{
				position = getVec((int)((angle[0]-minimum[0])/delta),
								  (int)((angle[1]-minimum[1])/delta),
								  (int)((angle[2]-minimum[2])/delta));
				return true;
			}
			return false;
		}

		float AngularMatrix::isComplete()
		{
			int maxElement = size*size*size;
			int uniqueElement = 0;
			for (int i=0; i<size; i++)
				for (int j=0; j<size; j++)
					for (int k=0; k<size; k++)
					{
						if (getVec(i,j,k)[2] != 0) uniqueElement++;
					}

			return (((float)uniqueElement)/((float)maxElement))*100.0f;
		}

		float AngularMatrix::getRange()
		{
			return range;
		}

		float AngularMatrix::getDelta()
		{
			return delta;
		}

		float AngularMatrix::getMaximumPositionDelta()
		{
			float deltaX = maximumPosition[0] - minimumPosition[0];
			float deltaY = maximumPosition[1] - minimumPosition[1];
			float deltaZ = maximumPosition[2] - minimumPosition[2];

			if (deltaX >= deltaY)
				if (deltaX >= deltaZ)
					return deltaX;
				else
					return deltaZ;
			else
				return deltaY;
		}

		void AngularMatrix::setVec(const unsigned int a, const unsigned int b, const unsigned int c, const gmtl::Vec3f& vector)
		{
			matrix[a][b][c][0]=vector[0];
			matrix[a][b][c][1]=vector[1];
			matrix[a][b][c][2]=vector[2];
		}

		void AngularMatrix::setVec(const unsigned int a, const unsigned int b, const unsigned int c, const gmtl::Point3f& vector)
		{
			matrix[a][b][c][0]=vector[0];
			matrix[a][b][c][1]=vector[1];
			matrix[a][b][c][2]=vector[2];
		}

		gmtl::Vec3f AngularMatrix::getVec(const unsigned int a, const unsigned int b, const unsigned int c)
		{
			return gmtl::Vec3f(matrix[a][b][c][0],matrix[a][b][c][1],matrix[a][b][c][2]);
		}

	}
}
