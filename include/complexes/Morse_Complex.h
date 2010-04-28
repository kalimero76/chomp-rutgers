/*
 *  Morse_Complex.h
 *  
 *
 *  Created by Shaun Harker on 10/21/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#ifndef CHOMP_MORSE_COMPLEX_
#define CHOMP_MORSE_COMPLEX_

#include <vector>

/* Forward Declarations */
template < class Cell_Complex > class Morse_Complex;

/* * * * * * * * * * * *
 * class Morse_Complex *
 * * * * * * * * * * * */
template < class Cell_Complex >
class Morse_Complex : public Cell_Complex {
public: 
  /* typedefs */
  typedef Cell_Complex::Ring Ring;
	typedef Cell_Complex::Cell Cell;
  typedef Cell_Complex::Chain Chain;
	typedef unsigned long size_type;
	typedef Cell_Complex::Cell key_type;
	typedef Cell_Complex::Cell value_type;
  typedef Cell_Complex::const_iterator const_iterator;
  typedef Cell_Complex::const_iterator iterator;
  /* Simple Associative Container, Unique Associative Container, Cell Container */
  using Cell_Complex::insert;
  using Cell_Complex::erase;
  using Cell_Complex::begin;
  using Cell_Complex::end;
  using Cell_Complex::find;
  using Cell_Complex::size;
  using Cell_Complex::boundary;
  using Cell_Complex::coboundary;
  using Cell_Complex::dimension;
  
  /** constructor */
  Morse_Complex ( const Cell_Complex & cell_complex );
  /** husband */
	const_iterator & husband ( const const_iterator & );
  /** value */
	typename unsigned int & value ( const const_iterator & );
  /** flags */
	unsigned char & flags ( const const_iterator & );
  /** canonicalize */
  Chain canonicalize ( const Chain & input ) const;
  /** complete */
  Chain complete ( const Chain & input ) const;
  /** project */
  Chain project ( const Chain & input ) const;

private:
  const Cell_Complex & cell_complex_;
  std::vector < typename Cell_Complex_Template::Container::const_iterator > husband_;
	std::vector < unsigned int > value_;
	std::vector < unsigned char > flags_;	
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Morse_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Morse_Complex.cpp"
#endif

#endif
