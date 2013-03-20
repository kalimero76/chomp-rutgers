/*
 *  Relative_Graph_Complex.hpp
 *  
 *  Created by Shaun Harker on 9/28/10.
 *
 */

#include <iterator>
#include <ctime>

#include "complexes/Adaptive_Complex.h" // debug
#include "tools/visualization.h"

namespace Relative_Graph_Complex_detail {
template < class Relative_Cell, class Complex >
void closure_in_subcomplex ( std::set < Relative_Cell > & return_value, 
                             const Complex & complex, 
                             const std::set < Relative_Cell > & cells_to_close ) {

  /* Introduce the top cells */
  BOOST_FOREACH ( const Relative_Cell & cell, cells_to_close ) {
    return_value . insert ( cell );
  } /* boost_foreach */

  std::set<Relative_Cell> work_set = cells_to_close;

  /* Now produce the closure */
  while ( not work_set . empty () ) {
    std::set<Relative_Cell> new_work_set;
    BOOST_FOREACH ( const Relative_Cell & cell, work_set ) {
      typename Complex::const_iterator it = complex . lookup ( cell . data () );
      typename Complex::Chain boundary_chain = complex . boundary ( it );
      BOOST_FOREACH ( typename Complex::Chain::value_type & term, boundary_chain ) {
        Relative_Cell bd_cell ( complex . index ( term . first ), term . first . dimension () );
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

#include "algorithms/basic.h"

template < class Toplex, class Combinatorial_Map >
template < class Subset >
Relative_Graph_Complex<Toplex,Combinatorial_Map>::Relative_Graph_Complex (const Toplex & T, 
                                                const Subset X,
                                                const Subset A,
                                                const Subset Y,
                                                const Subset B,
                                                const Combinatorial_Map & F ) : toplex_(T), A_(A.begin(), A.end() ), F_(F) {
  using namespace Relative_Graph_Complex_detail;

  typedef typename Toplex::Top_Cell Top_Cell;
  typedef typename Toplex::Complex Complex;

  clock_t start = clock ();
  //std::cout << "Relative Graph Complex constructor.\n";
  
  /* Generate the full_domain and full_codomain complexes */
  //std::cout << "Generating full_domain_.\n";
  toplex_ . complex ( full_domain_, X . begin(), X . end (), X_boxes_ );
  //std::cout << "Generating full_codomain_.\n";
  toplex_ . complex ( full_codomain_, Y . begin (), Y . end (), Y_boxes_ );

  //std::cout << "Elapsed time: " << (float)( clock () - start ) / (float) CLOCKS_PER_SEC << "\n";
  start = clock ();

  /* Produce inverse of X_boxes */
  //std::cout << "Generating inverse X_boxes.\n";
  typedef const std::pair < Top_Cell, typename Complex::const_iterator > Term;
  BOOST_FOREACH ( Term & term, X_boxes_ ) {
    X_boxes_inv_ [ term . second ] = term . first;
  } /* boost_foreach */
  
  //std::cout << "Elapsed time: " << (float)( clock () - start ) / (float) CLOCKS_PER_SEC << "\n";
  start = clock ();

  /* Morse Decompositions (AKQ) are necessary */
  
  //std::cout << "Constructing Morse Decompositions of domain and codomain...\n";
  full_domain_ -> decompose ();
  //std::cout << "Domain decomposition complete.\n";
  full_codomain_ -> decompose ();
  //std::cout << "Codomain decomposition complete.\n";
  
  //std::cout << "Elapsed time: " << (float)( clock () - start ) / (float) CLOCKS_PER_SEC << "\n";
  start = clock ();

  /* Initialize the subcomplex domain_ and codomain_ */
  //std::cout << "Constructing subcomplexes (relative complexes) for domain and codomain.\n";
  domain_ . construct ( full_domain_ );
  codomain_ . construct ( full_codomain_ );
  
  //std::cout << "Elapsed time: " << (float)( clock () - start ) / (float) CLOCKS_PER_SEC << "\n";
  start = clock ();

  /* For each cell in the "full_domain_" we record which top cells are involved in
     the combinatorial map calculation */
  
  //DEBUG
  //Relative_Complex test_full;
  //test_full . construct ( full_domain_ );
  /* Calculate the domain complex (X, A) */
  //std::cout << "Calculating Domain complex (X, A)\n";
  
  unsigned int X_dimension = full_domain_ -> dimension ();
  unsigned int Y_dimension = full_codomain_ -> dimension ();
  std::set < Relative_Cell > X_cells, X_top_cells, A_cells, A_top_cells;
  BOOST_FOREACH ( const Top_Cell & cell, X ) {
    X_top_cells . insert ( 
                          Relative_Cell ( full_domain_ -> index ( X_boxes_ [ cell ] ), 
                                         X_dimension ) );
  }
  //std::cout << "Elapsed time: " << (float)( clock () - start ) / (float) CLOCKS_PER_SEC << "\n";
  start = clock ();
  
  BOOST_FOREACH ( const Top_Cell & cell, A ) {
    A_top_cells . insert ( 
                          Relative_Cell ( full_domain_ -> index ( X_boxes_ [ cell ] ), 
                                         X_dimension ) );
  }
  //std::cout << "Elapsed time: " << (float)( clock () - start ) / (float) CLOCKS_PER_SEC << "\n";
  start = clock ();
  
  closure_in_subcomplex ( X_cells, *full_domain_, X_top_cells );
  
  //std::cout << "Elapsed time: " << (float)( clock () - start ) / (float) CLOCKS_PER_SEC << "\n";
  start = clock ();
  
  closure_in_subcomplex ( A_cells, *full_domain_, A_top_cells );
  
  //std::cout << "Elapsed time: " << (float)( clock () - start ) / (float) CLOCKS_PER_SEC << "\n";
  start = clock ();
  
  BOOST_FOREACH ( const Relative_Cell & cell, X_cells ) {
    //std::cout << "X cell " << cell . data () << " " << * full_domain_ -> lookup ( cell . data () ) << "\n";
    domain_ . insert ( cell, false );
    //test_full . insert ( cell );
  } /* boost_foreach */
  
  //std::cout << "Elapsed time: " << (float)( clock () - start ) / (float) CLOCKS_PER_SEC << "\n";
  start = clock ();
  
  BOOST_FOREACH ( const Relative_Cell & cell, A_cells ) {
    //std::cout << "A cell " << cell . data () << " " << * full_domain_ -> lookup ( cell . data () ) << "\n";
    domain_ . erase ( cell, false );
  } /* boost_foreach */

  //std::cout << "Elapsed time: " << (float)( clock () - start ) / (float) CLOCKS_PER_SEC << "\n";
  start = clock ();

  domain_ . finalize ();

  //std::cout << "Elapsed time: " << (float)( clock () - start ) / (float) CLOCKS_PER_SEC << "\n";
  start = clock ();
  
  /* Calculate the codomain complex (Y, B) */
  //std::cout << "Calculating Codomain complex (Y, B)\n";
  std::set < Relative_Cell > Y_cells, Y_top_cells, B_cells, B_top_cells;
  BOOST_FOREACH ( const Top_Cell & cell, Y ) {
    Y_top_cells . insert ( 
                          Relative_Cell ( full_codomain_ -> index ( Y_boxes_ [ cell ] ), 
                                         Y_dimension ) );
  }
  BOOST_FOREACH ( const Top_Cell & cell, B ) {
    B_top_cells . insert ( 
                          Relative_Cell ( full_codomain_ -> index ( Y_boxes_ [ cell ] ), 
                                         Y_dimension ) );
  }  
  closure_in_subcomplex ( Y_cells, *full_codomain_, Y_top_cells );
  closure_in_subcomplex ( B_cells, *full_codomain_, B_top_cells );
  BOOST_FOREACH ( const Relative_Cell & cell, Y_cells ) {
    codomain_ . insert ( cell, false );
  } /* boost_foreach */
  BOOST_FOREACH ( const Relative_Cell & cell, B_cells ) {
    codomain_ . erase ( cell, false );
  } /* boost_foreach */

  //std::cout << "Elapsed time: " << (float)( clock () - start ) / (float) CLOCKS_PER_SEC << "\n";
  start = clock ();

  codomain_ . finalize ();

  //std::cout << "Elapsed time: " << (float)( clock () - start ) / (float) CLOCKS_PER_SEC << "\n";
  start = clock ();
  /* debug */
//#define RGVISUALIZE_DEBUG0
#ifdef RGVISUALIZE_DEBUG0
  {
    Adaptive_Complex & complex = * full_domain_;
    ComplexVisualization < Adaptive_Complex > * cv = new ComplexVisualization < Adaptive_Complex > ( "Domain of Graph");
    cv -> drawRelativeComplex ( complex, domain (), 0, 100 );
    cv -> explore ();
    delete cv;
  }  
#endif
/*
  std::cout << "HERE IS FULL DOMAIN:\n";
  utility::inspect_complex ( test_full );
  std::cout << "END LISTING\n";
  */
} /* Relative_Graph_Complex<Toplex,Combinatorial_Map>::Relative_Graph_Complex */

template < class Toplex, class Combinatorial_Map >
Relative_Graph_Complex<Toplex,Combinatorial_Map>::~Relative_Graph_Complex ( void ) {
  delete full_domain_;
  delete full_codomain_;
} /* Relative_Graph_Complex<Toplex,Combinatorial_Map>::~Relative_Graph_Complex */

template < class Toplex, class Combinatorial_Map >
typename Relative_Graph_Complex<Toplex,Combinatorial_Map>::Relative_Complex & 
Relative_Graph_Complex<Toplex,Combinatorial_Map>::domain ( void ) {
  return domain_;
} /* Relative_Graph_Complex<Toplex,Combinatorial_Map>::domain */

template < class Toplex, class Combinatorial_Map >
typename Relative_Graph_Complex<Toplex,Combinatorial_Map>::Relative_Complex & 
Relative_Graph_Complex<Toplex,Combinatorial_Map>::codomain ( void ) {
  return codomain_;
} /* Relative_Graph_Complex<Toplex,Combinatorial_Map>::codomain */

template < class Toplex, class Combinatorial_Map >
int
Relative_Graph_Complex<Toplex,Combinatorial_Map>::cycleLift ( Chain * lifted_chain, const Relative_Chain & lift_me ) {
  /* Computes a Chain in the Graph which 
      1) is a cycle
      2) projects to "lift_me"
   */
  
  //std::cout << "CYCLELIFT! Running cycleLift on chain " << lift_me << "\n";
  
  typedef typename FactorChain::value_type FactorChainTerm;
  typedef typename Relative_Chain::value_type RelativeChainTerm;
  typedef std::vector < std::pair <size_type, Ring > > Index_Chain;
  
  /* Initialize "answer" to a Chain that projects to "lift_me",
     and compute its boundary as well. */
  Chain & answer = *lifted_chain;
  Chain answer_boundary;
  
  //std::cout << "STAGE I. CHAIN LIFT.\n";
  BOOST_FOREACH ( RelativeChainTerm lift_term, lift_me ) {
    Relative_Cell domain_cell = * lift_term . first;
    Relative_Complex fiber;
    //std::cout << "Lifting " << domain_cell << "\n";
    makeFiber ( fiber, domain_cell );
    //std::cout << "Inspecting the graph fiber\n";
    //utility::inspect_complex ( fiber );
    Relative_Chain fiber_chain;
    typename Relative_Complex::const_iterator choice_iterator = fiber . begin ();
    //std::cout << "Dimension of choice_iterator = " << choice_iterator . dimension () << "\n";
    fiber_chain . insert ( RelativeChainTerm ( choice_iterator, lift_term . second ) );
    answer [ domain_cell ] = fiber_chain;
    FactorChain domain_boundary = full_domain_ -> boundary ( full_domain_ -> lookup ( domain_cell . data () ) );

    BOOST_FOREACH ( FactorChainTerm term, domain_boundary ) {
      Relative_Cell fiber_name ( full_domain_ -> index ( term . first ), term . first . dimension () );
      answer_boundary [ fiber_name ] += RelativeChainTerm ( choice_iterator, 
                                                           term . second * lift_term . second );
    } /* boost_foreach */
//#define RGVISUALIZE_DEBUG
#ifdef RGVISUALIZE_DEBUG
    {
      Adaptive_Complex & complex = * full_domain_;
      ComplexVisualization < Adaptive_Complex > * cv = new ComplexVisualization < Adaptive_Complex > ( "Consistency Check: Projection of fiber into domain");
      cv -> drawRelativeComplex ( complex, domain (), 0, 100 );
      cv -> drawSpecialCell ( complex, * domain () . include ( domain_cell ), 200 );
    BOOST_FOREACH ( FactorChainTerm term, domain_boundary ) {
      Relative_Cell fiber_name ( full_domain_ -> index ( term . first ), term . first . dimension () );
      cv -> drawSpecialCell ( complex, * domain () . include ( fiber_name ), 200 );
      //std::cout << "<" << fiber_name << "> ";
    } /* boost_foreach */
      // explore a minute here
      cv -> explore ();
      delete cv;
    }  
#endif
  } /* boost_foreach */
  
  //std::cout << "   initial chain lift: "; printChain ( answer ); std::cout << "\n";
  //std::cout << "   boundary of initial chain lift = "; printChain ( answer_boundary ); std::cout << "\n";
  
  /* Main Loop: continually update "answer" and "answer_boundary" so that
       1) "answer_boundary" is always the boundary of "answer"
       2) "answer" is a Chain which projects to "lift_me"
       3) the loop terminates when "answer" is a cycle, eqv. "answer_boundary" == 0.
   */
  //std::cout << "STAGE 2. PREBOUNDARY LINKING.\n";
  while ( not answer_boundary . empty () ) {
    //std::cout << "   passing through loop...\n";
    /* Choose a cell in "answer_boundary" in largest dimensional fiber possible */
    Relative_Cell domain_cell = answer_boundary . rbegin () -> first;
    //std::cout << "---> Analyzing " << domain_cell . dimension () << " dimensional fiber ( " << domain_cell << "\n";
    Relative_Complex fiber;
    makeFiber ( fiber, domain_cell );
    //std::cout << "    considering fiber indexed by domain_cell = " << domain_cell << "\n";
    //std::cout << " Here is the " << domain_cell << " fiber: \n";
    //utility::inspect_complex ( fiber );
    //std::cout << " Done inspecting\n";
    /* Find the projection of the boundary terms into the fiber. */
    Relative_Chain fiber_chain = fiber . project ( answer_boundary . rbegin () -> second );
    //std::cout << "    the projection of answer_boundary in this fiber is " << fiber_chain << "\n";
    /* Find the preboundary of "fiber_chain" within "fiber" */
    Relative_Chain negative_preboundary = morse::preboundary ( fiber_chain, fiber );
    negative_preboundary *= -1;
#ifdef RGVISUALIZE_DEBUG
    {
      Adaptive_Complex & complex = * full_codomain_;
      ComplexVisualization < Adaptive_Complex > * cv = new ComplexVisualization < Adaptive_Complex > ( "Preboundary chain in fiber!");
      cv -> drawRelativeComplex ( complex, fiber, 0, 100 );
      cv -> drawChain ( complex, fiber . include ( fiber_chain ), 200 );
      cv -> drawChain ( complex, fiber . include ( negative_preboundary ), 200 );
      // explore a minute here
      cv -> explore ();
      delete cv;
    }
#endif
    
    //DEBUG
    Relative_Chain check = boundary ( negative_preboundary, fiber );
    check = sum < Relative_Complex > ( check, fiber_chain );
    if ( not check . empty () ) {
      std::cout << "PROBLEM WITH PREBOUNDARY IN CYCLE LIFT!\n";
      std::cout << "fiber_chain = " << fiber_chain << "\n";
      std::cout << "negative_preboundary = " << negative_preboundary << "\n";
      std::cout << "bd(neg_preboundary) = " << boundary ( negative_preboundary, fiber ) << "\n";
      std::cout << " should be zero: " << check << "\n";
      {
        Adaptive_Complex & complex = * full_codomain_;
        ComplexVisualization < Adaptive_Complex > * cv = new ComplexVisualization < Adaptive_Complex > ( "Preboundary chain in fiber!");
        cv -> drawRelativeComplex ( complex, fiber, 0, 100 );
        cv -> drawChain ( complex, fiber . include ( fiber_chain ), 200 );
        cv -> drawChain ( complex, fiber . include ( negative_preboundary ), 200 );
        // explore a minute here
        cv -> explore ();
        delete cv;
      }
      return 1; // error code
    }
    //std::cout << "   Here is the discovered preboundary: \n";
    //std::cout << "    " << domain_cell << " X (" << negative_preboundary << ")\n"; 
    /* Update "answer" and "answer_boundary" */
    answer [ domain_cell ] = negative_preboundary;
    answer_boundary . erase ( domain_cell );
    FactorChain domain_boundary = full_domain_ -> boundary ( full_domain_ -> lookup ( domain_cell . data () ) );
    BOOST_FOREACH ( FactorChainTerm term, domain_boundary ) {
      Relative_Cell fiber_name ( full_domain_ -> index ( term . first ), term . first . dimension () );
      Relative_Chain summand = negative_preboundary;
      summand *= term . second;
      answer_boundary [ fiber_name ] += summand;
      if ( answer_boundary [ fiber_name ] . empty () ) answer_boundary . erase ( fiber_name );
    } /* boost_foreach */ 
    //std::cout << "   current chain lift: "; printChain ( answer ); std::cout << "\n";
    //std::cout << "   boundary of current chain lift = "; printChain ( answer_boundary ); std::cout << "\n";
    
  } /* while */
    
  //std::cout << "   returning with answer = "; printChain ( answer ); std::cout << "\n";
  //std::cout << "   projected answer = "<< projectToDomain ( answer ) << "\n";
  //std::cout << "   should be same as: " << lift_me << "\n";
  /* Return with the computed cycle "answer" which projects to "lift_me" */
  return 0;
  
} /* Relative_Graph_Complex<Toplex,Combinatorial_Map>::cycleLift */

template < class Toplex, class Combinatorial_Map >
typename Relative_Graph_Complex<Toplex,Combinatorial_Map>::Relative_Chain 
Relative_Graph_Complex<Toplex,Combinatorial_Map>::projectToDomain ( const Chain & project_me ) {
  Relative_Chain answer;
  for ( typename Chain::const_iterator iter = project_me . begin (); iter != project_me . end (); ++ iter ) {
    if ( not iter -> second . empty () && iter -> second . begin () -> first . dimension () == 0 ) {
      Ring coefficient = 0;
      BOOST_FOREACH ( const typename Relative_Chain::value_type & term, iter -> second ) {
        coefficient += term . second;
      } /* boost_foreach */
      answer . insert ( typename Relative_Chain::value_type ( domain () . find ( iter -> first ), coefficient ) );
    } /* if */
  } /* for */  

  return answer;
} /* Relative_Graph_Complex<Toplex,Combinatorial_Map>::projectToDomain */

template < class Toplex, class Combinatorial_Map >
typename Relative_Graph_Complex<Toplex,Combinatorial_Map>::Relative_Chain 
Relative_Graph_Complex<Toplex,Combinatorial_Map>::projectToCodomain ( const Chain & project_me ) {
  Relative_Chain answer;
  for ( typename Chain::const_iterator iter = project_me . begin (); iter -> first . dimension () == 0; ++ iter ) {
    answer += codomain () . project ( iter -> second );
  } /* for */
  return answer;
} /* Relative_Graph_Complex<Toplex,Combinatorial_Map>::projectToCodomain */

template < class Toplex, class Combinatorial_Map >
void Relative_Graph_Complex<Toplex,Combinatorial_Map>::printChain ( const Chain & print_me ) {
  BOOST_FOREACH ( typename Chain::value_type term, print_me ) {
    std::cout << " ++ " << term . first << " X (" << term . second << ")\n";
  } /* boost_foreach */
} /* Relative_Graph_Complex<Toplex,Combinatorial_Map>::printChain */

/* Private Functions */

template < class Toplex, class Combinatorial_Map >
void Relative_Graph_Complex<Toplex,Combinatorial_Map>::makeFiber ( Relative_Complex & return_value, 
                                                                   const Relative_Cell & domain_cell ) {
  /* Determine the set of top cells in (X, A) whose intersection is "domain_cell */
  // note: X neighbors are such top cells in X; 
  //       A_neighbors are such top cells in A
  //       So A_neighbors is included in X_neighbors, since A is included in X
  typedef boost::unordered_set<typename Toplex::Top_Cell> HashSubset;
  HashSubset X_neighbors, A_neighbors;
  
  // DEBUG
  //std::cout << "makeFiber ( " << domain_cell << ") \n";
  
#ifdef RGVISUALIZE_DEBUG
  /*
  {
    Adaptive_Complex & complex = * full_domain_;
    ComplexVisualization < Adaptive_Complex > * cv0 = new ComplexVisualization < Adaptive_Complex > ( "fiber in domain");
    cv0 -> drawRelativeComplex ( complex, domain (), 0, 100 );
    cv0 -> drawCell ( complex, * domain () . include ( domain_cell ), 200 );
    cv0 -> drawNow ();
    // explore a minute here
    cv0 -> explore ();
    delete cv0;
  }
   */
#endif
  
  /* Collect the top-cells which can be reached through coboundary */
  std::set<Relative_Cell> work_set;
  work_set . insert ( domain_cell );
  while ( work_set . begin () -> dimension () < full_domain_ -> dimension () ) {
    std::set<Relative_Cell> new_work_set;
    BOOST_FOREACH ( const Relative_Cell & cell, work_set ) {
      typename FactorComplex::const_iterator it = full_domain_ -> lookup ( cell . data () );
      FactorChain coboundary_chain = full_domain_ -> coboundary ( it );
      BOOST_FOREACH ( typename FactorChain::value_type & term, coboundary_chain ) {
        Relative_Cell cbd_cell ( full_domain_ -> index ( term . first ), term . first . dimension () );
        new_work_set . insert ( cbd_cell );
      } /* boost_foreach */
    } /* boost_foreach */
    std::swap ( work_set, new_work_set );
    new_work_set . clear ();
  } /* while */
  
  /* Produce the Top cells which are neighbors of domain_cell */
  BOOST_FOREACH ( const Relative_Cell & cell, work_set ) {
    typename FactorComplex::const_iterator it = full_domain_ -> lookup ( cell . data () );
    typename Toplex::Top_Cell top_cell = X_boxes_inv_ [ it ];
    X_neighbors . insert ( top_cell );
    if ( A_ . find ( top_cell ) != A_ . end () ) {
      A_neighbors . insert ( top_cell );
    } /* if */
  } /* boost_foreach */
  
  //DEBUG
  /*
  std::cout << "X_neighbors = ";
  BOOST_FOREACH ( typename Toplex::Top_Cell neighbor, X_neighbors ) {
    std::cout << neighbor << " ";
  }
  std::cout << "\n";

  std::cout << "A_neighbors = ";
  BOOST_FOREACH ( typename Toplex::Top_Cell neighbor, A_neighbors ) {
    std::cout << neighbor << " ";
  }
  std::cout << "\n";
  */
  
  /* Produce the union of the images of the "neighbors" */
  HashSubset X_image, A_image;
  typedef typename Combinatorial_Map::mapped_type TopSubset;
  BOOST_FOREACH ( typename Toplex::Top_Cell neighbor, X_neighbors ) {
    const TopSubset & mapped = F_ . find ( neighbor ) -> second;
    std::insert_iterator<HashSubset> ii ( X_image, X_image . begin () );
    std::copy ( mapped . begin (), mapped . end (), ii ); 
  } /* boost_foreach */
  BOOST_FOREACH ( typename Toplex::Top_Cell neighbor, A_neighbors ) {
    const TopSubset & mapped = F_ . find ( neighbor ) -> second;
    std::insert_iterator<HashSubset> ii ( A_image, A_image . begin () );
    std::copy ( mapped . begin (), mapped . end (), ii ); 
  } /* boost_foreach */  
  
  //DEBUG
  /*
  std::cout << "A_ = ";
  BOOST_FOREACH ( typename Toplex::Top_Cell cell, A_ ) {
    std::cout << cell << " ";
  }
  std::cout << "\n";
  std::cout << "X_image = ";
  BOOST_FOREACH ( typename Toplex::Top_Cell cell, X_image ) {
    std::cout << cell << " ";
  }
  std::cout << "\n";
  std::cout << "A_image = ";
  BOOST_FOREACH ( typename Toplex::Top_Cell cell, A_image ) {
    std::cout << cell << " ";
  }
  std::cout << "\n";
   */
  
  /* Determine from image the largest cells in subcomplex */  
  std::set<Relative_Cell> X_work_set, A_work_set;

  BOOST_FOREACH ( typename Toplex::Top_Cell image_box, X_image ) {
    typename FactorComplex::const_iterator it = Y_boxes_ [ image_box ];
    Relative_Cell cell ( full_codomain_ -> index ( it ), it . dimension () );
    //std::cout << "Creating relative cell (" << cell << "\n";
    X_work_set . insert ( cell );
  } /* boost_foreach */

  BOOST_FOREACH ( typename Toplex::Top_Cell image_box, A_image ) {
    typename FactorComplex::const_iterator it = Y_boxes_ [ image_box ];
    Relative_Cell cell ( full_codomain_ -> index ( it ), it . dimension () );
    A_work_set . insert ( cell );
  } /* boost_foreach */
  
  /*
  std::cout << "\n";
  std::cout << "X_work_set = ";
  BOOST_FOREACH ( Relative_Cell cell, X_work_set ) {
    std::cout << cell << " ";
  }
  std::cout << "\n";
  std::cout << "A_work_set = ";
  BOOST_FOREACH ( Relative_Cell cell, A_work_set ) {
    std::cout << cell << " ";
  }
  std::cout << "\n";
  */
  
  /* Determine the Closure */
  std::set<Relative_Cell> X_closure_set, A_closure_set;
  Relative_Graph_Complex_detail::closure_in_subcomplex ( X_closure_set, *full_codomain_, X_work_set );
  Relative_Graph_Complex_detail::closure_in_subcomplex ( A_closure_set, *full_codomain_, A_work_set );


#ifdef RGVISUALIZE_DEBUG
  //std::cout << " X_closure_set . size () = " << X_closure_set . size () << "\n";
  //std::cout << " A_closure_set . size () = " << A_closure_set . size () << "\n";
  {
    Adaptive_Complex & complex = * full_codomain_;
    ComplexVisualization < Adaptive_Complex > * cv = new ComplexVisualization < Adaptive_Complex > ( "Making Fiber to find preboundary in.");
    cv -> drawComplex ( complex, 0 );
    BOOST_FOREACH ( const Relative_Cell & cell, X_closure_set ) {
      //std::cout << cell << " ---> " << * codomain () . include ( cell ) << "\n";
      cv -> drawSpecialCell ( complex, * codomain () . include ( cell ), 100 );
    }
    
    BOOST_FOREACH ( const Relative_Cell & cell, A_closure_set ) {
      cv -> drawSpecialCell ( complex, * codomain () . include ( cell ), 200 );
    }
    
    cv -> drawNow ();
    // explore a minute here
    cv -> explore ();
    delete cv;
  }
#endif
  
  /*
  std::cout << "X_closure_set = ";
  BOOST_FOREACH ( Relative_Cell cell, X_closure_set ) {
    std::cout << cell << " ";
  }
  std::cout << "\n";
  std::cout << "A_closure_set = ";
  BOOST_FOREACH ( Relative_Cell cell, A_closure_set ) {
    std::cout << cell << " ";
  }
  std::cout << "\n";
  */
  
  return_value . construct ( full_codomain_ );
  /* Assemble the subcomplex */
  BOOST_FOREACH ( const Relative_Cell & cell, X_closure_set ) {
    return_value . insert ( cell, false );
  } /* boost_foreach */

  BOOST_FOREACH ( const Relative_Cell & cell, A_closure_set ) {
    return_value . erase ( cell, false );
  } /* boost_foreach */
  
  return_value . finalize ();
  return_value . index ();
} /* Relative_Graph_Complex<Toplex,Combinatorial_Map>::makeFiber */
