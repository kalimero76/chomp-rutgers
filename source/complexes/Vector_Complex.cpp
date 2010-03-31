/*
 *  Vector_Complex.cpp
 *  
 *  Contains Library code.
 *
 *  Created by Shaun Harker on 1/5/10.
 *
 */

#include <iostream>
#include "complexes/Vector_Complex.h"

/* * * * * * * * * * * * * * * * * * * * * *
 *  Decomposition_Information Definitions  *
 * * * * * * * * * * * * * * * * * * * * * */

Decomposition_Information::Decomposition_Information ( void ) : flags(0) {};

/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Vector_Container::const_iterator Definitions   *
 * * * * * * * * * * * * * * * * * * * * * * * * * */

Vector_Container::const_iterator::const_iterator ( void ) {} /* Vector_Container::const_iterator::const_iterator */

Vector_Container::const_iterator::const_iterator ( const Vector_Container * const referral ) : referral(referral) 
{} /* Vector_Container::const_iterator::const_iterator */

Vector_Container::const_iterator::const_iterator ( const Vector_Container * const referral, const Vector_Container::Cell & dereference_value ) : referral(referral), dereference_value(dereference_value) 
{} /* Vector_Container::const_iterator::const_iterator */

Vector_Container::const_iterator::const_iterator ( const Vector_Container::iterator & convert_me ) : referral(convert_me . referral), dereference_value(convert_me . dereference_value) 
{} /* Vector_Container::const_iterator::const_iterator */

bool Vector_Container::const_iterator::operator != ( const Vector_Container::const_iterator & right_hand_side ) const {
    return dereference_value != * right_hand_side;
} /* Vector_Container::const_iterator::operator != */


const Vector_Container::value_type & Vector_Container::const_iterator::operator * ( void ) const {
    return dereference_value;
} /* Vector_Container::const_iterator::operator * */

const Vector_Container::value_type * Vector_Container::const_iterator::operator -> ( void ) const {
    return & dereference_value;
} /* Vector_Container::const_iterator::operator -> */

Vector_Container::const_iterator & Vector_Container::const_iterator::operator ++ ( void ) {
    while ( ( ++ dereference_value . name < referral -> end_index ) && (referral -> decomposition_data [ dereference_value . name ] . flags & Decomposition_Information::EXCISED ) );
    return *this;
} /* Vector_Container::const_iterator::operator ++ */

Vector_Container::const_iterator & Vector_Container::const_iterator::operator = ( const Vector_Container::iterator & right_hand_side ) {
    referral = right_hand_side . referral;
    dereference_value = right_hand_side . dereference_value;
    return *this;
} /* Vector_Container::const_iterator::operator = */

/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Vector_Container::iterator Definitions         *
 * * * * * * * * * * * * * * * * * * * * * * * * * */

Vector_Container::iterator::iterator ( void ) {} /* Vector_Container::iterator::iterator */

Vector_Container::iterator::iterator ( const Vector_Container * const referral ) : referral(referral) 
{} /* Vector_Container::iterator::iterator */

Vector_Container::iterator::iterator ( const Vector_Container * const referral, const Vector_Container::Cell & dereference_value ) : referral(referral), dereference_value(dereference_value) 
{} /* Vector_Container::iterator::iterator */

bool Vector_Container::iterator::operator != ( const Vector_Container::iterator & right_hand_side ) const {
    return dereference_value != * right_hand_side;
} /* Vector_Container::iterator::operator != */

const Vector_Container::value_type & Vector_Container::iterator::operator * ( void ) const {
    return dereference_value;
} /* Vector_Container::iterator::operator * */

const Vector_Container::value_type * Vector_Container::iterator::operator -> ( void ) const {
    return & dereference_value;
} /* Vector_Container::iterator::operator -> */

Vector_Container::iterator & Vector_Container::iterator::operator ++ ( void ) {
    while ( ( ++ dereference_value . name < referral -> end_index ) && (referral -> decomposition_data [ dereference_value . name ] . flags & Decomposition_Information::EXCISED ) );
    return *this;
} /* Vector_Container::iterator::operator ++ */

/* * * * * * * * * * * * * * * * * *
 *  Vector_Container Definitions   *
 * * * * * * * * * * * * * * * * * */

Vector_Container::size_type Vector_Container::size ( void ) const {
    return remembered_size;
} /* Vector_Container::size */

Vector_Container::const_iterator Vector_Container::find ( const Cell & find_me ) const {
    if ( ( find_me . name < begin_index ) || ( find_me . name >= end_index ) || ( decomposition_data [ find_me . name ] . flags & Decomposition_Information::EXCISED ) ) return end ();
    return const_iterator ( this, find_me );
} /* Vector_Container::find */

Vector_Container::const_iterator Vector_Container::begin ( void ) const {
    Cell begin_cell ( begin_index, dimension );
    return const_iterator ( this, begin_cell );
} /* Vector_Container::begin */

Vector_Container::const_iterator Vector_Container::end ( void ) const {
    Cell end_cell ( end_index, dimension );
    return const_iterator ( this, end_cell );
} /* Vector_Container::end */

Vector_Container::iterator Vector_Container::find ( const Cell & find_me ) {
    if ( ( find_me . name < begin_index ) || ( find_me . name >= end_index ) || ( decomposition_data [ find_me . name ] . flags & Decomposition_Information::EXCISED ) ) return end ();
    return iterator ( this, find_me );
} /* Vector_Container::find */

Vector_Container::iterator Vector_Container::begin ( void ) {
    Cell begin_cell ( begin_index, dimension );
    return iterator ( this, begin_cell );
} /* Vector_Container::begin */

Vector_Container::iterator Vector_Container::end ( void ) {
    Cell end_cell ( end_index, dimension );
    return iterator ( this, end_cell );
} /* Vector_Container::end */

void Vector_Container::resize ( Vector_Container::size_type new_size ) {
    decomposition_data . resize ( new_size );
    boundary_data . resize ( new_size );
    coboundary_data . resize ( new_size );
    end_index = new_size;
} /* Vector_Container::resize */

/* * * * * * * * * * * * * * * * *
 *  Vector_Complex Definitions   *
 * * * * * * * * * * * * * * * * */

Vector_Complex::Vector_Complex ( void ) {};

Vector_Complex::Chain & Vector_Complex::Boundary_Map ( Vector_Complex::Chain & output, const Vector_Complex::Container::const_iterator & input) const {
	return ( output = Boundary_Map ( input ) ); }
	
Vector_Complex::Chain & Vector_Complex::Coboundary_Map ( Vector_Complex::Chain & output, const Vector_Complex::Container::const_iterator & input) const {
	return ( output = Coboundary_Map ( input ) ); }
	
void Vector_Complex::Remove_Cell ( const Vector_Complex::Cell & remove_me ) {
#ifdef DEBUG
    /* If cell is already missing, do not remove it. Issue a warning. */
    if ( not (cells [ remove_me . dimension ] . find ( remove_me ) != cells [ remove_me . dimension ] . end ()) ) {
        std::cerr << "Warning. Vector_Complex::Remove_Cell tried to remove a cell not in the complex. \n";
        return;
    }
#endif
	/* Loop through the boundaries and remove mention of "remove_me" in their coboundaries */
	Chain & boundary_elements = cells [ remove_me . dimension ] . boundary_data [ remove_me . name ];
	for ( Chain::const_iterator chain_term = boundary_elements . begin (); chain_term != boundary_elements . end (); ++ chain_term )
		cells [ remove_me . dimension - 1 ] . coboundary_data [ chain_term -> first . name ] . erase ( remove_me );
	/* Loop through the coboundaries and remove mention of "remove_me" in their boundaries */
	Chain & coboundary_elements = cells [ remove_me . dimension ] . coboundary_data [ remove_me . name ];
	for ( Chain::const_iterator chain_term = coboundary_elements . begin (); chain_term != coboundary_elements . end (); ++ chain_term ) 
		cells [ remove_me . dimension + 1 ] . boundary_data [ chain_term -> first . name ] . erase ( remove_me );	
		
	/* Now clear the information */	
	boundary_elements . clear ();
	coboundary_elements . clear ();
	cells [ remove_me . dimension ] . decomposition_data [ remove_me . name ] . flags |= Decomposition_Information::EXCISED;

	/* Advance the beginning if necessary */
    if ( remove_me . name == cells [ remove_me . dimension ] . begin_index ) {
        const_iterator copy_cell_iterator ( & cells [ remove_me . dimension ] , remove_me );
        ++ copy_cell_iterator;
        cells [ remove_me . dimension ] . begin_index = copy_cell_iterator -> name;
    } /* if */

	/* Reduce the size by one */
	-- cells [ remove_me . dimension ] . remembered_size; 
	return; } /* endfunction */

Vector_Complex::Chain & Vector_Complex::Boundary_Map ( const Vector_Complex::Container::iterator & input ) {
	return cells [ input -> dimension ] . boundary_data [ input -> name ]; }

Vector_Complex::Chain & Vector_Complex::Coboundary_Map ( const Vector_Complex::Container::iterator & input ) {
	return cells [ input -> dimension ] . coboundary_data [ input -> name ];  }

const Vector_Complex::Chain & Vector_Complex::Boundary_Map ( const Vector_Complex::Container::const_iterator & input ) const {
	return cells [ input -> dimension ] . boundary_data [ input -> name ];  }

const Vector_Complex::Chain & Vector_Complex::Coboundary_Map ( const Vector_Complex::Container::const_iterator & input ) const {
	return cells [ input -> dimension ] . coboundary_data [ input -> name ];  }
		
/* Morse Traits */

Vector_Container::const_iterator & Vector_Complex::Husband ( const Vector_Container::const_iterator & input ) const {
	return cells [ input -> dimension ] . decomposition_data [ input -> name ] . husband; }

Morse_Traits<Vector_Complex>::morse_value_type & Vector_Complex::Morse_Value ( const Vector_Container::const_iterator & input) const {
	return cells [ input -> dimension ] . decomposition_data [ input -> name ] . morse_value;  }

unsigned char & Vector_Complex::Flags ( const Vector_Container::const_iterator & input ) const {
	return cells [ input -> dimension ] . decomposition_data [ input -> name ] . flags; }

#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */
#endif
