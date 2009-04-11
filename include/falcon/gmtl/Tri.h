/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          Tri.h,v
 * Date modified: 2009/02/12 21:44:47
 * Version:       1.14
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
#ifndef _GMTL_TRI_H_
#define _GMTL_TRI_H_

#include <falcon/gmtl/Point.h>
#include <falcon/gmtl/Vec.h>
#include <falcon/gmtl/VecOps.h>
 
namespace gmtl
{
/**
 * This class defines a triangle as a set of 3 points order in CCW fashion.
 *
 * Triangle points are tri(s,t) = b+s*e0+t*e1 where 0 <= s <= 1,
 * 0 <= t <= 1, and 0 <= s+t <= 1.
 * @ingroup Types
 */
template< class DATA_TYPE >
class  Tri
{
public:
   /**
    * Constructs a new triangle with all vertices at the origin.
    */
   Tri() {}

   /**
    * Constructs a new triangle with the given points. The points must be passed
    * in in CCW order.
    *
    * @param p1   vertex0
    * @param p2   vertex1
    * @param p3   vertex2
    *
    * @pre p1, p2, p3 must be in CCW order
    */
   Tri( const Point<DATA_TYPE, 3>& p1, const Point<DATA_TYPE, 3>& p2,
        const Point<DATA_TYPE, 3>& p3)
   {
      mVerts[0] = p1;
      mVerts[1] = p2;
      mVerts[2] = p3;
   }

   /**
    * Constructs a duplicate of the given triangle.
    *
    * @param tri     the triangle to copy
    */
   Tri( const Tri<DATA_TYPE>& tri )
   {
      mVerts[0] = tri[0];
      mVerts[1] = tri[1];
      mVerts[2] = tri[2];
   }

   /**
    * Gets the nth vertex in the triangle.
    *
    * @param idx     the index to the vertex in the triangle
    * @pre 0 <= idx <= 2
    *
    * @return  the nth vertex as a point
    */
   //@{
   Point<DATA_TYPE, 3>& operator[]( const unsigned idx )
   {
      gmtlASSERT( idx <= 2 );
      return mVerts[idx];
   }
   const Point<DATA_TYPE, 3>& operator[]( const unsigned idx ) const
   {
      gmtlASSERT( idx <= 2 );
      return mVerts[idx];
   }
   //@}

   /**
    * Gets the nth edge of the triangle where edge0 corresponds to the vector
    * from vertex 0 to 1, edge1 corresponds to the vector from vertex 1 to 2 and
    * edge2 corresponsds to the vector from vertex 2 to vertex 0.
    *
    * @param   idx     the ordered edge index
    * @pre 0 <= idx <= 2
    *
    * @return  a vector from vertex idx to vertex (idx+1)mod size
    */
   Vec<DATA_TYPE, 3> edge( int idx ) const
   {
      gmtlASSERT( (0 <= idx) && (idx <= 2) );
      int idx2 = ( idx == 2 ) ? 0 : idx + 1;
      return (mVerts[idx2] - mVerts[idx]);
   }

   /** get edge vec from two verts 
    *
    * @param   idx,idy     the ordered vertex indicies
    * @pre 0 <= id <= 2
    *
    * @return  a vector from vertex idx to vertex idy
    */
   Vec<DATA_TYPE, 3> edge( int idx, int idy ) const
   {
      gmtlASSERT( (0 <= idx) && (idx <= 2) );
      gmtlASSERT( (0 <= idy) && (idy <= 2) );
      return (mVerts[idy] - mVerts[idx]);
   }

 

   /**
    * Set the triangle with the given points. The points must be passed
    * in in CCW order.
    *
    * @param p1   vertex0
    * @param p2   vertex1
    * @param p3   vertex2
    *
    * @pre p1, p2, p3 must be in CCW order
    */
   void set( const Point<DATA_TYPE, 3>& p1, const Point<DATA_TYPE, 3>& p2,
           const Point<DATA_TYPE, 3>& p3 )
   {
      mVerts[0] = p1;
      mVerts[1] = p2;
      mVerts[2] = p3;
   }

public:
   /**
    * The vertices of the triangle.
    */
   Point<DATA_TYPE, 3> mVerts[3];
};

// convenience typedefs
typedef Tri<float> Trif;
typedef Tri<double> Trid;
typedef Tri<int> Trii;
}

/*
// Finds the point on the seg nearest to pt.
// on the perimeter of the triangle.
// Returns the nearest point in nearPt
inline
void sgTri::findNearestEdgePt(const sgVec3& pt, sgVec3& nearPt) const
{
    //      find all the closest pts on the segs that make the tri
    //      return the closest one

    sgSeg   seg1;   seg1.makePts(_v1, _v2);
    sgSeg   seg2;   seg2.makePts(_v2, _v3);
    sgSeg   seg3;   seg3.makePts(_v3, _v1);

    sgVec3  pt1;    seg1.findNearestPt(pt, pt1);
    sgVec3  pt2;    seg2.findNearestPt(pt, pt2);
    sgVec3  pt3;    seg3.findNearestPt(pt, pt3);

    float dist1 = SG_SQUARE(pt1[0]-pt[0]) + SG_SQUARE(pt1[1]-pt[1]) + SG_SQUARE(pt1[2]-pt[2]);
    float dist2 = SG_SQUARE(pt2[0]-pt[0]) + SG_SQUARE(pt2[1]-pt[1]) + SG_SQUARE(pt2[2]-pt[2]);
    float dist3 = SG_SQUARE(pt3[0]-pt[0]) + SG_SQUARE(pt3[1]-pt[1]) + SG_SQUARE(pt3[2]-pt[2]);

    if((dist1 < dist2) && (dist1 < dist3))      // dist1 is min
        nearPt = pt1;
    else if(dist2 < dist3)                      // dist2 is min
        nearPt = pt2;
    else
        nearPt = pt3;                           // dist3 is min
}



	// Returns wether the pt is in the triangle
inline
int sgTri::ptIn(const sgVec3& pt) const
{
		// Graphic Gems I: Page 392
	int i0, i1, i2;	    // Axis indices
	float u0, u1, u2, v0, v1, v2, alpha, beta;
	i0 = 0;
	
    sgVec3  triNormal = (_v2-_v1).cross((_v3-_v1));
    //sgPlane triPlane;
    //triPlane.makePts(_v1, _v2, _v3);

	// Find max index
    // NOTE: note the fabs.  I added because of bug in GG code with negative normals
	for(int index=0;index<3;index++)
	if (fabs(triNormal.vec[index]) > fabs(triNormal.vec[i0]))
		i0 = index;
	
	if(i0 == 0)
		{ i1 = 1; i2 = 2; }
	else if (i0 == 1)
		{ i1 = 2; i2 = 0; }
	else
		{ i1 =0; i2 = 1; }
	
	u0 = pt[i1] - _v1[i1];
	v0 = pt[i2] - _v1[i2];
	u1 = _v2[i1] - _v1[i1];
	u2 = _v3[i1] - _v1[i1];
	v1 = _v2[i2] - _v1[i2];
	v2 = _v3[i2] - _v1[i2];
	if(u1 == 0)
	{
		beta = (u0/u2);
		if((beta >= 0) && (beta<= 1))
			alpha = (v0 - (beta*v2))/v1;
	}
	else
	{
		beta = ((v0*u1) - (u0*v1))/((v2*u1) - (u2*v1));
		if((beta >= 0) && (beta<= 1))
			alpha = (u0 - (beta*u2))/u1;
	}
	
	return ((alpha >= 0) && (beta >= 0) && ((alpha+beta) <= 1));
}



// Finds the point on the seg nearest to pt.
// Returns the nearest point in nearPt
void sgTri::findNearestPt(const sgVec3& pt, sgVec3& nearPt) const
{
        // find nearest pt on plaane.
        //      if it is in the tri, return it.
        // Otherwise return the pt closest on the edge of the tri

    sgPlane triPlane;
    triPlane.makePts(_v1, _v2, _v3);
    triPlane.findNearestPt(pt, nearPt);

    if(!ptIn(nearPt))
        findNearestEdgePt(pt, nearPt);
}
*/

#endif

