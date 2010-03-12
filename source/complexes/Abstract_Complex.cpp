/*
 *  Abstract_Complex.cpp
 *  
 *  Created by Shaun Harker on 10/6/09.
 *
 */

#include "complexes/Abstract_Complex.h"

void Abstract_Complex::Load_From_File ( const char * FileName ) {
	std::cout << "Abstract_Complex::Load_From_File NOT IMPLEMENTED.\n"; }

Abstract_Complex::Chain & Abstract_Complex::Boundary_Map ( Abstract_Complex::Chain & output, const Abstract_Complex::Container::const_iterator & input) const {
	return ( output = Boundary_Map ( input ) ); }
	
Abstract_Complex::Chain & Abstract_Complex::Coboundary_Map ( Abstract_Complex::Chain & output, const Abstract_Complex::Container::const_iterator & input) const {
	return ( output = Coboundary_Map ( input ) ); }
	
void Abstract_Complex::Remove_Elementary_Chain ( const Abstract_Complex::Elementary_Chain & ) {
	std::cout << "Abstract_Complex::Remove_Elementary_Chain NOT IMPLEMENTED.\n"; }
	
Abstract_Complex::Chain & Abstract_Complex::Boundary_Map ( const Abstract_Complex::Container::iterator & input ) {
	return input -> second . first; }

Abstract_Complex::Chain & Abstract_Complex::Coboundary_Map ( const Abstract_Complex::Container::iterator & input ) {
	return input -> second . second; }

const Abstract_Complex::Chain & Abstract_Complex::Boundary_Map ( const Abstract_Complex::Container::const_iterator & input ) const {
	return input -> second . first; }

const Abstract_Complex::Chain & Abstract_Complex::Coboundary_Map ( const Abstract_Complex::Container::const_iterator & input ) const {
	return input -> second . second; }

template < class Cell_Complex_Archetype_Template >
void Abstract_Complex::Load_From_Cell_Complex_Archetype ( Cell_Complex_Archetype_Template & old_complex ) {
	std::cout << "Abstract_Complex::Load_From_Cell_Complex_Archetype NOT IMPLEMENTED.\n"; }

void Abstract_Complex::Geometric_Preprocessing ( void ) {}
