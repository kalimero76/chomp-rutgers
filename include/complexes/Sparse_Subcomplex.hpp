/* Sparse_Subcomplex.hpp
 * CHomP project
 * Shaun Harker 6/7/10
 */


/* * * * * * * * * * * * * * * * *
 * Subcomplex_Chain definitions  *
 * * * * * * * * * * * * * * * * */

template < class Cell_Complex >
Subcomplex_Chain<Cell_Complex>::Subcomplex_Chain ( const complex_type & container ) 
: Chain_Archetype < std::map < Subcomplex_const_iterator<Cell_Complex>, Default_Ring > > ( container ) {
}

/* * * * * * * * * * * * * *
 * Subcomplex definitions  *
 * * * * * * * * * * * * * */

template < class Cell_Complex > 
std::pair<typename Subcomplex<Cell_Complex>::iterator, bool> Subcomplex<Cell_Complex>::insert ( const value_type & insert_me ) {
  std::pair < Subcomplex_const_iterator<Cell_Complex>, bool > 
    return_value ( Subcomplex_const_iterator<Cell_Complex> ( this, 
                                                             insert_me . data (), 
                                                             insert_me . dimension () ), 
                   not bitmap_ [ insert_me . data () ] );
  if ( return_value . second ) {
    bitmap_ [ insert_me . data () ] = true;
    unsigned int dimension = insert_me . dimension ();
    /* Update size_ */
		++ size_ [ dimension ];
    ++ total_size_;
    /* Update begin_ */
    const_iterator iter = return_value . first;
    ++ iter;
    if ( iter == begin_ [ dimension ] ) {
      begin_ [ dimension ] = return_value . first;
    } /* if */
  } /* if */
  return return_value;
} /* Subcomplex<>::insert */

template < class Cell_Complex > 
void Subcomplex<Cell_Complex>::erase ( const iterator & erase_me ) {
  unsigned int dimension = erase_me . dimension ();
  /* Update size_ */
  -- size_ [ dimension ]; 
  -- total_size_;
  /* Update begin_ */
  if ( erase_me == begin_ [ dimension ] ) {
    ++ begin_ [ dimension ];
    while ( dimension > 0 ) {
      if ( begin_ [ dimension - 1 ] == erase_me ) begin_ [ dimension - 1 ] = begin_ [ dimension ];
      else break;
      -- dimension;
    }
  }
  bitmap_ [ erase_me . data_ ] = false;
} /* Subcomplex<>::erase */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::iterator Subcomplex<Cell_Complex>::find ( const key_type & find_me ) const {
  if ( bitmap_ [ find_me . data () ] == false ) return end_;
  return typename Subcomplex<Cell_Complex>::iterator ( this, find_me . data (), find_me . dimension () );
} /* Subcomplex<>::find */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::iterator Subcomplex<Cell_Complex>::begin ( void ) const {
	return begin_ [ 0 ];  
} /* Subcomplex<>::begin */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::iterator Subcomplex<Cell_Complex>::end ( void ) const {
  return end_;
} /* Subcomplex<>::end */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::size_type Subcomplex<Cell_Complex>::size ( void ) const {
	size_type return_value = 0;
	for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) 
		return_value += size_ [ dimension_index ];
	return return_value; 
} /* Subcomplex<>::size */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::iterator Subcomplex<Cell_Complex>::begin ( unsigned int dimension ) const {
  return begin_ [ dimension ];
} /* Subcomplex<>::begin */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::iterator Subcomplex<Cell_Complex>::end ( unsigned int dimension ) const {
  return begin_ [ dimension + 1 ];
} /* Subcomplex<>::end */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::size_type Subcomplex<Cell_Complex>::size ( unsigned int dimension ) const {
  return size_ [ dimension ];
} /* Subcomplex<>::size */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::Chain Subcomplex<Cell_Complex>::boundary ( const const_iterator & input ) const {
  typename Cell_Complex::Chain super_boundary = super_complex_ . boundary ( super_complex_ . lookup ( input . data_ ) );
  return project ( super_boundary );
} /* Subcomplex<>::boundary */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::Chain Subcomplex<Cell_Complex>::coboundary ( const const_iterator & input ) const {
  typename Cell_Complex::Chain super_coboundary = super_complex_ . coboundary ( super_complex_ . lookup ( input . data_ ) );
  return project ( super_coboundary );
} /* Subcomplex<>::coboundary */

template < class Cell_Complex > 
unsigned int Subcomplex<Cell_Complex>::dimension ( void ) const {
  return dimension_;
} /* Subcomplex<>::dimension */

template < class Cell_Complex > 
Subcomplex<Cell_Complex>::
Subcomplex ( const Cell_Complex & super_complex ) : super_complex_(super_complex) {
  dimension_ = super_complex . dimension ();
  bitmap_size_ = super_complex_ . size ();
  end_ = const_iterator ( this, bitmap_size_ , dimension_ );
  begin_ . resize ( dimension_ + 2, end_ );
  size_ . resize ( dimension_ + 1, 0 );
  total_size_ = 0;
  bitmap_ . resize ( bitmap_size_, true );
  /* Load in begin_, size_, total_size_ data. */
  for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) {
    begin_ [ dimension_index ] = Subcomplex_const_iterator < Cell_Complex > 
      ( this, super_complex_ . index ( super_complex_ . begin ( dimension_index ) ),
       super_complex_ . begin ( dimension_index ) . dimension () );
    total_size_ += size_ [ dimension_index ] = super_complex_ . size ( dimension_index );
  }
} /* Subcomplex<>::Subcomplex */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::Chain Subcomplex<Cell_Complex>::project ( const typename Cell_Complex::Chain & project_me ) const {
  Chain return_value ( *this );
  //unsigned int dimension = project_me . dimension (); TODO make this work instead of calling dimension on all the terms
  for ( typename Cell_Complex::Chain::const_iterator chain_term = project_me . begin (); chain_term != project_me . end (); ++ chain_term ) {
    unsigned long indx =  super_complex_ . index ( chain_term -> first );
    if ( bitmap_ [ indx ] ) return_value += 
      typename Chain::Chain_Term ( Subcomplex_const_iterator<Cell_Complex> ( this, indx, 
                                                               chain_term -> first . dimension () ), 
                                   chain_term -> second );
  } /* for */
  return return_value;
} /* Subcomplex<>::project */

template < class Cell_Complex > 
typename Cell_Complex::const_iterator Subcomplex<Cell_Complex>::include ( const const_iterator & include_me ) const {
  return super_complex_ . lookup ( include_me . data_ );
} /* Subcomplex<>::include */

template < class Cell_Complex > 
typename Cell_Complex::Chain Subcomplex<Cell_Complex>::include ( const Chain & include_me ) const {
  typename Cell_Complex::Chain return_value ( *this );
  for ( typename Chain::const_iterator chain_term = include_me . begin (); chain_term != include_me . end (); ++ chain_term )
    return_value += typename Cell_Complex::Chain::Chain_Term 
      ( super_complex_ . lookup ( chain_term -> first . data_ ), chain_term -> second );
  return return_value;
} /* Subcomplex<>::include */

template < class Cell_Complex > 
std::pair < unsigned long, typename Cell_Complex::Ring > 
Subcomplex<Cell_Complex>::mate ( const unsigned long mate_me ) const {
  typename Cell_Complex::Chain boundary_chain = super_complex_ . boundary 
    ( super_complex_ . lookup ( mate_me ) );
  /* Loop through terms until one is found */
  for ( typename Cell_Complex::Chain::const_iterator term_iterator = boundary_chain . begin ();
       term_iterator != boundary_chain . end (); ++ term_iterator ) {
    unsigned long cell_index;
    if ( bitmap_ [ cell_index = super_complex_ . index ( term_iterator -> first ) ] ) {
      return std::pair < unsigned long, Ring > ( cell_index, term_iterator -> second );
    } /* if */
  } /* for */
  std::cout << "Subcomplex<>::mate. Fatal error; there were no boundaries.\n";
  return std::pair < unsigned long, Ring > ();
} /* Subcomplex<>::mate */

template < class Cell_Complex > 
bool Subcomplex<Cell_Complex>::bitmap ( unsigned long address ) const {
  return bitmap_ [ address ];
} /* Subcomplex<>::bitmap */

template < class Cell_Complex > 
void Subcomplex<Cell_Complex>::erase ( unsigned long address ) {
  bitmap_ [ address ] = false;
} /* Subcomplex<>::bitmap */

/* * * * * * * * * * * * * * * * * * * *
 * Subcomplex_const_iterator definitions  *
 * * * * * * * * * * * * * * * * * * * */

template < class Cell_Complex > 
Subcomplex_const_iterator<Cell_Complex>::Subcomplex_const_iterator ( void ) : container_(NULL), dimension_(0) { 
} /* Subcomplex_const_iterator::Subcomplex_const_iterator */

template < class Cell_Complex > 
Subcomplex_const_iterator<Cell_Complex>::Subcomplex_const_iterator ( const Subcomplex<Cell_Complex> * const container, const unsigned long data ) : 
container_(container), data_(data) {
  /* We have to determine the dimension. */
  dimension_ = 0;
  while ( dimension_ < container_ -> dimension_ && 
          data_ >= container_ -> begin_ [ dimension_ + 1 ] . data_ ) 
    ++ dimension_;
} /* Subcomplex_const_iterator::Subcomplex_const_iterator */

template < class Cell_Complex > 
Subcomplex_const_iterator<Cell_Complex>::Subcomplex_const_iterator ( const Subcomplex<Cell_Complex> * const container, const unsigned long data, const unsigned int dimension ) : 
container_(container), data_(data), dimension_(dimension) {
} /* Subcomplex_const_iterator::Subcomplex_const_iterator */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::value_type Subcomplex_const_iterator<Cell_Complex>::operator * ( void ) const {
	return Default_Cell ( data_, dimension_ ); 
} /* Subcomplex_const_iterator::operator * */

template < class Cell_Complex > 
Subcomplex_const_iterator<Cell_Complex> & Subcomplex_const_iterator<Cell_Complex>::operator ++ ( void ) {
	++ data_;
  while ( data_ < container_ -> bitmap_size_ && container_ -> bitmap_ [ data_ ] == false ) ++ data_;
  while ( dimension_ < container_ -> dimension_ && data_ >= container_ -> begin_ [ dimension_ + 1 ] . data_ ) ++ dimension_; 
  return *this;
} /* Subcomplex_const_iterator::operator ++ */

template < class Cell_Complex > 
bool Subcomplex_const_iterator<Cell_Complex>::operator != ( const Subcomplex_const_iterator & right_hand_side ) const {
	return data_ != right_hand_side . data_;
} /* Subcomplex_const_iterator::operator != */

template < class Cell_Complex > 
bool Subcomplex_const_iterator<Cell_Complex>::operator == ( const Subcomplex_const_iterator & right_hand_side ) const {
	return data_ == right_hand_side . data_;
} /* Subcomplex_const_iterator::operator == */

template < class Cell_Complex > 
bool Subcomplex_const_iterator<Cell_Complex>::operator < ( const Subcomplex_const_iterator & right_hand_side ) const {
  if ( dimension_ == right_hand_side . dimension_ ) 
    return data_ < right_hand_side . data_;
  else
    return dimension_ < right_hand_side . dimension_;
} /* Subcomplex_const_iterator::operator < */

template < class Cell_Complex > 
unsigned int Subcomplex_const_iterator<Cell_Complex>::dimension () const {
  return dimension_;
} /* Subcomplex_const_iterator::dimension */

template < class Cell_Complex > 
const Subcomplex<Cell_Complex> & Subcomplex_const_iterator<Cell_Complex>::container () const {
  return container_;
} /* Subcomplex_const_iterator::container */

template < class Cell_Complex > 
unsigned long Subcomplex_const_iterator<Cell_Complex>::data () const {
  return data_;
} /* Subcomplex_const_iterator::data */
