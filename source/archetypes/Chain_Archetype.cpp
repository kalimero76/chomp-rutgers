/*
 * Chain_Archetype.cpp
 * CHomP
 * Shaun Harker
 *
 * This cpp file contains template instantiations.
 *
 */ 

#include "archetypes/Chain_Archetype.h"

/*
 * Default_Cell Implementation 
 */

Default_Cell::Default_Cell ( void ) { 
	name = 0;
	dimension = 0; }

Default_Cell::Default_Cell ( const unsigned long name, const unsigned int dimension ) : name(name), dimension(dimension) { }

bool Default_Cell::operator < ( const Default_Cell & right_hand_side ) const {
	if ( dimension < right_hand_side . dimension ) return true;
	if ( dimension > right_hand_side . dimension ) return false;
	if ( name < right_hand_side . name ) return true;
	return false; } 
	
bool Default_Cell::operator == ( const Default_Cell & right_hand_side ) const {
	if ( dimension != right_hand_side . dimension ) return false;
	if ( name != right_hand_side . name ) return false;
	return true; }

bool Default_Cell::operator != ( const Default_Cell & right_hand_side ) const {
	if ( dimension != right_hand_side . dimension ) return true;
	if ( name != right_hand_side . name ) return true;
	return false; }

std::ostream & operator << (std::ostream & output_stream, const Default_Cell & print_me) {
	output_stream << "(" << print_me . name << ", " << print_me . dimension << ")";
	return output_stream; }
	
#ifndef _CHOMP_HEADER_ONLY_
/* Template Instantiation */
template class Chain_Archetype <  std::map < Default_Cell, Default_Ring > >;
template std::ostream & operator << < std::map <  Default_Cell, Default_Ring > > 
	( std::ostream & , const Chain_Archetype <  std::map < Default_Cell, Default_Ring > > & ) ;

#include "complexes/Morse_Complex.h"

#include "complexes/Cubical_Complex.h"
template class Chain_Archetype < std::map < Morse_Value_Cell < Morse_Complex <Cubical_Complex> >, Morse_Complex<Cubical_Complex>::Cell_Complex_Type::Ring > >;
template std::ostream & operator << < std::map < Morse_Value_Cell < Morse_Complex <Cubical_Complex> >, Morse_Complex<Cubical_Complex>::Cell_Complex_Type::Ring > > 
	( std::ostream & , const Chain_Archetype < std::map < Morse_Value_Cell < Morse_Complex <Cubical_Complex> >, Morse_Complex<Cubical_Complex>::Cell_Complex_Type::Ring > > & ) ;

#include "complexes/Abstract_Complex.h"
template class Chain_Archetype < std::map < Morse_Value_Cell < Morse_Complex <Abstract_Complex> >, Morse_Complex<Abstract_Complex>::Cell_Complex_Type::Ring > >;
template std::ostream & operator << < std::map < Morse_Value_Cell < Morse_Complex <Abstract_Complex> >, Morse_Complex<Abstract_Complex>::Cell_Complex_Type::Ring > > 
	( std::ostream & , const Chain_Archetype < std::map < Morse_Value_Cell < Morse_Complex <Abstract_Complex> >, Morse_Complex<Abstract_Complex>::Cell_Complex_Type::Ring > > & ) ;
#endif
