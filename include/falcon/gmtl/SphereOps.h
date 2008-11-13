/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          SphereOps.h,v
 * Date modified: 2003/08/30 17:13:13
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
#ifndef _GMTL_SPHEREOPS_H_
#define _GMTL_SPHEREOPS_H_

#include <falcon/gmtl/Sphere.h>
#include <falcon/gmtl/VecOps.h>
#include <falcon/gmtl/Math.h>

namespace gmtl
{

/** @ingroup Compare Sphere 
 * @name Sphere Comparitors
 * @{
 */

/**
 * Compare two spheres to see if they are EXACTLY the same. 
 *
 * @param s1      the first sphere to compare
 * @param s2      the second sphere to compare
 *
 * @return  true if they are equal, false otherwise
 */
template< class DATA_TYPE >
inline bool operator==( const Sphere<DATA_TYPE>& s1, const Sphere<DATA_TYPE>& s2 )
{
   return ( (s1.mCenter == s2.mCenter) && (s1.mRadius == s2.mRadius) );
}

/**
 * Compare two spheres to see if they are not EXACTLY the same. 
 *
 * @param s1      the first sphere to compare
 * @param s2      the second sphere to compare
 *
 * @return  true if they are not equal, false otherwise
 */
template< class DATA_TYPE >
inline bool operator!=( const Sphere<DATA_TYPE>& s1, const Sphere<DATA_TYPE>& s2 )
{
   return (! (s1 == s2));
}

/**
 * Compare two spheres to see if they are the same within the given tolerance.
 *
 * @param s1      the first sphere to compare
 * @param s2      the second sphere to compare
 * @param eps     the tolerance value to use
 *
 * @pre eps must be >= 0
 *
 * @return  true if they are equal within a tolerance, false otherwise
 */
template< class DATA_TYPE >
inline bool isEqual( const Sphere<DATA_TYPE>& s1, const Sphere<DATA_TYPE>& s2, const DATA_TYPE& eps )
{
   gmtlASSERT( eps >= 0 );
   return ( (isEqual(s1.mCenter, s2.mCenter, eps)) &&
            (Math::isEqual(s1.mRadius, s2.mRadius, eps)) );
}
/** @} */

} // namespace gmtl

#endif

