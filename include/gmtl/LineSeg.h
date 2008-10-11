/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          LineSeg.h,v
 * Date modified: 2003/03/03 00:54:05
 * Version:       1.7
 * -----------------------------------------------------------------
 *
 *********************************************************** ggt-head end */
/*************************************************************** ggt-cpr beg
*
* GGT: The Generic Graphics Toolkit
* Copyright (C) 2001,2002 Allen Bierbaum
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
 ************************************************************ ggt-cpr end */
#ifndef _GMTL_LINESEG_H_
#define _GMTL_LINESEG_H_

#include <gmtl/Point.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Ray.h>

namespace gmtl {

/**
 * Describes a line segment. This is represented by a point origin O and a
 * vector spanning the length of the line segement originating at O. Thus any
 * point on the line segment can be described as
 *
 * P(s) = O + Vs
 *
 * where 0 <= s <= 1
 *
 * @param DATA_TYPE     the internal type used for the point and vector
 */
template <typename DATA_TYPE>
class LineSeg : public Ray<DATA_TYPE>
{
public:
   /**
    * Constructs a line segment at the origin with a zero vector.
    */
   LineSeg()
   {}

   /**
    * Constructs a line segment with the given origin and vector.
    *
    * @param origin     the point at which the line segment starts
    * @param dir        the vector describing the direction and length of the
    *                   line segment starting at origin
    */
   LineSeg( const Point<DATA_TYPE, 3>& origin, const Vec<DATA_TYPE, 3>& dir )
      : Ray<DATA_TYPE>( origin, dir )
   {}

   /**
    * Constructs an exact duplicate of the given line segment.
    *
    * @param ray    the line segment to copy
    */
   LineSeg( const LineSeg& ray ) : Ray<DATA_TYPE>( ray )
   {
   }

   /**
    * Constructs a line segment with the given beginning and ending points.
    *
    * @param beg     the point at the beginning of the line segment
    * @param end     the point at the end of the line segment
    */
   LineSeg( const Point<DATA_TYPE, 3>& beg, const Point<DATA_TYPE, 3>& end )
      : Ray<DATA_TYPE>()
   {
      this->mOrigin = beg;
      this->mDir = end - beg;
   }

   /**
    * Gets the length of this line segment.
    * @return the length of the line segment
    */
   DATA_TYPE getLength() const
   {
      return length(this->mDir);
   }
};


// --- helper types --- //
typedef LineSeg<float>  LineSegf;
typedef LineSeg<double> LineSegd;
}

#endif
