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
class Abstract_Container : public std::set < typename Chain_Type::Cell >  {
public:
	/* typedefs */	 
	typedef Chain_Type Chain;
  typedef typename Chain::Cell Cell;
	typedef typename Chain::Ring Ring;
  std::map < Cell, Chain > boundary_data;
  std::map < Cell, Chain > coboundary_data;
  
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
  using Cell_Complex_Archetype < Container > :: cells;
  using Cell_Complex_Archetype < Container > :: dimension;
  using Cell_Complex_Archetype < Container > :: Boundary_Map; 
	using Cell_Complex_Archetype < Container > :: Coboundary_Map;
  
	/* * * * * * * * * * * * * * * * * * * * * * * * *
   * Pure Virtual Functions That Must Be Overriden *
   * * * * * * * * * * * * * * * * * * * * * * * * */
	
	/** Returns a copy of the Boundary information. This is only a copy, so subsequently altering this chain does not alter the complex directly. */
	virtual Chain & Boundary_Map ( Chain &, const const_iterator & ) const;
	/** Returns a copy of the Boundary information. This is only a copy, so subsequently altering this chain does not alter the complex directly. */
	virtual Chain & Coboundary_Map ( Chain &, const const_iterator & ) const;
	/** Remove an elementary chain from the complex. All terms from all chains involving this elementary chain will be deleted. NOT IMPLEMENTED. */
	virtual void Remove_Cell ( const Cell & );
	
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