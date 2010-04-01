/* Product_Complex.hpp
 * CHomP project
 * Shaun Harker 3/31/10
 */

/* * * * * * * * * * * *
 * class Product_Cell  *
 * * * * * * * * * * * */

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Cell < First_Cell_Complex, Second_Cell_Complex > ::
Product_Cell ( void ) {}

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Cell < First_Cell_Complex, Second_Cell_Complex > ::
Product_Cell ( const typename First_Cell_Complex::Cell & first, 
              const typename Second_Cell_Complex::Cell & second ) 
: first(first), second(second) {}

template < class First_Cell_Complex, class Second_Cell_Complex >
bool Product_Cell < First_Cell_Complex, Second_Cell_Complex > ::
operator < ( const Product_Cell & right_hand_side ) const {
  if ( first < right_hand_side . first ) return true;
  if ( first != right_hand_side . first ) return false;
  if ( second < right_hand_side . second ) return true;
  return false;
}

template < class First_Cell_Complex, class Second_Cell_Complex >
bool Product_Cell < First_Cell_Complex, Second_Cell_Complex > ::
operator != ( const Product_Cell & right_hand_side ) const {
  if ( first != right_hand_side . first || second != right_hand_side . second ) return true;
  return false;
}

template < class First_Cell_Complex, class Second_Cell_Complex >
bool Product_Cell < First_Cell_Complex, Second_Cell_Complex > ::
operator == ( const Product_Cell & right_hand_side ) const {
  if ( first == right_hand_side . first && second == right_hand_side . second ) return true;
  return false;
}

template < class First_Cell_Complex, class Second_Cell_Complex >
std::ostream & operator << ( std::ostream & output_stream, const Product_Cell<First_Cell_Complex, Second_Cell_Complex> & print_me ) {
  return output_stream << "(" << print_me . first << " x " << print_me . second << ", " << print_me . dimension << ")";
}

/* * * * * * * * * * * * * *
 * class Product_Container *
 * * * * * * * * * * * * * */

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Container<First_Cell_Complex, Second_Cell_Complex>::
Product_Container ( const First_Cell_Complex & first_factor, const First_Cell_Complex & second_factor )
: first_factor(&first_factor), second_factor(&second_factor) {}

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Container<First_Cell_Complex, Second_Cell_Complex>::size_type 
Product_Container<First_Cell_Complex, Second_Cell_Complex>::
size ( void ) const {
  return remembered_size;
}

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Container<First_Cell_Complex, Second_Cell_Complex>::const_iterator 
Product_Container<First_Cell_Complex, Second_Cell_Complex>::
find ( const Cell & find_me ) const {
  typename First_Cell_Complex::const_iterator first_iterator = first_factor -> find ( find_me . first );
  typename Second_Cell_Complex::const_iterator second_iterator = second_factor -> find (find_me . second );
  return const_iterator ( this, first_iterator, second_iterator );
}

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Container<First_Cell_Complex, Second_Cell_Complex>::const_iterator 
Product_Container<First_Cell_Complex, Second_Cell_Complex>::
begin ( void ) const {
  return const_iterator ( this, first_factor -> begin (), second_factor -> begin () );
}

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Container<First_Cell_Complex, Second_Cell_Complex>::const_iterator 
Product_Container<First_Cell_Complex, Second_Cell_Complex>::
end ( void ) const {
  return const_iterator ( this, first_factor -> end (), second_factor -> end () );
}

/*
template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Container<First_Cell_Complex, Second_Cell_Complex>::iterator 
Product_Container<First_Cell_Complex, Second_Cell_Complex>::
find ( const Product_Container<First_Cell_Complex, Second_Cell_Complex>::Cell & find_me ) {
  typename First_Cell_Complex::iterator first_iterator = first_factor . find ( find_me . first );
  typename Second_Cell_Complex::iterator second_iterator = second_factor . find (find_me . second );
  return iterator ( this, first_iterator, second_iterator );
}

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Container<First_Cell_Complex, Second_Cell_Complex>::iterator 
Product_Container<First_Cell_Complex, Second_Cell_Complex>::
begin ( void ) {
  return iterator ( this, first_factor . begin (), second_factor . begin () );
}

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Container<First_Cell_Complex, Second_Cell_Complex>::iterator 
Product_Container<First_Cell_Complex, Second_Cell_Complex>::
end ( void ) {
  return iterator ( this, first_factor . end (), second_factor . end () );
}
*/

/* * * * * * * * * * * * * * * * * * *
 * Product_Container::const_iterator *
 * * * * * * * * * * * * * * * * * * */

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Container<First_Cell_Complex, Second_Cell_Complex>::const_iterator::
const_iterator ( void ) { }

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Container<First_Cell_Complex, Second_Cell_Complex>::const_iterator::
const_iterator ( const Product_Container * const referral ) : referral(referral) {}

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Container<First_Cell_Complex, Second_Cell_Complex>::const_iterator::
const_iterator ( const Product_Container * const referral, const typename First_Cell_Complex::const_iterator & first,
                const typename Second_Cell_Complex::const_iterator & second )
: referral(referral), first(first), second(second) {}

/*
template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Container<First_Cell_Complex, Second_Cell_Complex>::const_iterator::
const_iterator ( const iterator & convert_me ) {
  referral = convert_me . referral;
  first = convert_me . first;
  second = convert_me . second;
}
*/

template < class First_Cell_Complex, class Second_Cell_Complex >
bool Product_Container<First_Cell_Complex, Second_Cell_Complex>::const_iterator::
operator != ( const const_iterator & right_hand_side ) const {
  if ( first != right_hand_side . first || second != right_hand_side . second ) return true;
  return false;
}

template < class First_Cell_Complex, class Second_Cell_Complex >
const typename Product_Container<First_Cell_Complex, Second_Cell_Complex>::value_type & 
Product_Container<First_Cell_Complex, Second_Cell_Complex>::const_iterator::
operator * ( void ) const {
  dereference_value . first = * first;
  dereference_value . second = * second;
  dereference_value . dimension = first -> dimension + second -> dimension;
  return dereference_value;
}

template < class First_Cell_Complex, class Second_Cell_Complex >
const typename Product_Container<First_Cell_Complex, Second_Cell_Complex>::value_type * 
Product_Container<First_Cell_Complex, Second_Cell_Complex>::const_iterator::
operator -> ( void ) const {
  return & * * this;
}

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Container<First_Cell_Complex, Second_Cell_Complex>::const_iterator & 
Product_Container<First_Cell_Complex, Second_Cell_Complex>::const_iterator::
operator ++ ( void ) {
  /* Iteration pattern.
   We iterate through several types of product cells of types (K, L). A cell
   of type (K, L) has first factor of dimension K and second factor of dimension L.
   We iterate with fixed dimension N, so K+L = N it maintained. Hence we do types
   ( K, L ), (K+1, L-1), ... (K+i, L-i) where either K+i = dim first_factor or 
   L-i = 0.
   
   For each type, we iterate in dictionary order. The scheme described, taken together,
   is in fact dictionary order iteration through all product cells of dimension N, provided
   that the < operator of the parent Cell definitions respect dimensionality in the obvious
   manner.
   */
  const unsigned int first_dimension = first -> dimension;
  const unsigned int second_dimension = second -> dimension;
  ++ second;
  if ( second == referral -> second_factor -> cells [ second_dimension ] . end () ) ++ first;
  else return * this; /* Advanced one within type to (same, next) */
  
  if ( first == referral -> first_factor -> cells [ first_dimension ] . end () ) {
    if ( second_dimension == 0 || first_dimension == first_factor . dimension ) {
      return * this; /* Advanced to end() */
    } else {
      first = referral -> first_factor -> cells [ first_dimension + 1 ] . begin ();
      second = second_factor -> cells [ second_dimension - 1 ] . begin ();
      return * this; /* Advanced to beginning of next type. */
    }
  } else {
    second = referral -> second_factor -> cells [ second_dimension ] . begin ();
    return * this; /* Advance one within type to form (next, begin) */
  }
}

/*
template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Container<First_Cell_Complex, Second_Cell_Complex>::const_iterator & 
Product_Container<First_Cell_Complex, Second_Cell_Complex>::const_iterator::
operator = ( const iterator & right_hand_side ) {
  referral = right_hand_side . referral;
  first = right_hand_side . first;
  second = right_hand_side . second;
}
*/

/* * * * * * * * * * * * * 
 * class Product_Complex *
 * * * * * * * * * * * * */
template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
Product_Complex ( const First_Cell_Complex & first_factor, const Second_Cell_Complex & second_factor ) :
first_factor(first_factor), second_factor(second_factor) {
  /* Allocate the containers. */
  dimension = first_factor . dimension + second_factor . dimension;
  cells . resize ( dimension + 1, Product_Container<First_Cell_Complex, Second_Cell_Complex> ( first_factor, second_factor ) );
  for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) {
    cells [ dimension_index ] . dimension = dimension_index; 
    cells [ dimension_index ] . remembered_size = 0;
  }
  /* Store the sizes of the containers */
  for ( unsigned int first_dimension_index = 0; first_dimension_index <= first_factor . dimension; ++ first_dimension_index ) {
    for ( unsigned int second_dimension_index = 0; second_dimension_index <= second_factor . dimension; ++ second_dimension_index ) {
      cells [ first_dimension_index + second_dimension_index ] . remembered_size +=
      first_factor . cells [ first_dimension_index ] . size () *
      second_factor . cells [ second_dimension_index ] . size ();
    }
  }
}

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Chain < First_Cell_Complex, Second_Cell_Complex >::Chain_Term
Product_Complex<First_Cell_Complex, Second_Cell_Complex > ::
tensor_product ( const typename First_Cell_Complex::Cell & first_cell, const typename Second_Cell_Complex::Cell & second_cell ) const {
  return typename Chain::Chain_Term ( Product_Cell < First_Cell_Complex, Second_Cell_Complex> ( first_cell, second_cell ), 1 );  
}

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Chain < First_Cell_Complex, Second_Cell_Complex >
Product_Complex<First_Cell_Complex, Second_Cell_Complex > ::
tensor_product ( const typename First_Cell_Complex::Chain & first_chain, const typename Second_Cell_Complex::Cell & second_cell ) const {
  Chain return_value;
  for ( typename First_Cell_Complex::Chain::const_iterator first_chain_term = first_chain . begin ();
       first_chain_term != first_chain . end (); ++ first_chain_term )
    return_value += typename Chain::Chain_Term ( tensor_product ( first_chain_term -> first, second_cell ) . first , first_chain_term -> second );
  return return_value;
}

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Chain < First_Cell_Complex, Second_Cell_Complex > 
Product_Complex<First_Cell_Complex, Second_Cell_Complex > ::
tensor_product ( const typename First_Cell_Complex::Cell & first_cell, const typename Second_Cell_Complex::Chain & second_chain ) const {
  Chain return_value;
  for ( typename Second_Cell_Complex::Chain::const_iterator second_chain_term = second_chain . begin ();
       second_chain_term != second_chain . end (); ++ second_chain_term )
    return_value += typename Chain::Chain_Term ( tensor_product ( first_cell, second_chain_term -> first ) . first , second_chain_term -> second );
  return return_value;
}

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::Chain & 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
Boundary_Map ( Chain & boundary, const const_iterator & cell_iterator) const {
  /* produce boundary via formula d(a x b) = da x b +- a x db */
  /* Obtain da =: first_boundary, db =: second_boundary */
  typename First_Cell_Complex::Chain first_boundary; 
  first_factor . Boundary_Map ( first_boundary, cell_iterator . first );
  typename Second_Cell_Complex::Chain second_boundary; 
  second_factor . Boundary_Map ( second_boundary, cell_iterator . second );
  /* Construct boundary */
  const typename First_Cell_Complex::Cell & first_cell = * cell_iterator . first;
  const typename Second_Cell_Complex::Cell & second_cell = * cell_iterator . second;
  //boundary += 
  tensor_product ( first_boundary, second_cell );
  if ( cell_iterator . first -> dimension & 1 ) boundary -= tensor_product ( first_cell, second_boundary );
  else boundary += tensor_product ( first_cell, second_boundary );
  return boundary;
}

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::Chain & 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
Coboundary_Map ( Chain & coboundary, const const_iterator & cell_iterator ) const {
  /* produce coboundary via formula d'(a x b) = d'a x b +- a x d'b */
  /* Obtain da =: first_boundary, db =: second_boundary */
  typename First_Cell_Complex::Chain first_coboundary; 
  first_factor . Coboundary_Map ( first_coboundary, cell_iterator . first );
  typename Second_Cell_Complex::Chain second_coboundary; 
  second_factor . Coboundary_Map ( second_coboundary, cell_iterator . second );
  
  /* Construct coboundary */
  const typename First_Cell_Complex::Cell & first_cell = * cell_iterator . first;
  const typename Second_Cell_Complex::Cell & second_cell = * cell_iterator . second;
  coboundary += tensor_product ( first_coboundary, second_cell );
  if ( cell_iterator . first -> dimension & 1 ) coboundary -= tensor_product ( first_cell, second_coboundary );
  else coboundary += tensor_product ( first_cell, second_coboundary );
  return coboundary;
}
