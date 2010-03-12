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
#include "capd/matrixAlgorithms/intMatrixAlgorithms.hpp" /* for smithForm */

template < class Vector_of_Pairs , class Matrix_Template /* = Sparse_Matrix */>
void Smith_Normal_Form ( Vector_of_Pairs & normal_form, const Matrix_Template & input_matrix ) {
	/* Compress the input_matrix since we are going to make a dense version */
	const Matrix_Template compressed_matrix = input_matrix . compress ();
	/* Create a CAPD matrix. */
	capd::vectalg::Matrix<typename Matrix_Template::Ring, 0, 0> CAPD_matrix ( compressed_matrix . number_of_rows, compressed_matrix . number_of_columns);
	/* Copy the input_matrix into the CAPD matrix */
	for ( typename Matrix_Template::const_iterator sparse_iterator = compressed_matrix . begin (); 
	sparse_iterator != compressed_matrix . end (); ++ sparse_iterator ) {
		CAPD_matrix ( sparse_iterator -> first . row + 1, sparse_iterator -> first . column + 1) = sparse_iterator -> second; } /* for */
	/* Run the CAPD Smith Normal Form code */
	capd::vectalg::Matrix<typename Matrix_Template::Ring, 0, 0> Q, Qinv, R, Rinv; int s, t;
	capd::matrixAlgorithms::smithForm( CAPD_matrix, Q, Qinv, R, Rinv, s, t);
	/* Parse the smith form output into vector of pairs <scalar, multiplicity> */
	typename Matrix_Template::Ring current_diagonal_value ( 1 );
	normal_form . clear ();
	normal_form . push_back ( typename Vector_of_Pairs::value_type ( current_diagonal_value, 0 ) );
	int normal_form_index = 0;
	for ( int index = 0; index < t; ++ index ) {
		if ( current_diagonal_value == CAPD_matrix ( index + 1, index + 1 ) )
			++ ( normal_form [ normal_form_index ] . second );
		else {
			current_diagonal_value = CAPD_matrix ( index + 1, index + 1 );
			normal_form . push_back ( typename Vector_of_Pairs::value_type ( current_diagonal_value, 1 ) ); } /*if-else*/ } /* for */
	/* "normal_form" now contains the answer in the desired form, so we return. */
	return; } /* Smith_Normal_Form */
