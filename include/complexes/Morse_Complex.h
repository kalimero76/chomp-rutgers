/*
 *  Decomplex.h
 *  
 *
 *  Created by Shaun Harker on 10/21/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#ifndef CHOMP_Decomplex_
#define CHOMP_Decomplex_

#include <vector>

/* Forward Declarations */
template < class Cell_Complex > class Decomplex;

/* * * * * * * * * * * *
 * class Decomplex *
 * * * * * * * * * * * */

template < class Cell_Complex >
class Decomplex {
public: 
	/* typedefs */	 
	typedef Decomplex_Chain<Cell_Complex> Chain;
  typedef typename Cell_Complex::Cell Cell;
	typedef typename Cell_Complex::Ring Ring;
	typedef unsigned long size_type;
	typedef Cell key_type;
	typedef Cell value_type;
  typedef Subcomplex_const_iterator<Cell_Complex> const_iterator;
  typedef const_iterator iterator;
  /* Basic Container */ 
  std::pair<iterator, bool> insert ( const value_type & insert_me );
  void erase ( const iterator & erase_me );
  iterator find ( const key_type & find_me ) const;
  iterator begin ( void ) const;
  iterator end ( void ) const;
  size_type size ( void ) const;
  /* Cell Complex */
  iterator begin ( unsigned int dimension ) const;
  iterator end ( unsigned int dimension ) const;
  size_type size ( unsigned int dimension ) const;
  Chain boundary ( const const_iterator & input ) const;
  Chain coboundary ( const const_iterator & input ) const;
  unsigned int dimension ( void ) const;
  /* Decomposable Complex */
  /** constructor */
  Decomplex ( const Cell_Complex & cell_complex );
  /** husband */
	const_iterator & husband ( const const_iterator & );
  const const_iterator & husband ( const const_iterator & ) const;
  /** value */
	unsigned int & value ( const const_iterator & );
  const unsigned int & value ( const const_iterator & ) const;
  /** flags */
	unsigned char & flags ( const const_iterator & );
  const unsigned char & flags ( const const_iterator & ) const;

private:
  const Cell_Complex & cell_complex_;
  std::map < const_iterator, unsigned int > index_;
  std::vector < typename Cell_Complex::const_iterator > husband_;
	std::vector < unsigned int > value_;
	std::vector < unsigned char > flags_;	
};

/* * * * * * * * * * * * *
 * class Decomplex_Chain *
 * * * * * * * * * * * * */

template < class Cell_Complex >
class Decomplex_Chain {
};


#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Decomplex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Decomplex.cpp"
#endif

#endif
