/*
 *  Abstract_Complex.h
 *  
 *
 *  Created by Shaun Harker on 10/6/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CHOMP_ABSTRACT_COMPLEX_
#define CHOMP_ABSTRACT_COMPLEX_

#include <set> /* for map */
#include <utility> /* for pair */
#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for class Cell_Complex_Archetype */

/********************************************************************************
 *                            ABSTRACT COMPLEXES                                *
 ********************************************************************************/

/* Forward Declarations */
template < class > class Abstract_Container;
template < class > class Abstract_Complex;

/* * * * * * * * * * * * * * **
 ** class Abstract_Container  **
 ** * * * * * * * * * * * * * */

template < class Chain_Type = Default_Chain >
class Abstract_Container {
public:
	/* typedefs */	 
	typedef Chain_Type Chain;
  typedef typename Chain::Cell Cell;
	typedef typename Chain::Ring Ring;
  typedef size_t size_type;
  typedef Cell value_type;
  typedef Cell key_type;
  /* Comment: the following are identical in SGI defined STL library. */
  typedef typename std::set<Cell>::const_iterator const_iterator;
  typedef typename std::set<Cell>::iterator iterator;

  /* Simple Associative Container, Unique Associative Container, Cell Container, Boundary Container */
  Abstract_Container ( void );
  Abstract_Container ( unsigned int dimension );
  iterator find ( const key_type & find_me );
  const_iterator find ( const key_type & find_me ) const;
  std::pair<iterator, bool> insert ( const value_type & insert_me );
  void erase ( iterator erase_me );
  iterator begin ( unsigned int dimension ) const;
  iterator end ( unsigned int dimension ) const;
  size_type size ( unsigned int dimension ) const;
  Chain & boundary ( const iterator & input );
  const Chain & boundary ( const const_iterator & input ) const;
  Chain & coboundary ( const iterator & input );
  const Chain & coboundary ( const const_iterator & input ) const;
  unsigned int dimension ( void ) const;
  
private:
  std::set < Cell > cells_;
  std::vector < const_iterator > begin_;
  std::vector < const_iterator > end_;
  std::vector < size_type > size_;
  std::map < Cell, Chain > boundary_;
  std::map < Cell, Chain > coboundary_;
  unsigned int dimension_;
};

/* * * * * * * * * * * * * **
 ** class Abstract_Complex  **
 ** * * * * * * * * * * * * */
template < class Chain_Type = Default_Chain >
class Abstract_Complex : public Cell_Complex_Archetype < Abstract_Container < Chain_Type > > {
public:
  /* typedefs */
  typedef Abstract_Container < Chain_Type > Container;
	typedef Chain_Type Chain;
  typedef typename Chain::Cell Cell;
	typedef typename Chain::Ring Ring;
  typedef typename Container::const_iterator const_iterator;
  typedef typename Container::iterator iterator;

  /* Member variables */
  using Cell_Complex_Archetype < Container > :: dimension;
  using Cell_Complex_Archetype < Container > :: Boundary_Map; 
	using Cell_Complex_Archetype < Container > :: Coboundary_Map;
	
  /* * * * * * * * * * *
	 * New Functionality *
	 * * * * * * * * * * */
	
	/** Returns a reference to the Boundary already stored. This is not a copy, so subsequently altering this chain alters the complex directly. */
	virtual Chain & Boundary_Map ( const iterator & );
	/** Returns a reference to the Coboundary already stored. This is not a copy, so, subsequently altering this chain alters the complex directly. */
	virtual Chain & Coboundary_Map ( const iterator & );
	/** Returns a const reference to the Boundary already stored. This is not a copy, but cannot be altered. */
	virtual const Chain & Boundary_Map ( const const_iterator & ) const;
	/** Returns a const reference to the Coboundary already stored. This is not a copy, but cannot be altered. */
	virtual const Chain & Coboundary_Map ( const const_iterator & ) const;
  
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Abstract_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Abstract_Complex.cpp"
#endif

#endif
