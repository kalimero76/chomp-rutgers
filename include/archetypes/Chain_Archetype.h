/*
 *  Chain_Archetype.h
 *  srh_chomp
 *
 *  Created by Shaun Harker on 9/14/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#ifndef CHOMP_CHAIN_ARCHETYPE_
#define CHOMP_CHAIN_ARCHETYPE_

#include <iostream> /* For ostream, used by output streaming overload for Default_Cell , also debug*/
#include "boost/functional/hash.hpp"

/* Declarations */
class Default_Cell;
size_t hash_value ( const Default_Cell & hash_me );
template < class > class Chain_Archetype;

/* * * * * * * * * * * * * * * * **
** Elementary Chains and Chains  **
** * * * * * * * * * * * * * * * */

typedef long Default_Ring;

class Default_Cell {
public:
	Default_Cell ( void );
	Default_Cell ( const unsigned long name, const unsigned int dimension);
	bool operator < ( const Default_Cell & right_hand_side ) const;
	bool operator == ( const Default_Cell & right_hand_side ) const;
	bool operator != ( const Default_Cell & right_hand_side ) const;
	friend std::ostream & operator << ( std::ostream & output_stream, const Default_Cell & print_me );
  unsigned long & data ( void );
  const unsigned long & data ( void ) const;
  unsigned int & dimension ( void );
  const unsigned int & dimension ( void ) const;
  
private:
  friend size_t hash_value ( const Default_Cell & hash_me );
  unsigned long data_;
  unsigned int dimension_;
};

inline size_t hash_value ( const Default_Cell & hash_me ) {
  boost::hash<unsigned long> hasher;
  return hasher ( hash_me . data_ ); 
} /* hash_value for Default_Cell */

// Forward Declaration of friend functions for Chain_Archetype
template < class Pair_Associative_Container > class Chain_Archetype;
template < class Pair_Associative_Container > std::ostream & operator << ( std::ostream &, const Chain_Archetype < Pair_Associative_Container> &);

template < class Pair_Associative_Container > 
class Chain_Archetype : public Pair_Associative_Container {
public:
  /* typedefs */
  typedef typename Pair_Associative_Container::key_type::complex_type complex_type;
	typedef typename Pair_Associative_Container::mapped_type Ring;	/* SGI's STL calls this data_type as well, but GCC doesn't. */
	typedef typename Pair_Associative_Container::value_type Chain_Term;
  /* Chain */
	Chain_Archetype & operator += ( const Chain_Archetype & );
  Chain_Archetype & operator -= ( const Chain_Archetype & );
	Chain_Archetype & operator += ( const Chain_Term & );
  Chain_Archetype & operator -= ( const Chain_Term & );
	Chain_Archetype & operator *= ( const Ring & );
	friend std::ostream & operator << < Pair_Associative_Container > ( std::ostream &, const Chain_Archetype & );
  const complex_type & container () const;
  template < class InputIterator >
  Chain_Archetype ( InputIterator first, InputIterator last, const typename Pair_Associative_Container::key_compare & comp, const complex_type & container );
  Chain_Archetype ( const complex_type & container );
  Chain_Archetype ( void );
private:
  const complex_type * container_;
}; /* Chain_Archetype */

#ifndef CHOMP_LIBRARY_ONLY_
#include "archetypes/Chain_Archetype.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "archetypes/Chain_Archetype.cpp"
#endif

#endif
