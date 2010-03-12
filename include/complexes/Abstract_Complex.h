/*
 *  Abstract_Complex.h
 *  
 *
 *  Created by Shaun Harker on 10/6/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CHOMP_ABSTRACT_COMPLEX_
#define _CHOMP_ABSTRACT_COMPLEX_

#include <map> /* for map */
#include <utility> /* for pair */
#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for class Cell_Complex_Archetype */

/********************************************************************************
 *                            ABSTRACT COMPLEXES                                *
 ********************************************************************************/

/* * * * * * * * * * * * * * **
** class Abstract_Container  **
** * * * * * * * * * * * * * */

class Abstract_Container : public std::map < Default_Chain::Elementary_Chain, std::pair < Default_Chain, Default_Chain > >  {
public:
	/* typedefs */	 
	typedef Default_Chain Chain;
	typedef Default_Chain::Elementary_Chain Elementary_Chain;
	typedef Default_Chain::Ring Ring;
    
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
	
	/** Load abstract complex from file. NOT IMPLEMENTED. */
	virtual void Load_From_File ( const char * FileName );
	/** Returns a copy of the Boundary information. This is only a copy, so subsequently altering this chain does not alter the complex directly. */
	virtual Chain & Boundary_Map ( Chain &, const Container::const_iterator & ) const;
	/** Returns a copy of the Boundary information. This is only a copy, so subsequently altering this chain does not alter the complex directly. */
	virtual Chain & Coboundary_Map ( Chain &, const Container::const_iterator & ) const;
	/** Remove an elementary chain from the complex. All terms from all chains involving this elementary chain will be deleted. NOT IMPLEMENTED. */
	virtual void Remove_Elementary_Chain ( const Elementary_Chain & );
	
	/* New Functionality */
	
	/* using declarations to overcome C++ overloading/inheritance issue
	 * (C++ hides the functions in the base class if you overload in the 
	 * derived class.) */
	using Cell_Complex_Archetype<Abstract_Container>::Boundary_Map; 
	using Cell_Complex_Archetype<Abstract_Container>::Coboundary_Map;
	
	/** Returns a reference to the Boundary already stored. This is not a copy, so subsequently altering this chain alters the complex directly. */
	virtual Chain & Boundary_Map ( const Container::iterator & );
	/** Returns a reference to the Coboundary already stored. This is not a copy, so, subsequently altering this chain alters the complex directly. */
	virtual Chain & Coboundary_Map ( const Container::iterator & );
	/** Returns a const reference to the Boundary already stored. This is not a copy. */
	virtual const Chain & Boundary_Map ( const Container::const_iterator & ) const;
	/** Returns a const reference to the Coboundary already stored. This is not a copy. */
	virtual const Chain & Coboundary_Map ( const Container::const_iterator & ) const;
	
	/** Turns any Geometric Complex into an Abstract Complex */
	template < class Cell_Complex_Archetype_Template >
	void Load_From_Cell_Complex_Archetype ( Cell_Complex_Archetype_Template & old_complex );
	
	/* * * * * * * * * * * **
	** All other Overrides **
	** * * * * * * * * * * */
	
	void Geometric_Preprocessing ( void );

};

#ifndef _CHOMP_LIBRARY_ONLY_
#include "complexes/Abstract_Complex.hpp"
#endif

#ifdef _CHOMP_HEADER_ONLY_
#include "complexes/Abstract_Complex.cpp"
#endif

#endif
