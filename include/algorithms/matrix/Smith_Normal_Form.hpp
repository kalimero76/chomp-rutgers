/*
 * Smith_Normal_Form.hpp
 * CHomP
 * Shaun Harker
 *
 * This hpp file contains implementations for computing Smith Normal Form.
 *
 */ 

#include <iostream> /* for std::cout */
#include <vector>  /* for std::vector */
#include <utility> /* for std::pair */
#include <algorithm>

#ifdef USE_GMP
#include <gmpxx.h>
#endif

template < class Vector_of_Pairs, class Ring >
void Smith_Normal_Form ( Vector_of_Pairs & normal_form, const Sparse_Matrix<Ring> & input_matrix ) {
#ifdef USE_GMP
  typedef mpz_class ComputeRing;
#else
  typedef Ring ComputeRing;
#endif
  typedef Sparse_Matrix<ComputeRing> Matrix;  
  Matrix M ( input_matrix );
	/* Compute Smith Normal Form */
	Matrix U, Uinv, V, Vinv, D; 
	SmithNormalForm < ComputeRing > ( &U, &Uinv, &V, &Vinv, &D, M);
  
  int size_limit = std::min(D . number_of_rows (), D . number_of_columns ());
	/* Parse the smith form output into vector of pairs <scalar, multiplicity> */
	Ring current_diagonal_value ( 1 );
	normal_form . clear ();
	normal_form . push_back ( typename Vector_of_Pairs::value_type ( current_diagonal_value, 0 ) );
	int normal_form_index = 0;
	for ( int index = 0; index < size_limit; ++ index ) {
		if ( current_diagonal_value == D . read ( index, index ) )
			++ normal_form [ normal_form_index ] . second;
		else {
#ifdef USE_GMP
			current_diagonal_value = D . read ( index, index ) . get_si ();
#else
      current_diagonal_value = D . read ( index, index );
#endif
      
      if ( current_diagonal_value == Ring ( 0 ) ) break;
			normal_form . push_back ( typename Vector_of_Pairs::value_type ( current_diagonal_value, 1 ) ); 
      ++ normal_form_index;
    } /*if-else*/ 
  } /* for */
	/* "normal_form" now contains the answer in the desired form, so we return. */
	return; 
} /* Smith_Normal_Form */
