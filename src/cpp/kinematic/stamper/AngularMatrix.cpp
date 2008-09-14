//Original code by Kevin Ouellet
//kouellet@users.sourceforge.net
#include "AngularMatrix.h"
#include "StamperUtils.h"

#include "core/FalconGeometry.h"
#include <iostream>

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{

		AngularMatrix::AngularMatrix(gmtl::Vec3f center, float range, unsigned int size)
		{
			this->center = center;
			this->range = range;
			this->size = size;
	
			matrix.resize(boost::extents[size][size][size][3]);
	
			minimum[0] = center[0]-(range/2);
			minimum[1] = center[1]-(range/2);
			minimum[2] = center[2]-(range/2);

			maximum[0] = center[0]+(range/2);
			maximum[1] = center[1]+(range/2);
			maximum[2] = center[2]+(range/2);
	
			delta = range/size;
	
			minimumPosition[0] = 100;
			minimumPosition[1] = 100;
			minimumPosition[2] = 250;

			maximumPosition[0] = -100;
			maximumPosition[1] = -100;
			maximumPosition[2] = 50;
	
			for (int i=0; i<size; i++)
				for (int j=0; j<size; j++)
					for (int k=0; k<size; k++)
					{
						setVec(i,j,k,gmtl::Vec3f(0,0,0));
					}
		}

		AngularMatrix::~AngularMatrix()
		{
		}

		bool AngularMatrix::setPosition(gmtl::Point3f position)
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

		bool AngularMatrix::getPosition(gmtl::Vec3f angle, gmtl::Point3f &position)
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

		void AngularMatrix::setVec(unsigned int a, unsigned int b, unsigned int c, gmtl::Vec3f vector)
		{
			matrix[a][b][c][0]=vector[0];
			matrix[a][b][c][1]=vector[1];
			matrix[a][b][c][2]=vector[2];
		}

		gmtl::Vec3f AngularMatrix::getVec(unsigned int a, unsigned int b, unsigned int c)
		{
			return gmtl::Vec3f(matrix[a][b][c][0],matrix[a][b][c][1],matrix[a][b][c][2]);
		}

	}
}
