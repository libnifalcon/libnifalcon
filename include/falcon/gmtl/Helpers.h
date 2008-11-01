/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          Helpers.h,v
 * Date modified: 2004/09/16 19:40:35
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
#ifndef _GMTL_HELPERS_H_
#define _GMTL_HELPERS_H_

#include <gmtl/Config.h>

// Helper classes
namespace gmtl
{
namespace helpers
{

struct ConstructorCounter
{
   unsigned mCount;

   ConstructorCounter()
   { mCount = 0; }

   void inc()
   { mCount += 1; }
   unsigned get()
   { return mCount; }
};

// Global version of the contructor counters
//#ifdef GMTL_COUNT_CONSTRUCT_CALLS

//ConstructorCounter VecConstructCounter;   // Counter for vec objects
inline ConstructorCounter* VecCtrCounterInstance()
{
   static ConstructorCounter vec_counter;
   return &vec_counter;
}

//#endif


}
}

#endif


