/* Subcomplex.hpp
 * CHomP project
 * Shaun Harker 3/31/10
 */

template < class Cell_Complex >
Subcomplex<Cell_Complex>::
Subcomplex ( const Cell_Complex & super_complex ) : super_complex(super_complex) {
  dimension = super_complex . dimension;
  cells . resize ( dimension + 1 );
  unsigned long count = 0;
  for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) {
    std::cout << "dimension = " << dimension_index << "\n";
    for ( typename Cell_Complex::const_iterator cell = super_complex . cells [ dimension_index ] . begin (); 
         cell != super_complex . cells [ dimension_index ] . end (); ++ cell ) {
      cells [ dimension_index ] . insert ( *cell );
      ++ count;
      //std::cout << "Inserting " << *cell << "\n";
    }
    std::cout << "count = " << count << "\n";
  }
} /* Subcomplex */

template < class Cell_Complex >
typename Cell_Complex::Chain Subcomplex<Cell_Complex>::
project_chain ( const Chain & project_me ) const {
  Chain return_value;
  for ( typename Chain::const_iterator chain_term = project_me . begin (); chain_term != project_me . end (); ++ chain_term )
    if ( cells [ chain_term -> first . dimension ] . find ( chain_term -> first ) != cells [ chain_term -> first . dimension ] . end () ) 
      return_value += * chain_term;
  return return_value;
} /* project_chain */

template < class Cell_Complex >
typename Cell_Complex::Chain & Subcomplex<Cell_Complex>::
Boundary_Map ( Chain & boundary, const const_iterator & input) const {
  Chain super_boundary;
  super_complex . Boundary_Map ( super_boundary, *input );
  return boundary = project_chain ( super_boundary );
} /* Boundary_Map */

template < class Cell_Complex >
typename Cell_Complex::Chain & Subcomplex<Cell_Complex>::
Coboundary_Map ( Chain & coboundary, const const_iterator & input) const {
  Chain super_coboundary;
  super_complex . Coboundary_Map ( super_coboundary, *input );
  //std::cout << "super_coboundary = " << super_coboundary << "\n";
  return coboundary = project_chain ( super_coboundary );
} /* Coboundary_Map */

template < class Cell_Complex >
void Subcomplex<Cell_Complex>::
Remove_Cell ( const Cell & remove_me ) {
  //std::cout << "Removing " << 
  cells [ remove_me . dimension ] . erase ( remove_me );
  //<< " cells.\n";
} /* Remove_Cell */
