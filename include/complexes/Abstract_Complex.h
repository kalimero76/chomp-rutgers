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
class Abstract_Cell;
class Abstract_Chain;
class Abstract_Container;
class Abstract_Complex;

/* * * * * * * * * * * * * * **
 ** class Abstract_Container  **
 ** * * * * * * * * * * * * * */

class Abstract_Container : public std::set < Default_Cell >  {
public:
	/* typedefs */	 
	typedef Default_Chain Chain;
    typedef Chain::Cell Cell;
	typedef Chain::Ring Ring;
    std::map < Cell, Chain > boundary_data;
    std::map < Cell, Chain > coboundary_data;
    
};

/* * * * * * * * * * * * * **
 ** class Abstract_Complex  **
 ** * * * * * * * * * * * * */

class Abstract_Complex : public Cell_Complex_Archetype < Abstract_Container > {
public:
	/* See Cell_Complex_Archetype */
    
	/* * * * * * * * * * * * * * * * * * * * * * * * **
     ** Pure Virtual Functions That Must Be Overriden **
     ** * * * * * * * * * * * * * * * * * * * * * * * */
	
	/** Returns a copy of the Boundary information. This is only a copy, so subsequently altering this chain does not alter the complex directly. */
	virtual Chain & Boundary_Map ( Chain &, const Container::const_iterator & ) const;
	/** Returns a copy of the Boundary information. This is only a copy, so subsequently altering this chain does not alter the complex directly. */
	virtual Chain & Coboundary_Map ( Chain &, const Container::const_iterator & ) const;
	/** Remove an elementary chain from the complex. All terms from all chains involving this elementary chain will be deleted. NOT IMPLEMENTED. */
	virtual void Remove_Cell ( const Cell & );
	
    /* * * * * * * * * * *
	 * New Functionality *
	 * * * * * * * * * * */

	using Cell_Complex_Archetype<Abstract_Container>::Boundary_Map; 
	using Cell_Complex_Archetype<Abstract_Container>::Coboundary_Map;
	
	/** Returns a reference to the Boundary already stored. This is not a copy, so subsequently altering this chain alters the complex directly. */
	virtual Chain & Boundary_Map ( const Container::iterator & );
	/** Returns a reference to the Coboundary already stored. This is not a copy, so, subsequently altering this chain alters the complex directly. */
	virtual Chain & Coboundary_Map ( const Container::iterator & );
	/** Returns a const reference to the Boundary already stored. This is not a copy, but cannot be altered. */
	virtual const Chain & Boundary_Map ( const Container::const_iterator & ) const;
	/** Returns a const reference to the Coboundary already stored. This is not a copy, but cannot be altered. */
	virtual const Chain & Coboundary_Map ( const Container::const_iterator & ) const;
    
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Abstract_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Abstract_Complex.cpp"
#endif

#endif
