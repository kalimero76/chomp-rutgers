/*
 *  Abstract_Complex.hpp
 *  
 *  Created by Shaun Harker on 10/6/09.
 *
 */

#include "algorithms/Morse_Theory.h" /* for morse::decompose */

/* * * * * * * * * * * * * * * *
 * Abstract_Chain definitions  *
 * * * * * * * * * * * * * * * */

template < class Cell_Type >
Abstract_Chain<Cell_Type>::Abstract_Chain ( void  ) {
}

template < class Cell_Type >
Abstract_Chain<Cell_Type>::Abstract_Chain ( const complex_type & container ) 
: Chain_Archetype < std::map < Abstract_const_iterator<Cell_Type>, Default_Ring > > ( container ) {
}

/* * * * * * * * * * * * * * * * *
 * Abstract_Complex definitions  *
 * * * * * * * * * * * * * * * * */

template < class Cell_Type >
std::pair<typename Abstract_Complex<Cell_Type>::iterator, bool> Abstract_Complex<Cell_Type>::insert ( const value_type & insert_me ) {
  std::pair<typename std::set<Cell_Type>::iterator, bool> insert_value = cells_ . insert ( insert_me );
  std::pair<typename Abstract_Complex<Cell_Type>::iterator, bool> return_value ( typename Abstract_Complex<Cell_Type>::iterator ( this, insert_value . first, insert_me . dimension () ), insert_value . second );
  unsigned int dimension = insert_me . dimension ();
  if ( return_value . second ) { /* It really was inserted, wasn't already there. */
    /* Increment size */
    ++ size_ [ dimension ];
    /* Keep begin and end updated */
    iterator next = return_value . first; 
    ++ next; 
    if ( next == begin_ [ dimension ] ) {
      begin_ [ dimension ] = return_value . first;
      if ( dimension > 0 ) end_ [ dimension - 1 ] = return_value . first;
    } /* if */
  } /* if */
  return return_value;
} /* Abstract_Complex<Cell_Type>::insert */

template < class Cell_Type >
void Abstract_Complex<Cell_Type>::erase ( iterator erase_me ) {
  /* TODO : NOT YET IMPLEMENTED. LOOK AT COBOUNDARY, BOUNDARY, FIX THE INFO, THEN ERASE. */
} /* Abstract_Complex<Cell_Type>::erase */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::iterator Abstract_Complex<Cell_Type>::find ( const key_type & find_me ) {
  return typename Abstract_Complex<Cell_Type>::iterator ( this, cells_ . find ( find_me ), find_me . dimension () );
} /* Abstract_Complex<Cell_Type>::find */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::const_iterator Abstract_Complex<Cell_Type>::find ( const key_type & find_me ) const {
  return typename Abstract_Complex<Cell_Type>::iterator ( this, cells_ . find ( find_me ), find_me . dimension () );
} /* Abstract_Complex<Cell_Type>::find */

template < class Cell_Type > 
typename Abstract_Complex<Cell_Type>::iterator Abstract_Complex<Cell_Type>::begin ( void ) const {
	return begin_ [ 0 ];  
} /* Abstract_Complex<Cell_Type>::begin */

template < class Cell_Type > 
typename Abstract_Complex<Cell_Type>::iterator Abstract_Complex<Cell_Type>::end ( void ) const {
  return end_ [ dimension_ ];
} /* Abstract_Complex<Cell_Type>::end */

template < class Cell_Type > 
typename Abstract_Complex<Cell_Type>::size_type Abstract_Complex<Cell_Type>::size ( void ) const {
	size_type return_value = 0;
	for ( unsigned int dimension_index = 0; dimension_index <= dimension (); ++ dimension_index ) 
		return_value += size_ [ dimension_index ];
	return return_value; 
} /* Abstract_Complex<Cell_Type>::size */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::iterator Abstract_Complex<Cell_Type>::begin ( unsigned int dimension ) const {
  return begin_ [ dimension ];
} /* Abstract_Complex<Cell_Type>::begin */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::iterator Abstract_Complex<Cell_Type>::end ( unsigned int dimension ) const {
  return end_ [ dimension ];
} /* Abstract_Complex<Cell_Type>::end */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::size_type Abstract_Complex<Cell_Type>::size ( unsigned int dimension ) const {
  return size_ [ dimension ];
} /* Abstract_Complex<Cell_Type>::size */

template < class Cell_Type >
Abstract_Chain<Cell_Type> & Abstract_Complex<Cell_Type>::boundary ( const iterator & input ) {
  return boundary_ [ *input ];
} /* Abstract_Complex<Cell_Type>::boundary */

template < class Cell_Type >
const Abstract_Chain<Cell_Type> & Abstract_Complex<Cell_Type>::boundary ( const const_iterator & input ) const {
  return boundary_ . find ( *input ) -> second;
} /* Abstract_Complex<Cell_Type>::boundary */

template < class Cell_Type >
Abstract_Chain<Cell_Type> & Abstract_Complex<Cell_Type>::coboundary ( const iterator & input ) {
  return coboundary_ [ *input ];
} /* Abstract_Complex<Cell_Type>::coboundary */

template < class Cell_Type >
const Abstract_Chain<Cell_Type> & Abstract_Complex<Cell_Type>::coboundary ( const const_iterator & input ) const {
  return coboundary_ . find ( *input ) -> second;
} /* Abstract_Complex<Cell_Type>::coboundary */

template < class Cell_Type >
unsigned int Abstract_Complex<Cell_Type>::dimension ( void ) const {
  return dimension_;
} /* Abstract_Complex<Cell_Type>::dimension */

template < class Cell_Type >
void Abstract_Complex<Cell_Type>::index ( void ) {
  lookup_ . resize ( size () );
  unsigned long indx = 0;
  for ( const_iterator lookup = begin (); lookup != end (); ++ lookup, ++ indx ) { 
    index_ [ lookup ] = indx;
    lookup_ [ indx ] = lookup;
  } /* for */
} /* Abstract_Complex<Cell_Type>::index */

template < class Cell_Type >
unsigned long Abstract_Complex<Cell_Type>::index ( const const_iterator & lookup ) const {
  return index_ . find ( lookup ) -> second;
} /* Abstract_Complex<Cell_Type>::index */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::const_iterator Abstract_Complex<Cell_Type>::lookup ( unsigned long index ) const {
  return lookup_ [ index ];
} /* Abstract_Complex<Cell_Type>::lookup */

template < class Cell_Type >
void Abstract_Complex<Cell_Type>::decompose ( void ) {
  index (); 
  husband_ . resize ( size () );
  value_ . resize ( size () );
  flags_ . resize ( size () );
  morse::decompose ( *this );
} /*  Abstract_Complex<Cell_Type>::decompose */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::const_iterator & Abstract_Complex<Cell_Type>::husband ( const const_iterator & lookup ) {
  return husband_ [ index_ [ lookup ] ];
} /* Abstract_Complex<Cell_Type>::husband */

template < class Cell_Type >
const typename Abstract_Complex<Cell_Type>::const_iterator & Abstract_Complex<Cell_Type>::husband ( const const_iterator & lookup ) const {
  return husband_ [ index_ . find ( lookup ) -> second ];
} /* Abstract_Complex<Cell_Type>::husband */

template < class Cell_Type >
unsigned int & Abstract_Complex<Cell_Type>::value ( const const_iterator & lookup ) {
  return value_ [ index_ [ lookup ] ];
} /* Abstract_Complex<Cell_Type>::value */

template < class Cell_Type >
const unsigned int & Abstract_Complex<Cell_Type>::value ( const const_iterator & lookup ) const {
  return value_ [ index_ . find ( lookup ) -> second ];
} /* Abstract_Complex<Cell_Type>::value */

template < class Cell_Type >
unsigned char & Abstract_Complex<Cell_Type>::flags ( const const_iterator & lookup ) {
  return flags_ [ index_ [ lookup ] ];  
} /* Abstract_Complex<Cell_Type>::flags */

template < class Cell_Type >
const unsigned char & Abstract_Complex<Cell_Type>::flags ( const const_iterator & lookup ) const {
  return flags_ [ index_ . find ( lookup ) -> second ];  
} /* Abstract_Complex<Cell_Type>::flags */

template < class Cell_Type >
Abstract_Complex<Cell_Type>::Abstract_Complex ( void ) {
} /* Abstract_Complex<Cell_Type>::Abstract_Complex */

template < class Cell_Type >
Abstract_Complex<Cell_Type>::Abstract_Complex ( unsigned int dimension ) : dimension_(dimension) {
  const_iterator end_value ( this, cells_ . end (), dimension_ );
  begin_ . resize ( dimension_ + 1, end_value );
  end_ . resize ( dimension_ + 1, end_value );
  size_ . resize ( dimension_ + 1, 0 );
} /* Abstract_Complex<Cell_Type>::Abstract_Complex */

/* * * * * * * * * * * * * * * * * * * *
 * Abstract_const_iterator definitions  *
 * * * * * * * * * * * * * * * * * * * */

template < class Cell_Type > 
Abstract_const_iterator<Cell_Type>::Abstract_const_iterator ( void ) : container_(NULL), dimension_(0) { 
} /* Abstract_const_iterator::Abstract_const_iterator */

template < class Cell_Type > 
Abstract_const_iterator<Cell_Type>::Abstract_const_iterator ( const Abstract_Complex<Cell_Type> * const container, typename std::set<Cell_Type>::const_iterator data, const unsigned int dimension ) : 
container_(container), data_(data), dimension_(dimension) {
} /* Abstract_const_iterator::Abstract_const_iterator */

template < class Cell_Type > 
Cell_Type Abstract_const_iterator<Cell_Type>::operator * ( void ) const {
	return * data_; 
} /* Abstract_const_iterator::operator * */

template < class Cell_Type > 
const Cell_Type * Abstract_const_iterator<Cell_Type>::operator -> ( void ) const {
  return data_ . operator -> ();
} /* Abstract_const_iterator operator -> */

template < class Cell_Type > 
Abstract_const_iterator<Cell_Type> & Abstract_const_iterator<Cell_Type>::operator ++ ( void ) {
	++ data_;
  if ( dimension_ < container_ -> dimension_ && data_ == container_ -> begin_ [ dimension_ + 1 ] . data_ ) ++ dimension_;
  return *this;
} /* Abstract_const_iterator::operator ++ */

template < class Cell_Type > 
bool Abstract_const_iterator<Cell_Type>::operator != ( const Abstract_const_iterator & right_hand_side ) const {
	return data_ != right_hand_side . data_;
} /* Abstract_const_iterator::operator != */

template < class Cell_Type > 
bool Abstract_const_iterator<Cell_Type>::operator == ( const Abstract_const_iterator & right_hand_side ) const {
	return data_ == right_hand_side . data_;
} /* Abstract_const_iterator::operator == */

template < class Cell_Type > 
bool Abstract_const_iterator<Cell_Type>::operator < ( const Abstract_const_iterator & right_hand_side ) const {
  if ( dimension_ == right_hand_side . dimension_ ) 
    return * reinterpret_cast < const unsigned long * > ( & data_ ) < 
    * reinterpret_cast < const unsigned long * > ( & right_hand_side . data_ );
  else
    return dimension_ < right_hand_side . dimension_;
} /* Abstract_const_iterator::operator < */

template < class Cell_Type > 
unsigned int Abstract_const_iterator<Cell_Type>::dimension () const {
  return dimension_;
} /* Abstract_const_iterator::dimension */

template < class Cell_Type > 
const Abstract_Complex<Cell_Type> & Abstract_const_iterator<Cell_Type>::container () const {
  return container_;
} /* Abstract_const_iterator::container */
