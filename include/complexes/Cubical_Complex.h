/*
 *  Cubical_Complex.h
 *  srh_chomp
 *
 *  Created by Shaun Harker on 9/14/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CHOMP_CUBICAL_COMPLEX_
#define _CHOMP_CUBICAL_COMPLEX_

#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for Cell_Complex */

/********************************************************************************
 *                             CUBICAL COMPLEXES                                *
 ********************************************************************************/


/* * * * * * * * * * * * * **
** class Cubical_Container **
** * * * * * * * * * * * * */

class Cubical_Container {
public:
	/* typedefs */	 
	typedef Default_Chain Chain;
	typedef Chain::Elementary_Chain Elementary_Chain;
	typedef Chain::Ring Ring;
	typedef unsigned long size_type;
    
    /* data */
	unsigned int space_dimension;
	unsigned int cube_dimension; 
	unsigned long number_of_cubes;
	std::vector<unsigned int> * dimension_sizes; 
	std::vector<unsigned long> * jump_values; 
	std::vector<bool> * bitmap; 
	size_type remembered_size;
	mutable unsigned long first_address;
	
	/* STL container types */
	typedef Elementary_Chain key_type;
	typedef bool data_type;
	typedef bool mapped_type;
	typedef std::pair<Elementary_Chain, bool> value_type;

	/* Nested Classes*/
	 
	class const_iterator;
	friend class const_iterator;
	
	/* const_iterator */
	 
	class const_iterator {
	public:
		const Cubical_Container * referral;
		/** Because elementary chain's are stored implicitly,
		 * the arrow operator -> cannot be implemented without
		 * having an actual reference somewhere. The private
		 * variable dereference_value is the address returned
		 * by ->. Whenever -> is called, dereference_value is 
		 * updated, and never otherwise. 
		 * Notice we return a pair, to conform to pair associative
		 * containers. The second argument is a bool which is
		 * to be 'true'. This corresponds with the view of a set
		 * being a map from the set into {true, false}, where the
		 * set is the preimage of 'true'. 
		 * We have to make it mutable since -> should work for
		 * const const_iterator's as well.*/
		mutable Cubical_Container::value_type dereference_value;
		void next_piece_number ( void ); 
		unsigned long full_cube_number; 
		unsigned long piece_number; 
		const_iterator ( void );
		const_iterator ( const Cubical_Container * const); 
		const_iterator ( const Cubical_Container * const referral, const unsigned long full_cube_number, const unsigned long piece_number );

		const_iterator & operator ++ ( void );
		bool operator != ( const const_iterator & ) const;
		bool operator == ( const const_iterator & ) const;

		const Cubical_Container::value_type & operator * ( void ) const; 
		const Cubical_Container::value_type * operator -> ( void ) const;
	};
	size_type size ( void ) const;
	const_iterator begin ( void ) const;
	const_iterator end ( void ) const;
	const_iterator find ( const key_type & ) const;

};

/* * * * * * * * * * * * **
** class Cubical_Complex **
** * * * * * * * * * * * */

class Cubical_Complex : public Cell_Complex_Archetype < Cubical_Container > {
	/* Optimizations */
	bool is_a_full_complex;
	bool data_allocated;
	
public:
	/* See Cell_Complex_Archetype */
	
	/** Constructor */
	Cubical_Complex ( void );
	
	/** Copy constructor (deep copy required) */
	Cubical_Complex ( const Cubical_Complex & );
	
	/** Deconstructor (deep deconstruction required) */
	~Cubical_Complex ( void );
	
	/* * * * * * * * * * * * * * * * * * * * * * * * **
	** Pure Virtual Functions That Must Be Overriden **
	** * * * * * * * * * * * * * * * * * * * * * * * */
	
	virtual void Load_From_File ( const char * FileName );
	virtual Chain & Boundary_Map ( Chain &, const Container::const_iterator & ) const;
	virtual Chain & Coboundary_Map ( Chain &, const Container::const_iterator & ) const;
	virtual void Remove_Elementary_Chain ( const Elementary_Chain & );
	
	/* Overloaded declarations in a derived class
	 * hide the base class members */
	 
	using Cell_Complex_Archetype<Cubical_Container>::Boundary_Map;  
	using Cell_Complex_Archetype<Cubical_Container>::Coboundary_Map;
	
	/* * * * * * * * * * * **
	** All other functions **
	** * * * * * * * * * * */

	void Insert_Elementary_Cell ( const unsigned long address  );
	void Add_Full_Cube ( const std::vector<unsigned int> & coordinates );
	void Allocate_Bitmap ( const std::vector<unsigned int> & sizes );
};

#ifndef _CHOMP_LIBRARY_ONLY_
#include "complexes/Cubical_Complex.hpp"
#endif

#ifdef _CHOMP_HEADER_ONLY_
#include "complexes/Cubical_Complex.cpp"
#endif

#endif
