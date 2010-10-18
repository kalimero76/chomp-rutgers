/*
 *  Relative_Graph_Complex.hpp
 *  
 *  Created by Shaun Harker on 9/28/10.
 *
 */

namespace Relative_Graph_Complex_detail {
void closure_in_subcomplex ( std::set < Relative_Cell > & return_value, const FactorComplex & complex, const std::set < Relative_Cell > & cells_to_close ) {

  /* Introduce the top cells */
  BOOST_FOREACH ( Relative_Cell & cell, cells_to_close ) {
    return_value . insert ( cell );
  } /* boost_foreach */

  std::set<Relative_Cell> work_set = cells_to_close;

  /* Now produce the closure */
  while ( not work_set . empty () ) {
    std::set<Relative_Cell> new_work_set;
    BOOST_FOREACH ( Relative_Cell & cell, work_set ) {
      FactorComplex::const_iterator it = complex . lookup ( cell . name () );
      FactorChain boundary_chain = complex . boundary ( it );
      BOOST_FOREACH ( FactorChainTerm & term, boundary_chain ) {
        Relative_Cell bd_cell ( complex . index ( term . first ), term . first -> dimension () );
        new_work_set . insert ( bd_cell );
        return_value . insert ( bd_cell );
      } /* boost_foreach */
    } /* boost_foreach */
    std::swap ( work_set, new_work_set );
    new_work_set . clear ();
  } /* while */
  return;
} /* */
  
} /* namespace Relative_Graph_Complex_detail */


template < Combinatorial_Map >
Relative_Graph_Complex::Relative_Graph_Complex (const Toplex & T, 
                                                const typename Toplex::Subset X,
                                                const typename Toplex::Subset A,
                                                const typename Toplex::Subset Y,
                                                const typename Toplex::Subset B,
                                                const Combinatorial_Map & F ) : toplex_(T) {
  using namespace Relative_Graph_Complex_detail;

  typedef typename Toplex::Top_Cell Top_Cell;
  typedef typename Toplex::Complex Complex;

  /* Generate the full_domain and full_codomain complexes */
  full_domain_ = toplex_ . complex ( X, X_boxes_ );
  full_codomain_ = toplex_ . complex ( Y, Y_boxes_ );

  /* Produce inverse of X_boxes */
  BOOST_FOREACH ( std::pair < Top_Cell, Complex::const_iterator > & term, X_boxes_ ) {
    X_boxes_inv_ [ term . second ] = term . first;
  } /* boost_foreach */
  
  /* Morse Decompositions (AKQ) are necessary */
  
  full_domain_ . decompose ();
  full_codomain_ . decompose ();
  
  /* For each cell in the "full_domain_" we record which top cells are involved in
     the combinatorial map calculation */
  
  /* Calculate the domain complex (X, A) */
  std::set < Relative_Cell > X_cells, A_cells;
  closure_in_subcomplex ( X_cells, full_domain_, X );
  closure_in_subcomplex ( A_cells, full_domain_, A );
  BOOST_FOREACH ( Relative_Cell & cell, X_cells ) {
    domain_ . insert ( cell );
  } /* boost_foreach */
  BOOST_FOREACH ( Relative_Cell & cell, A_cells ) {
    domain_ . erase ( cell );
  } /* boost_foreach */

  /* Calculate the codomain complex (Y, B) */
  std::set < Relative_Cell > Y_cells, B_cells;
  closure_in_subcomplex ( Y_cells, full_codomain_, Y );
  closure_in_subcomplex ( B_cells, full_codomain_, B );
  BOOST_FOREACH ( Relative_Cell & cell, Y_cells ) {
    codomain_ . insert ( cell );
  } /* boost_foreach */
  BOOST_FOREACH ( Relative_Cell & cell, B_cells ) {
    codomain_ . erase ( cell );
  } /* boost_foreach */
} /* Relative_Graph_Complex<Toplex>::Relative_Graph_Complex */

Relative_Graph_Complex<Toplex>::~Relative_Graph_Complex ( void ) {
} /* Relative_Graph_Complex<Toplex>::~Relative_Graph_Complex */

Relative_Complex & Relative_Graph_Complex<Toplex>::domain ( void ) {
  return domain_;
} /* Relative_Graph_Complex<Toplex>::domain */

Relative_Complex & Relative_Graph_Complex<Toplex>::codomain ( void ) {
  return codomain_;
} /* Relative_Graph_Complex<Toplex>::codomain */

Chain Relative_Graph_Complex<Toplex>::cycleLift ( const Relative_Chain & lift_me ) {
  /* Computes a Chain in the Graph which 
      1) is a cycle
      2) projects to "lift_me"
   */
  
  typedef FactorChain::value_type FactorChainTerm;
  typedef Relative_Chain::value_type RelativeChainTerm;
  typedef std::vector < std::pair <size_type, Ring > > Index_Chain;
  
  /* Initialize "answer" to a Chain that projects to "lift_me",
     and compute its boundary as well. */
  Chain answer;
  Chain answer_boundary;
  
  BOOST_FOREACH ( RelativeChainTerm lift_term, lift_me ) {
    Relative_Cell domain_cell = * lift_term . first;
    Relative_Complex fiber = makeFiber ( domain_cell );
    Relative_Chain fiber_chain;
    Relative_Complex::const_iterator choice_iterator = fiber . begin ();
    fiber_chain . insert ( RelativeChainTerm ( choice_iterator, lift_term . second ) );
    answer [ domain_cell ] = fiber_chain;
    FactorChain domain_boundary = full_domain_ . boundary ( full_domain_ . lookup ( domain_cell . name () ) );
    Relative_Complex::Ring coefficient = term . second * lift_term . second;
    BOOST_FOREACH ( FactorChainTerm term, domain_boundary ) {
      Relative_Cell fiber_name ( full_domain_ . index ( term . first ), term . first -> dimension () );
      answer_boundary [ fiber_name ] . insert ( RelativeChainTerm ( choice_iterator, coefficient ) );
    } /* boost_foreach */
  } /* boost_foreach */
  
  /* Main Loop: continually update "answer" and "answer_boundary" so that
       1) "answer_boundary" is always the boundary of "answer"
       2) "answer" is a Chain which projects to "lift_me"
       3) the loop terminates when "answer" is a cycle, eqv. "answer_boundary" == 0.
   */
  while ( not answer_boundary . empty () ) {
    /* Choose a cell in "answer_boundary" in largest dimensional fiber possible */
    Relative_Cell domain_cell = * answer_boundary . rbegin () -> first;
    Relative_Complex fiber = makeFiber ( domain_cell );
    /* Find the projection of the boundary terms into the fiber. */
    Relative_Chain fiber_chain = fiber . relative_chain ( answer_boundary . rbegin () -> second );
    /* Find the preboundary of "fiber_chain" within "fiber" */
    Relative_Chain negative_preboundary = -1 * morse::preboundary ( fiber_chain, fiber );
    /* Update "answer" and "answer_boundary" */
    answer [ domain_cell ] = negative_preboundary;
    answer_boundary . erase ( domain_cell );
    FactorChain domain_boundary = full_domain_ . boundary ( full_domain_ . lookup ( domain_cell . name () ) );
    BOOST_FOREACH ( FactorChainTerm term, domain_boundary ) {
      Relative_Cell fiber_name ( full_domain_ . index ( term . first ), term . first -> dimension () );
      answer_boundary [ fiber_name ] += term . second * negative_preboundary;
      if ( answer_boundary [ fiber_name ] . empty () ) answer_boundary . erase ( fiber_name );
    } /* boost_foreach */ 
  } /* while */
    
  /* Return with the computed cycle "answer" which projects to "lift_me" */
  return answer;
  
} /* Relative_Graph_Complex<Toplex>::cycleLift */

Relative_Chain Relative_Graph_Complex<Toplex>::projectToDomain ( const Chain & project_me ) {
  Relative_Chain answer;
  std::cout << "ERROR,  Relative_Graph_Complex<Toplex>::projectToDomain not implemented. n";
  return answer;
} /* Relative_Graph_Complex<Toplex>::projectToDomain */

Relative_Chain Relative_Graph_Complex<Toplex>::projectToCodomain ( const Chain & project_me ) {
  Relative_Chain answer;
  for ( Chain::const_iterator iter = project_me . begin (); iter -> first . dimension () == 0; ++ iter ) {
    answer += codomain () . project ( iter -> second );
  } /* for */
  return answer;
} /* Relative_Graph_Complex<Toplex>::projectToCodomain */

/* Private Functions */

Relative_Complex Relative_Graph_Complex<Toplex>::makeFiber ( const Relative_Cell & domain_cell ) {
  /* Determine the set of top cells in X whose intersection is "domain_cell */
  Toplex::Subset neighbors;
  
  /* Collect the top-cells which can be reached through coboundary */
  std::set<Relative_Cell> work_set;
  work_set . insert ( domain_cell );
  while ( work_set . begin () -> dimension () < full_domain_ . dimension () ) {
    std::set<Relative_Cell> new_work_set;
    BOOST_FOREACH ( Relative_Cell & cell, work_set ) {
      FactorComplex::const_iterator it = full_domain_ . lookup ( cell . name () );
      FactorChain coboundary_chain = full_domain_ . coboundary ( it );
      BOOST_FOREACH ( FactorChainTerm & term, coboundary_chain ) {
        Relative_Cell cbd_cell ( full_domain_ . index ( term . first ), term . first -> dimension () );
        new_work_set . insert ( cbd_cell );
      } /* boost_foreach */
    } /* boost_foreach */
    std::swap ( work_set, new_work_set );
    new_work_set . clear ();
  } /* while */
  
  /* Produce the Top cells which are neighbors of domain_cell */
  BOOST_FOREACH ( Relative_Cell & cell, work_set ) {
    FactorComplex::const_iterator it = full_domain_ . lookup ( cell . name () );
    neighbors . insert ( X_Boxes_inv_ [ it ] );
  } /* boost_foreach */
  
  /* Produce the union of the images of the "neighbors" */
  Toplex::Subset image;
  BOOST_FOREACH ( Toplex::Top_Cell neighbor, neighbors ) {
    image . insert ( F ( neighbor ) );
  } /* boost_foreach */
  
  /* Determine from image the largest cells in subcomplex */
  Relative_Complex return_value;
  
  std::set<Relative_Cell> work_set;
  BOOST_FOREACH ( Toplex::Top_Cell image_box, image ) {
    FactorComplex::const_iterator it = Y_Boxes_ [ image_box ];
    Relative_Cell cell ( full_domain_ . index ( it ), it . dimension () );
    work_set . insert ( cell );
  } /* boost_foreach */
  
  /* Determine the Closure */
  std::set<Relative_Cell> closure_set;
  Relative_Graph_Complex_detail::closure_in_subcomplex ( closure_set, full_domain_, work_set );
  
  /* Assemble the subcomplex */
  BOOST_FOREACH ( Relative_Cell & cell, closure_set ) {
    return_value . insert ( cell );
  } /* boost_foreach */
  
  /* Return the appropriate subcomplex */
  return return_value;
} /* Relative_Graph_Complex<Toplex>::makeFiber */
