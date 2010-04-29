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

#include "complexes/Morse_Complex.h"

template < class Cell_Complex>
Morse_Complex<Cell_Complex> Ace_King_Queen_Algorithm ( const Cell_Complex & cell_complex );

#ifndef CHOMP_LIBRARY_ONLY_
#include "algorithms/Morse_Theory.hpp"
#endif

//#ifdef CHOMP_HEADER_ONLY_
//#include "algorithms/Morse_Theory.cpp"
//#endif

#endif
