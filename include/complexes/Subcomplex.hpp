/* Subcomplex.hpp
 * CHomP project
 * Shaun Harker 3/31/10
 */

template < class Cell_Complex > 
std::pair<Subcomplex_Container<Cell_Complex>::iterator, bool> Subcomplex_Container<Cell_Complex>::insert ( const value_type & insert_me ) {
  std::pair<Subcomplex_Container<Cell_Complex>::iterator, bool> return_value;
  return_value = data_ . insert ( insert_me );
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
  unsigned int dimension = erase_me . dimension ();
  /* Update size_ */
  -- size_ [ dimension ]; 
  /* Update begin_, end_ */
  if ( erase_me == begin_ [ dimension ] ) {
    ++ begin_ [ dimension ];
    if ( dimension > 0 ) end_ [ dimension - 1 ] = begin_ [ dimension ];
  }
  data_ . erase ( erase_me );
} /* Subcomplex_Container<>::erase */

template < class Cell_Complex > 
Subcomplex_Container<Cell_Complex>::iterator Subcomplex_Container<Cell_Complex>::find ( const key_type & find_me ) const {
  data_ . find ( find_me );
} /* Subcomplex_Container<>::find */

template < class Cell_Complex > 
Subcomplex_Container<Cell_Complex>::iterator Subcomplex_Container<Cell_Complex>::begin ( unsigned int dimension ) const {
  return begin_ [ dimension ];
} /* Subcomplex_Container<>::begin */

template < class Cell_Complex > 
Subcomplex_Container<Cell_Complex>::iterator Subcomplex_Container<Cell_Complex>::end ( unsigned int dimension ) const {
  return end_ [ dimension ];
} /* Subcomplex_Container<>::end */

template < class Cell_Complex > 
size_type Subcomplex_Container<Cell_Complex>::size ( unsigned int dimension ) const {
  return size_ [ dimension ];
} /* Subcomplex_Container<>::size */

template < class Cell_Complex > 
Subcomplex_Container<Cell_Complex>::Chain Subcomplex_Container<Cell_Complex>::boundary ( const const_iterator & input ) const {
  typename Cell_Complex::Chain super_boundary = super_complex_ . boundary ( *input );
  return boundary = project ( super_boundary );
} /* Subcomplex_Container<>::boundary */

template < class Cell_Complex > 
Subcomplex_Container<Cell_Complex>::Chain Subcomplex_Container<Cell_Complex>::coboundary ( const const_iterator & input ) const {
  typename Cell_Complex::Chain super_coboundary = super_complex_ . coboundary ( *input );
  return project ( super_coboundary );
} /* Subcomplex_Container<>::coboundary */

template < class Cell_Complex > 
unsigned int Subcomplex_Container<Cell_Complex>::dimension ( void ) const {
  return super_complex_ . dimension ();
} /* Subcomplex_Container<>::dimension */

template < class Cell_Complex > 
Subcomplex_Container<Cell_Complex>::Chain Subcomplex_Container<Cell_Complex>::project ( const typename Cell_Complex::Chain & project_me ) const {
  Chain return_value;
  const_iterator projected;
  for ( typename Cell_Complex::Chain::const_iterator chain_term = project_me . begin (); chain_term != project_me . end (); ++ chain_term )
    if ( ( projected = find ( chain_term -> first ) ) != data_ . end () ) 
      return_value += Chain::Chain_Term ( projected, chain_term -> second );
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
Subcomplex<Cell_Complex>::
Subcomplex ( const Cell_Complex & super_complex ) : super_complex_(super_complex) {
  begin_ . resize ( super_complex_ . dimension (), data_ . end () );
  end_ . resize ( super_complex_ . dimension (), data_ . end () );
  size_ . resize ( super_complex_ . dimension (), 0 );
  for ( typename Cell_Complex::const_iterator cell = super_complex . begin (); cell != super_complex . end (); ++ cell )
    insert ( cell );
} /* Subcomplex<>::Subcomplex */

template < class Cell_Complex >
typename Cell_Complex::Chain Subcomplex<Cell_Complex>::
project_chain ( const Chain & project_me ) const {
  Chain return_value;
  for ( typename Chain::const_iterator chain_term = project_me . begin (); chain_term != project_me . end (); ++ chain_term )
    if ( cells [ chain_term -> first . dimension ] . find ( chain_term -> first ) != cells [ chain_term -> first . dimension ] . end () ) 
      return_value += * chain_term;
  return return_value;
} /* Subcomplex<>::project_chain */
