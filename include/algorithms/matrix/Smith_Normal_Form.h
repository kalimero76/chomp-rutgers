/*
 * Smith_Normal_Form.h
 * CHomP
 * Shaun Harker
 *
 * This header contains declarations for computing Smith Normal Form.
 *
 */ 

#ifndef CHOMP_SMITH_NORMAL_FORM_
#define CHOMP_SMITH_NORMAL_FORM_

template < class Vector_of_Pairs, class Matrix_Template /* = Sparse_Matrix */>
void Smith_Normal_Form ( Vector_of_Pairs & normal_form, const Matrix_Template & matrix );

#ifndef CHOMP_LIBRARY_ONLY_
#include "algorithms/matrix/Smith_Normal_Form.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "algorithms/matrix/Smith_Normal_Form.cpp"
#endif

#endif
