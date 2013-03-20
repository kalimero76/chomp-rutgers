/*
 * Smith_Normal_Form.cpp
 * CHomP
 * Shaun Harker
 *
 * This cpp file contains template instantiations.
 *
 */ 

#include "algorithms/matrix/Smith_Normal_Form.h"
	
#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */

#include "algorithms/matrix/Sparse_Matrix.h" /* For default template implementation of Smith_Normal_Form */
#include "archetypes/Chain_Archetype.h" /* ditto */

typedef std::vector < std::pair < Default_Ring, int > > smith_form_output_type;
typedef Sparse_Matrix < Default_Ring > matrix_type;

template void Smith_Normal_Form < smith_form_output_type, matrix_type > 
( smith_form_output_type & normal_form, const matrix_type & matrix );
#endif
