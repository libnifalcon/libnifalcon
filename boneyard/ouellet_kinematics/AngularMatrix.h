/***
 * @file AngularMatrix.h
 * @brief Lookup table to find cartesian coordinates of end effector based on leg angles
 * @author Kevin Ouellet (kouellet@users.sourceforge.net) / Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

#ifndef ANGULARMATRIX_H_
#define ANGULARMATRIX_H_

#include "stdint.h"
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
			/**
			 * Constructor
			 *
			 * @param center Cartesian point representing center of the workspace (in meters)
			 * @param range Range of each axis of the workspace (in meters)
			 * @param size Dimension for single dimension of the lookup table (lookup table is 3D)
			 */
			AngularMatrix(const gmtl::Vec3f &center, const float &range, const unsigned int size);
			/**
			 * Destructor
			 *
			 *
			 */
			virtual ~AngularMatrix();
			/**
			 * Given a position of the end effector, find and store the angles needed to achieve it
			 *
			 * @param position Cartesian point representing the end effector position
			 *
			 * @return true is position is reachable, false otherwise
			 */
			bool setPosition(const gmtl::Point3f &position);

			/**
			 * Returns amount of lookup table currently filled
			 *
			 *
			 * @return Percent of lookup table filled
			 */
			float isComplete();

			/**
			 * Returns the range of the cubic workspace axes
			 *
			 *
			 * @return range of a single axis of the cubic workspace
			 */
			float getRange();

			/**
			 * Given a set of leg angles (in degrees), return the end effector position it represents
			 *
			 * @param angle Angle of the legs
			 * @param position Resulting position of the end effector
			 *
			 * @return true is position is reachable, false otherwise
			 */
			bool getPosition(const gmtl::Vec3f &angle, gmtl::Point3f &position);

			/**
			 * Returns the distance between values in the lookup table
			 *
			 *
			 * @return Distance between positional values in lookup table
			 */
			float getDelta();

			/**
			 * Returns the maximum distance between values on any single axis
			 *
			 *
			 * @return Maximum distance between values on any single axis
			 */
			float getMaximumPositionDelta();

			typedef boost::multi_array<float, 4> angular_array;
			typedef angular_array::index angular_array_index;

			gmtl::Vec3f getVec(unsigned int a, unsigned int b, unsigned int c);
			void copyLookupTable();
		private:
			/**
			 * Copies the value of vector into a cell into the current matrix
			 *
			 * @param a Indexer to first dimension of matrix (Top encoder value)
			 * @param b Indexer to second dimension of matrix (Right encoder value)
			 * @param c Indexer to third dimension of matrix (Left encoder value)
			 * @param vector Vector to copy
			 */
			void setVec(const unsigned int a, const unsigned int b, const unsigned int c, const gmtl::Vec3f &vector);

			/**
			 * Conveinence. Does converstion and calls above setVec function
			 */
			void setVec(const unsigned int a, const unsigned int b, const unsigned int c, const gmtl::Point3f &vector);

			/**
			 * Given encoder values a, b, c, return
			 *
			 * @param a Indexer to first dimension of matrix (Top encoder value)
			 * @param b Indexer to second dimension of matrix (Right encoder value)
			 * @param c Indexer to third dimension of matrix (Left encoder value)
			 *
			 * @return Position at the given encoder values
			 */

			angular_array matrix; /**< The lookup table */
			gmtl::Vec3f center; /**< The center of the workspace */
			float range;		/**< The range of each axis of the workspace */
			float delta;		/**< The distance between positions in the lookup table */
			unsigned int size;	/**< The size of the lookup table */
			gmtl::Vec3f minimum; /**< Minimum angle set */
			gmtl::Vec3f maximum; /**< Maximum angle set */
			gmtl::Point3f minimumPosition; /**< Minimum point achievable */
			gmtl::Point3f maximumPosition; /**< Maximum point achievable */
		};

	}
}
#endif /*ANGULARMATRIX_H_*/
