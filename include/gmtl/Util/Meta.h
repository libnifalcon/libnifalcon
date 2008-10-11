/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          Meta.h,v
 * Date modified: 2004/10/30 18:24:34
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
#ifndef _GMTL_METAPROGRAMMING_H
#define _GMTL_METAPROGRAMMING_H

#include <gmtl/Defines.h>

/*** STRINGIZE and JOIN macros */
/* Taken from boost (see boost.org) */

//
// Helper macro GMTL_STRINGIZE:
// Converts the parameter X to a string after macro replacement
// on X has been performed.
//
#define GMTL_STRINGIZE(X) GMTL_DO_STRINGIZE(X)
#define GMTL_DO_STRINGIZE(X) #X

//
// Helper macro GMTL_JOIN:
// The following piece of macro magic joins the two
// arguments together, even when one of the arguments is
// itself a macro (see 16.3.1 in C++ standard).  The key
// is that macro expansion of macro arguments does not
// occur in BOOST_DO_JOIN2 but does in BOOST_DO_JOIN.
//
#define GMTL_JOIN( X, Y ) GMTL_DO_JOIN( X, Y )
#define GMTL_DO_JOIN( X, Y ) GMTL_DO_JOIN2(X,Y)
#define GMTL_DO_JOIN2( X, Y ) X##Y


/** Meta programming classes */
namespace gmtl
{
   /** @ingroup Meta */
   //@{

   /** A lightweight identifier you can pass to overloaded functions
    *  to typefy them.
    *
    *  Type2Type lets you transport the type information about T to functions
    */
   template <typename T>
   struct Type2Type
   {
      typedef T OriginalType;
   };

   //@}

   /** @ingroup HelperMeta */
   //@{
   template <class T> inline void ignore_unused_variable_warning(const T&) { }

   //@}

} // end namespace

#ifndef GMTL_NO_METAPROG
namespace gmtl
{
namespace meta
{

// ------ LOOP UnRolling ------------ //
template<int ELT, typename T>
struct AssignVecUnrolled
{
   static void func(T& lVec, const T& rVec)
   {
      AssignVecUnrolled<ELT-1,T>::func(lVec, rVec);
      lVec[ELT] = rVec[ELT];
   }
};

template<typename T>
struct AssignVecUnrolled<0,T>
{
   static void func(T& lVec, const T& rVec)
   { lVec[0] = rVec[0]; }
};

// Template programs for array assignment unrolled
template<int ELT, typename T>
struct AssignArrayUnrolled
{
   static void func(T* lVec, const T* rVec)
   {
      AssignArrayUnrolled<ELT-1,T>::func(lVec, rVec);
      lVec[ELT] = rVec[ELT];
   }
};

template<typename T>
struct AssignArrayUnrolled<0,T>
{
   static void func(T* lVec, const T* rVec)
   { lVec[0] = rVec[0]; }
};

}  // namespace meta
}  // namespace gmtl
#endif /* ! GMTL_NO_METAPROG */

#endif
