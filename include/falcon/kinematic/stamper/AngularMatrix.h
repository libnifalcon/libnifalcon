/***
 * @file AngularMatrix.h
 * @brief Lookup table to find cartesian coordinates of end effector based on leg angles
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
#ifndef ANGULARMATRIX_H_
#define ANGULARMATRIX_H_

#include "falcon/gmtl/gmtl.h"
#include "InverseKinematic.h"
#include "boost/multi_array.hpp"

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{

		class AngularMatrix
		{ 
		public:
			AngularMatrix(const gmtl::Vec3f &center, const float &range, const uint32_t size);
			virtual ~AngularMatrix();
			bool setPosition(const gmtl::Point3f &position);
			float isComplete();
			float getRange();
			bool getPosition(const gmtl::Vec3f &angle, gmtl::Point3f &position);
			float getDelta();
			float getMaximumPositionDelta();
			typedef boost::multi_array<float, 4> angular_array;
			typedef angular_array::index angular_array_index;
		private:
			void setVec(const uint32_t a, const uint32_t b, const uint32_t c, const gmtl::Vec3f &vector);
			void setVec(const uint32_t a, const uint32_t b, const uint32_t c, const gmtl::Point3f &vector);
			gmtl::Vec3f getVec(uint32_t a, uint32_t b, uint32_t c);

			angular_array matrix;
			gmtl::Vec3f center;
			float range;
			float delta;
			unsigned int size;
			gmtl::Vec3f minimum;
			gmtl::Vec3f maximum;
			gmtl::Point3f minimumPosition;
			gmtl::Point3f maximumPosition;
		};

	}
}
#endif /*ANGULARMATRIX_H_*/
