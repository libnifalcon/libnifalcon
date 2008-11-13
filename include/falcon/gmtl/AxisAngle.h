/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          AxisAngle.h,v
 * Date modified: 2003/03/30 00:56:58
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
#ifndef _GMTL_AXISANGLE_H_
#define _GMTL_AXISANGLE_H_

#include <falcon/gmtl/Math.h>     // for rotation order
#include <falcon/gmtl/VecBase.h>  // base class of AxisAngle
#include <falcon/gmtl/Vec.h>      // axis data format
 
namespace gmtl
{

/** AxisAngle: Represents a "twist about an axis"
 *  AxisAngle is used to specify a rotation in 3-space.
 *
 *  To some people this rotation format can be more intuitive to specify than
 *  Matrix, Quat, or EulerAngle formatted rotation.
 *
 *  AxisAngle is very similar to Quat, except it is human readable.
 *  For efficiency, you should use Quat instead (Quat or Matrix are preferred).
 *
 *  The internal data format is an array of 4 DATA_TYPE values.
 *  Angle is first, the axis is the last 3.
 *
 * @pre angles are in radians, the axis is usually normalized by the user.
 *
 * @see AxisAnglef, AxisAngled
 * @see Matrix, Quat, EulerAngle
 * @ingroup Types
 */
template <typename DATA_TYPE>
class AxisAngle : public VecBase<DATA_TYPE, 4>
{
public:
   enum Params { Size = 4 };
   
   /** default constructor. initializes to identity rotation (no rotation). */
   AxisAngle() : 
      VecBase<DATA_TYPE, 4>( (DATA_TYPE)0.0, (DATA_TYPE)1.0, 
                             (DATA_TYPE)0.0, (DATA_TYPE)0.0 )
   {
   }
   
   /** copy constructor. */
   AxisAngle( const AxisAngle& e ) : VecBase<DATA_TYPE, 4>( e )
   {
   }
   
   /** data constructor (angle/x,y,z).   angles are in radians. */
   AxisAngle( const DATA_TYPE& rad_angle, const DATA_TYPE& x, 
              const DATA_TYPE& y, const DATA_TYPE& z ) :
            VecBase<DATA_TYPE, 4>( rad_angle, x, y, z )
   {
   }
   
   /** data constructor (angle/Vec3).   angles are in radians. */
   AxisAngle( const DATA_TYPE& rad_angle, const Vec<DATA_TYPE, 3>& axis ) :
            VecBase<DATA_TYPE, 4>( rad_angle, axis[0], axis[1], axis[2] )
   {
   }
   
   /** set raw data.   angles are in radians. */
   void set( const DATA_TYPE& rad_angle, const DATA_TYPE& x, 
             const DATA_TYPE& y, const DATA_TYPE& z )
   {
      VecBase<DATA_TYPE, 4>::set( rad_angle, x, y, z );
   }
   
   /** set data.   angles are in radians. */
   void set( const DATA_TYPE& rad_angle, const Vec<DATA_TYPE, 3>& axis )
   {
      VecBase<DATA_TYPE, 4>::set( rad_angle, axis[0], axis[1], axis[2] );
   }
   
   /** set the axis portion of the AxisAngle
    *   @param axis     the desired 3D vector axis to rotate about
    *   @post           the axis of the object is set
    */
   void setAxis( const Vec<DATA_TYPE, 3>& axis ) 
   { 
      VecBase<DATA_TYPE, 4>::operator[]( 1 ) = axis[0];
      VecBase<DATA_TYPE, 4>::operator[]( 2 ) = axis[1];
      VecBase<DATA_TYPE, 4>::operator[]( 3 ) = axis[2];
   }
   
   /** set the angle (twist) part of the AxisAngle, as a radian value.
    *  @param rad_angle    the desired twist angle, in radians
    *  @post the angle of the object is set
    */
   void setAngle( const DATA_TYPE& rad_angle ) 
   {
      VecBase<DATA_TYPE, 4>::operator[]( 0 ) = rad_angle;
   }
   
   /** get the axis portion of the AxisAngle
    *  @return  a vector of the axis, which may or may not be normalized.
    */
   Vec<DATA_TYPE, 3> getAxis() const 
   { 
      return Vec<DATA_TYPE, 3>( VecBase<DATA_TYPE, 4>::operator[]( 1 ),
                                VecBase<DATA_TYPE, 4>::operator[]( 2 ),
                                VecBase<DATA_TYPE, 4>::operator[]( 3 ) );
   }
   
   /** get the angle (twist) part of the AxisAngle.
    *  @return the twist value in radians
    */
   const DATA_TYPE& getAngle() const 
   {
      return VecBase<DATA_TYPE, 4>::operator[]( 0 );
   }
};

const AxisAngle<float> AXISANGLE_IDENTITYF( 0.0f, 1.0f, 0.0f, 0.0f );
const AxisAngle<double> AXISANGLE_IDENTITYD( 0.0, 1.0, 0.0, 0.0 );

typedef AxisAngle<float> AxisAnglef;
typedef AxisAngle<double> AxisAngled;

} // end of namespace gmtl

#endif
