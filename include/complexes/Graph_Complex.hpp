/*
 *  Graph_Complex.hpp
 *  
 *  Created by Shaun Harker on 6/7/10.
 *
 */

template < class Toplex >
template < class Map >
Graph_Complex<Toplex>::Graph_Complex ( const Toplex & X, const Toplex & Y, const Map & f ) :
domain_ ( X . complex () ), 
codomain_ ( Y . complex () ),
product_ ( domain_, codomain_ ) /* Must be declared in correct order */ {
  Sparse_Subcomplex < Product_Complex < 
  typename Toplex::Complex, typename Toplex::Complex > >::construct ( product_ );
  for ( typename Toplex::const_iterator domain_iterator = X . begin ();
       domain_iterator != X . end (); ++ domain_iterator ) {
    typedef Product_Complex < typename Toplex::Complex, typename Toplex::Complex > Product_Type;
    typename Toplex::Complex fiber_X = X . complex ( domain_iterator );
    typename Toplex::Complex fiber_Y = Y . complex ( Y . cover ( f ( X . geometry ( domain_iterator ) ) ) );
    Product_Type fiber_complex ( fiber_X, fiber_Y );
    for ( typename Product_Type::const_iterator cell_iterator = fiber_complex . begin ();
         cell_iterator != fiber_complex . end (); ++ cell_iterator ) {
      typename Graph_Complex::Cell insert_me = product_ . find ( * cell_iterator );
      std::cout << "Inserting " << * insert_me << " into graph complex.\n";
      std::pair<typename Graph_Complex::iterator, bool> insert_value =
        insert ( insert_me ); 
      if ( insert_value . second == true ) std::cout << "actually inserted " << * * insert_value . first << "\n";
      else std::cout << "already have inserted " << * * insert_value . first << "\n";
      
      /* A Graph_Cell is a Sparse_Subcomplex Cell
         is a Product_iterator*/
    } /* for */
  } /* for */
} /* Graph_Complex<>::Graph_Complex */

template < class Toplex >
typename Toplex::Complex & 
Graph_Complex<Toplex>::codomain ( void ) {
  return codomain_; 
} /* Graph_Complex<>::codomain */

template < class Toplex >
typename Toplex::Complex::Chain Graph_Complex<Toplex>::
projectToCodomain ( const Chain & project_me ) {
  return product_ . projectSecond ( include ( project_me ) );
} /* Graph_Complex<>::projectToCodomain */
