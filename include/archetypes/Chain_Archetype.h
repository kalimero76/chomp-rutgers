/*
 *  Chain_Archetype.h
 *  srh_chomp
 *
 *  Created by Shaun Harker on 9/14/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CHOMP_CHAIN_ARCHETYPE_
#define _CHOMP_CHAIN_ARCHETYPE_

#include <ext/hash_map> /* for hash<> specialization */
namespace std { using namespace __gnu_cxx; }
#include <map> /* For map<...>, used by Default_Chain typedef */
#include <iostream> /* For ostream, used by output streaming overload for Default_Elementary_Chain , also debug*/

/* * * * * * * * * * * * * * * * **
** Elementary Chains and Chains  **
** * * * * * * * * * * * * * * * */

typedef long Default_Ring;

class Default_Elementary_Chain {
public:
	unsigned long name;
	unsigned int dimension;
	Default_Elementary_Chain ( void );
	Default_Elementary_Chain ( const unsigned long, const unsigned int );
	bool operator < ( const Default_Elementary_Chain & ) const;
	bool operator == ( const Default_Elementary_Chain & ) const;
	bool operator != ( const Default_Elementary_Chain & ) const;
	/** The arrow operator \verbatim -> \endverbatim is overloaded because the iterators of certain containers yield Default_Elementary_Chain objects when dereferenced. For example, see
	 * \see{Cubical_Container::const_iterator}.
	 * Since we would rather write \verbatim iterator -> name \endverbatim instead of \verbatim ( *iterator ) . name, \endverbatim we must overload the arrow operator. We should think that 
	 * this would be done in the iterator class, but it happens to be the case that the elementary chain is only stored implicitly, so dereferencing of
	 * the pointer creates a temporary object. Hence, we cannot overload the arrow operator of the iterator class to return the address of the default elementary 
	 * chain as it would be the address of a temporary object that is destroyed by the time we get it. So instead we pass the temporary object itself with the 
	 * overloading of the arrow operator, and then overload the arrow operator of Default_Elementary_Chain to give its own address. */
	Default_Elementary_Chain * operator -> ( void );
	const Default_Elementary_Chain * operator -> ( void ) const;
	friend std::ostream & operator << ( std::ostream &, const Default_Elementary_Chain & );
};

namespace __gnu_cxx {
template <>
struct hash<Default_Elementary_Chain> {
	//hash<unsigned long> internal_hasher;
	size_t operator () (const Default_Elementary_Chain & hash_me) const {
			//return internal_hasher ( hash_me . name ); 
			return hash_me . name;
	} 
};
} /* end namespace */

// Forward Declaration of friend functions for Chain_Archetype
template < class Pair_Associative_Container > class Chain_Archetype;
template < class Pair_Associative_Container > std::ostream & operator << ( std::ostream &, const Chain_Archetype < Pair_Associative_Container> &);

template < class Pair_Associative_Container > 
class Chain_Archetype : public Pair_Associative_Container {
public:
	typedef typename Pair_Associative_Container::key_type Elementary_Chain; 
	typedef typename Pair_Associative_Container::mapped_type Ring;	/* SGI's STL calls this data_type as well, but GCC doesn't dig it. */
	typedef typename Pair_Associative_Container::value_type Chain_Term;
	Chain_Archetype & operator += ( const Chain_Archetype & );
	Chain_Archetype & operator += ( const Chain_Term & );
	Chain_Archetype & operator *= ( const Ring & );
	friend std::ostream & operator << < Pair_Associative_Container > ( std::ostream &, const Chain_Archetype & );

	/* todo: should use an overloaded binary * not a member, but friended by class so we can multiply scalars from the left as users will want to do */
}; /* Chain_Archetype */
	
/* * * * * * * * * * * * * * * * * * * * * * * *
 * Default_Chain : An implementation of Chains *
 *		Follows Chain_Archetype interface            *
 * * * * * * * * * * * * * * * * * * * * * * * */
 
/** The default chain is the template specialization of the Chain_Archetype to the usage of STL maps with default elementary chain and
 * ring types. */
typedef Chain_Archetype <  std::map < Default_Elementary_Chain, Default_Ring > > Default_Chain;

#ifndef _CHOMP_LIBRARY_ONLY_
#include "archetypes/Chain_Archetype.hpp"
#endif

#ifdef _CHOMP_HEADER_ONLY_
#include "archetypes/Chain_Archetype.cpp"
#endif

#endif
