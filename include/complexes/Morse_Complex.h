/*
 *  Morse_Complex.h
 *  
 *
 *  Created by Shaun Harker on 10/21/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CHOMP_MORSE_COMPLEX_
#define CHOMP_MORSE_COMPLEX_

#include <map> /* for std::map */
//#include <ext/hash_map> /* for std::hash_map */
//namespace std { using namespace __gnu_cxx; }
#include <set> /* for std::set */
#include "complexes/Abstract_Complex.h" /* for class Abstract_Complex */


/* Forward Declarations */
template < class Cell_Complex_Template > class Morse_Traits;
template < class Cell_Complex_Template > class Morse_Complex;

/** Morse_Complex class. */
template < class Cell_Complex_Template >
class Morse_Complex : public Abstract_Complex {
public: 
	mutable std::map < typename Cell_Complex_Template::Cell, typename Cell_Complex_Template::Container::const_iterator > husband_pointers;
	mutable std::map < typename Cell_Complex_Template::Cell, typename Morse_Traits<Cell_Complex_Template>::morse_value_type > morse_values;
	mutable std::map < typename Cell_Complex_Template::Cell, unsigned char > flags;	

	typename Cell_Complex_Template::Container::const_iterator & Husband ( const typename Cell_Complex_Template::Container::const_iterator & ) const;
	typename Morse_Traits<Cell_Complex_Template>::morse_value_type & Morse_Value ( const typename Cell_Complex_Template::Container::const_iterator &) const ;
	unsigned char & Flags ( const typename Cell_Complex_Template::Container::const_iterator & ) const;

	
	enum { ACE = 0x01, KING = 0x02, QUEEN = 0x04, ALIVE = 0x08, EXCISED = 0x10 };

	bool Is_an_Ace ( unsigned char ) const; 
	bool Is_a_King ( unsigned char ) const; 
	bool Is_a_Queen ( unsigned char ) const; 
	bool Is_Alive ( unsigned char ) const; 
	bool Is_Excised ( unsigned char ) const; 

public:
	typedef Cell_Complex_Template Cell_Complex_Type;
	const Cell_Complex_Template & original_complex; /* notice features in complex will have to be specifically declared mutable */
	
	Morse_Complex ( const Cell_Complex_Template & original_complex); 
	void Ace_King_Queen_Algorithm ( void );
	void Morse_Boundary_Algorithm ( void );
	typename Cell_Complex_Template::Chain & Chain_Correspondence_Algorithm ( typename Cell_Complex_Template::Chain & canonical_chain, const Abstract_Complex::Chain & morse_chain );
	
};

	
/** Default Morse traits class. The defaults say that the features are missing. This tells Morse_Complex to
 * pick up the slack. If a complex has a feature mentioned here, specialize this template and choose the non-
 * default choice. */
template < class Cell_Complex_Template >
class Morse_Traits {
public:
	/* Tags for Morse_Traits for feature checking */
	struct doesnt_store_husband_pointers {};
	struct does_store_husband_pointers {};
	struct doesnt_store_morse_values {};
	struct does_store_morse_values {};
	struct doesnt_store_flags {};
	struct does_store_flags {};

	/* Default Tag Choices */
	typedef doesnt_store_husband_pointers does_it_store_husband_pointers;
	typedef doesnt_store_morse_values does_it_store_morse_values;
	typedef doesnt_store_flags does_it_store_flags;
	/* Default Data Types */
	typedef unsigned int morse_value_type;
};

/* Classes used by Morse_Complex implementation -- maybe ought to put in .hpp */

/* A Chain Class for the upcoming algorithm */
// Forward Declaration of friend functions for ABC_Chain
template < class Morse_Complex_Template > class Morse_Value_Cell ;
template < class Morse_Complex_Template > std::ostream & operator << ( std::ostream &, const Morse_Value_Cell< Morse_Complex_Template > &);


template < class Morse_Complex_Template >
class Morse_Value_Cell {
public:
	typedef typename Morse_Complex_Template::Cell_Complex_Type Cell_Complex_Template;
	typedef typename Morse_Traits<Cell_Complex_Template>::morse_value_type Morse_Value_Type;
	Morse_Value_Type morse_value;
	typename Cell_Complex_Template::Container::const_iterator location;
	/* Constructor */
	Morse_Value_Cell ( Morse_Value_Type morse_value,  typename Cell_Complex_Template::Container::const_iterator location );
	/* We order the Morse Values so the larger morse values are near 'begin' */
	bool operator < ( const Morse_Value_Cell & right_hand_side ) const;
	friend std::ostream & operator << < Morse_Complex_Template > ( std::ostream &, const Morse_Value_Cell & );

};

template < class Morse_Complex_Template >
class Morse_Value_Chain : public  Chain_Archetype < std::map < Morse_Value_Cell < Morse_Complex_Template >, typename Morse_Complex_Template::Cell_Complex_Type::Ring > > {
public:		
	typedef typename Morse_Complex_Template::Cell_Complex_Type Cell_Complex_Type;
	typedef typename Morse_Traits<Cell_Complex_Type>::morse_value_type Morse_Value_Type;
	typedef typename Cell_Complex_Type::Chain Original_Chain;
	typedef typename Cell_Complex_Type::Container::const_iterator Original_const_iterator;
	Morse_Value_Chain ( void );
	/* Copy an original chain into the Morse Value Chain format by
	 * fetching the Morse Values and storing iterators instead */
	Morse_Value_Chain ( const Original_Chain & copy_me, const Morse_Complex_Template & morse_complex );
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Morse_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Morse_Complex.cpp"
#endif

#endif
