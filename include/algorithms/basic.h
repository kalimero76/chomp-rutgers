/*
 *  basic.h
 *  
 *
 *  Created by Shaun Harker on 4/29/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef CHOMP_BASIC_H
#define CHOMP_BASIC_H

#include <vector>

template < class Complex >
typename Complex::Chain 
boundary ( const typename Complex::Chain & input, const Complex & complex );

template < class Complex >
typename Complex::Chain 
coboundary ( const typename Complex::Chain & input, const Complex & complex );

template < class Complex >
typename Complex::Chain sum ( const typename Complex::Chain & lhs, const typename Complex::Chain & rhs );


/* Solve AX = B. warning -- writes over A */
template < class Matrix >
Matrix matrix_solve ( Matrix & A, Matrix & B );

/* Functions to verify complex property and morse decompositions */
template < class Cell_Complex > void verify_complex ( Cell_Complex & my_complex );
template < class Cell_Complex > void verify_decomposition ( Cell_Complex & my_complex );

/* * * * * * * * * * * * * * * * * * * * * * * *
 * TEST UTILITIES -- Common testing functions  *
 * * * * * * * * * * * * * * * * * * * * * * * */
//#define TEST_PROGRAM
//#ifdef TEST_PROGRAM
namespace utility {
  
/* Declar/ations */
class compute_results;
template < class Cell_Complex_Template > compute_results compute_example ( Cell_Complex_Template & my_complex );
template < class T > void print_my_vector ( std::vector<T> print_me );
template < class T > T vector_sum ( const std::vector<T> add_my_elements_together );
template < class T > T vector_prod ( const std::vector<T> multiply_my_elements_together );
template < class Cell_Complex > void inspect_complex ( Cell_Complex & my_complex );
template < class Cell_Complex > void AKQ_inspect ( const Cell_Complex & my_complex );

//extern int do_preprocess;

/* Definitions */
class compute_results {
public:
	unsigned long original_size;
	unsigned long morse_size;
	unsigned long homology_size;
	float total_time;
	float morse_time;
	float smith_time;
};

} /* namespace utility */

//#endif

#ifndef CHOMP_LIBRARY_ONLY_
#include "algorithms/basic.hpp"
#endif

//#ifdef CHOMP_HEADER_ONLY_
//#include "algorithms/basic.cpp"
//#endif

#endif
