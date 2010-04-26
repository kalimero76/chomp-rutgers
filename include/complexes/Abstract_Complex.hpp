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

template < class Cell_Type >
Abstract_Container<Cell_Type>::Abstract_Container ( void ) {
} /* Abstract_Container<Cell_Type>::Abstract_Container */

template < class Cell_Type >
Abstract_Container<Cell_Type>::Abstract_Container ( unsigned int dimension ) : dimension_(dimension) {
} /* Abstract_Container<Cell_Type>::Abstract_Container */

template < class Cell_Type >
typename Abstract_Container<Cell_Type>::iterator Abstract_Container<Cell_Type>::find ( const key_type & find_me ) {
  return cells_ . find ( find_me );
} /* Abstract_Container<Cell_Type>::find */

template < class Cell_Type >
typename Abstract_Container<Cell_Type>::const_iterator Abstract_Container<Cell_Type>::find ( const key_type & find_me ) const {
  return cells_ . find ( find_me );
} /* Abstract_Container<Cell_Type>::find */

template < class Cell_Type >
std::pair<typename Abstract_Container<Cell_Type>::iterator, bool> Abstract_Container<Cell_Type>::insert ( const value_type & insert_me ) {
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
} /* Abstract_Container<Cell_Type>::insert */

template < class Cell_Type >
void Abstract_Container<Cell_Type>::erase ( iterator erase_me ) {
  /* TODO : NOT YET IMPLEMENTED. LOOK AT COBOUNDARY, BOUNDARY, FIX THE INFO, THEN ERASE. */
} /* Abstract_Container<Cell_Type>::erase */

template < class Cell_Type >
typename Abstract_Container<Cell_Type>::iterator Abstract_Container<Cell_Type>::begin ( unsigned int dimension ) const {
  return begin_ [ dimension ];
} /* Abstract_Container<Cell_Type>::begin */

template < class Cell_Type >
typename Abstract_Container<Cell_Type>::iterator Abstract_Container<Cell_Type>::end ( unsigned int dimension ) const {
  return end_ [ dimension ];
} /* Abstract_Container<Cell_Type>::end */

template < class Cell_Type >
typename Abstract_Container<Cell_Type>::size_type Abstract_Container<Cell_Type>::size ( unsigned int dimension ) const {
  return size_ [ dimension ];
} /* Abstract_Container<Cell_Type>::size */

template < class Cell_Type >
Abstract_Chain<Cell_Type> & Abstract_Container<Cell_Type>::boundary ( const iterator & input ) {
  return boundary_ [ *input ];
} /* Abstract_Container<Cell_Type>::boundary */

template < class Cell_Type >
const Abstract_Chain<Cell_Type> & Abstract_Container<Cell_Type>::boundary ( const const_iterator & input ) const {
  return boundary_ . find ( *input ) -> second;
} /* Abstract_Container<Cell_Type>::boundary */

template < class Cell_Type >
Abstract_Chain<Cell_Type> & Abstract_Container<Cell_Type>::coboundary ( const iterator & input ) {
  return coboundary_ [ *input ];
} /* Abstract_Container<Cell_Type>::coboundary */

template < class Cell_Type >
const Abstract_Chain<Cell_Type> & Abstract_Container<Cell_Type>::coboundary ( const const_iterator & input ) const {
  return coboundary_ . find ( *input ) -> second;
} /* Abstract_Container<Cell_Type>::coboundary */

template < class Cell_Type >
unsigned int Abstract_Container<Cell_Type>::dimension ( void ) const {
  return dimension_;
} /* Abstract_Container<Cell_Type>::dimension */


/* * * * * * * * * * * * * * * * *
 * Abstract_Complex definitions  *
 * * * * * * * * * * * * * * * * */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::Chain & Abstract_Complex<Cell_Type>::Boundary_Map ( const iterator & input ) {
  return boundary ( input ); 
} /* Abstract_Complex<Cell_Type>::Boundary_Map */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::Chain & Abstract_Complex<Cell_Type>::Coboundary_Map ( const iterator & input ) {
  return coboundary ( input );  
} /* Abstract_Complex<Cell_Type>::Coboundary_Map */

template < class Cell_Type >
const typename Abstract_Complex<Cell_Type>::Chain & Abstract_Complex<Cell_Type>::Boundary_Map ( const const_iterator & input ) const {
  return boundary ( input ); 
} /* Abstract_Complex<Cell_Type>::Boundary_Map */

template < class Cell_Type >
const typename Abstract_Complex<Cell_Type>::Chain & Abstract_Complex<Cell_Type>::Coboundary_Map ( const const_iterator & input ) const {
  return coboundary ( input ); 
} /* Abstract_Complex<Cell_Type>::Coboundary_Map */
