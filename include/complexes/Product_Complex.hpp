/* Product_Complex.hpp
 * CHomP project
 * Shaun Harker 3/31/10
 */

/* * * * * * * * * * * *
 * class Product_Cell  *
 * * * * * * * * * * * */

template < class First_Cell_Complex, class Second_Cell_Complex >
std::ostream & operator << ( std::ostream & output_stream, const Product_Cell<First_Cell_Complex, Second_Cell_Complex> & print_me ) {
  return output_stream << "(" << print_me . first_ << " x " << print_me . second_ << ", " << print_me . dimension_ << ")";
} /* operator << for Product_Cell<> */

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Cell < First_Cell_Complex, Second_Cell_Complex > ::
Product_Cell ( void ) {
} /* Product_Cell<>::Product_Cell */

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Cell < First_Cell_Complex, Second_Cell_Complex > ::
Product_Cell ( const typename First_Cell_Complex::Cell & first, 
              const typename Second_Cell_Complex::Cell & second ) 
: first_(first), second_(second), dimension_(first . dimension () + second . dimension () ) {
} /* Product_Cell<>::Product_Cell */

template < class First_Cell_Complex, class Second_Cell_Complex >
bool Product_Cell < First_Cell_Complex, Second_Cell_Complex > ::
operator < ( const Product_Cell & right_hand_side ) const {
  if ( first_ < right_hand_side . first_ ) return true;
  if ( first_ != right_hand_side . first_ ) return false;
  if ( second_ < right_hand_side . second_ ) return true;
  return false;
} /* Product_Cell<>::operator < */

template < class First_Cell_Complex, class Second_Cell_Complex >
bool Product_Cell < First_Cell_Complex, Second_Cell_Complex > ::
operator != ( const Product_Cell & right_hand_side ) const {
  if ( first_ != right_hand_side . first_ || second_ != right_hand_side . second_ ) return true;
  return false;
} /* Product_Cell<>::operator != */

template < class First_Cell_Complex, class Second_Cell_Complex >
bool Product_Cell < First_Cell_Complex, Second_Cell_Complex > ::
operator == ( const Product_Cell & right_hand_side ) const {
  if ( first_ == right_hand_side . first_ && second_ == right_hand_side . second_ ) return true;
  return false;
} /* Product_Cell<>::operator == */

template < class First_Cell_Complex, class Second_Cell_Complex >
unsigned int Product_Cell < First_Cell_Complex, Second_Cell_Complex > ::
dimension ( void ) const {
  return dimension_;
} /* Product_Cell<>::dimension */

/* * * * * * * * * * * * *
 * class Product_Complex *
 * * * * * * * * * * * * */

/* Basic Implicit Container */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::const_iterator 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
find ( const Cell & find_me ) const {
  typename First_Cell_Complex::const_iterator first_iterator = first_factor -> find ( find_me . first_ );
  typename Second_Cell_Complex::const_iterator second_iterator = second_factor -> find (find_me . second_ );
  return const_iterator ( this, first_iterator, second_iterator );
} /* Product_Complex<>::find */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::const_iterator 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
begin ( void ) const {
  return begin_ [ 0 ];
} /* Product_Complex<>::begin */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::const_iterator 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
end ( void ) const {
  return end_;
} /* Product_Complex<>::end */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::size_type 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
size ( void ) const {
  return total_size_;
} /* Product_Complex<>::size */

/* Cell Complex */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::const_iterator 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
begin ( unsigned int dimension ) const {
  return begin_ [ dimension ];
} /* Product_Complex<>::begin */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::const_iterator 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
end ( unsigned int dimension ) const {
  return begin_ [ dimension + 1 ];
} /* Product_Complex<>::end */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::size_type 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
size ( unsigned int dimension ) const {
  return size_ [ dimension ];
} /* Product_Complex<>::size */


template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::Chain 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
boundary ( const const_iterator & cell_iterator) const {
  Chain return_value;
  /* produce boundary via formula d(a x b) = da x b +- a x db */
  /* Obtain da =: first_boundary, db =: second_boundary */
  typename First_Cell_Complex::Chain first_boundary = 
    first_factor -> boundary ( cell_iterator . first_ );
  typename Second_Cell_Complex::Chain second_boundary = 
    second_factor -> boundary ( cell_iterator . second_ );
  /* Construct boundary */
  return_value += tensor_product ( first_boundary, cell_iterator . second_ );
  if ( cell_iterator . first_ . dimension () & 1 ) {
    return_value -= tensor_product ( cell_iterator . first_, second_boundary );
  } else {
    return_value += tensor_product ( cell_iterator . first_, second_boundary );
  } /* if-else */
  return return_value;
} /* Product_Complex<>::boundary */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::Chain
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
coboundary ( const const_iterator & cell_iterator ) const {
  Chain return_value;
  /* produce coboundary via formula d'(a x b) = d'a x b +- a x d'b */
  /* Obtain da =: first_boundary, db =: second_boundary */
  typename First_Cell_Complex::Chain first_coboundary =
    first_factor -> coboundary ( cell_iterator . first_ );
  typename Second_Cell_Complex::Chain second_coboundary =
    second_factor -> coboundary ( cell_iterator . second_ );
  /* Construct coboundary */
  return_value += tensor_product ( first_coboundary, cell_iterator . second_ );
  if ( cell_iterator . first_ . dimension () & 1 ) {
    return_value -= tensor_product ( cell_iterator . first_, second_coboundary );
  } else {
    return_value += tensor_product ( cell_iterator . first_, second_coboundary );
  } /* if-else */
  return return_value;
} /* Product_Complex<>::coboundary */

template < class First_Cell_Complex, class Second_Cell_Complex >
unsigned int Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
dimension ( void ) const {
  return dimension_;
} /* Product_Complex<>::dimension */
 
/* Index Complex */

template < class First_Cell_Complex, class Second_Cell_Complex >
void Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
index ( void ) {
  lookup_ . resize ( size () + 1 );
  connection_ . resize ( size (), 0 );
  size_type cell_index = 0;
  for ( const_iterator cell_iterator = begin (); cell_iterator != end (); ++ cell_iterator, ++ cell_index ) {
    index_ [ cell_iterator ] = cell_index;
    lookup_ [ cell_index ] = cell_iterator;
  } /* for */
  index_ [ end_ ] = total_size_;
  lookup_ [ total_size_ ] = end_;
  index_begin_ . resize ( dimension_ + 2, 0 );
  size_type sum = 0;
  for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) {
    sum += size_ [ dimension_index ];
    index_begin_ [ dimension_index + 1 ] = sum;
  } /* for */
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::index */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::size_type 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
index_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ]; 
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::index_begin */ 

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::size_type 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
index_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ];
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::index_end */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::size_type 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
index ( const const_iterator & lookup ) const {
  return index_ . find ( lookup ) -> second;
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::index */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::size_type & 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
index ( const const_iterator & lookup ) {
  return index_ [ lookup ];
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::index */

template < class First_Cell_Complex, class Second_Cell_Complex >
std::vector < Product_const_iterator<First_Cell_Complex, Second_Cell_Complex> > & 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
lookup ( void ) {
  return lookup_;
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::lookup */

template < class First_Cell_Complex, class Second_Cell_Complex >
const Product_const_iterator<First_Cell_Complex, Second_Cell_Complex> & 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
lookup ( size_type index ) const {
  return lookup_ [ index ];
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::lookup */

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_const_iterator<First_Cell_Complex, Second_Cell_Complex> & 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
lookup ( size_type index ) {
  return lookup_ [ index ];
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::lookup */

template < class First_Cell_Complex, class Second_Cell_Complex >
std::vector < int > 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
count_all_boundaries ( void ) const {
  std::vector < int > number_of_boundaries ( total_size_ );
  for ( const_iterator cell_iterator = begin_ [ 0 ]; cell_iterator != end_; ++ cell_iterator )
    number_of_boundaries [ index ( cell_iterator ) ] = boundary ( cell_iterator ) . size ();
  return number_of_boundaries;
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::count_all_boundaries */

template < class First_Cell_Complex, class Second_Cell_Complex >
void Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
boundary ( std::vector < size_type > & output, const size_type input ) const {
  output . clear ();
  Chain boundary_chain = boundary ( lookup_ [ input ] );
  for ( typename Chain::const_iterator term_iterator = boundary_chain . begin (); 
       term_iterator != boundary_chain . end (); ++ term_iterator ) {
    output . push_back ( index ( term_iterator -> first ) );
  } /* for */
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::boundary */

template < class First_Cell_Complex, class Second_Cell_Complex >
void Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
coboundary ( std::vector < size_type > & output, const size_type input ) const {
  output . clear ();
  Chain coboundary_chain = coboundary ( lookup_ [ input ] );
  for ( typename Chain::const_iterator term_iterator = coboundary_chain . begin (); 
       term_iterator != coboundary_chain . end (); ++ term_iterator ) {
    output . push_back ( index ( term_iterator -> first ) );
  } /* for */
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::coboundary */

template < class First_Cell_Complex, class Second_Cell_Complex >
void Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
boundary ( std::vector < std::pair< size_type, Ring > > & output, const size_type input ) const {
  output . clear ();
  Chain boundary_chain = boundary ( lookup_ [ input ] );
  for ( typename Chain::const_iterator term_iterator = boundary_chain . begin (); 
       term_iterator != boundary_chain. end (); ++ term_iterator ) {
    output . push_back ( std::pair < size_type, Ring > ( index ( term_iterator -> first ), 
                                                        term_iterator -> second ) );
  } /* for */
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::boundary */

template < class First_Cell_Complex, class Second_Cell_Complex >
void Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
coboundary ( std::vector < std::pair< size_type, Ring > > & output, const size_type input ) const {
  output . clear ();
  Chain coboundary_chain = coboundary ( lookup_ [ input ] );
  for ( typename Chain::const_iterator term_iterator = coboundary_chain . begin (); 
       term_iterator != coboundary_chain . end (); ++ term_iterator ) {
    output . push_back ( std::pair < size_type, Ring > ( index ( term_iterator -> first ), 
                                                        term_iterator -> second ) );
  } /* for */
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::coboundary */

/* Decomposable Complex */

template < class First_Cell_Complex, class Second_Cell_Complex >
void Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
decompose ( void ) {
  index (); 
  king_count_ = morse::decompose ( *this );
} /*  Product_Complex<First_Cell_Complex, Second_Cell_Complex>::decompose */

template < class First_Cell_Complex, class Second_Cell_Complex >
char Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
type ( size_type index, unsigned int dimension ) const {
  if ( index < index_begin_ [ dimension ] + king_count_ [ dimension + 1 ] ) return 0; /* QUEEN */
  if ( index < index_begin_ [ dimension + 1 ] - king_count_ [ dimension ] ) return 1; /* ACE */
  return 2; /* KING */
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::type */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::size_type 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
mate ( size_type queen_index, unsigned int dimension ) const {
  return index_begin_ [ dimension ] + index_begin_ [ dimension + 2 ] - queen_index - 1;
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::mate */

template < class First_Cell_Complex, class Second_Cell_Complex >
const typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::Ring & 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
connection ( size_type queen_index ) const {
  return connection_ [ queen_index ];
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::connection */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::Ring & 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
connection ( size_type queen_index ) {
  return connection_ [ queen_index ];
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::connection */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::size_type 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
ace_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ] + king_count_ [ dimension + 1 ];
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::ace_begin */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::size_type 
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
ace_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ] - king_count_ [ dimension ];
} /* Product_Complex<First_Cell_Complex, Second_Cell_Complex>::ace_end */

/* Product Complex */

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
Product_Complex ( void ) {
} /* Product_Complex<>::Product_Complex */

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
Product_Complex ( const First_Cell_Complex & first_factor, const Second_Cell_Complex & second_factor ) :
first_factor(&first_factor), second_factor(&second_factor) {
  construct ( first_factor, second_factor );
} /* Product_Complex<>::Product_Complex */

template < class First_Cell_Complex, class Second_Cell_Complex >
void Product_Complex<First_Cell_Complex, Second_Cell_Complex>::
construct ( const First_Cell_Complex & first_factor_in, const Second_Cell_Complex & second_factor_in ) {
  first_factor = & first_factor_in;
  second_factor = & second_factor_in;
  /* Initialize dimension_, total_size_ */
  dimension_ = first_factor -> dimension () + second_factor -> dimension ();
  total_size_ = first_factor -> size () * second_factor -> size ();
  /* Initialize end_ */
  end_ = const_iterator ( this, first_factor -> end (), second_factor -> end () );
  /* Initialize begin_, size_ */
  begin_ . resize ( dimension_ + 2, end_ );
  size_ . resize ( dimension_ + 1, 0 );
  for ( unsigned int first_dimension_index = 0; 
        first_dimension_index <= first_factor -> dimension (); 
        ++ first_dimension_index ) {
    for ( unsigned int second_dimension_index = 0; 
          second_dimension_index <= second_factor -> dimension (); 
          ++ second_dimension_index ) {
      const unsigned int dimension_sum = first_dimension_index + second_dimension_index;
      const size_type first_size = first_factor -> size ( first_dimension_index );
      const size_type second_size = first_factor -> size ( second_dimension_index );
      if ( first_size == 0 || second_size == 0 ) continue;
      size_ [ dimension_sum ] += first_size * second_size;
      if ( begin_ [ dimension_sum ] == end_ ) {
        begin_ [ dimension_sum ] = 
          const_iterator ( this, 
                           first_factor -> begin ( first_dimension_index ),
                           second_factor -> begin ( second_dimension_index ) );
      } /* if */
    } /* for */
  } /* for */
  /* TODO: worry about intermediate chain groups being empty */
} /* Product_Complex<>::construct */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename First_Cell_Complex::Chain 
Product_Complex<First_Cell_Complex, Second_Cell_Complex > ::
projectFirst ( const Chain & project_me ) {
  typename First_Cell_Complex::Chain return_value;
  for ( typename Chain::const_iterator term_iterator = project_me . begin ();
       term_iterator != project_me . end (); ++ term_iterator ) {
    typename First_Cell_Complex::Chain::Chain_Term projected_term 
    ( term_iterator -> first . first_, term_iterator -> second );
    if ( term_iterator -> first . second_ . dimension () == 0  ) 
      return_value += projected_term;
  } /* for */
  return return_value;
} /* Product_Complex<>::projectFirst */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Second_Cell_Complex::Chain 
Product_Complex<First_Cell_Complex, Second_Cell_Complex > ::
projectSecond ( const Chain & project_me ) {
  //std::cout << "Projecting " << project_me << "\n";
  typename Second_Cell_Complex::Chain return_value;
  for ( typename Chain::const_iterator term_iterator = project_me . begin ();
       term_iterator != project_me . end (); ++ term_iterator ) {
    typename Second_Cell_Complex::Chain::Chain_Term projected_term 
    ( term_iterator -> first . second_, term_iterator -> second );
    if ( term_iterator -> first . first_ . dimension () == 0 ) {
      return_value += projected_term;
      //std::cout << "projecting " << * projected_term . first << " " << projected_term . second << "\n";
    } /* if */
  } /* for */
  return return_value;
} /* Product_Complex<>::projectSecond */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Chain < First_Cell_Complex, Second_Cell_Complex >::Chain_Term
Product_Complex<First_Cell_Complex, Second_Cell_Complex > ::
tensor_product ( const typename First_Cell_Complex::const_iterator & first_iter, const typename Second_Cell_Complex::const_iterator & second_iter ) const {
  return typename Chain::Chain_Term ( Product_const_iterator < First_Cell_Complex, Second_Cell_Complex> ( this, first_iter, second_iter ), 1 );  
} /* Product_Complex<>::tensor_product */


template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Chain < First_Cell_Complex, Second_Cell_Complex >
Product_Complex<First_Cell_Complex, Second_Cell_Complex > ::
tensor_product ( const typename First_Cell_Complex::Chain & first_chain, const typename Second_Cell_Complex::const_iterator & second_iter ) const {
  Chain return_value;
  for ( typename First_Cell_Complex::Chain::const_iterator first_chain_term = first_chain . begin ();
       first_chain_term != first_chain . end (); ++ first_chain_term )
    return_value += typename Chain::Chain_Term ( tensor_product ( first_chain_term -> first, second_iter ) . first , first_chain_term -> second );
  return return_value;
} /* Product_Complex<>::tensor_product */


template < class First_Cell_Complex, class Second_Cell_Complex >
Product_Chain < First_Cell_Complex, Second_Cell_Complex > 
Product_Complex<First_Cell_Complex, Second_Cell_Complex > ::
tensor_product ( const typename First_Cell_Complex::const_iterator & first_iter, const typename Second_Cell_Complex::Chain & second_chain ) const {
  Chain return_value;
  for ( typename Second_Cell_Complex::Chain::const_iterator second_chain_term = second_chain . begin ();
       second_chain_term != second_chain . end (); ++ second_chain_term )
    return_value += typename Chain::Chain_Term ( tensor_product ( first_iter, second_chain_term -> first ) . first , second_chain_term -> second );
  return return_value;
} /* Product_Complex<>::tensor_product */

/* * * * * * * * * * * * * *
 * Product_const_iterator  *
 * * * * * * * * * * * * * */

template < class First_Cell_Complex, class Second_Cell_Complex >
std::ostream & operator << ( std::ostream & output_stream, const Product_const_iterator < First_Cell_Complex, Second_Cell_Complex > & print_me ) {
  return output_stream << "(" << * print_me . first_ << ", " << * print_me . second_ << ")";
} /* operator << for Product_const_iterator <> */

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_const_iterator<First_Cell_Complex, Second_Cell_Complex>::
Product_const_iterator ( void ) {
} /* Product_const_iterator<>::Product_const_iterator */

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_const_iterator<First_Cell_Complex, Second_Cell_Complex>::
Product_const_iterator ( const Complex * const container ) : container_(container) {
} /* Product_const_iterator<>::Product_const_iterator */


template < class First_Cell_Complex, class Second_Cell_Complex >
Product_const_iterator<First_Cell_Complex, Second_Cell_Complex>::
Product_const_iterator ( const Complex * const container, const typename First_Cell_Complex::const_iterator & first,
                const typename Second_Cell_Complex::const_iterator & second )
: container_(container), first_(first), second_(second) {
} /* Product_const_iterator<>::Product_const_iterator */

template < class First_Cell_Complex, class Second_Cell_Complex >
bool Product_const_iterator<First_Cell_Complex, Second_Cell_Complex>::
operator != ( const Product_const_iterator & right_hand_side ) const {
  if ( first_ != right_hand_side . first_ || second_ != right_hand_side . second_ ) return true;
  return false;
} /* Product_const_iterator<>::operator != */


template < class First_Cell_Complex, class Second_Cell_Complex >
bool Product_const_iterator<First_Cell_Complex, Second_Cell_Complex>::
operator == ( const Product_const_iterator & right_hand_side ) const {
  if ( first_ != right_hand_side . first_ || second_ != right_hand_side . second_ ) return false;
  return true;
} /* Product_const_iterator<>::operator == */

template < class First_Cell_Complex, class Second_Cell_Complex >
bool Product_const_iterator<First_Cell_Complex, Second_Cell_Complex>::
operator < ( const Product_const_iterator & right_hand_side ) const {
  if ( first_ == right_hand_side . first_ ) {
    return second_ < right_hand_side . second_;
  } else {
    return first_ < right_hand_side . first_;
  } /* if-else */
} /* Product_const_iterator<>::operator == */

template < class First_Cell_Complex, class Second_Cell_Complex >
typename Product_Complex<First_Cell_Complex, Second_Cell_Complex>::value_type 
Product_const_iterator<First_Cell_Complex, Second_Cell_Complex>::
operator * ( void ) const {
  return value_type ( *first_, *second_ );
} /* Product_const_iterator<>::operator * */

template < class First_Cell_Complex, class Second_Cell_Complex >
Product_const_iterator<First_Cell_Complex, Second_Cell_Complex> & 
Product_const_iterator<First_Cell_Complex, Second_Cell_Complex>::
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
  unsigned int first_dimension = first_ . dimension ();
  unsigned int second_dimension = second_ . dimension ();
  ++ second_;
  /* If advanced one within (K, L) type return answer immediately */
  if ( second_ != container_ -> second_factor -> end ( second_dimension ) ) return *this;
  ++ first_;
  /* If remaining in type (K, L) loop, return answer */
  if ( first_ != container_ -> first_factor -> end ( first_dimension ) ) {
    second_ = container_ -> second_factor -> begin ( second_dimension );
    return *this;
  } /* if */

  /* Find next non-empty type. */
  unsigned int dimension = first_dimension + second_dimension;
  const unsigned int K_max = container_ -> first_factor -> dimension ();
  const unsigned int L_max = container_ -> second_factor -> dimension ();
  while ( 1 ) {
    if ( first_dimension == K_max || second_dimension == 0 ) {
      /* The type is exhausted. */
      ++ dimension;
      if ( dimension > container_ -> dimension_ ) {
        /* We are completely finished */
        return *this = container_ -> end_;
      } /* if */
      /* Obtain the next type */
      if ( L_max < dimension ) {
        first_dimension = dimension - L_max;
        second_dimension = L_max;
      } else {
        first_dimension = 0;
        second_dimension = dimension;
      } /* if-else */
    } else {
      ++ first_dimension;
      -- second_dimension;
    } /* if-else */
    /* If have found the next type, return answer */
    if ( container_ -> first_factor -> size ( first_dimension ) > 0 
        && container_ -> second_factor -> size ( second_dimension ) > 0 ) {
      first_ = container_ -> first_factor -> begin ( first_dimension );
      second_ = container_ -> second_factor -> begin ( second_dimension );
      return * this;
    } /* if */
  } /* while */
} /* Product_const_iterator<>::operator ++ */

template < class First_Cell_Complex, class Second_Cell_Complex >
unsigned int Product_const_iterator<First_Cell_Complex, Second_Cell_Complex>::
dimension ( void ) const {
  return first_ . dimension () + second_ . dimension ();
} /* Product_const_iterator<>::dimension */

template < class First_Cell_Complex, class Second_Cell_Complex >
const Product_Complex<First_Cell_Complex, Second_Cell_Complex> & 
Product_const_iterator<First_Cell_Complex, Second_Cell_Complex>::
container ( void ) const {
  return container_;
} /* Product_const_iterator<>::container */
