/*
 *  Decomplex.hpp
 *
 *  Created by Shaun Harker on 10/21/09.
 *
 */

#include <deque> /* for deque in AKQ algorithm */
#include <set> /* for set in AKQ algorithm */
#include <functional> /* for greater<> in morse value orderings */
#include <iostream>

#include <time.h>

/* Decomplex Class */

template < class Cell_Complex >
Decomplex<Cell_Complex>::Decomplex ( const Cell_Complex & cell_complex ) : Cell_Complex(cell_complex) {
  unsigned int count = 0;
  for ( const_iterator iter = begin (); iter != end (); ++ iter ) index_ [ iter ] = count ++; 
  husband_ . resize ( count );
  value_ . resize ( count );
  flags_ . resize ( count );
} /* Decomplex<>::Decomplex */

template < class Cell_Complex > typename Decomplex<Cell_Complex>::const_iterator & 
Decomplex<Cell_Complex>::husband ( const const_iterator & input ) {
  return husband_ [ index_ [ input ] ];
} /* Decomplex<>::husband */

template < class Cell_Complex > const typename Decomplex<Cell_Complex>::const_iterator & 
Decomplex<Cell_Complex>::husband ( const const_iterator & input ) const {
  return husband_ [ index_ . find ( input ) -> second ];
} /* Decomplex<>::husband */

template < class Cell_Complex > unsigned int & 
Decomplex<Cell_Complex>::value ( const const_iterator & input ) {
  return value_ [ index_ [ input ] ];
} /* Decomplex<>::value */

template < class Cell_Complex > const unsigned int & 
Decomplex<Cell_Complex>::value ( const const_iterator & input ) const {
  return value_ [ index_ . find ( input ) -> second ];
} /* Decomplex<>::value */

template < class Cell_Complex > unsigned char & 
Decomplex<Cell_Complex>::flags ( const const_iterator & input ) {
  return flags_ [ index_ [ input ] ];  
} /* Decomplex<>::flags */

template < class Cell_Complex > const unsigned char & 
Decomplex<Cell_Complex>::flags ( const const_iterator & input ) const {
  return flags_ [ index_ . find ( input ) -> second ];  
} /* Decomplex<>::flags */
