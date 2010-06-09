/* Sparse_Subcomplex.hpp
 * CHomP project
 * Shaun Harker 6/7/10
 */

/* * * * * * * * * * * * * * * * *
 * Sparse_Subcomplex definitions *
 * * * * * * * * * * * * * * * * */

template < class Cell_Complex > 
std::pair<typename Sparse_Subcomplex<Cell_Complex>::iterator, bool> 
Sparse_Subcomplex<Cell_Complex>::
insert ( const value_type & insert_me ) {
  const unsigned int dimension = insert_me . dimension ();
  std::pair < typename std::unordered_set < typename Cell_Complex::const_iterator, boost::hash < typename Cell_Complex::const_iterator > >::iterator, bool > 
    insert_value = data_ [ dimension ] . insert ( insert_me );
  std::pair < Sparse_Subcomplex_const_iterator<Cell_Complex>, bool > return_value 
    ( Sparse_Subcomplex_const_iterator<Cell_Complex> ( this, 
                                                       insert_value . first, 
                                                       insert_me . dimension () ), 
      insert_value . second );
                
  if ( return_value . second ) {
    /* Update size_ */
		++ size_ [ dimension ];
    ++ total_size_;
  } /* if */
  return return_value;
} /* Sparse_Subcomplex<>::insert */

template < class Cell_Complex > 
void Sparse_Subcomplex<Cell_Complex>::erase ( const iterator & erase_me ) {
  const unsigned int dimension = erase_me . dimension ();
  /* Update size_ */
  -- size_ [ dimension ]; 
  -- total_size_;
  data_ [ dimension ] . erase ( erase_me . data_ );
} /* Sparse_Subcomplex<>::erase */

template < class Cell_Complex > 
void Sparse_Subcomplex<Cell_Complex>::clear ( void ) {
  for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) {
    data_ [ dimension_index ] . clear ();
    size_ [ dimension_index ] = 0;
  } /* for */
  total_size_ = 0;
  /* Index Complex */
  index_ . clear ();
  lookup_ . clear ();
  lookup_ . reserve ( 0 );
  index_begin_ . clear ();
  index_begin_ . reserve ( 0 );

  /* Decomposable Complex */
  connection_ . clear ();
  connection_ . reserve ( 0 );
  boundary_count_ . clear ();
  boundary_count_ . reserve ( 0 );
  king_count_ . clear ();
  king_count_ . reserve ( 0 );
} /* Sparse_Subcomplex<>::erase */

template < class Cell_Complex > 
typename Sparse_Subcomplex<Cell_Complex>::iterator Sparse_Subcomplex<Cell_Complex>::find ( const key_type & find_me ) const {
  const unsigned int dimension = find_me . dimension ();
  typename std::unordered_set < typename Cell_Complex::const_iterator, boost::hash < typename Cell_Complex::const_iterator > >::const_iterator find_value =
    data_ [ dimension ] . find ( find_me );
  if ( find_value == data_ [ dimension ] . end () ) return end_;
  return typename Sparse_Subcomplex<Cell_Complex>::iterator ( this, find_value, dimension );
} /* Sparse_Subcomplex<>::find */

template < class Cell_Complex > 
typename Sparse_Subcomplex<Cell_Complex>::iterator Sparse_Subcomplex<Cell_Complex>::begin ( void ) const {
	unsigned int dimension = 0;
  while ( dimension <= dimension_ && data_ [ dimension ] . size () == 0 ) ++ dimension;
  if ( dimension > dimension_ ) return end_;
  return typename Sparse_Subcomplex<Cell_Complex>::iterator ( this, 
                                                             data_ [ dimension ] . begin (), 
                                                             dimension );
} /* Sparse_Subcomplex<>::begin */

template < class Cell_Complex > 
typename Sparse_Subcomplex<Cell_Complex>::iterator Sparse_Subcomplex<Cell_Complex>::end ( void ) const {
  return end_;
} /* Sparse_Subcomplex<>::end */

template < class Cell_Complex > 
typename Sparse_Subcomplex<Cell_Complex>::size_type Sparse_Subcomplex<Cell_Complex>::size ( void ) const {
	return total_size_; 
} /* Sparse_Subcomplex<>::size */

template < class Cell_Complex > 
typename Sparse_Subcomplex<Cell_Complex>::iterator Sparse_Subcomplex<Cell_Complex>::begin ( unsigned int dimension ) const {
  while ( dimension <= dimension_ && data_ [ dimension ] . size () == 0 ) ++ dimension;
  if ( dimension > dimension_ ) return end_;
  return typename Sparse_Subcomplex<Cell_Complex>::iterator ( this, 
                                                              data_ [ dimension ] . begin (), 
                                                              dimension );
} /* Sparse_Subcomplex<>::begin */

template < class Cell_Complex > 
typename Sparse_Subcomplex<Cell_Complex>::iterator Sparse_Subcomplex<Cell_Complex>::end ( unsigned int dimension ) const {
  ++ dimension;
  while ( dimension <= dimension_ && data_ [ dimension ] . size () == 0 ) ++ dimension;
  if ( dimension > dimension_ ) return end_;
  return typename Sparse_Subcomplex<Cell_Complex>::iterator ( this, 
                                                             data_ [ dimension ] . begin (), 
                                                             dimension );
} /* Sparse_Subcomplex<>::end */

template < class Cell_Complex > 
typename Sparse_Subcomplex<Cell_Complex>::size_type Sparse_Subcomplex<Cell_Complex>::size ( unsigned int dimension ) const {
  return size_ [ dimension ];
} /* Sparse_Subcomplex<>::size */

template < class Cell_Complex > 
typename Sparse_Subcomplex<Cell_Complex>::Chain Sparse_Subcomplex<Cell_Complex>::boundary ( const const_iterator & input ) const {
  typename Cell_Complex::Chain super_boundary = super_complex_ -> boundary ( * input );
  return project ( super_boundary );
} /* Sparse_Subcomplex<>::boundary */

template < class Cell_Complex > 
typename Sparse_Subcomplex<Cell_Complex>::Chain Sparse_Subcomplex<Cell_Complex>::coboundary ( const const_iterator & input ) const {
  typename Cell_Complex::Chain super_coboundary = super_complex_ -> coboundary ( * input );
  return project ( super_coboundary );
} /* Sparse_Subcomplex<>::coboundary */

template < class Cell_Complex > 
unsigned int Sparse_Subcomplex<Cell_Complex>::dimension ( void ) const {
  return dimension_;
} /* Sparse_Subcomplex<>::dimension */

/* Index Complex */

template < class Cell_Complex >
void Sparse_Subcomplex<Cell_Complex>::
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
} /* Sparse_Subcomplex<Cell_Complex>::index */

template < class Cell_Complex >
typename Sparse_Subcomplex<Cell_Complex>::size_type 
Sparse_Subcomplex<Cell_Complex>::
index_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ]; 
} /* Sparse_Subcomplex<Cell_Complex>::index_begin */ 

template < class Cell_Complex >
typename Sparse_Subcomplex<Cell_Complex>::size_type 
Sparse_Subcomplex<Cell_Complex>::
index_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ];
} /* Sparse_Subcomplex<Cell_Complex>::index_end */

template < class Cell_Complex >
typename Sparse_Subcomplex<Cell_Complex>::size_type 
Sparse_Subcomplex<Cell_Complex>::
index ( const const_iterator & lookup ) const {
  return index_ . find ( lookup ) -> second;
} /* Sparse_Subcomplex<Cell_Complex>::index */

template < class Cell_Complex >
typename Sparse_Subcomplex<Cell_Complex>::size_type & 
Sparse_Subcomplex<Cell_Complex>::
index ( const const_iterator & lookup ) {
  return index_ [ lookup ];
} /* Sparse_Subcomplex<Cell_Complex>::index */

template < class Cell_Complex >
std::vector < Sparse_Subcomplex_const_iterator<Cell_Complex> > & 
Sparse_Subcomplex<Cell_Complex>::
lookup ( void ) {
  return lookup_;
} /* Sparse_Subcomplex<Cell_Complex>::lookup */

template < class Cell_Complex >
const Sparse_Subcomplex_const_iterator<Cell_Complex> & 
Sparse_Subcomplex<Cell_Complex>::
lookup ( size_type index ) const {
  return lookup_ [ index ];
} /* Sparse_Subcomplex<Cell_Complex>::lookup */

template < class Cell_Complex >
Sparse_Subcomplex_const_iterator<Cell_Complex> & 
Sparse_Subcomplex<Cell_Complex>::
lookup ( size_type index ) {
  return lookup_ [ index ];
} /* Sparse_Subcomplex<Cell_Complex>::lookup */

template < class Cell_Complex >
std::vector < int > 
Sparse_Subcomplex<Cell_Complex>::
count_all_boundaries ( void ) const {
  std::vector < int > number_of_boundaries ( total_size_ );
  for ( const_iterator cell_iterator = begin (); cell_iterator != end_; ++ cell_iterator )
    number_of_boundaries [ index ( cell_iterator ) ] = boundary ( cell_iterator ) . size ();
  return number_of_boundaries;
} /* Sparse_Subcomplex<Cell_Complex>::count_all_boundaries */

template < class Cell_Complex >
void Sparse_Subcomplex<Cell_Complex>::
boundary ( std::vector < size_type > & output, const size_type input ) const {
  output . clear ();
  Chain boundary_chain = boundary ( lookup_ [ input ] );
  for ( typename Chain::const_iterator term_iterator = boundary_chain . begin (); 
       term_iterator != boundary_chain . end (); ++ term_iterator ) {
    output . push_back ( index ( term_iterator -> first ) );
  } /* for */
} /* Sparse_Subcomplex<Cell_Complex>::boundary */

template < class Cell_Complex >
void Sparse_Subcomplex<Cell_Complex>::
coboundary ( std::vector < size_type > & output, const size_type input ) const {
  output . clear ();
  Chain coboundary_chain = coboundary ( lookup_ [ input ] );
  for ( typename Chain::const_iterator term_iterator = coboundary_chain . begin (); 
       term_iterator != coboundary_chain . end (); ++ term_iterator ) {
    output . push_back ( index ( term_iterator -> first ) );
  } /* for */
} /* Sparse_Subcomplex<Cell_Complex>::coboundary */

template < class Cell_Complex >
void Sparse_Subcomplex<Cell_Complex>::
boundary ( std::vector < std::pair< size_type, Ring > > & output, const size_type input ) const {
  output . clear ();
  Chain boundary_chain = boundary ( lookup_ [ input ] );
  for ( typename Chain::const_iterator term_iterator = boundary_chain . begin (); 
       term_iterator != boundary_chain. end (); ++ term_iterator ) {
    output . push_back ( std::pair < size_type, Ring > ( index ( term_iterator -> first ), 
                                                        term_iterator -> second ) );
  } /* for */
} /* Sparse_Subcomplex<Cell_Complex>::boundary */

template < class Cell_Complex >
void Sparse_Subcomplex<Cell_Complex>::
coboundary ( std::vector < std::pair< size_type, Ring > > & output, const size_type input ) const {
  output . clear ();
  Chain coboundary_chain = coboundary ( lookup_ [ input ] );
  for ( typename Chain::const_iterator term_iterator = coboundary_chain . begin (); 
       term_iterator != coboundary_chain . end (); ++ term_iterator ) {
    output . push_back ( std::pair < size_type, Ring > ( index ( term_iterator -> first ), 
                                                        term_iterator -> second ) );
  } /* for */
} /* Sparse_Subcomplex<Cell_Complex>::coboundary */

/* Decomposable Complex */

template < class Cell_Complex >
void Sparse_Subcomplex<Cell_Complex>::
decompose ( void ) {
  index (); 
  king_count_ = morse::decompose ( *this );
} /*  Sparse_Subcomplex<Cell_Complex>::decompose */

template < class Cell_Complex >
char Sparse_Subcomplex<Cell_Complex>::
type ( size_type index, unsigned int dimension ) const {
  if ( index < index_begin_ [ dimension ] + king_count_ [ dimension + 1 ] ) return 0; /* QUEEN */
  if ( index < index_begin_ [ dimension + 1 ] - king_count_ [ dimension ] ) return 1; /* ACE */
  return 2; /* KING */
} /* Sparse_Subcomplex<Cell_Complex>::type */

template < class Cell_Complex >
typename Sparse_Subcomplex<Cell_Complex>::size_type 
Sparse_Subcomplex<Cell_Complex>::
mate ( size_type queen_index, unsigned int dimension ) const {
  return index_begin_ [ dimension ] + index_begin_ [ dimension + 2 ] - queen_index - 1;
} /* Sparse_Subcomplex<Cell_Complex>::mate */

template < class Cell_Complex >
const typename Sparse_Subcomplex<Cell_Complex>::Ring & 
Sparse_Subcomplex<Cell_Complex>::
connection ( size_type queen_index ) const {
  return connection_ [ queen_index ];
} /* Sparse_Subcomplex<Cell_Complex>::connection */

template < class Cell_Complex >
typename Sparse_Subcomplex<Cell_Complex>::Ring & 
Sparse_Subcomplex<Cell_Complex>::
connection ( size_type queen_index ) {
  return connection_ [ queen_index ];
} /* Sparse_Subcomplex<Cell_Complex>::connection */

template < class Cell_Complex >
typename Sparse_Subcomplex<Cell_Complex>::size_type 
Sparse_Subcomplex<Cell_Complex>::
ace_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ] + king_count_ [ dimension + 1 ];
} /* Sparse_Subcomplex<Cell_Complex>::ace_begin */

template < class Cell_Complex >
typename Sparse_Subcomplex<Cell_Complex>::size_type 
Sparse_Subcomplex<Cell_Complex>::
ace_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ] - king_count_ [ dimension ];
} /* Sparse_Subcomplex<Cell_Complex>::ace_end */

/* Sparse Subcomplex */

template < class Cell_Complex > 
Sparse_Subcomplex<Cell_Complex>::
Sparse_Subcomplex ( void ) {
} /* Sparse_Subcomplex<>::Sparse_Subcomplex */
  
template < class Cell_Complex > 
Sparse_Subcomplex<Cell_Complex>::
Sparse_Subcomplex ( const Cell_Complex & super_complex ) {
  construct ( super_complex );
} /* Sparse_Subcomplex<>::Sparse_Subcomplex */

template < class Cell_Complex > 
void Sparse_Subcomplex<Cell_Complex>::
construct ( const Cell_Complex & super_complex ) {
  super_complex_ = &super_complex;
  dimension_ = super_complex . dimension ();
  end_ = const_iterator ( this, 
                         typename std::unordered_set < typename Cell_Complex::const_iterator, 
                         boost::hash < typename Cell_Complex::const_iterator > >::const_iterator (), 
                         dimension_ + 1 );
  size_ . resize ( dimension_ + 1, 0 );
  total_size_ = 0;
  data_ . resize ( dimension_ + 1 );
} /* Sparse_Subcomplex<>::construct */

template < class Cell_Complex > 
typename Sparse_Subcomplex<Cell_Complex>::Chain Sparse_Subcomplex<Cell_Complex>::project ( const typename Cell_Complex::Chain & project_me ) const {
  Chain return_value;
  for ( typename Cell_Complex::Chain::const_iterator chain_term = project_me . begin (); chain_term != project_me . end (); ++ chain_term ) {
    Sparse_Subcomplex_const_iterator<Cell_Complex> found = find ( chain_term -> first );
    if ( found != end_ ) return_value += typename Chain::Chain_Term ( found, chain_term -> second );
  } /* for */
  return return_value;
} /* Sparse_Subcomplex<>::project */

template < class Cell_Complex > 
typename Cell_Complex::const_iterator Sparse_Subcomplex<Cell_Complex>::include ( const const_iterator & include_me ) const {
  return * include_me;
} /* Sparse_Subcomplex<>::include */

template < class Cell_Complex > 
typename Cell_Complex::Chain Sparse_Subcomplex<Cell_Complex>::include ( const Chain & include_me ) const {
  typename Cell_Complex::Chain return_value;
  for ( typename Chain::const_iterator chain_term = include_me . begin (); chain_term != include_me . end (); ++ chain_term )
    return_value += typename Cell_Complex::Chain::Chain_Term ( * chain_term -> first, 
                                                               chain_term -> second );
  return return_value;
} /* Sparse_Subcomplex<>::include */

/* * * * * * * * * * * * * * * * * * * * * * * * *
 * Sparse_Subcomplex_const_iterator definitions  *
 * * * * * * * * * * * * * * * * * * * * * * * * */

template < class Cell_Complex > 
Sparse_Subcomplex_const_iterator<Cell_Complex>::Sparse_Subcomplex_const_iterator ( void ) : container_(NULL), dimension_(0) { 
} /* Sparse_Subcomplex_const_iterator::Sparse_Subcomplex_const_iterator */

template < class Cell_Complex > 
Sparse_Subcomplex_const_iterator<Cell_Complex>::Sparse_Subcomplex_const_iterator 
  ( const Sparse_Subcomplex<Cell_Complex> * const container, 
    const typename std::unordered_set < typename Cell_Complex::const_iterator, boost::hash < typename Cell_Complex::const_iterator > >::const_iterator data, 
    const unsigned int dimension ) : 
container_(container), data_(data), dimension_(dimension) {
} /* Sparse_Subcomplex_const_iterator::Sparse_Subcomplex_const_iterator */

template < class Cell_Complex > 
typename Sparse_Subcomplex<Cell_Complex>::value_type Sparse_Subcomplex_const_iterator<Cell_Complex>::operator * ( void ) const {
	return * data_; 
} /* Sparse_Subcomplex_const_iterator::operator * */

template < class Cell_Complex > 
Sparse_Subcomplex_const_iterator<Cell_Complex> & Sparse_Subcomplex_const_iterator<Cell_Complex>::operator ++ ( void ) {
	++ data_;
  if ( data_ != container_ -> data_ [ dimension_ ] . end () ) return *this;
  else return *this = container_ -> begin ( dimension_ + 1 );
} /* Sparse_Subcomplex_const_iterator::operator ++ */

template < class Cell_Complex > 
bool Sparse_Subcomplex_const_iterator<Cell_Complex>::operator != ( const Sparse_Subcomplex_const_iterator & right_hand_side ) const {
	if ( dimension_ == container_ -> dimension_ + 1 || right_hand_side . dimension_ == container_ -> dimension_ + 1) {
    return dimension_ != right_hand_side . dimension_;
  } else {
    return data_ != right_hand_side . data_;
  } /* if-else */
} /* Sparse_Subcomplex_const_iterator::operator != */

template < class Cell_Complex > 
bool Sparse_Subcomplex_const_iterator<Cell_Complex>::operator == ( const Sparse_Subcomplex_const_iterator & right_hand_side ) const {
	if ( dimension_ == container_ -> dimension_ + 1 || right_hand_side . dimension_ == container_ -> dimension_ + 1) {
    return dimension_ == right_hand_side . dimension_;
  } else {
    return data_ == right_hand_side . data_;
  } /* if-else */
} /* Sparse_Subcomplex_const_iterator::operator == */

template < class Cell_Complex > 
bool Sparse_Subcomplex_const_iterator<Cell_Complex>::operator < ( const Sparse_Subcomplex_const_iterator & right_hand_side ) const {
  if ( dimension_ == right_hand_side . dimension_ ) 
    return * reinterpret_cast < const unsigned long * > ( & data_ ) < 
    * reinterpret_cast < const unsigned long * > ( & right_hand_side . data_ );
  else
    return dimension_ < right_hand_side . dimension_;
} /* Sparse_Subcomplex_const_iterator::operator < */

template < class Cell_Complex > 
unsigned int Sparse_Subcomplex_const_iterator<Cell_Complex>::dimension () const {
  return dimension_;
} /* Sparse_Subcomplex_const_iterator::dimension */

template < class Cell_Complex > 
const Sparse_Subcomplex<Cell_Complex> & Sparse_Subcomplex_const_iterator<Cell_Complex>::container () const {
  return container_;
} /* Sparse_Subcomplex_const_iterator::container */
