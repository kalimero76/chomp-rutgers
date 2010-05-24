/*
 *  Adaptive_Cubical_Complex.h
 *
 *
 *  Created by Shaun Harker on 4/19/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef CHOMP_ADAPTIVE_COMPLEX_
#define CHOMP_ADAPTIVE_COMPLEX_

#include <deque>
#include <utility>

#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for Cell_Complex */

/* Forward Declarations */

/* * * * * * * * * * * * *
 * Adaptive_Cubical_Cell *
 * * * * * * * * * * * * */

/* * * * * * * * * * * * * *
 * Adaptive_Cubical_Chain  *
 * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * *
 * Adaptive_Cubical_Complex  *
 * * * * * * * * * * * * * * */

class Adaptive_Cubical_Complex {
public:
	/* typedefs */	 
  typedef unsigned int size_type; // for > 4E9 cubes, use unsigned long
	typedef Adaptive_Cubical_Chain Chain;
	typedef Adaptive_Cubical_Cell Cell;
	typedef Default_Ring Ring;
	typedef Cell key_type;
	typedef Cell value_type;
  typedef Cubical_const_iterator const_iterator;
  typedef const_iterator iterator;
  /* Basic Container */
  std::pair<iterator, bool> insert ( const value_type & insert_me );
  void erase ( const iterator & erase_me );
  void clear ( void );
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
  /* Index Complex */
  void index ( void );
  size_type index_begin ( unsigned int dimension ) const;
  size_type index_end ( unsigned int dimension ) const;
  size_type index ( const const_iterator & lookup ) const;
  size_type & index ( const const_iterator & lookup );
  std::vector < const_iterator > & lookup ( void );
  const const_iterator & lookup ( size_type index ) const;
  const_iterator & lookup ( size_type index );
  std::vector < int > count_all_boundaries ( void ) const;
  void boundary ( std::vector < size_type > & output, const size_type index ) const;
  void coboundary ( std::vector < size_type > & output, const size_type index ) const;
  void boundary ( std::vector < std::pair <size_type, Ring > > & output, const size_type input ) const;
  void coboundary ( std::vector < std::pair <size_type, Ring > > & output, const size_type input ) const;
  /* Decomposable Complex */
  void decompose ( void );
  char type ( size_type index, unsigned int dimension ) const;
  size_type mate ( size_type queen_index, unsigned int dimension ) const;
  const Ring & connection ( size_type queen_index ) const;
  Ring & connection ( size_type queen_index );  
  size_type ace_begin ( unsigned int dimension ) const;
  size_type ace_end ( unsigned int dimension ) const;
  void preprocess ( void );
  
  /* Adaptive Cubical Complex */
  
  
  
private:  
	friend class Cubical_const_iterator;
  /* Cell Complex */
  size_type total_size_;
  std::vector<const_iterator> begin_;
  const_iterator end_;
  std::vector<size_type> size_;
  unsigned int dimension_; 
  /* Index Complex */
  std::vector < size_type > index_;
  std::vector < const_iterator > lookup_;
  std::vector<size_type> index_begin_;
  /* Decomposable Complex */
  std::vector<Ring> connection_;
  std::vector< int > boundary_count_;
  std::vector<size_type> king_count_;
  /* Adaptive Cubical Complex */
	std::vector<bool> bitmap_;
};

/* * * * * * * * * * * * * * *
 * Adaptive_Cubical_iterator *
 * * * * * * * * * * * * * * */

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Adaptive_Cubical_Complex.cpp"
#endif

#endif
