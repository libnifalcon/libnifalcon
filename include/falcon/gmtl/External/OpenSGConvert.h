/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          OpenSGConvert.h,v
 * Date modified: 2007/06/25 22:47:55
 * Version:       1.10
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
#ifndef GMTL_OPENSG_CONVERT_H_
#define GMTL_OPENSG_CONVERT_H_

/** @file OpenSGConvert.h GMTL/OpenSG conversion functions
 * 
 * Methods to convert between GTML and OpenSG matrix classes.
 */

#include <falcon/gmtl/Matrix.h>
#include <falcon/gmtl/Generate.h>
#include <OpenSG/OSGMatrix.h>

namespace gmtl
{

/**
 * Converts an OpenSG matrix to a gmtl::Matrix.
 *
 * @param mat        The matrix to write the OpenSG matrix data into.
 * @param osgMat     The source OpenSG matrix.
 *
 * @return The equivalent GMTL matrix.
 */
inline Matrix44f& set(Matrix44f& mat, const OSG::Matrix& osgMat)
{
   mat.set(osgMat.getValues());
   return mat;
}

/**
 * Converts a GMTL matrix to an OpenSG matrix.
 *
 * @param osgMat     The matrix to write the GMTL matrix data into.
 * @param mat        The source GMTL matrix.
*
 * @return The equivalent OpenSG matrix.
 */
inline OSG::Matrix& set(OSG::Matrix& osgMat, const Matrix44f& mat)
{
   osgMat.setValue(mat.getData());
   return osgMat;
}

}

#endif
