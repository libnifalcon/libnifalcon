/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          Math.h,v
 * Date modified: 2005/06/23 21:13:28
 * Version:       1.40
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
#ifndef _GMTL_MATH_H_
#define _GMTL_MATH_H_

#include <math.h>
#include <stdlib.h>
#include <gmtl/Defines.h>
#include <gmtl/Util/Assert.h>

namespace gmtl
{

/** Base class for Rotation orders
 *  @ingroup Defines
 * @see XYZ, ZYX, ZXY
 */
struct RotationOrderBase { enum { IS_ROTORDER = 1 }; };

/** XYZ Rotation order
 *  @ingroup Defines */
struct XYZ : public RotationOrderBase { enum { ID = 0 }; };

/** ZYX Rotation order
 *  @ingroup Defines */
struct ZYX : public RotationOrderBase { enum { ID = 1 }; };

/** ZXY Rotation order
 *  @ingroup Defines */
struct ZXY : public RotationOrderBase { enum { ID = 2 }; };

namespace Math
{
   /** @ingroup Math
    *  @name Mathematical constants
    *  @{
    */
   const float TWO_PI = 6.28318530717958647692f;
   const float PI = 3.14159265358979323846f; //3.14159265358979323846264338327950288419716939937510;
   const float PI_OVER_2 = 1.57079632679489661923f;
   const float PI_OVER_4 = 0.78539816339744830962f;
   /** @} */

   /** @ingroup Math
    *  @name C Math Abstraction
    *  @{
    */
//----------------------------------------------------------------------------
template <typename T>
inline T abs( T iValue )
{
    return T( iValue >= ((T)0) ? iValue : -iValue );
}

inline float abs(float iValue)
{  return fabsf(iValue); }
inline double abs(double iValue)
{  return fabs(iValue); }
inline int abs(int iValue)
{  return ::abs(iValue); }
inline long abs(long iValue)
{  return labs(iValue); }

//----------------------------------------------------------------------------
template <typename T>
inline T ceil( T fValue );
inline float ceil( float fValue )
{
#ifdef NO_CEILF
   return float(::ceil(fValue));
#else
   return float( ::ceilf( fValue ) );
#endif
}
inline double ceil( double fValue )
{
    return double( ::ceil( fValue ) );
}
//----------------------------------------------------------------------------
template <typename T>
inline T floor( T fValue ); // why do a floor of int?  shouldn't compile...
inline float floor( float fValue )
{
#ifdef NO_FLOORF
   return float(::floor(fValue));
#else
   return float( ::floorf( fValue ) );
#endif
}
inline double floor( double fValue )
{
    return double( ::floor( fValue ) );
}
//----------------------------------------------------------------------------
template <typename T>
inline int sign( T iValue )
{
   if (iValue > T(0))
   {
      return 1;
   }
   else
   {
      if (iValue < T(0))
      {
         return -1;
      }
      else
      {
         return 0;
      }
   }
}
//----------------------------------------------------------------------------
/**
 * Clamps the given value down to zero if it is within epsilon of zero.
 *
 * @param value      the value to clamp
 * @param eps        the epsilon tolerance or zero by default
 *
 * @return  zero if the value is close to 0, the value otherwise
 */
template <typename T>
inline T zeroClamp( T value, T eps = T(0) )
{
   return ( (gmtl::Math::abs(value) <= eps) ? T(0) : value );
}

//----------------------------------------------------------------------------
// don't allow non-float types, because their ret val wont be correct
// i.e. with int, the int retval will be rounded up or down.
//      we'd need a float retval to do it right, but we can't specialize by ret
template <typename T>
inline T aCos( T fValue );
inline float aCos( float fValue )
{
    if ( -1.0f < fValue )
    {
        if ( fValue < 1.0f )
        {
#ifdef NO_ACOSF
            return float(::acos(fValue));
#else
            return float( ::acosf( fValue ) );
#endif
        }
        else
            return 0.0f;
    }
    else
    {
        return (float)gmtl::Math::PI;
    }
}
inline double aCos( double fValue )
{
    if ( -1.0 < fValue )
    {
        if ( fValue < 1.0 )
            return double( ::acos( fValue ) );
        else
            return 0.0;
    }
    else
    {
        return (double)gmtl::Math::PI;
    }
}
//----------------------------------------------------------------------------
template <typename T>
inline T aSin( T fValue );
inline float aSin( float fValue )
{
    if ( -1.0f < fValue )
    {
        if ( fValue < 1.0f )
        {
#ifdef NO_ASINF
            return float(::asin(fValue));
#else
            return float( ::asinf( fValue ) );
#endif
        }
        else
            return (float)-gmtl::Math::PI_OVER_2;
    }
    else
    {
        return (float)gmtl::Math::PI_OVER_2;
    }
}
inline double aSin( double fValue )
{
    if ( -1.0 < fValue )
    {
        if ( fValue < 1.0 )
            return double( ::asin( fValue ) );
        else
            return (double)-gmtl::Math::PI_OVER_2;
    }
    else
    {
        return (double)gmtl::Math::PI_OVER_2;
    }
}
//----------------------------------------------------------------------------
template <typename T>
inline T aTan( T fValue );
inline double aTan( double fValue )
{
    return ::atan( fValue );
}
inline float aTan( float fValue )
{
#ifdef NO_TANF
   return float(::atan(fValue));
#else
   return float( ::atanf( fValue ) );
#endif
}
//----------------------------------------------------------------------------
template <typename T>
inline T aTan2( T fY, T fX );
inline float aTan2( float fY, float fX )
{
#ifdef NO_ATAN2F
   return float(::atan2(fY, fX));
#else
   return float( ::atan2f( fY, fX ) );
#endif
}
inline double aTan2( double fY, double fX )
{
    return double( ::atan2( fY, fX ) );
}
//----------------------------------------------------------------------------
template <typename T>
inline T cos( T fValue );
inline float cos( float fValue )
{
#ifdef NO_COSF
   return float(::cos(fValue));
#else
   return float( ::cosf( fValue ) );
#endif
}
inline double cos( double fValue )
{
    return double( ::cos( fValue ) );
}
//----------------------------------------------------------------------------
template <typename T>
inline T exp( T fValue );
inline float exp( float fValue )
{
#ifdef NO_EXPF
   return float(::exp(fValue));
#else
   return float( ::expf( fValue ) );
#endif
}
inline double exp( double fValue )
{
    return double( ::exp( fValue ) );
}
//----------------------------------------------------------------------------
template <typename T>
inline T log( T fValue );
inline double log( double fValue )
{
    return double( ::log( fValue ) );
}
inline float log( float fValue )
{
#ifdef NO_LOGF
   return float(::log(fValue));
#else
   return float( ::logf( fValue ) );
#endif
}
//----------------------------------------------------------------------------
inline double pow( double fBase, double fExponent)
{
    return double( ::pow( fBase, fExponent ) );
}
inline float pow( float fBase, float fExponent)
{
#ifdef NO_POWF
   return float(::pow(fBase, fExponent));
#else
   return float( ::powf( fBase, fExponent ) );
#endif
}
//----------------------------------------------------------------------------
template <typename T>
inline T sin( T fValue );
inline double sin( double fValue )
{
    return double( ::sin( fValue ) );
}
inline float sin( float fValue )
{
#ifdef NO_SINF
   return float(::sin(fValue));
#else
   return float( ::sinf( fValue ) );
#endif
}
//----------------------------------------------------------------------------
template <typename T>
inline T tan( T fValue );
inline double tan( double fValue )
{
    return double( ::tan( fValue ) );
}
inline float tan( float fValue )
{
#ifdef NO_TANF
   return float(::tan(fValue));
#else
   return float( ::tanf( fValue ) );
#endif
}
//----------------------------------------------------------------------------
template <typename T>
inline T sqr( T fValue )
{
    return T( fValue * fValue );
}
//----------------------------------------------------------------------------
template <typename T>
inline T sqrt( T fValue )
{
#ifdef NO_SQRTF
   return T(::sqrt(((float)fValue)));
#else
   return T( ::sqrtf( ((float)fValue) ) );
#endif
}
inline double sqrt( double fValue )
{
    return double( ::sqrt( fValue ) );
}

/** Fast inverse square root.
 */
inline float fastInvSqrt(float x)
{
   const float xhalf(0.5f*x);
   long i = *(long*)&x;
   i = 0x5f3759df - (i>>1);    // This hides a good amount of math
   x = *(float*)&i;
   x = x*(1.5f - xhalf*x*x);   // Repeat for more accuracy
   return x;
}

inline float fastInvSqrt2(float x)
{
   const float xhalf(0.5f*x);
   long i = *(long*)&x;
   i = 0x5f3759df - (i>>1);    // This hides a good amount of math
   x = *(float*)&i;
   x = x*(1.5f - xhalf*x*x);   // Repeat for more accuracy
   x = x*(1.5f - xhalf*x*x);
   return x;
}

inline float fastInvSqrt3(float x)
{
   const float xhalf(0.5f*x);
   long i = *(long*)&x;
   i = 0x5f3759df - (i>>1);    // This hides a good amount of math
   x = *(float*)&i;
   x = x*(1.5f - xhalf*x*x);   // Repeat for more accuracy
   x = x*(1.5f - xhalf*x*x);
   x = x*(1.5f - xhalf*x*x);
   return x;
}


//----------------------------------------------------------------------------
/**
 * Seeds the pseudorandom number generator with the given seed.
 *
 * @param seed  the seed for the pseudorandom number generator.
 */
inline void seedRandom(unsigned int seed)
{
   ::srand(seed);
}

/** get a random number between 0 and 1
 * @post returns number between 0 and 1
 */
inline float unitRandom()
{
   return float(::rand())/float(RAND_MAX);
}

/** return a random number between x1 and x2
 * RETURNS: random number between x1 and x2
 */
inline float rangeRandom( float x1, float x2 )
{
   float r = gmtl::Math::unitRandom();
   float size = x2 - x1;
   return float( r * size + x1 );
}

/*
float SymmetricRandom ()
{
    return 2.0*float(rand())/float(RAND_MAX) - 1.0;
}
*/
//----------------------------------------------------------------------------

inline float deg2Rad( float fVal )
{
   return float( fVal * (float)(gmtl::Math::PI/180.0) );
}
inline double deg2Rad( double fVal )
{
   return double( fVal * (double)(gmtl::Math::PI/180.0) );
}

inline float rad2Deg( float fVal )
{
   return float( fVal * (float)(180.0/gmtl::Math::PI) );
}
inline double rad2Deg( double fVal )
{
   return double( fVal * (double)(180.0/gmtl::Math::PI) );
}
//----------------------------------------------------------------------------

/** Is almost equal?
 * test for equality within some tolerance...
 * @PRE: tolerance must be >= 0
 */
template <class T>
inline bool isEqual( const T& a, const T& b, const T& tolerance )
{
   gmtlASSERT( tolerance >= (T)0 );
   return bool( gmtl::Math::abs( a - b ) <= tolerance );
}
//----------------------------------------------------------------------------
/** cut off the digits after the decimal place */
template <class T>
inline T trunc( T val )
{
   return T( (val < ((T)0)) ? gmtl::Math::ceil( val ) : gmtl::Math::floor( val ) );
}
/** round to nearest integer */
template <class T>
inline T round( T p )
{
   return T( gmtl::Math::floor( p + (T)0.5 ) );
}
//----------------------------------------------------------------------------
/** min returns the minimum of 2 values */
template <class T>
inline T Min( const T& x, const T& y )
{
   return ( x > y ) ? y : x;
}
/** min returns the minimum of 3 values */
template <class T>
inline T Min( const T& x, const T& y, const T& z )
{
   return Min( gmtl::Math::Min( x, y ), z );
}
/** min returns the minimum of 4 values */
template <class T>
inline T Min( const T& w, const T& x, const T& y, const T& z )
{
   return gmtl::Math::Min( gmtl::Math::Min( w, x ), gmtl::Math::Min( y, z ) );
}

/** max returns the maximum of 2 values */
template <class T>
inline T Max( const T& x, const T& y )
{
   return ( x > y ) ? x : y;
}
/** max returns the maximum of 3 values */
template <class T>
inline T Max( const T& x, const T& y, const T& z )
{
   return Max( gmtl::Math::Max( x, y ), z );
}
/** max returns the maximum of 4 values */
template <class T>
inline T Max( const T& w, const T& x, const T& y, const T& z )
{
   return gmtl::Math::Max( gmtl::Math::Max( w, x ), gmtl::Math::Max( y, z ) );
}
//----------------------------------------------------------------------------
/** Compute the factorial.
 * give - an object who's type has operator++, operator=, operator<=, and operator*= defined.
 *        it should be a single valued scalar type such as an int, float, double etc....
 * NOTE: This could be faster with a lookup table, but then wouldn't work templated : kevin
 */
template<class T>
inline T factorial(T rhs)
{
   T lhs = (T)1;

   for( T x = (T)1; x <= rhs; ++x )
   {
      lhs *= x;
   }

   return lhs;
}
/** @} */

/**
 * clamp "number" to a range between lo and hi
 */
template <class T>
inline T clamp( T number, T lo, T hi )
{
   if (number > hi) number = hi;
   else if (number < lo) number = lo;
   return number;
}

/** @ingroup Interp
 * @name Scalar type interpolation (for doubles, floats, etc...)
 * @{
 */

/** Linear Interpolation between number [a] and [b].
 *  lerp=0.0 returns a, lerp=1.0 returns b
 *  @pre use double or float only...
 */
template <class T, typename U>
inline void lerp( T& result, const U& lerp, const T& a, const T& b )
{
    T size = b - a;
    result = ((U)a) + (((U)size) * lerp);
}
/** @} */

/**
 * Uses the quadratic formula to compute the 2 roots of the given 2nd degree
 * polynomial in the form of Ax^2 + Bx + C.
 *
 * @param r1   set to the first root
 * @param r2   set to the second root
 * @param a    the coefficient to x^2
 * @param b    the coefficient to x^1
 * @param c    the coefficient to x^0
 *
 * @return  true if both r1 and r2 are real; false otherwise
 */
template <class T>
inline bool quadraticFormula(T& r1, T& r2, const T& a, const T& b, const T& c)
{
   const T q = b*b - T(4)*a*c;

   // the result has real roots
   if (q >= 0)
   {
      const T sq = gmtl::Math::sqrt(q);
      const T d = T(1) / (T(2) * a);
      r1 = (-b + sq) * d;
      r2 = (-b - sq) * d;
      return true;
   }
   // the result has complex roots
   else
   {
      return false;
   }
}

} // end namespace Math
} // end namespace gmtl

#endif
