/*
 *  Cell_Complex_Archetype.hpp
 *  
 *
 *  Created by Shaun Harker on 10/6/09.
 *
 */

/* * * * * * * * * * * * * * * * * * * * *
 * Cell_Complex_Archetype<> definitions  *
 * * * * * * * * * * * * * * * * * * * * */

template < class Container_Template >
Cell_Complex_Archetype<Container_Template>::Cell_Complex_Archetype ( void ) { }

template < class Container_Template >
Cell_Complex_Archetype<Container_Template>::~Cell_Complex_Archetype ( void ) { }

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::Chain & Cell_Complex_Archetype<Container_Template>::
Boundary_Map ( typename Cell_Complex_Archetype < Container_Template >::Chain & output_chain, const const_iterator & input ) const {
  return output_chain = boundary ( input );
} /* Boundary_Map */

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::Chain & Cell_Complex_Archetype<Container_Template>::
Boundary_Map ( typename Cell_Complex_Archetype < Container_Template >::Chain & output_chain, const typename Cell_Complex_Archetype < Container_Template >::Cell & cell ) const {
	return output_chain = boundary ( find ( cell ) ); 
} /* Boundary_Map */

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::Chain & Cell_Complex_Archetype<Container_Template>::
Boundary_Map ( typename Cell_Complex_Archetype < Container_Template >::Chain & output_chain, const typename Cell_Complex_Archetype < Container_Template >::Chain & input_chain ) const {
	output_chain . clear ();
	for ( typename Chain::const_iterator chain_term = input_chain . begin (); chain_term != input_chain . end (); ++ chain_term ) {
    Chain temporary_boundary;
		Boundary_Map ( temporary_boundary, chain_term -> first );
		temporary_boundary *= chain_term -> second;
		output_chain += temporary_boundary; 
  } /* for */
	return output_chain; 
} /* Boundary_Map */

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::Chain & Cell_Complex_Archetype<Container_Template>::
Coboundary_Map ( typename Cell_Complex_Archetype < Container_Template >::Chain & output_chain, const const_iterator & input ) const {
  return output_chain = coboundary ( input );
} /* Coboundary_Map */

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::Chain & Cell_Complex_Archetype<Container_Template>::
Coboundary_Map ( typename Cell_Complex_Archetype < Container_Template >::Chain & output_chain, const typename Cell_Complex_Archetype < Container_Template >::Cell & cell ) const {
	return output_chain = coboundary ( find ( cell ) ); 
} /* Coboundary_Map */

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::Chain & Cell_Complex_Archetype< Container_Template >::
Coboundary_Map ( typename Cell_Complex_Archetype < Container_Template >::Chain & output_chain, 
                const typename Cell_Complex_Archetype < Container_Template >::Chain & input_chain ) const {	
	output_chain . clear ();
	for ( typename Chain::const_iterator iter = input_chain . begin (); iter != input_chain . end (); ++ iter ) {
		Chain temporary_chain;
		Coboundary_Map ( temporary_chain, ( *iter ) . first );
		temporary_chain *= ( *iter ) . second; 
		output_chain += temporary_chain; 
  } /* for */
	return output_chain; 
} /* Coboundary_Map */

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::size_type
Cell_Complex_Archetype < Container_Template >::
Count_Boundaries ( const typename Cell_Complex_Archetype < Container_Template >::Cell & input_chain ) const {
	Chain temporary_chain; 
	return ( size_type ) Boundary_Map ( temporary_chain, input_chain ) . size (); 
} /* Count_Boundaries */

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::size_type
Cell_Complex_Archetype < Container_Template >::
Count_Coboundaries ( const typename Cell_Complex_Archetype < Container_Template >::Cell & input_chain ) const {	
	Chain temporary_chain; 
	return ( size_type ) Coboundary_Map ( temporary_chain, input_chain ) . size (); 
} /* Count_Coboundaries */

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::size_type
Cell_Complex_Archetype < Container_Template >::
Count_Boundaries ( const typename Container_Template::const_iterator & input) const {
	Chain temporary_chain; 
	return ( size_type ) Boundary_Map ( temporary_chain, input ) . size (); 
} /* Count_Boundaries */

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::size_type
Cell_Complex_Archetype < Container_Template >::
Count_Coboundaries ( const typename Container_Template::const_iterator & input ) const {	
	Chain temporary_chain; 
	return ( size_type ) Coboundary_Map ( temporary_chain, input ) . size (); 
} /* Count_Coboundaries */

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::size_type
Cell_Complex_Archetype < Container_Template >::
size ( void ) const {
	size_type return_value = 0;
	for ( unsigned int dimension_index = 0; dimension_index <= dimension (); ++ dimension_index ) 
		return_value += size ( dimension_index );
	return return_value; 
} /* size */

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::Container::const_iterator  Cell_Complex_Archetype < Container_Template >::
begin ( void ) const {
	return begin ( 0 );  
} /* begin */

template < class Container_Template >
typename Cell_Complex_Archetype < Container_Template >::Container::const_iterator  Cell_Complex_Archetype < Container_Template >::
end ( void ) const {
  return end ( dimension () );
} /* end */

