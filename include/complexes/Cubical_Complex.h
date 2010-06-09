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
#include <map>
#include <vector>
#include "boost/functional/hash.hpp"
#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for Cell_Complex */
#include "algorithms/Morse_Theory.h"

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

size_t hash_value ( const Cubical_const_iterator & hash_me );

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
  friend size_t hash_value ( const Cubical_const_iterator & hash_me );
  const Cubical_Complex * container_;
  unsigned long address_;
  unsigned int dimension_;
  void next_type ( void ); 
};

size_t hash_value ( const Cubical_const_iterator & hash_me ) {
  boost::hash < unsigned long > hasher;
  return hasher ( hash_me . address_ );
} /* hash_value */

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
  typedef unsigned int size_type; // for > 4E9 cubes, use unsigned long
	typedef Cubical_Chain Chain;
	typedef Cubical_Cell Cell;
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
  /* Cubical Complex */
  /** Load_From_File.
   Cubical format with full cubes. (n1, n2, ... ).
   It automatically translates the tuples so that the minimal entry for a given
   ordinate is set to 0. */
	void Load_From_File ( const char * FileName );
  
  /** Remove_Full_Cube.
   Remove the full cube (3^d cells) of coordinates. */
  void Remove_Full_Cube ( const std::vector<unsigned int> & cube_coordinates );

  /** Add_Full_Cube.
     Adds the full cube (3^d cells) of coordinates. if update == true, begin_ and size_ updated. */
  void Add_Full_Cube ( const std::vector<unsigned int> & coordinates, bool update = true );
  
  /** finalize. sets correct values to size_, total_size_, and begin_ from bitmap_ data */
  void finalize ( void );

  /** Allocate_Bitmap.
   Initializes bitmap so that the complex may contain a full cubical complex
   which full cubes (0,0,...,0) to (sizes[0] - 1, sizes[1] - 1, ... , sizes [ dimension - 1] - 1).
   Internally this is stored with \prod_i ( sizes[i]+1 ) * 2^d bits, using a lower left and upper right
   buffer for the following technical reasons:
      - the lower left buffer is so the coboundary routine doesn't need a special check to see if it is at 
      the edge of memory
      - the upper right buffer is to store the lower dimensional cells which may boundaries */
	void Allocate_Bitmap ( const std::vector<unsigned int> & sizes );
  
  const std::vector<unsigned int> & dimension_sizes ( void ) const; 

  void preprocess ( void );
  void coreductions ( void );
  void reductions ( void );

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
  bool boundary_count_available_;
  std::vector<size_type> king_count_;
  /* Cubical Complex */
  unsigned long bitmap_size_;
	std::vector<unsigned int> dimension_sizes_; 
	std::vector<unsigned long> jump_values_; 
  std::vector<unsigned long> types_;
  std::vector<unsigned long> types_inv_;
  std::vector<unsigned int> type_dims_;
  unsigned long mask_;
	std::vector<bool> bitmap_;
  bool closed_complex_;
  void native_boundary ( std::vector < unsigned long > & output, unsigned long address ) const;
  void native_coboundary ( std::vector < unsigned long > & output, unsigned long address ) const;
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Cubical_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Cubical_Complex.cpp"
#endif

#endif
