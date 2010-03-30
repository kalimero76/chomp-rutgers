/*
 *  Abstract_Complex.cpp
 *  
 *  Created by Shaun Harker on 10/6/09.
 *
 */

#include "complexes/Abstract_Complex.h"

Abstract_Complex::Chain & Abstract_Complex::Boundary_Map ( Abstract_Complex::Chain & output, const Abstract_Complex::Container::const_iterator & input) const {
	return ( output = Boundary_Map ( input ) ); }
	
Abstract_Complex::Chain & Abstract_Complex::Coboundary_Map ( Abstract_Complex::Chain & output, const Abstract_Complex::Container::const_iterator & input) const {
	return ( output = Coboundary_Map ( input ) ); }
	
void Abstract_Complex::Remove_Cell ( const Abstract_Complex::Cell & ) {
	std::cout << "Abstract_Complex::Remove_Cell NOT IMPLEMENTED.\n"; }
	
Abstract_Complex::Chain & Abstract_Complex::Boundary_Map ( const Abstract_Complex::Container::iterator & input ) {
	return cells [ input -> dimension ] . boundary_data [ *input ]; }

Abstract_Complex::Chain & Abstract_Complex::Coboundary_Map ( const Abstract_Complex::Container::iterator & input ) {
	return cells [ input -> dimension ] . coboundary_data [ *input ];  }

const Abstract_Complex::Chain & Abstract_Complex::Boundary_Map ( const Abstract_Complex::Container::const_iterator & input ) const {
	return cells [ input -> dimension ] . boundary_data . find ( *input ) -> second;  }

const Abstract_Complex::Chain & Abstract_Complex::Coboundary_Map ( const Abstract_Complex::Container::const_iterator & input ) const {
	return cells [ input -> dimension ] . coboundary_data . find ( *input ) -> second;  }
