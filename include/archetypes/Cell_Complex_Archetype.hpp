/*
 *  Cell_Complex_Archetype.hpp
 *  
 *
 *  Created by Shaun Harker on 10/6/09.
 *
 */

/*
 * Cell_Complex_Archetype Implementation 
 */
 
template < class Container_Template >
Cell_Complex_Archetype<Container_Template>::Cell_Complex_Archetype ( void ) { }

template < class Container_Template >
Cell_Complex_Archetype<Container_Template>::~Cell_Complex_Archetype ( void ) { }

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::Chain & Cell_Complex_Archetype<Container_Template>::
Boundary_Map ( typename Cell_Complex_Archetype < Container_Template >::Chain & output_chain, const typename Cell_Complex_Archetype < Container_Template >::Chain & input_chain ) const {
	output_chain . clear ();
	for ( typename Chain::const_iterator chain_term = input_chain . begin (); chain_term != input_chain . end (); ++ chain_term ) {
		Chain temporary_boundary;
		Boundary_Map ( temporary_boundary, chain_term -> first );
		temporary_boundary *= chain_term -> second;
		output_chain += temporary_boundary; }
	return output_chain; }

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::Chain & Cell_Complex_Archetype<Container_Template>::
Boundary_Map ( typename Cell_Complex_Archetype < Container_Template >::Chain & output_chain, const typename Cell_Complex_Archetype < Container_Template >::Cell & input_chain ) const {
	/* Since this is a const member function, cells is const, so "find" refers to the const version.
	 * Accordingly, a const_iterator is returned, which is the correct type for the user-supplied Boundary_Map */
	return Boundary_Map ( output_chain, cells [ input_chain . dimension ] . find ( input_chain ) ); }
	
template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::Chain & Cell_Complex_Archetype<Container_Template>::
Coboundary_Map ( typename Cell_Complex_Archetype < Container_Template >::Chain & output_chain, const typename Cell_Complex_Archetype < Container_Template >::Cell & input_chain ) const {
	/* Since this is a const member function, cells is const, so "find" refers to the const version.
	 * Accordingly, a const_iterator is returned, which is the correct type for the user-supplied Coboundary_Map */
	return Coboundary_Map ( output_chain, cells [ input_chain . dimension ] . find ( input_chain ) ); }
	
template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::Chain & Cell_Complex_Archetype< Container_Template >::
Coboundary_Map ( typename Cell_Complex_Archetype < Container_Template >::Chain & output_chain, 
const typename Cell_Complex_Archetype < Container_Template >::Chain & input_chain ) const {	
	output_chain . clear ();
	for ( typename Chain::const_iterator iter = input_chain . begin (); iter != input_chain . end (); ++ iter ) {
		Chain temporary_chain;
		Coboundary_Map ( temporary_chain, ( *iter ) . first );
		temporary_chain *= ( *iter ) . second ; 
		output_chain += temporary_chain; }
	return output_chain; }

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::size_type
Cell_Complex_Archetype < Container_Template >::
Count_Boundaries ( const typename Cell_Complex_Archetype < Container_Template >::Cell & input_chain ) const {
	Chain temporary_chain; 
	return ( size_type ) Boundary_Map( temporary_chain, input_chain ) . size (); } 

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::size_type
Cell_Complex_Archetype < Container_Template >::
Count_Coboundaries ( const typename Cell_Complex_Archetype < Container_Template >::Cell & input_chain ) const {	
	Chain temporary_chain; 
	return ( size_type ) Coboundary_Map( temporary_chain, input_chain ) . size (); } 

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::size_type
Cell_Complex_Archetype < Container_Template >::
Count_Boundaries ( const typename Container_Template::const_iterator & input) const {
	Chain temporary_chain; 
	return ( size_type ) Boundary_Map( temporary_chain, input ) . size (); } 

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::size_type
Cell_Complex_Archetype < Container_Template >::
Count_Coboundaries ( const typename Container_Template::const_iterator & input ) const {	
	Chain temporary_chain; 
	return ( size_type ) Coboundary_Map( temporary_chain, input ) . size (); } 
	
template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::size_type
Cell_Complex_Archetype < Container_Template >::
size ( void ) const {
	size_type return_value = 0;
	for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) 
		return_value += cells [ dimension_index ] . size ();
	return return_value; } /* endfunction */

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::Container::const_iterator  Cell_Complex_Archetype < Container_Template >::
find ( const Cell_Complex_Archetype<Container_Template>::Cell & find_me ) const {
	const Container & search_group = cells [ find_me . dimension ];
	return search_group . find ( find_me );  } /* endfunction */

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::Container::const_iterator  Cell_Complex_Archetype < Container_Template >::
begin ( void ) const {
	typename Container::const_iterator return_value;
	for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) {	
		const Container & search_group = cells [ dimension_index ];
		if ( (return_value = search_group . begin () ) != search_group . end () ) break; } /* for */
	return return_value;  } /* endfunction */
	
template < class Container_Template >
void Cell_Complex_Archetype < Container_Template >::
Geometric_Preprocessing ( void ) { /* trivial function */ }
