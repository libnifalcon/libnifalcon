/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          Comparitors.h,v
 * Date modified: 2002/01/26 23:47:53
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
#ifndef _GMTL_COMPARITORS_H_
#define _GMTL_COMPARITORS_H_

// This file contains helper comparitors
//
// They can be used as comparison functors for STL container
// operations (or for anything else you may want to use them for...)
//

#include <gmtl/Vec3.h>
#include <gmtl/Point3.h>

namespace gmtl
{
   // Allows for the comparison of projected point distances
   // onto a given vector
   struct CompareIndexPointProjections
   {
   public:
      CompareIndexPointProjections() : points(NULL)
      {;}

      bool operator()(const unsigned x, const unsigned y)
      {
         float xVal = sortDir.dot((*points)[x]);
         float yVal = sortDir.dot((*points)[y]);

         return (xVal < yVal);
      }

      const std::vector<Point3>* points;
      gmtl::Vec3                 sortDir;       // Direction to sort by
   };
};

#endif

