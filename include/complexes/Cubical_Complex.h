/*
 *  Cubical_Complex.h
 *
 *  Created by Shaun Harker on 9/14/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#ifndef CHOMP_CUBICAL_COMPLEX_
#define CHOMP_CUBICAL_COMPLEX_

#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for Cell_Complex */

/********************************************************************************
 *                             CUBICAL COMPLEXES                                *
 ********************************************************************************/

class Cubical_const_iterator;
class Cubical_Container;
class Cubical_Complex;

/* * * * * * * * * * * * * *
 * class Cubical_Container *
 * * * * * * * * * * * * * */

class Cubical_Container {
public:
	/* typedefs */	 
	typedef Default_Chain Chain;
	typedef Chain::Cell Cell;
	typedef Chain::Ring Ring;
	typedef unsigned long size_type;
	typedef Cell key_type;
	typedef Cell value_type;
  typedef Cubical_const_iterator const_iterator;
  typedef const_iterator iterator;
  /* Simple Associative Container, Unique Associative Container, Cell Container */
  std::pair<iterator, bool> insert ( const value_type & insert_me );
  void erase ( const iterator & erase_me );
  iterator begin ( unsigned int dimension ) const;
  iterator end ( unsigned int dimension ) const;
  iterator find ( const key_type & find_me ) const;
  size_type size ( unsigned int dimension ) const;
  Chain boundary ( const const_iterator & input ) const;
  Chain coboundary ( const const_iterator & input ) const;
  unsigned int dimension ( void ) const;
private:  
	friend class Cubical_const_iterator;
  friend class Cubical_Complex;
	unsigned int dimension_; 
	std::vector<unsigned int> dimension_sizes_; 
	std::vector<unsigned long> jump_values_; 
	std::vector<bool> bitmap_; 
  std::vector<const_iterator> begin_;
  std::vector<const_iterator> end_;
  std::vector<size_type> size_;
  unsigned long bitmap_size_;
};

/* * * * * * * * * * * * * * * * *
 * class Cubical_const_iterator  *
 * * * * * * * * * * * * * * * * */

class Cubical_const_iterator {
public:
  Cubical_const_iterator ( void );
  Cubical_const_iterator ( const Cubical_Container * const); 
  Cubical_const_iterator ( const Cubical_Container * const referral, const unsigned long address, const unsigned int dimension ); 
  Cubical_const_iterator & operator ++ ( void );
  bool operator != ( const Cubical_const_iterator & ) const;
  bool operator == ( const Cubical_const_iterator & ) const;
  Cubical_Container::value_type operator * ( void ) const; 
private:
  friend class Cubical_Container;
  friend class Cubical_Complex;
  const Cubical_Container * referral_;
  unsigned long address_;
  unsigned int dimension_;
  void next_type ( void ); 
};

/* * * * * * * * * * * * *
 * class Cubical_Complex *
 * * * * * * * * * * * * */

class Cubical_Complex : public Cell_Complex_Archetype < Cubical_Container > {
public:
  /** Load_From_File 
      Cubical format with full cubes. (n1, n2, ... ), all non-negative integers. */
	void Load_From_File ( const char * FileName );
	
  /** Insert_Elementary_Cell */
  void Insert_Elementary_Cell ( const unsigned long address  );
	
  /** Add_Full_Cube */
  void Add_Full_Cube ( const std::vector<unsigned int> & coordinates );
  
  /** Allocate_Bitmap
      Initializes bitmap so that the complex may contain a full cubical complex
      which full cubes (0,0,...,0) to (sizes[0] - 1, sizes[1] - 1, ... , sizes [ dimension - 1] - 1) */
	void Allocate_Bitmap ( const std::vector<unsigned int> & sizes );
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Cubical_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Cubical_Complex.cpp"
#endif

#endif
