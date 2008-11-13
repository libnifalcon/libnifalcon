/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          VecOpsMeta.h,v
 * Date modified: 2005/05/16 14:19:44
 * Version:       1.3
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
#ifndef _GMTL_VEC_OPS_META_H
#define _GMTL_VEC_OPS_META_H

#include <falcon/gmtl/Util/Meta.h>

/** Meta programming classes for vec operations
 */
namespace gmtl
{
namespace meta
{
/** @ingroup VecOpsMeta */
//@{

/** meta class to unroll dot products. */
template<int ELT, typename T1, typename T2>
struct DotVecUnrolled
{
   static typename T1::DataType func(const T1& v1, const T2& v2)
   {  return (v1[ELT]*v2[ELT]) + DotVecUnrolled<ELT-1,T1,T2>::func(v1,v2); }
};

/** base cas for dot product unrolling. */
template<typename T1, typename T2>
struct DotVecUnrolled<0,T1,T2>
{
   static typename T1::DataType func(const T1& v1, const T2& v2)
   {   return (v1[0]*v2[0]); }
};

/** meta class to unroll length squared operation. */
template<int ELT, typename T>
struct LenSqrVecUnrolled
{
   static typename T::DataType func(const T& v)
   {  return (v[ELT]*v[ELT]) + LenSqrVecUnrolled<ELT-1,T>::func(v); }
};

/** base cas for dot product unrolling. */
template<typename T>
struct LenSqrVecUnrolled<0,T>
{
   static typename T::DataType func(const T& v)
   {   return (v[0]*v[0]); }
};

/** meta class to test vector equality. */
template<int ELT, typename VT>
struct EqualVecUnrolled
{
   static bool func(const VT& v1, const VT& v2)
   {  return (v1[ELT]==v2[ELT]) && EqualVecUnrolled<ELT-1,VT>::func(v1,v2); }
};

/** base cas for dot product unrolling. */
template<typename VT>
struct EqualVecUnrolled<0,VT>
{
   static bool func(const VT& v1, const VT& v2)
   {   return (v1[0]==v2[0]); }
};
//@}

} // namespace meta
} // end namespace


#endif
