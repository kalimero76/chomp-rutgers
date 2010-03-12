/*
 * Chain_Archetype.hpp
 * CHomP
 * Shaun Harker
 *
 * This hpp file contains implementations for the chains of CHomP.
 *
 */ 

#include <map> /* For map<...> */

/*
 * Chain_Archetype Implementation
 */

template < class Pair_Associative_Container > 
Chain_Archetype < Pair_Associative_Container > & Chain_Archetype < Pair_Associative_Container > ::
operator += ( const Chain_Archetype < Pair_Associative_Container > & right_hand_side ) {
/* This might be made slightly better: as long as the rings are the same, we don't need the r.h.s. to have same implementation */
/* But for now we can safely assume that all chains getting added together are the same format */
	for ( typename Chain_Archetype < Pair_Associative_Container > :: const_iterator term = right_hand_side . begin ();
	term != right_hand_side . end (); term ++ ) this -> operator += ( *term );
	return *this; } /* endfunction */

template < class Pair_Associative_Container > 
Chain_Archetype < Pair_Associative_Container > & Chain_Archetype < Pair_Associative_Container > ::
operator += ( const typename Chain_Archetype < Pair_Associative_Container >::Chain_Term & term ) {
/* This might be made slightly better: as long as the rings are the same, we don't need the r.h.s. to have same implementation */
/* But for now we can safely assume that all chains getting added together are the same format */
	typename Chain_Archetype < Pair_Associative_Container > :: iterator existing_term = find ( term . first );
	if ( existing_term == this -> end () ) {
        if ( term . second != 0 ) this -> insert ( term ); // no term? then introduce it, we are done.
	} else if ( ( existing_term -> second += term . second ) == 0 )	// side effect does the work... 
		this -> erase ( existing_term );									// and if term zeros out we erase it.
	return *this; } /* endfunction */

template < class Pair_Associative_Container > 
Chain_Archetype < Pair_Associative_Container > & Chain_Archetype < Pair_Associative_Container > ::
operator *= ( const Chain_Archetype <Pair_Associative_Container> :: Ring & right_hand_side ) {
	/* First check to see if we are multiplying by zero. */
	if ( right_hand_side == 0 ) { 
		/* We are multiplying by zero. Clear the chain and return. */
		this -> clear ();
		return *this; }
	/* We are not multiplying by zero. Loop through the terms and multiply the coefficients. */
	for ( typename Chain_Archetype < Pair_Associative_Container > :: iterator term = this -> begin ();
	term != this -> end (); term ++ ) ( *term ) . second *= right_hand_side; 
	return *this; } /* endfunction */

template < class Pair_Associative_Container > 
std::ostream & operator << ( std::ostream & output_stream, const Chain_Archetype<Pair_Associative_Container> & print_me) {
	bool first_print = true;
	/* If chain is empty, output 0 and return */
	if ( print_me . size () == 0 ) {
		output_stream << "0";
		return output_stream; } /* if */
	/* The chain is not empty. Loop through the terms and print them. */
	for ( typename Chain_Archetype<Pair_Associative_Container>::const_iterator chain_term = print_me . begin (); 
	chain_term != print_me . end (); chain_term ++ ) {
		/* If this is not the very first term to be printed, print " + ". */
		if ( !first_print ) output_stream << " + "; else first_print = false;
		/* Print the term with its coefficient. */
		output_stream << chain_term  -> second << " * [" <<  chain_term -> first << "]"; } /* for */
	return output_stream; } /* endfunction */
	
/* should use an overloaded binary * not a member, but friended by class so we can multiply scalars from the left as users will want to do */
