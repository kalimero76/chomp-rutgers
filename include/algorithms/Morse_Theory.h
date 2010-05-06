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

/* * * * * * * * * * * *
 * typedef Morse_Chain *
 * * * * * * * * * * * */
typedef std::map < unsigned long, Default_Ring, std::greater < unsigned long > > Morse_Chain;


namespace morse {
  /* Give an AKQ decomposition to the input complex. 
     Return type is the king count on each dimension. */
  template < class Cell_Complex > std::vector < unsigned long > 
  decompose ( Cell_Complex & cell_complex );

#if 0
  /* Canonicalizes chains. alpha = id + boundary o gamma */
  template < class Cell_Complex > typename Cell_Complex::Chain 
  alpha ( const typename Cell_Complex::Chain & input );
  
  /* Completes Chains. beta = id + gamma o boundary */
  template < class Cell_Complex > typename Cell_Complex::Chain 
  beta ( const typename Cell_Complex::Chain & input );
  
  /* Finds the king chain k such that c + dk is canonical */
  template < class Cell_Complex > typename Cell_Complex::Chain 
  gamma ( const typename Cell_Complex::Chain & input );
  
  /* Projects chains. That is, removes all terms that are not aces. */
  template < class Cell_Complex > typename Cell_Complex::Chain 
  project ( const typename Cell_Complex::Chain & input );
#endif
  /* An optimized composition of project and alpha */
  template < class Cell_Complex > Morse_Chain 
  morse_boundary ( Morse_Chain input, const Cell_Complex & complex);
} /* namespace morse */

#include "complexes/Abstract_Complex.h"
typedef Abstract_Complex< Default_Cell > Morse_Complex;

namespace morse {
  
  template < class Cell_Complex > Morse_Complex 
  reduction ( const Cell_Complex & complex );

} /* namespace morse */

#ifndef CHOMP_LIBRARY_ONLY_
#include "algorithms/Morse_Theory.hpp"
#endif

//#ifdef CHOMP_HEADER_ONLY_
//#include "algorithms/Morse_Theory.cpp"
//#endif

#endif
