/*
 *  Morse_Theory.h
 *  
 *
 *  Created by Shaun Harker on 4/28/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef CHOMP_MORSE_THEORY_H
#define CHOMP_MORSE_THEORY_H

#include "complexes/Abstract_Complex.h"

namespace morse {
  
  template < class Cell_Complex > void 
  decompose ( Cell_Complex & cell_complex );
  
  template < class Cell_Complex > typename Cell_Complex::Chain 
  alpha ( const typename Cell_Complex::Chain & input );
  
  template < class Cell_Complex > typename Cell_Complex::Chain 
  beta ( const typename Cell_Complex::Chain & input );
  
  template < class Cell_Complex > typename Cell_Complex::Chain 
  gamma ( const typename Cell_Complex::Chain & input );
  
  template < class Cell_Complex > typename Cell_Complex::Chain 
  project ( const typename Cell_Complex::Chain & input );
  
  template < class Cell_Complex > Abstract_Complex< typename Cell_Complex::const_iterator> 
  reduction ( const Cell_Complex & complex );

} /* namespace morse */

#ifndef CHOMP_LIBRARY_ONLY_
#include "algorithms/Morse_Theory.hpp"
#endif

//#ifdef CHOMP_HEADER_ONLY_
//#include "algorithms/Morse_Theory.cpp"
//#endif

#endif
