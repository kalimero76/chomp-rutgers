/*
 *  Abstract_Complex.hpp
 *  
 *
 *  Created by Shaun Harker on 10/6/09.
 *
 */

/* * * * * * * * * * * * * * * * * * *
 * Abstract_Container<> definitions  *
 * * * * * * * * * * * * * * * * * * */

template < class Chain_Type >
Abstract_Container<Chain_Type>::Abstract_Container ( void ) {
} /* Abstract_Container<Chain_Type>::Abstract_Container */

template < class Chain_Type >
Abstract_Container<Chain_Type>::Abstract_Container ( unsigned int dimension ) : dimension_(dimension) {
} /* Abstract_Container<Chain_Type>::Abstract_Container */

template < class Chain_Type >
typename Abstract_Container<Chain_Type>::iterator Abstract_Container<Chain_Type>::find ( const key_type & find_me ) {
  return cells_ . find ( find_me );
} /* Abstract_Container<Chain_Type>::find */

template < class Chain_Type >
typename Abstract_Container<Chain_Type>::const_iterator Abstract_Container<Chain_Type>::find ( const key_type & find_me ) const {
  return cells_ . find ( find_me );
} /* Abstract_Container<Chain_Type>::find */

template < class Chain_Type >
std::pair<typename Abstract_Container<Chain_Type>::iterator, bool> Abstract_Container<Chain_Type>::insert ( const value_type & insert_me ) {
  std::pair<iterator, bool> return_value = cells_ . insert ( insert_me );
  if ( not return_value . second ) return; 
  ++ size_ [ insert_me . dimension ];
  iterator next = return_value . first; 
  ++ next; 
  if ( next == begin_ [ insert_me . dimension ] ) {
    begin_ [ insert_me . dimension ] = return_value . first;
    if ( insert_me . dimension > 0 ) end_ [ insert_me . dimension - 1 ] = return_value . first;
  }
  return return_value;
} /* Abstract_Container<Chain_Type>::insert */

template < class Chain_Type >
void Abstract_Container<Chain_Type>::erase ( iterator erase_me ) {
  /* TODO : NOT YET IMPLEMENTED. LOOK AT COBOUNDARY, BOUNDARY, FIX THE INFO, THEN ERASE. */
} /* Abstract_Container<Chain_Type>::erase */

template < class Chain_Type >
typename Abstract_Container<Chain_Type>::iterator Abstract_Container<Chain_Type>::begin ( unsigned int dimension ) const {
  return begin_ [ dimension ];
} /* Abstract_Container<Chain_Type>::begin */

template < class Chain_Type >
typename Abstract_Container<Chain_Type>::iterator Abstract_Container<Chain_Type>::end ( unsigned int dimension ) const {
  return end_ [ dimension ];
} /* Abstract_Container<Chain_Type>::end */

template < class Chain_Type >
typename Abstract_Container<Chain_Type>::size_type Abstract_Container<Chain_Type>::size ( unsigned int dimension ) const {
  return size_ [ dimension ];
} /* Abstract_Container<Chain_Type>::size */

template < class Chain_Type >
Chain_Type & Abstract_Container<Chain_Type>::boundary ( const iterator & input ) {
  return boundary_ [ *input ];
} /* Abstract_Container<Chain_Type>::boundary */

template < class Chain_Type >
const Chain_Type & Abstract_Container<Chain_Type>::boundary ( const const_iterator & input ) const {
  return boundary_ . find ( *input ) -> second;
} /* Abstract_Container<Chain_Type>::boundary */

template < class Chain_Type >
Chain_Type & Abstract_Container<Chain_Type>::coboundary ( const iterator & input ) {
  return coboundary_ [ *input ];
} /* Abstract_Container<Chain_Type>::coboundary */

template < class Chain_Type >
const Chain_Type & Abstract_Container<Chain_Type>::coboundary ( const const_iterator & input ) const {
  return coboundary_ . find ( *input ) -> second;
} /* Abstract_Container<Chain_Type>::coboundary */

template < class Chain_Type >
unsigned int Abstract_Container<Chain_Type>::dimension ( void ) const {
  return dimension_;
} /* Abstract_Container<Chain_Type>::dimension */


/* * * * * * * * * * * * * * * * *
 * Abstract_Complex definitions  *
 * * * * * * * * * * * * * * * * */

template < class Chain_Type >
typename Abstract_Complex<Chain_Type>::Chain & Abstract_Complex<Chain_Type>::Boundary_Map ( const iterator & input ) {
  return boundary ( input ); 
} /* Abstract_Complex<Chain_Type>::Boundary_Map */

template < class Chain_Type >
typename Abstract_Complex<Chain_Type>::Chain & Abstract_Complex<Chain_Type>::Coboundary_Map ( const iterator & input ) {
  return coboundary ( input );  
} /* Abstract_Complex<Chain_Type>::Coboundary_Map */

template < class Chain_Type >
const typename Abstract_Complex<Chain_Type>::Chain & Abstract_Complex<Chain_Type>::Boundary_Map ( const const_iterator & input ) const {
  return boundary ( input ); 
} /* Abstract_Complex<Chain_Type>::Boundary_Map */

template < class Chain_Type >
const typename Abstract_Complex<Chain_Type>::Chain & Abstract_Complex<Chain_Type>::Coboundary_Map ( const const_iterator & input ) const {
  return coboundary ( input ); 
} /* Abstract_Complex<Chain_Type>::Coboundary_Map */
