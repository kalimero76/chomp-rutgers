/* Subcomplex.hpp
 * CHomP project
 * Shaun Harker 3/31/10
 */

/* * * * * * * * * * * * * * * * * * *
 * Subcomplex_Container definitions  *
 * * * * * * * * * * * * * * * * * * */

template < class Cell_Complex > 
std::pair<typename Subcomplex_Container<Cell_Complex>::iterator, bool> Subcomplex_Container<Cell_Complex>::insert ( const value_type & insert_me ) {
  std::pair<typename std::set < typename Cell_Complex::const_iterator >::const_iterator, bool> insert_value;
  insert_value = data_ . insert ( insert_me );
  std::pair < Subcomplex_const_iterator<Cell_Complex>, bool > return_value ( Subcomplex_const_iterator<Cell_Complex> ( this, insert_value . first, insert_me . dimension () ), insert_value . second );
  if ( return_value . second ) {
    unsigned int dimension = insert_me . dimension ();
    /* Update size_ */
		++ size_ [ dimension ]; 
    /* Update begin_, end_ */
    const_iterator iter = return_value . first;
    ++ iter;
    if ( iter == begin_ [ dimension ] ) {
      begin_ [ dimension ] = return_value . first;
      if ( dimension  > 0 ) end_ [ dimension - 1 ] = begin_ [ dimension ];
    } /* if */
  } /* if */
  return return_value;
} /* Subcomplex_Container<>::insert */

template < class Cell_Complex > 
void Subcomplex_Container<Cell_Complex>::erase ( const iterator & erase_me ) {
  std::cout << "subcomplex erase of " << * * erase_me << "\n";
  unsigned int dimension = erase_me . dimension ();
  /* Update size_ */
  -- size_ [ dimension ]; 
  /* Update begin_, end_ */
  if ( erase_me == begin_ [ dimension ] ) {
    ++ begin_ [ dimension ];
    if ( dimension > 0 ) end_ [ dimension - 1 ] = begin_ [ dimension ];
  }
  data_ . erase ( erase_me . data_ );
} /* Subcomplex_Container<>::erase */

template < class Cell_Complex > 
typename Subcomplex_Container<Cell_Complex>::iterator Subcomplex_Container<Cell_Complex>::find ( const key_type & find_me ) const {
  return typename Subcomplex_Container<Cell_Complex>::iterator ( this, data_ . find ( find_me ), find_me . dimension () );
} /* Subcomplex_Container<>::find */

template < class Cell_Complex > 
typename Subcomplex_Container<Cell_Complex>::iterator Subcomplex_Container<Cell_Complex>::begin ( unsigned int dimension ) const {
  return begin_ [ dimension ];
} /* Subcomplex_Container<>::begin */

template < class Cell_Complex > 
typename Subcomplex_Container<Cell_Complex>::iterator Subcomplex_Container<Cell_Complex>::end ( unsigned int dimension ) const {
  return end_ [ dimension ];
} /* Subcomplex_Container<>::end */

template < class Cell_Complex > 
typename Subcomplex_Container<Cell_Complex>::size_type Subcomplex_Container<Cell_Complex>::size ( unsigned int dimension ) const {
  return size_ [ dimension ];
} /* Subcomplex_Container<>::size */

template < class Cell_Complex > 
typename Subcomplex_Container<Cell_Complex>::Chain Subcomplex_Container<Cell_Complex>::boundary ( const const_iterator & input ) const {
  typename Cell_Complex::Chain super_boundary = super_complex_ -> boundary ( *input );
  return project ( super_boundary );
} /* Subcomplex_Container<>::boundary */

template < class Cell_Complex > 
typename Subcomplex_Container<Cell_Complex>::Chain Subcomplex_Container<Cell_Complex>::coboundary ( const const_iterator & input ) const {
  typename Cell_Complex::Chain super_coboundary = super_complex_ -> coboundary ( *input );
  return project ( super_coboundary );
} /* Subcomplex_Container<>::coboundary */

template < class Cell_Complex > 
unsigned int Subcomplex_Container<Cell_Complex>::dimension ( void ) const {
  return dimension_;
} /* Subcomplex_Container<>::dimension */

template < class Cell_Complex > 
typename Subcomplex_Container<Cell_Complex>::Chain Subcomplex_Container<Cell_Complex>::project ( const typename Cell_Complex::Chain & project_me ) const {
  Chain return_value;
  const_iterator projected;
  for ( typename Cell_Complex::Chain::const_iterator chain_term = project_me . begin (); chain_term != project_me . end (); ++ chain_term )
    if ( ( projected = find ( chain_term -> first ) ) != end_ [ dimension_ ]  ) 
      return_value += typename Chain::Chain_Term ( projected, chain_term -> second );
  return return_value;
} /* Subcomplex_Container<>::project */

template < class Cell_Complex > 
typename Cell_Complex::Chain Subcomplex_Container<Cell_Complex>::include ( const Chain & include_me ) const {
  typename Cell_Complex::Chain return_value;
  for ( typename Chain::const_iterator chain_term = include_me . begin (); chain_term != include_me . end (); ++ chain_term )
    return_value += Chain::Chain_Term ( * chain_term -> first, chain_term -> second );
  return return_value;
} /* Subcomplex_Container<>::include */

template < class Cell_Complex > 
void Subcomplex_Container<Cell_Complex>::
initialize ( const Cell_Complex & super_complex ) {
  super_complex_ = &super_complex;
  dimension_ = super_complex . dimension ();
  typename Subcomplex_Container<Cell_Complex>::const_iterator end_value ( this, data_ . end () , dimension_ );
  begin_ . resize ( dimension_ + 1, end_value );
  end_ . resize ( dimension_ + 1, end_value );
  size_ . resize ( dimension_ + 1, 0 );
  for ( typename Cell_Complex::const_iterator cell = super_complex . begin (); cell != super_complex . end (); ++ cell )
    insert ( cell );
} /* Subcomplex_Container<>::initialize */

/* * * * * * * * * * * * * * * * * * * *
 * Subcomplex_const_iterator definitions  *
 * * * * * * * * * * * * * * * * * * * */

template < class Cell_Complex > 
Subcomplex_const_iterator<Cell_Complex>::Subcomplex_const_iterator ( void ) : referral_(NULL), dimension_(0) { 
} /* Subcomplex_const_iterator::Subcomplex_const_iterator */

template < class Cell_Complex > 
Subcomplex_const_iterator<Cell_Complex>::Subcomplex_const_iterator ( const Subcomplex_Container<Cell_Complex> * const referral, typename std::set < typename Cell_Complex::const_iterator >::const_iterator data, const unsigned int dimension ) : 
referral_(referral), data_(data), dimension_(dimension) {
} /* Subcomplex_const_iterator::Subcomplex_const_iterator */

template < class Cell_Complex > 
typename Subcomplex_Container<Cell_Complex>::value_type Subcomplex_const_iterator<Cell_Complex>::operator * ( void ) const {
	return * data_; 
} /* Subcomplex_const_iterator::operator * */

template < class Cell_Complex > 
Subcomplex_const_iterator<Cell_Complex> & Subcomplex_const_iterator<Cell_Complex>::operator ++ ( void ) {
	++ data_;
  if ( dimension_ < referral_ -> dimension_ && data_ == referral_ -> begin_ [ dimension_ + 1 ] . data_ ) ++ dimension_;
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
    return * reinterpret_cast < const unsigned long * > ( &data_ ) < * reinterpret_cast < const unsigned long * > ( & right_hand_side . data_ );
  else
    return dimension_ < right_hand_side . dimension_;
} /* Subcomplex_const_iterator::operator < */

template < class Cell_Complex > 
unsigned int Subcomplex_const_iterator<Cell_Complex>::dimension () const {
  return dimension_;
}  /* Subcomplex_const_iterator::dimension */

/* * * * * * * * * * * * * *
 * Subcomplex definitions  *
 * * * * * * * * * * * * * */
template < class Cell_Complex >
Subcomplex<Cell_Complex>::
Subcomplex ( const Cell_Complex & super_complex ) {
  initialize ( super_complex );
} /* Subcomplex<>::Subcomplex */
