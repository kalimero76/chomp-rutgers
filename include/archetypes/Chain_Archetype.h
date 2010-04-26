/*
 *  Chain_Archetype.h
 *  srh_chomp
 *
 *  Created by Shaun Harker on 9/14/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CHOMP_CHAIN_ARCHETYPE_
#define CHOMP_CHAIN_ARCHETYPE_

#include <ext/hash_map> /* for hash<> specialization */
namespace std { using namespace __gnu_cxx; }
#include <map> /* For map<...>, used by Default_Chain typedef */
#include <iostream> /* For ostream, used by output streaming overload for Default_Cell , also debug*/

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
  friend class __gnu_cxx::hash<Default_Cell>;
private:
  unsigned int dimension_;
  unsigned long data_;
};


// Forward Declaration of friend functions for Chain_Archetype
template < class Pair_Associative_Container > class Chain_Archetype;
template < class Pair_Associative_Container > std::ostream & operator << ( std::ostream &, const Chain_Archetype < Pair_Associative_Container> &);

template < class Pair_Associative_Container > 
class Chain_Archetype : public Pair_Associative_Container {
public:
	typedef typename Pair_Associative_Container::mapped_type Ring;	/* SGI's STL calls this data_type as well, but GCC doesn't dig it. */
	typedef typename Pair_Associative_Container::value_type Chain_Term;
	Chain_Archetype & operator += ( const Chain_Archetype & );
  Chain_Archetype & operator -= ( const Chain_Archetype & );
	Chain_Archetype & operator += ( const Chain_Term & );
  Chain_Archetype & operator -= ( const Chain_Term & );
	Chain_Archetype & operator *= ( const Ring & );
	friend std::ostream & operator << < Pair_Associative_Container > ( std::ostream &, const Chain_Archetype & );

	/* todo: should use an overloaded binary * not a member, but friended by class so we can multiply scalars from the left as users will want to do */
}; /* Chain_Archetype */

#ifndef CHOMP_LIBRARY_ONLY_
#include "archetypes/Chain_Archetype.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "archetypes/Chain_Archetype.cpp"
#endif

#endif
