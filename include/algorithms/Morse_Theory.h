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

#include <vector>
#include <list>

/* * * * * * * * * * * *
 * typedef Morse_Chain *
 * * * * * * * * * * * */
template < class size_type >
class Morse_Chain : public std::map < size_type, Default_Ring, std::greater < size_type > > {};

namespace morse {
  /* Give an AKQ decomposition to the input complex. 
     Return type is the king count on each dimension. */
  template < class Cell_Complex > std::vector < typename Cell_Complex::size_type > 
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
  template < class Cell_Complex > Morse_Chain<typename Cell_Complex::size_type>
  morse_boundary ( Morse_Chain<typename Cell_Complex::size_type> input, 
                   const Cell_Complex & complex );
} /* namespace morse */

#include "complexes/Abstract_Complex.h"
typedef Abstract_Complex< Default_Cell > Morse_Complex;

namespace morse {
  
  template < class Cell_Complex > void
  reduction ( Morse_Complex & reduced_complex, const Cell_Complex & complex );

  template < class Cell_Complex > std::list<Morse_Complex> 
  reduction_tower ( Cell_Complex & complex );
  
  /* Destroy original complex to save memory, and only remember the most
     reduced complex found. */
  template < class Cell_Complex > Morse_Complex *
  deep_reduction ( Cell_Complex & complex );
  
} /* namespace morse */

#ifndef CHOMP_LIBRARY_ONLY_
#include "algorithms/Morse_Theory.hpp"
#endif

//#ifdef CHOMP_HEADER_ONLY_
//#include "algorithms/Morse_Theory.cpp"
//#endif

#endif
