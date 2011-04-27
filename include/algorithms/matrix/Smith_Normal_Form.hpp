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

template < class Vector_of_Pairs, class Ring >
void Smith_Normal_Form ( Vector_of_Pairs & normal_form, const Sparse_Matrix<Ring> & input_matrix ) {
  typedef Sparse_Matrix<Ring> Matrix;
	/* Compute Smith Normal Form */
	Matrix U, Uinv, V, Vinv, D; 
	SmithNormalForm < Ring > ( &U, &Uinv, &V, &Vinv, &D, input_matrix);
  
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
			current_diagonal_value = D . read ( index, index );
      if ( current_diagonal_value == Ring ( 0 ) ) break;
			normal_form . push_back ( typename Vector_of_Pairs::value_type ( current_diagonal_value, 1 ) ); 
      ++ normal_form_index;
    } /*if-else*/ 
  } /* for */
	/* "normal_form" now contains the answer in the desired form, so we return. */
	return; 
} /* Smith_Normal_Form */
