/*
 * Dense_Matrix.h
 * CHomP
 * Shaun Harker
 *
 * This header contains declarations for Sparse Matrices.
 *
 */ 

#ifndef _CHOMP_DENSE_MATRIX_
#define _CHOMP_DENSE_MATRIX_

#include "capd/matrixAlgorithms/intMatrixAlgorithms.hpp" /* for smithForm */
#include <vector>

/* C++ doesn't allow templated typedef's yet. This is the standard workaround. 
 * The syntax is pretty close:
 *        If c++ had templated typedefs I'd have syntax Dense_Matrix<>
 *        This workaround gives me the syntax           Dense<>::Matrix  */
template < class Ring_Template >
struct Dense {
	typedef capd::vectalg::Matrix<Ring_Template, 0, 0> Matrix;
};

/** Dense_Matrix_Boundary_Map creates a sparse representation of the boundary map \f$\partial_k : C_k \rightarrow C_{k-1}\f$. The output is returned in "output_matrix",
* which is supplied by reference. The dimension \f$k\f$ is supplied as the parameter "dimension" */
template < class Cell_Complex >
void Dense_Matrix_Boundary_Map ( typename Dense < typename Cell_Complex::Ring >::Matrix & output_matrix,
                                 std::vector < typename Cell_Complex::Cell > & translation_table,
                                 const Cell_Complex & complex, const unsigned int dimension );

#ifndef _CHOMP_LIBRARY_ONLY_
#include "algorithms/matrix/Dense_Matrix.hpp"
#endif

#ifdef _CHOMP_HEADER_ONLY_
#include "algorithms/matrix/Dense_Matrix.cpp"
#endif

#endif
