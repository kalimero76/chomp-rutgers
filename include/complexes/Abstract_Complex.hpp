/*
 *  Abstract_Complex.hpp
 *  
 *
 *  Created by Shaun Harker on 10/6/09.
 *
 */

template < class Chain_Type >
typename Abstract_Complex<Chain_Type>::Chain & Abstract_Complex<Chain_Type>::Boundary_Map ( Chain & output, const const_iterator & input) const {
  return ( output = Boundary_Map ( input ) ); 
}

template < class Chain_Type >
typename Abstract_Complex<Chain_Type>::Chain & Abstract_Complex<Chain_Type>::Coboundary_Map ( Chain & output, const const_iterator & input) const {
  return ( output = Coboundary_Map ( input ) ); 
}

template < class Chain_Type >
void Abstract_Complex<Chain_Type>::Remove_Cell ( const Abstract_Complex::Cell & ) {
  std::cout << "Abstract_Complex::Remove_Cell NOT IMPLEMENTED.\n"; exit (-1);
}

template < class Chain_Type >
typename Abstract_Complex<Chain_Type>::Chain & Abstract_Complex<Chain_Type>::Boundary_Map ( const iterator & input ) {
  return cells [ input -> dimension ] . boundary_data [ *input ]; 
}

template < class Chain_Type >
typename Abstract_Complex<Chain_Type>::Chain & Abstract_Complex<Chain_Type>::Coboundary_Map ( const iterator & input ) {
  return cells [ input -> dimension ] . coboundary_data [ *input ];  
}

template < class Chain_Type >
const typename Abstract_Complex<Chain_Type>::Chain & Abstract_Complex<Chain_Type>::Boundary_Map ( const const_iterator & input ) const {
  return cells [ input -> dimension ] . boundary_data . find ( *input ) -> second;  
}

template < class Chain_Type >
const typename Abstract_Complex<Chain_Type>::Chain & Abstract_Complex<Chain_Type>::Coboundary_Map ( const const_iterator & input ) const {
  return cells [ input -> dimension ] . coboundary_data . find ( *input ) -> second;  
}
