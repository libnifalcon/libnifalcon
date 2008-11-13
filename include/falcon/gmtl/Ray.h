/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          Ray.h,v
 * Date modified: 2003/02/23 07:16:49
 * Version:       1.2
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
#ifndef _GMTL_RAY_H_
#define _GMTL_RAY_H_

#include <falcon/gmtl/Point.h>
#include <falcon/gmtl/Vec.h>
#include <falcon/gmtl/VecOps.h>

namespace gmtl {

/**
 * Describes a ray. This is represented by a point origin O and a
 * normalized vector direction. Any point on the ray can be described as
 *
 * P(s) = O + Vs
 *
 * where 0 <= s <= 1
 *
 * @param DATA_TYPE     the internal type used for the point and vector
 */
template< class DATA_TYPE >
class Ray
{
public:
   /**
    * Constructs a ray at the origin with a zero vector.
    */
   Ray()
   {}

   /**
    * Constructs a ray with the given origin and vector.
    *
    * @param origin     the point at which the ray starts
    * @param dir        the vector describing the direction and length of the
    *                   ray starting at origin
    */
   Ray( const Point<DATA_TYPE, 3>& origin, const Vec<DATA_TYPE, 3>& dir )
      : mOrigin( origin ), mDir( dir )
   {}

   

   /**
    * Constructs an exact duplicate of the given ray.
    *
    * @param lineseg    the ray to copy
    */
   Ray( const Ray& lineseg )
   {
      mOrigin = lineseg.mOrigin;
      mDir = lineseg.mDir;
   }

   /**
    * Gets the origin of the ray.
    *
    * @return  the point at the beginning of the line
    */
   const Point<DATA_TYPE, 3>& getOrigin() const
   {
      return mOrigin;
   }

   /**
    * Sets the origin point for this ray.
    *
    * @param origin     the point at which the ray starts
    */
   void setOrigin( const Point<DATA_TYPE, 3>& origin )
   {
      mOrigin = origin;
   }

   /**
    * Gets the vector describing the direction and length of the ray.
    *
    * @return  the ray's vector
    */
   const Vec<DATA_TYPE, 3>& getDir() const
   {
      return mDir;
   }

   /**
    * Sets the vector describing the direction and length of the ray.
    *
    * @param dir     the ray's vector
    */
   void setDir( const Vec<DATA_TYPE, 3>& dir )
   {
      mDir = dir;
   }

public:
   /**
    * The origin of the ray.
    */
   Point<DATA_TYPE, 3> mOrigin;

   /**
    * The vector along which the ray lies.
    */
   Vec<DATA_TYPE, 3> mDir;
};


// --- helper types --- //
typedef Ray<float>  Rayf;
typedef Ray<double> Rayd;
}

#endif
