// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_MatrixTypes_hpp
#define CF_MatrixTypes_hpp

////////////////////////////////////////////////////////////////////////////////

#include <Eigen/Dense>

#include <Common/CF.hpp>

////////////////////////////////////////////////////////////////////////////////

namespace CF {

////////////////////////////////////////////////////////////////////////////////


/// Dynamic sized matrix of Real scalars
typedef Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic> RealMatrix;

/// Fixed size matrices for 2x2, 3x3 and 4x4
typedef Eigen::Matrix<Real, 2, 2> RealMatrix2;
typedef Eigen::Matrix<Real, 3, 3> RealMatrix3;
typedef Eigen::Matrix<Real, 4, 4> RealMatrix4;

/// Dynamic sized vector (colum)
typedef Eigen::Matrix<Real, Eigen::Dynamic, 1> RealVector;

/// Fixed size vectors for 2, 3 and 4 elements
typedef Eigen::Matrix<Real, 2, 1> RealVector2;
typedef Eigen::Matrix<Real, 3, 1> RealVector3;
typedef Eigen::Matrix<Real, 4, 1> RealVector4;

////////////////////////////////////////////////////////////////////////////////

} // namespace CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_MatrixTypes_hpp