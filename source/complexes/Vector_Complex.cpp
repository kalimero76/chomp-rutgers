/*
 *  Vector_Complex.cpp
 *  
 *  Contains Library code.
 *
 *  Created by Shaun Harker on 1/5/10.
 *
 */

#include "complexes/Vector_Complex.h"

/* decomposition_information implementation */

Decomposition_Information::Decomposition_Information ( void ) : flags(0) {};

/* container implementations */

Vector_Container::const_iterator Vector_Container::find ( const Vector_Container::Elementary_Chain & find_me ) const {
	return vector_type::begin () + find_me . name; } /* endfunction */

Vector_Container::size_type Vector_Container::size ( void ) const {
	return remembered_size; } /* endfunction */

Vector_Container::const_iterator Vector_Container::begin ( void ) const {
	return vector_type::begin () + beginning_index;  } /* endfunction */
	
/* complex implementations */

Vector_Complex::Vector_Complex ( void ) {};

Vector_Complex::Chain & Vector_Complex::Boundary_Map ( Vector_Complex::Chain & output, const Vector_Complex::Container::const_iterator & input) const {
	return ( output = Boundary_Map ( input ) ); }
	
Vector_Complex::Chain & Vector_Complex::Coboundary_Map ( Vector_Complex::Chain & output, const Vector_Complex::Container::const_iterator & input) const {
	return ( output = Coboundary_Map ( input ) ); }
	
void Vector_Complex::Remove_Elementary_Chain ( const Vector_Complex::Elementary_Chain & remove_me ) {
	/* Loop through the boundaries and remove mention of "remove_me" in their coboundaries */
	Chain & boundary_elements = Chain_Groups [ remove_me . dimension ] [ remove_me . name ] . second . second . first;
	for ( Chain::const_iterator chain_term = boundary_elements . begin (); chain_term != boundary_elements . end (); ++ chain_term )
		Chain_Groups [ remove_me . dimension - 1 ] [ chain_term -> first . name ] . second . second . second . erase ( remove_me );
	/* Loop through the coboundaries and remove mention of "remove_me" in their boundaries */
	Chain & coboundary_elements = Chain_Groups [ remove_me . dimension ] [ remove_me . name ] . second . second . second;
	for ( Chain::const_iterator chain_term = coboundary_elements . begin (); chain_term != coboundary_elements . end (); ++ chain_term ) 
		Chain_Groups [ remove_me . dimension + 1 ] [ chain_term -> first . name ] . second . second . first . erase ( remove_me );	
		

	/* Now clear the information */	
	boundary_elements . clear ();
	coboundary_elements . clear ();
	Chain_Groups [ remove_me . dimension ] [ remove_me . name ] . second . first . flags |= Decomposition_Information::EXCISED;

	/* Advance the beginning if necessary */
	unsigned long & beginning_index = Chain_Groups [ remove_me . dimension ] . beginning_index; 
	const Vector_Container & const_container = Chain_Groups [ remove_me . dimension ] ;
	while ( beginning_index < const_container . Vector_Container::vector_type::size () && ( const_container [ beginning_index ] . second . first . flags & Decomposition_Information::EXCISED ) ) {
			++ beginning_index ; }

	/* Reduce the size by one */
	-- Chain_Groups [ remove_me . dimension ] . remembered_size; 
	return; } /* endfunction */

Vector_Complex::Chain & Vector_Complex::Boundary_Map ( const Vector_Complex::Container::iterator & input ) {
	return input -> second . second . first; }

Vector_Complex::Chain & Vector_Complex::Coboundary_Map ( const Vector_Complex::Container::iterator & input ) {
	return input -> second . second . second; }

const Vector_Complex::Chain & Vector_Complex::Boundary_Map ( const Vector_Complex::Container::const_iterator & input ) const {
	return input -> second . second . first; }

const Vector_Complex::Chain & Vector_Complex::Coboundary_Map ( const Vector_Complex::Container::const_iterator & input ) const {
	return input -> second . second . second; }
		
/* morse decomposition interface implementations */

Vector_Container::const_iterator & Vector_Complex::Husband ( const Vector_Container::const_iterator & input ) const {
	return input -> second . first . husband; }

Morse_Traits<Vector_Complex>::morse_value_type & Vector_Complex::Morse_Value ( const Vector_Container::const_iterator & input) const {
	return input -> second . first . morse_value;  }

unsigned char & Vector_Complex::Flags ( const Vector_Container::const_iterator & input ) const {
	return input -> second . first . flags; }


