/*
 *  Vector_Complex.h
 *  
 *
 *  Created by Shaun Harker on 1/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _CHOMP_VECTOR_COMPLEX_
#define _CHOMP_VECTOR_COMPLEX_

#include <map> /* for map */
#include <utility> /* for pair */
#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for class Cell_Complex_Archetype */
#include "complexes/Morse_Complex.h"  /* for Morse_Traits */

/********************************************************************************
 *                            VECTOR COMPLEXES                                  *
 ********************************************************************************/

/*
 * Vector Chain 
 */
 
class Decomposition_Information {
public:
	Decomposition_Information ( void );
	mutable unsigned char flags; 
	enum { ACE = 0x01, KING = 0x02, QUEEN = 0x04, ALIVE = 0x08, EXCISED = 0x10 };
	mutable std::vector < std::pair < Default_Elementary_Chain, std::pair< Decomposition_Information, std::pair < Default_Chain, Default_Chain > > > > ::const_iterator husband;
	mutable unsigned int morse_value; 
};

/* * * * * * * * * * * * * * **
** class Vector_Container  **
** * * * * * * * * * * * * * */

class Vector_Container : public std::vector < std::pair < Default_Elementary_Chain, std::pair< Decomposition_Information, std::pair < Default_Chain, Default_Chain > > > > {
public:
	/* typedefs */	 
	typedef std::vector < std::pair < Default_Elementary_Chain, std::pair< Decomposition_Information, std::pair < Default_Chain, Default_Chain > > > >  vector_type;
	typedef Default_Chain Chain;
	typedef Default_Chain::Elementary_Chain Elementary_Chain;
	typedef Default_Chain::Ring Ring;
    typedef unsigned long size_type;
	size_type remembered_size;
	unsigned long beginning_index;
	
	/* Overrides */
	vector_type::const_iterator find ( const Elementary_Chain & find_me ) const;
	size_type size ( void ) const;
	vector_type::const_iterator begin ( void ) const;
};


/* * * * * * * * * * * * * **
** class Vector_Complex  **
** * * * * * * * * * * * * */

class Vector_Complex : public Cell_Complex_Archetype < Vector_Container > {
public:
	/* See Cell_Complex_Archetype */
	using Cell_Complex_Archetype<Vector_Container>::Boundary_Map; 
	using Cell_Complex_Archetype<Vector_Container>::Coboundary_Map;
	
	/* Constructors */
	Vector_Complex ( void );
	
	template < class Cell_Complex_Template >
	Vector_Complex ( Cell_Complex_Template & original_complex );
	
	/* * * * * * * * * * * * * * * * * * * * * * * * **
	** Pure Virtual Functions That Must Be Overriden **
	** * * * * * * * * * * * * * * * * * * * * * * * */
	
	/** Returns a copy of the Boundary information. This is only a copy, so subsequently altering this chain does not alter the complex directly. */
	virtual Chain & Boundary_Map ( Chain &, const Container::const_iterator & ) const;
	/** Returns a copy of the Boundary information. This is only a copy, so subsequently altering this chain does not alter the complex directly. */
	virtual Chain & Coboundary_Map ( Chain &, const Container::const_iterator & ) const;
	/** Remove an elementary chain from the complex. All terms from all chains involving this elementary chain will be deleted. NOT IMPLEMENTED. */
	virtual void Remove_Elementary_Chain ( const Elementary_Chain & );
	
	/* * * * * * **
	** Features  **
	** * * * * * */
	
	/** Returns a reference to the Boundary already stored. This is not a copy, so subsequently altering this chain alters the complex directly. */
	virtual Chain & Boundary_Map ( const Container::iterator & );
	/** Returns a reference to the Coboundary already stored. This is not a copy, so, subsequently altering this chain alters the complex directly. */
	virtual Chain & Coboundary_Map ( const Container::iterator & );
	/** Returns a const reference to the Boundary already stored. This is not a copy. */
	virtual const Chain & Boundary_Map ( const Container::const_iterator & ) const;
	/** Returns a const reference to the Coboundary already stored. This is not a copy. */
	virtual const Chain & Coboundary_Map ( const Container::const_iterator & ) const;
	/** Turns any Cell Complex into a Vector Complex */
	template < class Cell_Complex_Template >
	void Load_From_Cell_Complex ( Cell_Complex_Template & old_complex );
	/** Make a Vector Complex from the Cartesian Product of Two Complexes of Any Type */
	template < class First_Cell_Complex_Template, class Second_Cell_Complex_Template >
	void Product_Complex ( const First_Cell_Complex_Template & first_complex, const Second_Cell_Complex_Template & second_complex );
	
/* morse decomposition interface implementations */
	Vector_Container::const_iterator & Husband ( const Vector_Container::const_iterator & input ) const;
	unsigned int & Morse_Value ( const Vector_Container::const_iterator & input) const;
	unsigned char & Flags ( const Vector_Container::const_iterator & input ) const;
};


template <>
class Morse_Traits <Vector_Complex> {
public:
	/* Tags for Morse_Traits for feature checking */
	struct doesnt_store_husband_pointers {};
	struct does_store_husband_pointers {};
	struct doesnt_store_morse_values {};
	struct does_store_morse_values {};
	struct doesnt_store_flags {};
	struct does_store_flags {};

	/* Default Tag Choices */
	typedef does_store_husband_pointers does_it_store_husband_pointers;
	typedef does_store_morse_values does_it_store_morse_values;
	typedef does_store_flags does_it_store_flags;
	/* Default Data Types */
	typedef unsigned int morse_value_type;
};

#ifndef _CHOMP_LIBRARY_ONLY_
#include "complexes/Vector_Complex.hpp"
#endif

#ifdef _CHOMP_HEADER_ONLY_
#include "complexes/Vector_Complex.cpp"
#endif

#endif
