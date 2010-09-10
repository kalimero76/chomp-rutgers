/*
 * Homology.cpp
 * CHomP
 * Shaun Harker
 *
 * This cpp file contains explicit instantiations 
 * to put in library to reduce compile time.
 *
 */ 

#include "algorithms/Homology.h"

/* Conley Index */
Conley_Index_t::Conley_Index_t ( void ) {
  undefined_flag = true;
} /* Conley_Index_t::Conley_Index_t */

Conley_Index_t::undefined ( void ) {
  return undefined_flag;
} /* Conley_Index_t::undefined */

#if 0
#ifndef CHOMP_HEADER_ONLY_
/* Explicit Template Instantiations */

#include "complexes/Cubical_Complex.h"
template void Homology < Cubical_Complex > ( const Cubical_Complex & );

#include "complexes/Abstract_Complex.h"
template void Homology < Abstract_Complex <> > ( const Abstract_Complex <> & );

#include "complexes/Morse_Complex.h"
template void Homology < Morse_Complex < Cubical_Complex > > ( const Morse_Complex < Cubical_Complex > & );
template void Homology < Morse_Complex < Abstract_Complex <> > > ( const Morse_Complex < Abstract_Complex <> > & );
#endif
#endif
