/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          AABoxOps.h,v
 * Date modified: 2003/03/17 02:14:48
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
#ifndef _GMTL_AABOXOPS_H_
#define _GMTL_AABOXOPS_H_

#include <falcon/gmtl/AABox.h>
#include <falcon/gmtl/VecOps.h>

namespace gmtl
{

/** @ingroup Compare AABox 
 * @name AABox Comparitors
 * @{
 */

/**
 * Compare two AABoxes to see if they are EXACTLY the same. In other words,
 * this comparison is done with zero tolerance.
 *
 * @param b1      the first box to compare
 * @param b2      the second box to compare
 *
 * @return  true if they are equal, false otherwise
 */
template< class DATA_TYPE >
inline bool operator==( const AABox<DATA_TYPE>& b1, const AABox<DATA_TYPE>& b2 )
{
   return ( (b1.isEmpty() == b2.isEmpty()) &&
            (b1.getMin() == b2.getMin()) &&
            (b1.getMax() == b2.getMax()) );
}

/**
 * Compare two AABoxes to see if they are not EXACTLY the same. In other words,
 * this comparison is done with zero tolerance.
 *
 * @param b1      the first box to compare
 * @param b2      the second box to compare
 *
 * @return  true if they are not equal, false otherwise
 */
template< class DATA_TYPE >
inline bool operator!=( const AABox<DATA_TYPE>& b1, const AABox<DATA_TYPE>& b2 )
{
   return (! (b1 == b2));
}

/**
 * Compare two AABoxes to see if they are the same within the given tolerance.
 *
 * @param b1      the first box to compare
 * @param b2      the second box to compare
 * @param eps     the tolerance value to use
 *
 * @pre eps must be >= 0
 *
 * @return  true if their points are within the given tolerance of each other, false otherwise
 */
template< class DATA_TYPE >
inline bool isEqual( const AABox<DATA_TYPE>& b1, const AABox<DATA_TYPE>& b2, const DATA_TYPE& eps )
{
   gmtlASSERT( eps >= 0 );
   return (b1.isEmpty() == b2.isEmpty()) &&
          isEqual( b1.getMin(), b2.getMin(), eps ) &&
          isEqual( b1.getMax(), b2.getMax(), eps );
}
/** @} */

}

#endif

