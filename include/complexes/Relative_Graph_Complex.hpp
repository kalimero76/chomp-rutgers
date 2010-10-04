/*
 *  Relative_Graph_Complex.hpp
 *  
 *  Created by Shaun Harker on 9/28/10.
 *
 */

template < Combinatorial_Map >
Relative_Graph_Complex::Relative_Graph_Complex (const Toplex & T, 
                                                const typename Toplex::Subset X,
                                                const typename Toplex::Subset A,
                                                const typename Toplex::Subset Y,
                                                const typename Toplex::Subset B,
                                                const Combinatorial_Map & F ) : toplex_(T) {
  full_domain_ = toplex_ . complex ( X, X_boxes_ );
  full_codomain_ = toplex_ . complex ( Y, Y_boxes_ );
  /* For each cell in the "full_domain_" we record which top cells are involved in
     the combinatorial map calculation */
  neighbors_ = /* ... */
  /* Calculate the domain complex (X, A) */
  domain_ = /* ... */
  /* Calculate the codomain complex (Y, B) */
  codomain_ = /* ... */
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
  
  typedef Relative_Chain::value_type RelativeChainTerm;
  
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
    Relative_Chain domain_boundary;
    total_domain_ . boundary ( domain_boundary, domain_cell . name () );
    Relative_Complex::Ring coefficient = term . second * lift_term . second;
    BOOST_FOREACH ( RelativeChainTerm term, domain_boundary ) {
      answer_boundary [ term . first ] . insert ( RelativeChainTerm ( choice_iterator, coefficient ) );
    } /* boost_foreach */
  } /* boost_foreach */
  
  /* Main Loop: continually update "answer" and "answer_boundary" so that
       1) "answer_boundary" is always the boundary of "answer"
       2) "answer" is a Chain which projects to "lift_me"
       3) the loop terminates when "answer" is a cycle, eqv. "answer_boundary" == 0.
   */
  while ( not answer_boundary . empty () ) {
    /* Choose a cell in "answer_boundary" in largest dimensional fiber possible */
    FactorCell domain_cell = answer_boundary . rbegin () -> first;
    FactorComplex fiber = makeFiber ( domain_cell );
    /* Find the projection of the boundary terms into the fiber. */
    FactorChain fiber_chain = fiber . relative_chain ( answer_boundary . rbegin () -> second );
    /* Find the preboundary of "fiber_chain" within "fiber" */
    FactorChain negative_preboundary = -1 * morse::preboundary ( fiber_chain, fiber );
    /* Update "answer" and "answer_boundary" */
    answer [ domain_cell ] = negative_preboundary;
    answer_boundary . erase ( domain_cell );
    Relative_Chain domain_boundary = domain () . total_boundary ( domain_cell );
    BOOST_FOREACH ( FiberChainTerm term, domain_boundary ) {
      answer_boundary [ term . first ] += term . second * negative_preboundary;
      if ( answer_boundary [ term . first ] . empty ) answer_boundary . erase ( term . first );
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
    answer += iter -> second;
  } /* for */
  return codomain () . relative_chain ( answer );
} /* Relative_Graph_Complex<Toplex>::projectToCodomain */

/* Private Functions */

Relative_Complex Relative_Graph_Complex<Toplex>::makeFiber ( const FactorCell & domain_cell ) {
  /* Determine the set of top cells in X whose intersection is "domain_cell */
  Toplex::Subset neighbors;
  /* ... */
  /* Produce the union of the images of the "neighbors" */
  Toplex::Subset image;
  BOOST_FOREACH ( Toplex::Top_Cell neighbor, neighbors ) {
    image += F ( neighbor );
  } /* boost_foreach */
  /* Determine from image the appropriate subcomplex */
  // TODO
  /* Return the appropriate subcomplex */
  // TODO
  return ;
} /* Relative_Graph_Complex<Toplex>::makeFiber */
