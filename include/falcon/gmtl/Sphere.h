/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          Sphere.h,v
 * Date modified: 2002/05/20 22:39:22
 * Version:       1.5
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
#ifndef _GMTL_SPHERE_H_
#define _GMTL_SPHERE_H_

#include <falcon/gmtl/Point.h>

namespace gmtl
{

/**
 * Describes a sphere in 3D space by its center point and its radius.
 *
 * @param DATA_TYPE     the internal type used for the point and radius
 * @ingroup Types
 */
template<class DATA_TYPE>
class Sphere
{
public:
   typedef DATA_TYPE DataType;

public:
   /**
    * Constructs a sphere centered at the origin with a radius of 0.
    */
   Sphere()
      : mRadius( 0 )
   {}

   /**
    * Constructs a sphere with the given center and radius.
    *
    * @param center     the point at which to center the sphere
    * @param radius     the radius of the sphere
    */
   Sphere( const Point<DATA_TYPE, 3>& center, const DATA_TYPE& radius )
      : mCenter( center ), mRadius( radius )
   {}

   /**
    * Constructs a duplicate of the given sphere.
    *
    * @param sphere     the sphere to make a copy of
    */
   Sphere( const Sphere<DATA_TYPE>& sphere )
      : mCenter( sphere.mCenter ), mRadius( sphere.mRadius )
   {}

   /**
    * Gets the center of the sphere.
    *
    * @return  the center point of the sphere
    */
   const Point<DATA_TYPE, 3>& getCenter() const
   {
      return mCenter;
   }

   /**
    * Gets the radius of the sphere.
    *
    * @return  the radius of the sphere
    */
   const DATA_TYPE& getRadius() const
   {
      return mRadius;
   }

   /**
    * Sets the center point of the sphere.
    *
    * @param center     the new point at which to center the sphere
    */
   void setCenter( const Point<DATA_TYPE, 3>& center )
   {
      mCenter = center;
   }

   /**
    * Sets the radius of the sphere.
    *
    * @param radius     the new radius of the sphere
    */
   void setRadius( const DATA_TYPE& radius )
   {
      mRadius = radius;
   }

public:
   /**
    * The center of the sphere.
    */
   Point<DATA_TYPE, 3> mCenter;

   /**
    * The radius of the sphere.
    */
   DATA_TYPE mRadius;
};

// --- helper types --- //
typedef Sphere<float>   Spheref;
typedef Sphere<double>  Sphered;

};

#endif
