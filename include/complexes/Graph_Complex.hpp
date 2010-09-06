/*
 *  Graph_Complex.hpp
 *  
 *  Created by Shaun Harker on 6/7/10.
 *
 */

#include <set>
namespace Graph_detail {
  template < class Toplex >
  Sparse_Subcomplex < typename Toplex::Complex > closure ( const typename Toplex::Complex::const_iterator & cell_iterator, const typename Toplex::Complex & complex ) {
    Sparse_Subcomplex < typename Toplex::Complex > return_value ( complex );
    std::set < typename Toplex::Complex::const_iterator > working_set;
    working_set . insert ( cell_iterator );
    while ( not working_set . empty () ) {
      typename Toplex::Complex::const_iterator work_iterator = * working_set . begin ();
      if ( return_value . find ( work_iterator ) == return_value . end () ) {
        typename Toplex::Complex::Chain work_chain = complex . boundary ( work_iterator );
        for ( typename Toplex::Complex::Chain::const_iterator term_iterator = work_chain . begin ();
             term_iterator != work_chain . end (); ++ term_iterator ) {
          working_set . insert ( term_iterator -> first );
        } /* for */
        return_value . insert ( work_iterator );
      } /* if */
      working_set . erase ( work_iterator );
    } /* while */
    return return_value;
  } /* Graph_detail::closure */
} /* namespace Graph_detail */

template < class Toplex >
template < class Map >
Graph_Complex<Toplex>::Graph_Complex ( const Toplex & X, const Toplex & Y, const Map & f ) {
  typedef Sparse_Subcomplex < typename Toplex::Complex > Subcomplex;
  typedef Product_Complex < typename Toplex::Complex, typename Toplex::Complex > Product_Type;
  typedef Sparse_Subcomplex < Product_Type > Fiber;
  /* construct domain_, codomain_, product_, and inheritance */
  domain_ = new typename Toplex::Complex ( X . complex ( domain_boxes_ ) );
  codomain_ = new typename Toplex::Complex ( Y . complex ( codomain_boxes_ ) );
  product_ = new Product_Type ( *domain_, *codomain_ );
    
  Sparse_Subcomplex < Product_Complex < typename Toplex::Complex, typename Toplex::Complex > >::construct ( *product_ );
  /* construct the fibers of the graph */
  for ( typename Toplex::const_iterator domain_iterator = X . begin ();
       domain_iterator != X . end (); ++ domain_iterator ) {
    Subcomplex sub_X = Graph_detail::closure < Toplex > ( domain_boxes_ [ *domain_iterator ], *domain_ );
    typename Toplex::Subset fiber = Y . cover ( f ( X . geometry ( domain_iterator ) ) );
    for ( typename Toplex::Subset::const_iterator codomain_iterator = fiber . begin ();
           codomain_iterator != fiber . end (); ++ codomain_iterator ) {
      //std::cout << "Making box (" << * domain_iterator << ", " << * codomain_iterator << ")\n";
      Subcomplex sub_Y = Graph_detail::closure < Toplex > ( codomain_boxes_ [ * codomain_iterator ], *codomain_ );
      for ( typename Subcomplex::const_iterator X_iterator = sub_X . begin ();
            X_iterator != sub_X . end (); ++ X_iterator ) {
        for ( typename Subcomplex::const_iterator Y_iterator = sub_Y . begin ();
              Y_iterator != sub_Y . end (); ++ Y_iterator ) {
          typename Product_Type::Cell insert_me ( * X_iterator, * Y_iterator );
          typename Product_Type::const_iterator insert_iterator = product_ -> find ( insert_me );
          insert ( insert_iterator );
          //std::cout << "     inserting cell " << insert_me << "\n";
        } /* for */
      } /* for */
    } /* for */
  } /* for */
} /* Graph_Complex<>::Graph_Complex */

template < class Toplex >
Graph_Complex<Toplex>::~Graph_Complex ( void ) {
  delete product_;
  delete domain_;
  delete codomain_;
} /* Graph_Complex<>::~Graph_Complex */

template < class Toplex >
typename Toplex::Complex & 
Graph_Complex<Toplex>::domain ( void ) {
  return * domain_; 
} /* Graph_Complex<>::codomain */

template < class Toplex >
typename Toplex::Complex::Chain Graph_Complex<Toplex>::
projectToDomain ( const Chain & project_me ) {
  return product_ -> projectFirst ( include ( project_me ) );
} /* Graph_Complex<>::projectToCodomain */

template < class Toplex >
typename Toplex::Complex & 
Graph_Complex<Toplex>::codomain ( void ) {
  return * codomain_; 
} /* Graph_Complex<>::codomain */

template < class Toplex >
typename Toplex::Complex::Chain Graph_Complex<Toplex>::
projectToCodomain ( const Chain & project_me ) {
  return product_ -> projectSecond ( include ( project_me ) );
} /* Graph_Complex<>::projectToCodomain */
