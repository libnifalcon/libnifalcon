/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          EulerAngle.h,v
 * Date modified: 2003/04/01 13:21:47
 * Version:       1.11
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
#ifndef _GMTL_EULERANGLE_H_
#define _GMTL_EULERANGLE_H_

#include <falcon/gmtl/Math.h>     // for rotation order
 
namespace gmtl
{

/** EulerAngle: Represents a group of euler angles.
 *  Euler angle can be used to represent rotations in 3-space.
 *
 *  To some people this rotation format can be more intuitive to specify than
 *  Matrix, Quat, or AxisAngle formatted rotation.
 *
 *  For efficiency and to minimize problems from gimbal-lock, you should use
 *  one of the other rotation formats instead (Quat or Matrix are preferred).
 *
 *  The internal data format is an array of 3 DATA_TYPE angle values, 
 *  plus a RotationOrder that specifies how to build a rotation transform
 *  from the 3 angle value.
 *
 *  IMPORTANT:
 *  The 3 angles are in the order set ::getOrder(), not XYZ.
 *  The values do not swap when order is changed after setting the angles.
 *
 * @pre all angles are in radians.
 *
 * @see EulerAnglef, EulerAngled
 * @see Matrix, Quat, AxisAngle
 * @ingroup Types
 */
template <typename DATA_TYPE, typename ROTATION_ORDER>
class EulerAngle
{
public:
   /// Use this to declare single value types of the same type as this object.
   typedef DATA_TYPE DataType;

   enum Params { Size = 3, Order = ROTATION_ORDER::ID };

   /** default constructor. initializes to identity rotation (no rotation). */
   EulerAngle()
   {
      gmtlASSERT( ROTATION_ORDER::IS_ROTORDER == 1 && 
            "you must specify a RotationOrder derived type for the rotationorder in euler angle." );
      mData[0] = DATA_TYPE( 0 );
      mData[1] = DATA_TYPE( 0 );
      mData[2] = DATA_TYPE( 0 );
   }

   /** copy constructor. */
   EulerAngle( const EulerAngle& e )
   {
      mData[0] = e.mData[0];
      mData[1] = e.mData[1];
      mData[2] = e.mData[2];
   }

   /** data constructor.   angles are in radians. */
   EulerAngle( DATA_TYPE p0, DATA_TYPE p1, DATA_TYPE p2 )
   {
      mData[0] = p0;
      mData[1] = p1;
      mData[2] = p2;
   }
   
   /** set data.   angles are in radians. */
   void set( const DATA_TYPE& p0, const DATA_TYPE& p1, 
             const DATA_TYPE& p2 )
   {
      mData[0] = p0;
      mData[1] = p1;
      mData[2] = p2;
   }
   
   //@{
   /** Gets the ith component in this EulerAngle.
    *  @param i    the zero-based index of the component to access.
    *  @pre 0 <= i < 3
    *  @return  a reference to the ith component
    */
   inline DATA_TYPE& operator[]( const unsigned i )
   {
      gmtlASSERT( i < Size );
      return mData[i];
   }
   inline const DATA_TYPE&  operator[]( const unsigned i ) const
   {
      gmtlASSERT( i < Size );
      return mData[i];
   }
   //@}
   
   //@{
   /** Gets the internal array of the components.
    *  @return  a pointer to the component array with length SIZE
    */
   DATA_TYPE* getData() { return mData; }
   
   /** Gets the internal array of the components (const version)
    *  @return  a pointer to the component array with length SIZE
    */
   const DATA_TYPE* getData() const { return mData; }
   //@}
   
private:
   DATA_TYPE mData[Size];
};

const EulerAngle<float, XYZ> EULERANGLE_IDENTITY_XYZF( 0.0f, 0.0f, 0.0f );
const EulerAngle<double, XYZ> EULERANGLE_IDENTITY_XYZD( 0.0, 0.0, 0.0 );
const EulerAngle<float, ZYX> EULERANGLE_IDENTITY_ZYXF( 0.0f, 0.0f, 0.0f );
const EulerAngle<double, ZYX> EULERANGLE_IDENTITY_ZYXD( 0.0, 0.0, 0.0 );
const EulerAngle<float, ZXY> EULERANGLE_IDENTITY_ZXYF( 0.0f, 0.0f, 0.0f );
const EulerAngle<double, ZXY> EULERANGLE_IDENTITY_ZXYD( 0.0, 0.0, 0.0 );

typedef EulerAngle<float, XYZ> EulerAngleXYZf;
typedef EulerAngle<double, XYZ> EulerAngleXYZd;
typedef EulerAngle<float, ZYX> EulerAngleZYXf;
typedef EulerAngle<double, ZYX> EulerAngleZYXd;
typedef EulerAngle<float, ZXY> EulerAngleZXYf;
typedef EulerAngle<double, ZXY> EulerAngleZXYd;

} // end of namespace gmtl

#endif
