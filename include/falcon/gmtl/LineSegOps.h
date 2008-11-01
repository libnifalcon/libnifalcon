/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          LineSegOps.h,v
 * Date modified: 2004/09/16 21:21:09
 * Version:       1.8
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
#ifndef _GMTL_LINESEGOPS_H_
#define _GMTL_LINESEGOPS_H_

#include <gmtl/LineSeg.h>
#include <gmtl/RayOps.h>

namespace gmtl {

/**
 * Finds the closest point on the line segment to a given point.
 *
 * @param lineseg    the line segment to test
 * @param pt         the point which to test against lineseg
 *
 * @return  the point on the line segment closest to pt
 */
template< class DATA_TYPE >
Point<DATA_TYPE, 3> findNearestPt( const LineSeg<DATA_TYPE>& lineseg,
                                   const Point<DATA_TYPE, 3>& pt )
{
   // result = origin + dir * dot((pt-origin), dir)
   return ( lineseg.mOrigin + lineseg.mDir *
            dot(pt - lineseg.mOrigin, lineseg.mDir) );
}

/**
 * Computes the shortest distance from the line segment to the given point.
 *
 * @param lineseg    the line segment to test
 * @param pt         the point which to test against lineseg
 *
 * @return  the shortest distance from pt to lineseg
 */
template< class DATA_TYPE >
inline DATA_TYPE distance( const LineSeg<DATA_TYPE>& lineseg,
                           const Point<DATA_TYPE, 3>& pt )
{
   return gmtl::length(gmtl::Vec<DATA_TYPE, 3>(pt - findNearestPt(lineseg, pt)));
}

/**
 * Computes the shortest distance from the line segment to the given point.
 *
 * @param lineseg    the line segment to test
 * @param pt         the point which to test against lineseg
 *
 * @return  the squared shortest distance from pt to lineseg (value is squared, this func is slightly faster since it doesn't involve a sqrt)
 */
template< class DATA_TYPE >
inline DATA_TYPE distanceSquared( const LineSeg<DATA_TYPE>& lineseg,
                           const Point<DATA_TYPE, 3>& pt )
{
   return gmtl::lengthSquared(gmtl::Vec<DATA_TYPE, 3>(pt - findNearestPt(lineseg, pt)));
}


} // namespace gmtl
#endif
