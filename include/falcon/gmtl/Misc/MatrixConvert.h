/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          MatrixConvert.h,v
 * Date modified: 2007/06/25 22:59:14
 * Version:       1.1
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

#ifndef _GMTL_MATRIX_CONVERT_H_
#define _GMTL_MATRIX_CONVERT_H_

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/lambda/lambda.hpp>
#include <gmtl/Matrix.h>


namespace gmtl
{

/**
 * Converts a matrix of one data type to another, such as gmtl::Matrix44f to
 * gmtl::Matrix44d. Internally, this uses loop unrolling to optimize
 * performance.
 *
 * \code
 * const gmtl::Matrix44f m_float = getMatrix();
 * gmtl::Matrix44d m_double = gmtl::convertTo<double>(m_float);
 * \endcode
 *
 * @pre The input matrix and output matrix must have matching dimensions.
 *
 * @note The compiler will enforce the pre-condition about the matrix
 *       dimensions, but the error message may not always be clear. Use of a
 *       static assertion may help with that.
 *
 * @since 0.5.1
 */
template<typename DATA_TYPE_OUT, typename DATA_TYPE_IN, unsigned ROWS, unsigned COLS>
inline gmtl::Matrix<DATA_TYPE_OUT, ROWS, COLS>
convertTo(const gmtl::Matrix<DATA_TYPE_IN, ROWS, COLS>& in)
{
   using namespace boost::lambda;

   gmtl::Matrix<DATA_TYPE_OUT, ROWS, COLS> out;

   // Accessing in.mData and out.mData in this way allows for use of
   // Boost.Lambda so that a separate helper function is not required to do
   // the assignment.
   const DATA_TYPE_IN* in_data(in.mData);
   DATA_TYPE_OUT* out_data(out.mData);

   // This relies on implicit casting between data types.
   boost::mpl::for_each< boost::mpl::range_c<unsigned int, 0, ROWS * COLS> >(
      *(out_data + _1) = *(in_data + _1)
   );

   return out;
}

}


#endif /* _GMTL_MATRIX_CONVERT_H_ */
