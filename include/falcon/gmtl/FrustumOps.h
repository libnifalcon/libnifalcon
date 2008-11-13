/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Benjamin Schulz
 *
 * -----------------------------------------------------------------
 * File:          FrustumOps.h,v
 * Date modified: 2007/06/13 19:42:29
 * Version:       1.4
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

#ifndef _GMTL_FRUSTUM_OPS_H_
#define _GMTL_FRUSTUM_OPS_H_

#include <falcon/gmtl/Defines.h>
#include <falcon/gmtl/Frustum.h>
#include <falcon/gmtl/Math.h>


namespace gmtl
{

template<class DATA_TYPE>
void normalize(Frustum<DATA_TYPE>& f)
{
   for ( unsigned int i = 0; i < 6; ++i )
   {
      Vec<DATA_TYPE, 3> n = f.mPlanes[i].getNormal();
      DATA_TYPE o = f.mPlanes[i].getOffset();
      DATA_TYPE len = Math::sqrt( n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
      n[0] /= len;
      n[1] /= len;
      n[2] /= len;
      o /= len;
      f.mPlanes[i].setNormal(n);
      f.mPlanes[i].setOffset(o);
   }
}

}


#endif /* _GMTL_FRUSTUM_OPS_H_ */
