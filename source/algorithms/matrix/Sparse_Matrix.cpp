/*
 * Sparse_Matrix.cpp
 * CHomP
 * Shaun Harker
 *
 * This cpp file contains template instantiations.
 *
 */ 

#include "algorithms/matrix/Sparse_Matrix.h"

#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */

template class Sparse_Matrix < long >; /* Default_Ring = long */
template Sparse_Matrix<long> operator * < long > ( const Sparse_Matrix<long>  & left_hand_side, const Sparse_Matrix<long>  & right_hand_side);
template std::ostream & operator << <long> ( std::ostream & output_stream, const Sparse_Matrix<long> & print_me);
#endif
