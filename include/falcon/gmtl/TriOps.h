/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          TriOps.h,v
 * Date modified: 2004/09/16 21:21:10
 * Version:       1.9
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
#ifndef _GMTL_TRIOPS_H_
#define _GMTL_TRIOPS_H_

#include <falcon/gmtl/Tri.h>
#include <falcon/gmtl/Generate.h>
#include <falcon/gmtl/VecOps.h>

namespace gmtl
{
/** @ingroup Ops Tri
 *  @name Triangle Operations
 * @{
 */

/**
 * Computes the point at the center of the given triangle.
 *
 * @param tri     the triangle to find the center of
 *
 * @return  the point at the center of the triangle
 */
template< class DATA_TYPE >
Point<DATA_TYPE, 3> center( const Tri<DATA_TYPE>& tri )
{
   const float one_third = (1.0f/3.0f);
   return (tri[0] + tri[1] + tri[2]) * DATA_TYPE(one_third);
}

/**
 * Computes the normal for this triangle.
 *
 * @param tri     the triangle for which to compute the normal
 *
 * @return  the normal vector for tri
 */
template< class DATA_TYPE >
Vec<DATA_TYPE, 3> normal( const Tri<DATA_TYPE>& tri )
{
   Vec<DATA_TYPE, 3> normal = makeCross( gmtl::Vec<DATA_TYPE,3>(tri[1] - tri[0]), gmtl::Vec<DATA_TYPE,3>(tri[2] - tri[0]) );
   normalize( normal );
   return normal;
}
/** @} */

/** @ingroup Compare Tri
 *  @name Triangle Comparitors
 *  @{
 */

/**
 * Compare two triangles to see if they are EXACTLY the same.
 *
 * @param tri1    the first triangle to compare
 * @param tri2    the second triangle to compare
 *
 * @return  true if they are equal, false otherwise
 */
template< class DATA_TYPE >
bool operator==( const Tri<DATA_TYPE>& tri1, const Tri<DATA_TYPE>& tri2 )
{
   return ( (tri1[0] == tri2[0]) &&
            (tri1[1] == tri2[1]) &&
            (tri1[2] == tri2[2]) );
}

/**
 * Compare two triangle to see if they are not EXACTLY the same.
 *
 * @param tri1    the first triangle to compare
 * @param tri2    the second triangle to compare
 *
 * @return  true if they are not equal, false otherwise
 */
template< class DATA_TYPE >
bool operator!=( const Tri<DATA_TYPE>& tri1, const Tri<DATA_TYPE>& tri2 )
{
   return (! (tri1 == tri2));
}

/**
 * Compare two triangles to see if they are the same within the given tolerance.
 *
 * @param tri1    the first triangle to compare
 * @param tri2    the second triangle to compare
 * @param eps     the tolerance value to use
 *
 * @pre  eps must be >= 0
 *
 * @return  true if they are equal within the tolerance, false otherwise
 */
template< class DATA_TYPE >
bool isEqual( const Tri<DATA_TYPE>& tri1, const Tri<DATA_TYPE>& tri2,
              const DATA_TYPE& eps )
{
   gmtlASSERT( eps >= 0 );
   return ( isEqual(tri1[0], tri2[0], eps) &&
            isEqual(tri1[1], tri2[1], eps) &&
            isEqual(tri1[2], tri2[2], eps) );
}
/** @} */

} // namespace gmtl

#endif

