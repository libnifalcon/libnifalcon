/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          Point.h,v
 * Date modified: 2007/04/24 11:40:35
 * Version:       1.19
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
#ifndef _GMTL_POINT_H_
#define _GMTL_POINT_H_

#include <falcon/gmtl/Defines.h>
#include <falcon/gmtl/VecBase.h>

namespace gmtl
{

/** Point
 *  Use points when you need to represent a position.  Don't use points to
 *  represent a Vector.  One difference you should note is that
 *  ceratain matrix operations are different between Point and Vec such as
 *  xform and operator*.  A Vec xform by matrix is simply a rotation,
 *  while a Point xformed by a matrix is a full matrix transform
 *  (rotation, skew, translation, scale).
 *
 * @see Point3f
 * @see Point4f
 * @see Point3d
 * @see Point4f
 * @ingroup Types
 */
template<class DATA_TYPE, unsigned SIZE>
class Point : public VecBase<DATA_TYPE, SIZE>
{
public:
   typedef DATA_TYPE DataType;
   enum Params { Size = SIZE };

   /** Placeholder for the base type */
   typedef VecBase<DATA_TYPE, SIZE> BaseType;
   typedef Point<DATA_TYPE, SIZE> VecType;

public:
   /** Default constructor
    */
   Point()
   {
      for (unsigned i = 0; i < SIZE; ++i)
         this->mData[i] = (DATA_TYPE)0;
   }

   /** @name Value constructors
    * Construct with copy of rVec
    */
   //@{
   /*
   Point(const Point<DATA_TYPE, SIZE>& rVec)
      : BaseType(static_cast<BaseType>(rVec))
   {;}
   */

#ifdef GMTL_NO_METAPROG
   Point(const VecBase<DATA_TYPE, SIZE>& rVec)
      : BaseType(rVec)
   {;}
#else
   template<typename REP2>
   Point( const VecBase<DATA_TYPE, SIZE, REP2>& rVec )
      : BaseType( rVec )
   {
   }
#endif

   /**
    * Construct a 2-D point with 2 given values
    */
   Point(const DATA_TYPE& val0,const DATA_TYPE& val1)
   : BaseType(val0, val1)
   {
      // @todo need compile time assert
      gmtlASSERT( SIZE == 2 && "out of bounds element access in Point" );
   }

   /**
    * Construct a 3-D point with 2 given values
    */
   Point(const DATA_TYPE& val0,const DATA_TYPE& val1,const DATA_TYPE& val2)
   : BaseType(val0, val1, val2)
   {
      // @todo need compile time assert
      gmtlASSERT( SIZE == 3 && "out of bounds element access in Point" );
   }

   /**
    * Construct a 4-D point with 2 given values
    */
   Point(const DATA_TYPE& val0,const DATA_TYPE& val1,const DATA_TYPE& val2,const DATA_TYPE& val3)
   : BaseType(val0, val1, val2, val3)
   {
      // @todo need compile time assert
      gmtlASSERT( SIZE == 4 && "out of bounds element access in Point" );
   }
   //@}

   /** Assign from different rep. */
#ifdef GMTL_NO_METAPROG
   inline VecType& operator=(const VecBase<DATA_TYPE,SIZE>& rhs)
   {
      BaseType::operator=(rhs);
      return *this;
   }
#else
   template<typename REP2>
   inline VecType& operator=(const VecBase<DATA_TYPE,SIZE,REP2>& rhs)
   {
      BaseType::operator=(rhs);
      return *this;
   }
#endif

};

// --- helper types --- //
typedef Point<int,2> Point2i;
typedef Point<float,2> Point2f;
typedef Point<double,2> Point2d;
typedef Point<int, 3> Point3i;
typedef Point<float,3> Point3f;
typedef Point<double,3> Point3d;
typedef Point<int, 4> Point4i;
typedef Point<float,4> Point4f;
typedef Point<double,4> Point4d;


}

#endif
