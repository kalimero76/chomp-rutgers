/*
 *  Vector_Complex.h
 *  
 *  Created by Shaun Harker on 1/5/10.
 *
 */

/* problems.
 uses morse traits "excised" to determine missing indices and
 implement Remove_Cell. This currently causes no problems, but
 could lead to bugs if Morse Complex is rewritten. In general,
 this situation shouldn't be handled this way:
 current plan:  
 * Rewrite Morse Complex to use class Subcomplex for decomposition.
 * Rewrite Vector Complex to support Remove_Cell in a different manner,
 probably through the inclusion of std::vector<bool> bitmap variable.
 
 There is also a potential problem when trying to make a vector complex from
 a comples that does not use default cells.
 */

#ifndef CHOMP_VECTOR_COMPLEX_
#define CHOMP_VECTOR_COMPLEX_

#include <map> /* for map */
#include <utility> /* for pair */
#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for class Cell_Complex_Archetype */
#include "complexes/Morse_Complex.h"  /* for Morse_Traits */

/********************************************************************************
 *                            VECTOR COMPLEXES                                  *
 ********************************************************************************/

/* Forward declarations */
class Decomposition_Information;
class Vector_Container;
class Vector_Complex;

/* * * * * * * * * * * * * * 
 * class Vector_Container  *
 * * * * * * * * * * * * * */

class Vector_Container {
public:
	/* Cell Container types */	 
	typedef Default_Chain Chain;
	typedef Chain::Cell Cell;
	typedef Chain::Ring Ring;
  
  /* Simple Associative Container types */
	typedef Cell key_type;
	typedef Cell value_type;
  typedef unsigned long size_type;
	
  /* Member Variables */
  unsigned int dimension;
  size_type remembered_size;
	size_type begin_index;
  size_type end_index;
  std::vector < Decomposition_Information > decomposition_data;
  std::vector < Chain > boundary_data;
  std::vector < Chain > coboundary_data;
  
  /* Forward Declarations */
  class const_iterator;
  class iterator;
  
  /* Member Functions */
  size_type size ( void ) const;
	const_iterator find ( const Cell & find_me ) const;
	const_iterator begin ( void ) const;
  const_iterator end ( void ) const;
	iterator find ( const Cell & find_me );
	iterator begin ( void );
  iterator end ( void );
  void resize ( size_type new_size );
  
  /* Nested Classes */
  
  /* * * * * * * * * * * * * * * * * * *
   * Vector_Container::const_iterator  *
   * * * * * * * * * * * * * * * * * * */
  
	class const_iterator {
    const Vector_Container * referral;
    mutable Vector_Container::Cell dereference_value;
  public:
		const_iterator ( void );
		const_iterator ( const Vector_Container * const referral ); 
    const_iterator ( const Vector_Container * const referral, const Cell & dereference_value );
    const_iterator ( const iterator & convert_me );
		bool operator != ( const const_iterator & right_hand_side ) const;        
		const Vector_Container::value_type & operator * ( void ) const; 
		const Vector_Container::value_type * operator -> ( void ) const;    
    const_iterator & operator ++ ( void );
    const_iterator & operator = ( const iterator & right_hand_side );
  };
  
  /* * * * * * * * * * * * * * * *
   * Vector_Container::iterator  *
   * * * * * * * * * * * * * * * */
  
	class iterator {
    friend class Vector_Container::const_iterator;
    const Vector_Container * referral;
    mutable Vector_Container::Cell dereference_value;
  public:
		iterator ( void );
		iterator ( const Vector_Container * const referral ); 
    iterator ( const Vector_Container * const referral, const Cell & dereference_value ); 
		bool operator != ( const iterator & right_hand_side ) const;        
		const Vector_Container::value_type & operator * ( void ) const; 
		const Vector_Container::value_type * operator -> ( void ) const;    
    iterator & operator ++ ( void );
  };
};

/* * * * * * * * * * * * * * * * * * *
 * class Decomposition_Information   *
 * * * * * * * * * * * * * * * * * * */

class Decomposition_Information {
public:
	Decomposition_Information ( void );
	mutable unsigned char flags; 
	enum { ACE = 0x01, KING = 0x02, QUEEN = 0x04, ALIVE = 0x08, EXCISED = 0x10 };
	mutable Vector_Container::const_iterator husband;
	mutable unsigned int morse_value; 
};

/* * * * * * * * * * * * * 
 * class Vector_Complex  *
 * * * * * * * * * * * * */

class Vector_Complex : public Cell_Complex_Archetype < Vector_Container > {
public:
	/* See Cell_Complex_Archetype */
	using Cell_Complex_Archetype<Vector_Container>::Boundary_Map; 
	using Cell_Complex_Archetype<Vector_Container>::Coboundary_Map;
	
	/* Constructors */
	Vector_Complex ( void );
	
	template < class Cell_Complex_Template >
	Vector_Complex ( Cell_Complex_Template & original_complex );
	
	/* * * * * * * * * * * * * *
	 * Pure Virtual Functions  *
	 * * * * * * * * * * * * * */
	
	/** Returns a copy of the Boundary information. This is only a copy, so subsequently altering this chain does not alter the complex directly. */
	virtual Chain & Boundary_Map ( Chain &, const Container::const_iterator & ) const;
	/** Returns a copy of the Boundary information. This is only a copy, so subsequently altering this chain does not alter the complex directly. */
	virtual Chain & Coboundary_Map ( Chain &, const Container::const_iterator & ) const;
	/** Remove an elementary chain from the complex. All terms from all chains involving this elementary chain will be deleted. NOT IMPLEMENTED. */
	virtual void Remove_Cell ( const Cell & );
	
	/* * * * * * *
	 * Features  *
	 * * * * * * */
	
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
	
  /* Morse Traits */
	Vector_Container::const_iterator & Husband ( const Vector_Container::const_iterator & input ) const;
	unsigned int & Morse_Value ( const Vector_Container::const_iterator & input) const;
	unsigned char & Flags ( const Vector_Container::const_iterator & input ) const;
};


template <>
class Morse_Traits <Vector_Complex> {
public:
	/* Tags for Morse_Traits for feature checking */
	struct yes {};
  struct no {};
  
	/* Default Tag Choices */
	typedef yes does_it_store_husband_pointers;
	typedef yes does_it_store_morse_values;
	typedef yes does_it_store_flags;
	/* Default Data Types */
	typedef unsigned int morse_value_type;
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Vector_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Vector_Complex.cpp"
#endif

#endif
