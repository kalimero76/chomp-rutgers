/*
 *  Cubical_Complex.h
 *
 *  Created by Shaun Harker on 9/14/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#ifndef CHOMP_CUBICAL_COMPLEX_
#define CHOMP_CUBICAL_COMPLEX_

#include <iostream>
#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for Cell_Complex */

/********************************************************************************
 *                             CUBICAL COMPLEXES                                *
 ********************************************************************************/

/* forward declarations */
class Cubical_Complex;
class Cubical_const_iterator;

/* * * * * * * * * * * * *
 * typedef Cubical_Cell  *
 * * * * * * * * * * * * */

typedef Default_Cell Cubical_Cell;

/* * * * * * * * * * * * * * * * *
 * class Cubical_const_iterator  *
 * * * * * * * * * * * * * * * * */

class Cubical_const_iterator {
public:
  typedef Cubical_Complex complex_type;
  typedef Cubical_Cell Cell;
  Cubical_const_iterator ( void );
  Cubical_const_iterator ( const Cubical_Complex * const); 
  Cubical_const_iterator ( const Cubical_Complex * const referral, const unsigned long address, const unsigned int dimension ); 
  Cubical_const_iterator & operator ++ ( void );
  bool operator != ( const Cubical_const_iterator & right_hand_side ) const;
  bool operator == ( const Cubical_const_iterator & right_hand_side ) const;
  bool operator < ( const Cubical_const_iterator & right_hand_side ) const;
  Cubical_Cell operator * ( void ) const; 
  unsigned int dimension () const;
  const Cubical_Complex & container () const;
private:
  friend std::ostream & operator << ( std::ostream & output_stream, const Cubical_const_iterator & print_me);
  friend class Cubical_Complex;
  const Cubical_Complex * container_;
  unsigned long address_;
  unsigned int dimension_;
  void next_type ( void ); 
};

/* * * * * * * * * * * * *
 * typedef Cubical_Chain *
 * * * * * * * * * * * * */

typedef Chain_Archetype < std::map < Cubical_const_iterator, Default_Ring > > Cubical_Chain;

/* * * * * * * * * * * * *
 * class Cubical_Complex *
 * * * * * * * * * * * * */

class Cubical_Complex {
public:
	/* typedefs */	 
	typedef Cubical_Chain Chain;
	typedef Cubical_Cell Cell;
	typedef Default_Ring Ring;
	typedef unsigned long size_type;
	typedef Cell key_type;
	typedef Cell value_type;
  typedef Cubical_const_iterator const_iterator;
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
  void initialize_for_decomposition ( void );
	const_iterator & husband ( const const_iterator & );
  const const_iterator & husband ( const const_iterator & ) const;
	unsigned int & value ( const const_iterator & );
  const unsigned int & value ( const const_iterator & ) const;
	unsigned char & flags ( const const_iterator & );
  const unsigned char & flags ( const const_iterator & ) const;
  /* Cubical Complex */
  /** Load_From_File.
   Cubical format with full cubes. (n1, n2, ... ), all non-negative integers. */
	void Load_From_File ( const char * FileName );
  
  /** Add_Full_Cube. */
  void Add_Full_Cube ( const std::vector<unsigned int> & coordinates );
  
  /** Allocate_Bitmap.
   Initializes bitmap so that the complex may contain a full cubical complex
   which full cubes (0,0,...,0) to (sizes[0] - 1, sizes[1] - 1, ... , sizes [ dimension - 1] - 1) */
	void Allocate_Bitmap ( const std::vector<unsigned int> & sizes );
  
private:  
	friend class Cubical_const_iterator;
	unsigned int dimension_; 
	std::vector<unsigned int> dimension_sizes_; 
	std::vector<unsigned long> jump_values_; 
	std::vector<bool> bitmap_; 
  std::vector<const_iterator> begin_;
  std::vector<const_iterator> end_;
  std::vector<size_type> size_;
  unsigned long bitmap_size_;
  std::map < const_iterator, unsigned int > index_;
  std::vector < const_iterator > husband_;
	std::vector < unsigned int > value_;
	std::vector < unsigned char > flags_;
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Cubical_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Cubical_Complex.cpp"
#endif

#endif
