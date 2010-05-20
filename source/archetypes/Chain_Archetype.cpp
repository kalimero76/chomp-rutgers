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
	data_ = 0;
	dimension_ = 0; 
} /* Default_Cell::Default_Cell */

Default_Cell::Default_Cell ( const unsigned long data, const unsigned int dimension ) : data_(data), dimension_(dimension) { 
} /* Default_Cell::Default_Cell */

bool Default_Cell::operator < ( const Default_Cell & right_hand_side ) const {
	if ( dimension_ < right_hand_side . dimension_ ) return true;
	if ( dimension_ > right_hand_side . dimension_ ) return false;
	if ( data_ < right_hand_side . data_ ) return true;
	return false; 
} /* Default_Cell::operator < */
	
bool Default_Cell::operator == ( const Default_Cell & right_hand_side ) const {
	if ( dimension_ != right_hand_side . dimension_ ) return false;
	if ( data_ != right_hand_side . data_ ) return false;
	return true; 
} /* Default_Cell::operator == */

bool Default_Cell::operator != ( const Default_Cell & right_hand_side ) const {
	if ( dimension_ != right_hand_side . dimension_ ) return true;
	if ( data_ != right_hand_side . data_ ) return true;
	return false; 
} /* Default_Cell::operator != */

unsigned long & Default_Cell::data ( void ) {
  return data_;
} /* Default_Cell::data */

const unsigned long & Default_Cell::data ( void ) const {
  return data_;
} /* Default_Cell::data */

unsigned int & Default_Cell::dimension ( void ) {
  return dimension_;
} /* Default_Cell::dimension */

const unsigned int & Default_Cell::dimension ( void ) const {
  return dimension_;
} /* Default_Cell::dimension */

std::ostream & operator << ( std::ostream & output_stream, const Default_Cell & print_me ) {
	output_stream << "(" << print_me . data_ << ", " << print_me . dimension_ << ")";
	return output_stream; 
} /* operator << */
	
#if 0
#ifndef CHOMP_HEADER_ONLY_
/* Template Instantiation */
template class Chain_Archetype <  std::map < Default_Cell, Default_Ring > >;
template std::ostream & operator << < std::map <  Default_Cell, Default_Ring > > 
	( std::ostream & , const Chain_Archetype <  std::map < Default_Cell, Default_Ring > > & ) ;

#include "complexes/Morse_Complex.h"

#include "complexes/Cubical_Complex.h"
template class Chain_Archetype < std::map < Morse_Value_Cell < Morse_Complex <Cubical_Complex> >, Morse_Complex<Cubical_Complex>::Cell_Complex::Ring > >;
template std::ostream & operator << < std::map < Morse_Value_Cell < Morse_Complex <Cubical_Complex> >, Morse_Complex<Cubical_Complex>::Cell_Complex::Ring > > 
	( std::ostream & , const Chain_Archetype < std::map < Morse_Value_Cell < Morse_Complex <Cubical_Complex> >, Morse_Complex<Cubical_Complex>::Cell_Complex::Ring > > & ) ;

#include "complexes/Abstract_Complex.h"
template class Chain_Archetype < std::map < Morse_Value_Cell < Morse_Complex <Abstract_Complex <> > >, Morse_Complex<Abstract_Complex <> >::Cell_Complex::Ring > >;
template std::ostream & operator << < std::map < Morse_Value_Cell < Morse_Complex <Abstract_Complex <> > >, Morse_Complex<Abstract_Complex <> >::Cell_Complex::Ring > > 
	( std::ostream & , const Chain_Archetype < std::map < Morse_Value_Cell < Morse_Complex <Abstract_Complex <> > >, Morse_Complex<Abstract_Complex <> >::Cell_Complex::Ring > > & ) ;
#endif
#endif
