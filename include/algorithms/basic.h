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

template < class Complex >
typename Complex::Chain 
boundary ( const typename Complex::Chain & input, const Complex & complex );

template < class Chain >
Chain boundary ( const Chain & input );

template < class Chain >
Chain coboundary ( const Chain & input );

/* still toying with this 

template < class Chain >
Chain operator + ( const Chain & left_hand_side, const Chain & right_hand_side );

template < class Chain >
Chain operator - ( const Chain & left_hand_side, const Chain & right_hand_side );

template < class Chain >
Chain & operator += ( Chain & left_hand_side, const Chain & right_hand_side );

template < class Chain >
Chain & operator -= ( Chain & left_hand_side, const Chain & right_hand_side );

template < class Chain >
Chain & operator += ( Chain & left_hand_side, const typename Chain::value_type & right_hand_side );

template < class Chain >
Chain & operator -= ( Chain & left_hand_side, const typename Chain::value_type & right_hand_side );

template < class Chain >
Chain & operator *= ( Chain & left_hand_side, const typename Chain::mapped_type & right_hand_side );
*/

/* * * * * * * * * * * * * * * * * * * * * * * *
 * TEST UTILITIES -- Common testing functions  *
 * * * * * * * * * * * * * * * * * * * * * * * */
#ifdef TEST_PROGRAM
namespace utility {
  
/* Declar/ations */
class compute_results;
template < class Cell_Complex_Template > compute_results compute_example ( Cell_Complex_Template & my_complex );
template < class T > void print_my_vector ( std::vector<T> print_me );
template < class T > T vector_sum ( const std::vector<T> add_my_elements_together );
template < class T > T vector_prod ( const std::vector<T> multiply_my_elements_together );
template < class Cell_Complex > void inspect_complex ( Cell_Complex & my_complex );
template < class Cell_Complex > void AKQ_inspect ( const Cell_Complex & my_complex );

int do_preprocess;

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

#endif

#ifndef CHOMP_LIBRARY_ONLY_
#include "algorithms/basic.hpp"
#endif

//#ifdef CHOMP_HEADER_ONLY_
//#include "algorithms/basic.cpp"
//#endif

#endif
