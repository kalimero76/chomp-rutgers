/*
 * Chain_Archetype.hpp
 * CHomP
 * Shaun Harker
 *
 * This hpp file contains implementations for the chains of CHomP.
 *
 */ 

/* * * * * * * * * * * * * * * *
 * Chain_Archetype Definitions *
 * * * * * * * * * * * * * * * */

template < class Pair_Associative_Container > 
Chain_Archetype < Pair_Associative_Container > & Chain_Archetype < Pair_Associative_Container > ::
operator += ( const Chain_Archetype < Pair_Associative_Container > & right_hand_side ) {
	for ( typename Chain_Archetype < Pair_Associative_Container > :: const_iterator term = right_hand_side . begin ();
	term != right_hand_side . end (); ++ term ) this -> operator += ( *term );
	return *this; 
} /* Chain_Archetype<>::operator += */

template < class Pair_Associative_Container > 
Chain_Archetype < Pair_Associative_Container > & Chain_Archetype < Pair_Associative_Container > ::
operator -= ( const Chain_Archetype < Pair_Associative_Container > & right_hand_side ) {
	for ( typename Chain_Archetype < Pair_Associative_Container > :: const_iterator term = right_hand_side . begin ();
       term != right_hand_side . end (); ++ term ) this -> operator -= ( *term );
	return *this; 
} /* Chain_Archetype<>::operator -= */

template < class Pair_Associative_Container > 
Chain_Archetype < Pair_Associative_Container > & Chain_Archetype < Pair_Associative_Container > ::
operator += ( const typename Chain_Archetype < Pair_Associative_Container >::Chain_Term & term ) {
	typename Chain_Archetype < Pair_Associative_Container > :: iterator existing_term = find ( term . first );
	if ( existing_term == this -> end () ) {
        if ( term . second != 0 ) this -> insert ( term ); // no term? then introduce it, we are done.
	} else if ( ( existing_term -> second += term . second ) == 0 )	// side effect does the work... 
		this -> erase ( existing_term );									// and if term zeros out we erase it.
	return *this; 
} /* Chain_Archetype<>::operator += */

template < class Pair_Associative_Container > 
Chain_Archetype < Pair_Associative_Container > & Chain_Archetype < Pair_Associative_Container > ::
operator -= ( const typename Chain_Archetype < Pair_Associative_Container >::Chain_Term & term ) {
	typename Chain_Archetype < Pair_Associative_Container > :: iterator existing_term = find ( term . first );
	if ( existing_term == this -> end () ) {
    if ( term . second != 0 ) this -> insert ( term ); // no term? then introduce it, we are done.
	} else if ( ( existing_term -> second -= term . second ) == 0 )	// side effect does the work... 
		this -> erase ( existing_term );									// and if term zeros out we erase it.
	return *this; 
} /* Chain_Archetype<>::operator -= */

template < class Pair_Associative_Container > 
Chain_Archetype < Pair_Associative_Container > & Chain_Archetype < Pair_Associative_Container > ::
operator *= ( const Chain_Archetype <Pair_Associative_Container> :: Ring & right_hand_side ) {
	/* First check to see if we are multiplying by zero. */
	if ( right_hand_side == 0 ) { 
		/* We are multiplying by zero. Clear the chain and return. */
		this -> clear ();
		return *this; 
  } /* if */
	/* We are not multiplying by zero. Loop through the terms and multiply the coefficients. */
	for ( typename Chain_Archetype < Pair_Associative_Container > :: iterator term = this -> begin ();
	term != this -> end (); ++ term ) term -> second *= right_hand_side; 
	return *this; 
} /* Chain_Archetype<>::operator *= */

template < class Pair_Associative_Container > 
const typename Chain_Archetype<Pair_Associative_Container>::complex_type & Chain_Archetype<Pair_Associative_Container>::container () const {
  return * container_;
} /* Chain_Archetype<>::container */

template < class Pair_Associative_Container > 
template < class InputIterator >
Chain_Archetype<Pair_Associative_Container>::Chain_Archetype ( InputIterator first, InputIterator last, const typename Pair_Associative_Container::key_compare & comp, const complex_type & container ) 
: Pair_Associative_Container ( first, last, comp ), container_(&container) {
} /* Chain_Archetype<>::Chain_Archetype */

template < class Pair_Associative_Container > 
Chain_Archetype<Pair_Associative_Container>::Chain_Archetype ( const complex_type & container ) 
: container_(&container) {
} /* Chain_Archetype<>::Chain_Archetype */

template < class Pair_Associative_Container > 
Chain_Archetype<Pair_Associative_Container>::Chain_Archetype ( void ) {
} /* Chain_Archetype<>::Chain_Archetype */

template < class Pair_Associative_Container > 
std::ostream & operator << ( std::ostream & output_stream, const Chain_Archetype<Pair_Associative_Container> & print_me) {
	bool first_print = true;
	/* If chain is empty, output 0 and return */
	if ( print_me . size () == 0 ) {
		output_stream << "0";
		return output_stream; } /* if */
	/* The chain is not empty. Loop through the terms and print them. */
	for ( typename Chain_Archetype<Pair_Associative_Container>::const_iterator chain_term = print_me . begin (); 
	chain_term != print_me . end (); ++ chain_term ) {
		/* If this is not the very first term to be printed, print " + ". */
		if ( !first_print ) output_stream << " + "; else first_print = false;
		/* Print the term with its coefficient. */
		output_stream << chain_term  -> second << " * [" <<  * chain_term -> first << "]"; 
  } /* for */
	return output_stream; 
} /* operator << */
