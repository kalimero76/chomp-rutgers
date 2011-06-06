/*
 *  Simplicial_Complex.h
 *
 *  Created by Shaun Harker on 4/26/11.
 *  Copyright 2011. All rights reserved.
 *
 */

#ifndef CHOMP_SIMPLICIAL_COMPLEX_
#define CHOMP_SIMPLICIAL_COMPLEX_

#include <iostream>
#include <map>
#include <vector>

#include "boost/unordered_map.hpp"


#include "boost/functional/hash.hpp"
#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for Cell_Complex */
#include "algorithms/Morse_Theory.h"

/********************************************************************************
 *                             Simplicial COMPLEXES                                *
 ********************************************************************************/

/* forward declarations */
class Simplicial_Complex;
class Simplicial_const_iterator;

/* * * * * * * * * * * * *
 * typedef Simplicial_Cell  *
 * * * * * * * * * * * * */

typedef std::vector<unsigned int> Simplicial_Cell;
typedef Simplicial_Cell Simplex;

/* * * * * * * * * * * * * * * * *
 * class Simplicial_const_iterator  *
 * * * * * * * * * * * * * * * * */

size_t hash_value ( const Simplicial_const_iterator & hash_me );

class Simplicial_const_iterator {
public:
  typedef Simplicial_Complex complex_type;
  typedef unsigned int size_type; // should refer to Simplicial_Complex::size_type, but compiler throws

  typedef Simplicial_Cell Cell;
  Simplicial_const_iterator ( void );
  Simplicial_const_iterator ( const Simplicial_Complex * const); 
  Simplicial_const_iterator ( const Simplicial_Complex * const referral, const unsigned long & handle ); 
  Simplicial_const_iterator & operator ++ ( void );
  bool operator != ( const Simplicial_const_iterator & right_hand_side ) const;
  bool operator == ( const Simplicial_const_iterator & right_hand_side ) const;
  bool operator < ( const Simplicial_const_iterator & right_hand_side ) const;
  Simplicial_Cell operator * ( void ) const; 
  unsigned int dimension () const;
  const Simplicial_Complex & container () const;

  const Simplicial_Complex * container_;
  unsigned long handle_;

private:
  friend std::ostream & operator << ( std::ostream & output_stream, const Simplicial_const_iterator & print_me);
  friend class Simplicial_Complex;
  friend size_t hash_value ( const Simplicial_const_iterator & hash_me );
};

inline size_t hash_value ( const Simplicial_const_iterator & hash_me ) {
  boost::hash < unsigned long > hasher;
  return hasher ( hash_me . handle_ );
} /* hash_value */

/* * * * * * * * * * * * *
 * typedef Simplicial_Chain *
 * * * * * * * * * * * * */

typedef Chain_Archetype < std::map < Simplicial_const_iterator, Default_Ring > > Simplicial_Chain;

/* * * * * * * * * * * * *
 * class Simplicial_Complex *
 * * * * * * * * * * * * */

class Simplicial_Complex {
public:
	/* typedefs */	 
  typedef unsigned int size_type; // for > 4E9 cubes, use unsigned long
	typedef Simplicial_Chain Chain;
	typedef Simplicial_Cell Cell;
	typedef Default_Ring Ring;
	typedef Cell key_type;
	typedef Cell value_type;
  typedef Simplicial_const_iterator const_iterator;
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
  /* Simplicial Complex */
  Simplicial_Complex ( void );
  Simplicial_Complex ( const char * FileName );

  /** Load_From_File.
   Simplicial format with full cubes. (n1, n2, ... ).
   It automatically translates the tuples so that the minimal entry for a given
   ordinate is set to 0. */
	void Load_From_File ( const char * FileName );
  
  /** Remove_Full_Simplex.
   Remove the full simplex. */
  void Remove_Full_Simplex ( const std::vector<unsigned int> & simplex );

  /** Add_Full_Simplex.
     Adds the full simplex. if update == true, begin_ and size_ updated. */
  void Add_Full_Simplex ( const std::vector<unsigned int> & simplex );
  
private:  
	friend class Simplicial_const_iterator;
  /* Cell Complex */
  size_type total_size_;
  /* Index Complex */
  boost::unordered_map < const_iterator, size_type, boost::hash < const_iterator > > index_;
  std::vector < const_iterator > lookup_;
  std::vector<size_type> index_begin_;
  /* Decomposable Complex */
  std::vector<Ring> connection_;
  std::vector< int > boundary_count_;
  bool boundary_count_available_;
  std::vector<size_type> king_count_;
  /* Simplicial Complex */
  boost::unordered_map < size_type, unsigned long > vertex_hashes_;
  boost::unordered_map < unsigned long, size_type > smallest_vertex_;
  boost::unordered_map < unsigned long, std::pair < size_type, size_type > > handle_to_fixed_index_;
  std::vector < std::vector < unsigned long > > fixed_index_to_handle_;
  unsigned long simplex_to_handle_ ( const std::vector<unsigned int> & simplex ) const;

};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Simplicial_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Simplicial_Complex.cpp"
#endif

#endif
