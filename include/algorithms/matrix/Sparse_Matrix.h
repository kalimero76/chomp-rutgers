/*
 * Sparse_Matrix.h
 * CHomP
 * Shaun Harker
 *
 * This header contains declarations for Sparse Matrices.
 *
 */ 

#ifndef _CHOMP_SPARSE_MATRIX_
#define _CHOMP_SPARSE_MATRIX_

#include <map> /* for map<...> */
#include <iostream> /* for streams */
class Matrix_Indices {
public:
	long row;
	long column;
	Matrix_Indices ( long, long );
	bool operator < ( const Matrix_Indices & ) const;
};


// Forward Declaration of friend functions for Sparse_Matrix
template < class Ring_Template > class Sparse_Matrix;
template < class Ring_Template > Sparse_Matrix < Ring_Template > operator * ( const Sparse_Matrix < Ring_Template > &, const Sparse_Matrix < Ring_Template > & );
template < class Ring_Template > std::ostream & operator << ( std::ostream &, const Sparse_Matrix < Ring_Template > & );

template < class Ring_Template >
class Sparse_Matrix : public std::map < Matrix_Indices, Ring_Template > {
public:
	long number_of_rows;
	long number_of_columns;
	typedef Ring_Template Ring;
	Sparse_Matrix ( void ) ; 
	void set_entry ( const long , const long, const Ring );
	Ring get_entry ( const long, const long ) const;
	Sparse_Matrix compress ( void ) const;
	friend Sparse_Matrix operator * < Ring_Template > ( const Sparse_Matrix &, const Sparse_Matrix & );
	friend std::ostream & operator << < Ring_Template > ( std::ostream &, const Sparse_Matrix & );

	void debug_info ( void ) const;
};

/** Sparse_Matrix_Boundary_Map creates a sparse representation of the boundary map \f$\partial_k : C_k \rightarrow C_{k-1}\f$. The output is returned in "output_matrix",
* which is supplied by reference. The dimension \f$k\f$ is supplied as the parameter "dimension" */
template < class Cell_Complex >
void Sparse_Matrix_Boundary_Map ( Sparse_Matrix < typename Cell_Complex::Ring > & output_matrix, 
                                  const Cell_Complex & complex, const unsigned int dimension ); 

#ifndef _CHOMP_LIBRARY_ONLY_
#include "algorithms/matrix/Sparse_Matrix.hpp"
#endif

#ifdef _CHOMP_HEADER_ONLY_
#include "algorithms/matrix/Sparse_Matrix.cpp"
#endif

#endif
