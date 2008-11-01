/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          OOBox.h,v
 * Date modified: 2002/05/20 22:39:22
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
#ifndef _GMTL_OOBox_H_
#define _GMTL_OOBox_H_

#include <gmtl/Vec3.h>
#include <gmtl/Point3.h>
#include <gmtl/matVecFuncs.h>

namespace gmtl
{

// AABox : Defines an object aligned box
//
// For definition of an OOB, see pg 293-294 of Real-Time Rendering
class  OOBox
{

public:
   OOBox()
   { ident(); }

   OOBox(OOBox& box);

public:
   // Accessors
   Point3& center();
   const Point3& center() const;

   Vec3& axis (int i);
   const Vec3& axis (int i) const;
   Vec3* axes ();
   const Vec3* axes () const;

   float& halfLen(int i);
   const float& halfLen(int i) const;
   float* halfLens();
   const float* halfLens() const;

   // Assignment
   OOBox& operator=(const OOBox& box);

   // Comparison
   bool operator==(const OOBox& box) const;

   //  return the verts that define the box
   // Order: XYZ: 000, 100, 110, 010,
   //             001, 101, 111, 011
   void getVerts(Point3 verts[8]) const;

   // Merge current box with other box to get new box covering both
   void mergeWith(const OOBox& box);

   void ident()
   {
      mCenter = ZeroVec3;
      mAxis[0] = XUnitVec3;
      mAxis[1] = YUnitVec3;
      mAxis[2] = ZUnitVec3;
      mHalfLen[0] = mHalfLen[1] = mHalfLen[2] = 0.0f;
   }

public:
   Point3  mCenter;     // The center point of the box
   Vec3    mAxis[3];    // The axes of the oriented box (xAxis, yAxis, zAxis)
   float   mHalfLen[3]; // Half lengths of the box  ASSERT: HalfLens >= 0.0f
};


// ------------------------------------------------- //
// --------------- Member definitions -------------- //
// ------------------------------------------------- //
inline
OOBox::OOBox(OOBox& box)
{
   mCenter = box.mCenter;
   mAxis[0] = box.mAxis[0];
   mAxis[1] = box.mAxis[1];
   mAxis[2] = box.mAxis[2];
   mHalfLen[0] = box.mHalfLen[0];
   mHalfLen[1] = box.mHalfLen[1];
   mHalfLen[2] = box.mHalfLen[2];
}

// Accessors
inline Point3& OOBox::center()
{
   return mCenter;
}

inline const Point3& OOBox::center() const
{
   return mCenter;
}

inline Vec3& OOBox::axis (int i)
{
   return mAxis[i];
}

inline const Vec3& OOBox::axis (int i) const
{
   return mAxis[i];
}

inline Vec3* OOBox::axes ()
{
   return mAxis;
}

inline const Vec3* OOBox::axes () const
{
   return mAxis;
}

inline float& OOBox::halfLen(int i)
{
   return mHalfLen[i];
}

inline const float& OOBox::halfLen(int i) const
{
   return mHalfLen[i];
}

inline float* OOBox::halfLens()
{
   return mHalfLen;
}

inline const float* OOBox::halfLens() const
{
   return mHalfLen;
}

// Assignment
inline OOBox& OOBox::operator=(const OOBox& box)
{
   mCenter = box.mCenter;
   mAxis[0] = box.mAxis[0];
   mAxis[1] = box.mAxis[1];
   mAxis[2] = box.mAxis[2];
   mHalfLen[0] = box.mHalfLen[0];
   mHalfLen[1] = box.mHalfLen[1];
   mHalfLen[2] = box.mHalfLen[2];
   return *this;
}

// Comparison
inline bool OOBox::operator==(const OOBox& box) const
{
   return ((mCenter == box.mCenter) &&
           (mAxis[0] == box.mAxis[0]) &&
           (mAxis[1] == box.mAxis[1]) &&
           (mAxis[2] == box.mAxis[2]) &&
           (mHalfLen[0] == box.mHalfLen[0]) &&
           (mHalfLen[1] == box.mHalfLen[1]) &&
           (mHalfLen[2] == box.mHalfLen[2]));
}

inline void OOBox::getVerts(Point3 verts[8]) const
{
   Vec3 x_half_axis = mAxis[0]*mHalfLen[0];
   Vec3 y_half_axis = mAxis[1]*mHalfLen[1];
   Vec3 z_half_axis = mAxis[2]*mHalfLen[2];

   verts[0] = mCenter - x_half_axis - y_half_axis - z_half_axis;
   verts[1] = mCenter + x_half_axis - y_half_axis - z_half_axis;
   verts[2] = mCenter + x_half_axis + y_half_axis - z_half_axis;
   verts[3] = mCenter - x_half_axis + y_half_axis - z_half_axis;
   verts[4] = mCenter - x_half_axis - y_half_axis + z_half_axis;
   verts[5] = mCenter + x_half_axis - y_half_axis + z_half_axis;
   verts[6] = mCenter + x_half_axis + y_half_axis + z_half_axis;
   verts[7] = mCenter - x_half_axis + y_half_axis + z_half_axis;
}

};

#endif
