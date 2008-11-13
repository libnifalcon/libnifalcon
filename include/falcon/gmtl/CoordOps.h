/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          CoordOps.h,v
 * Date modified: 2004/05/25 16:36:28
 * Version:       1.6
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
#ifndef _GMTL_COORD_OPS_H_
#define _GMTL_COORD_OPS_H_

#include <falcon/gmtl/Coord.h>

namespace gmtl
{
/** @ingroup Compare Coord 
 * @name Coord Comparitors
 * @{
 */

   /** Compare two coordinate frames for equality.
    * @param c1   the first Coord
    * @param c2   the second Coord
    * @return     true if c1 is the same as c2, false otherwise
    */
   template <typename POS_TYPE, typename ROT_TYPE>
   inline bool operator==( const Coord<POS_TYPE, ROT_TYPE>& c1, 
                           const Coord<POS_TYPE, ROT_TYPE>& c2 )
   {
      return bool( c1.getPos() == c2.getPos() &&
                   c1.getRot() == c2.getRot() );
   }

   /** Compare two coordinate frames for not-equality.
    * @param c1   the first Coord
    * @param c2   the second Coord
    * @return     true if c1 is different from c2, false otherwise
    */
   template <typename POS_TYPE, typename ROT_TYPE>
   inline bool operator!=( const Coord<POS_TYPE, ROT_TYPE>& c1, 
                           const Coord<POS_TYPE, ROT_TYPE>& c2 )
   {
      return !operator==( c1, c2 );
   }

   /** Compare two coordinate frames for equality with a given tolerance.
    * @param c1   the first Coord
    * @param c2   the second Coord
    * @param tol  the tolerance coordinate frame of the same type as c1 and c2
    * @return     true if c1 is equal within a tolerance of c2, false otherwise
    */
   template <typename POS_TYPE, typename ROT_TYPE>
   inline bool isEqual( const Coord<POS_TYPE, ROT_TYPE>& c1, 
                        const Coord<POS_TYPE, ROT_TYPE>& c2, 
                        typename Coord<POS_TYPE, ROT_TYPE>::DataType tol = 0 )
   {
      return bool( isEqual( c1.getPos(), c2.getPos(), tol ) &&
                   isEqual( c1.getRot(), c2.getRot(), tol )     );
   }
/** @} */

}

#endif
