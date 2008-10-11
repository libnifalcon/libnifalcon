/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          EulerAngleOps.h,v
 * Date modified: 2004/05/25 16:36:28
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
#ifndef _GMTL_EULER_ANGLE_OPS_H_
#define _GMTL_EULER_ANGLE_OPS_H_

#include <gmtl/EulerAngle.h>

namespace gmtl
{

/** @ingroup Compare
 *  @name EulerAngle Comparitors
 *  @{
 */

/**
 * Compares 2 EulerAngles (component-wise) to see if they are exactly the same.
 *
 * @param e1   the first EulerAngle
 * @param e2   the second EulerAngle
 *
 * @return  true if e1 equals e2; false if they differ
 */
template<class DATA_TYPE, typename ROT_ORDER>
inline bool operator==(const EulerAngle<DATA_TYPE, ROT_ORDER>& e1,
                       const EulerAngle<DATA_TYPE, ROT_ORDER>& e2)
{
   // @todo metaprogramming.
   if (e1[0] != e2[0]) return false;
   if (e1[1] != e2[1]) return false;
   if (e1[2] != e2[2]) return false;
   return true;
}

/**
 * Compares e1 and e2 (component-wise) to see if they are NOT exactly the same.
 *
 * @param e1   the first EulerAngle
 * @param e2   the second EulerAngle
 *
 * @return  true if e1 does not equal e2; false if they are equal
 */
template<class DATA_TYPE, typename ROT_ORDER>
inline bool operator!=(const EulerAngle<DATA_TYPE, ROT_ORDER>& e1,
                       const EulerAngle<DATA_TYPE, ROT_ORDER>& e2)
{
   return(! (e1 == e2));
}

/**
 * Compares e1 and e2 (component-wise) to see if they are the same within a
 * given tolerance.
 *
 * @pre eps must be >= 0
 *
 * @param e1   the first EulerAngle
 * @param e2   the second EulerAngle
 * @param eps  the epsilon tolerance value, in radians
 *
 * @return  true if e1 is within the tolerance of e2; false if not
 */
template<class DATA_TYPE, typename ROT_ORDER>
inline bool isEqual( const EulerAngle<DATA_TYPE, ROT_ORDER>& e1,
                     const EulerAngle<DATA_TYPE, ROT_ORDER>& e2,
                     const DATA_TYPE eps = 0 )
{
   gmtlASSERT(eps >= (DATA_TYPE)0);
   
   // @todo metaprogramming.
   if (!Math::isEqual( e1[0], e2[0], eps )) return false;
   if (!Math::isEqual( e1[1], e2[1], eps )) return false;
   if (!Math::isEqual( e1[2], e2[2], eps )) return false;
   return true;
}

// @todo write isEquiv function for EulerAngle


/** @} */

} // namespace

#endif
