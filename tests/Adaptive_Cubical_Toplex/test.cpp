/* Test program for Adaptive_Cubical_Toplex
 * Shaun Harker 
 */

#include <iostream>

#define CHOMP_HEADER_ONLY_
#define TEST_PROGRAM
#include "toplexes/Adaptive_Cubical_Toplex.h"
#include "complexes/Adaptive_Complex.h"
#include "algorithms/basic.h"
#include <boost/foreach.hpp>

using namespace Adaptive_Cubical;

void subdivide_toplex ( Toplex & my_toplex ) {
  /* subdivide every top cell */ /* only for [0,1]^n */
  Geometric_Description bounds ( 2, Real ( 0 ), Real ( 1 ) );
  Toplex_Subset my_subset = my_toplex . cover ( bounds );
  for ( Toplex_Subset::const_iterator it = my_subset . begin (); it != my_subset . end (); ++ it )
    my_toplex . subdivide ( my_toplex . find ( *it ) );
};

void randomly_subdivide_toplex ( Toplex & my_toplex ) {
  /* subdivide every top cell */ /* only for [0,1]^n */
  Geometric_Description bounds ( 2, Real ( 0 ), Real ( 1 ) );
  Toplex_Subset my_subset = my_toplex . cover ( bounds );
  for ( Toplex_Subset::const_iterator it = my_subset . begin (); it != my_subset . end (); ++ it ) {
    if ( rand () % 2 == 0 ) my_toplex . subdivide ( my_toplex . find ( *it ) );
  }
};

void randomly_poke_toplex ( Toplex & my_toplex ) {
  /* subdivide every top cell */ /* only for [0,1]^n */
  Geometric_Description bounds ( 2, Real ( 0 ), Real ( 1 ) );
  Toplex_Subset my_subset = my_toplex . cover ( bounds );
  for ( Toplex_Subset::const_iterator it = my_subset . begin (); it != my_subset . end (); ++ it ) {
    if ( rand () % 10 == 0 ) {
      std::cout << "Poking " << *it << "\n";
      my_toplex . erase ( my_toplex . find ( *it ) );
    }
  }
};

void inspect_toplex ( const Toplex & my_toplex ) {
  std::cout << "\n Inspection. \n";
  std::cout << "Toplex has " << my_toplex . size () << " top cells.\n";
  for ( Toplex::const_iterator it = my_toplex . begin (); it != my_toplex . end (); ++ it )
    std::cout << "top cell " << * it << " has geometry " 
    << my_toplex . geometry ( it ) << "\n";
} /* inspect_toplex */

void inspect_subset ( const Toplex & my_toplex, const Toplex_Subset & my_subset ) {
  for ( Toplex_Subset::const_iterator it = my_subset . begin (); it != my_subset . end (); ++ it )
    std::cout << "top cell " << * it << " has geometry " << my_toplex . geometry ( my_toplex . find ( *it ) ) << "\n";
} /* inspect_toplex */

int main (int argc, char * const argv[]) {
  /* Initialize Toplex */
  //std::cout << "\nInitializing:\n";
  int D = 3;
  Geometric_Description bounds ( D, Real ( 0 ), Real ( 1 ) );
  Toplex my_toplex ( bounds );
  
  /* Inspection: */
  //std::cout << "\nInspection:\n";
  inspect_toplex ( my_toplex );
  
  /* Subdivide toplex */
  //std::cout << "\nSubdividing " << * my_toplex . begin () << "\n";
  Toplex_Subset children = my_toplex . subdivide ( my_toplex . begin () );
  
  /* Inspection: */
  //std::cout << "\nInspection:\n";
  inspect_toplex ( my_toplex );
  
  /* Children spawned: */
  //std::cout << "\nInspection of children of last subdivided cell:\n";
  //inspect_subset ( my_toplex, children );

  /* Subdivide toplex */
  //std::cout << "\nSubdividing " << * children . begin () << "\n";
  children = my_toplex . subdivide ( my_toplex . find ( * children . begin () ) );
  randomly_poke_toplex ( my_toplex );

  /* Inspection: */
  //std::cout << "\nInspection:\n";
  //inspect_toplex ( my_toplex );
  
  /* Children spawned: */
  //std::cout << "\nInspection of children of last subdivided cell:\n";
  //inspect_subset ( my_toplex, children );

  /* Outer cover: */
  Geometric_Description square ( D, Real ( .4 ), Real ( .6 ) );
  std::cout << "\nOuter cover of " << square << ":\n";
  children = my_toplex . cover ( square );
  //inspect_subset ( my_toplex, children );
  
  
  /* erasure */
  
  Geometric_Description rectangle ( D, Real ( .4 ), Real ( .45 ) );
  std::cout << "\nOuter cover of " << rectangle << ":\n";
  children = my_toplex . cover ( rectangle );
  //inspect_subset ( my_toplex, children );
  std::cout << "\nErasing " << * children . begin () << "\n";
  my_toplex . erase ( my_toplex . find ( * children . begin () ) );

  
  /* Inspection: */
  //std::cout << "\nInspection:\n";
  //inspect_toplex ( my_toplex );

  /* Coarsening */
  //std::cout << "Coarsening\n";
  //my_toplex . coarsen ();

  //inspect_toplex ( my_toplex );

  //randomly_subdivide_toplex ( my_toplex );
  //randomly_poke_toplex ( my_toplex );
  //randomly_subdivide_toplex ( my_toplex );
  //randomly_poke_toplex ( my_toplex );

  //randomly_subdivide_toplex ( my_toplex );
  //randomly_subdivide_toplex ( my_toplex );

  //inspect_toplex ( my_toplex );

  //std::cout << "Coarsening\n";
  //my_toplex . coarsen ();
  
  std::cout << "Done changing toplex.\n";
  inspect_toplex ( my_toplex );
  
  /* Outer cover: */
  //std::cout << "\nOuter cover of " << square << ":\n";
  children = my_toplex . cover ( square );
  //inspect_subset ( my_toplex, children );
  
  //randomly_subdivide_toplex ( my_toplex );
  //randomly_subdivide_toplex ( my_toplex );
  //randomly_subdivide_toplex ( my_toplex );
  //randomly_subdivide_toplex ( my_toplex );
  //randomly_subdivide_toplex ( my_toplex );
  //randomly_subdivide_toplex ( my_toplex );
  //randomly_subdivide_toplex ( my_toplex );
  //randomly_subdivide_toplex ( my_toplex );
  //randomly_subdivide_toplex ( my_toplex );

  /* Complex */
  children = my_toplex . cover ( bounds );
  clock_t start_clock, stop_clock;
  std::cout << "Initializing complex...\n";
  start_clock = clock ();
  std::cout << "Number of top cells = " << my_toplex . size () << "\n";
  Adaptive_Complex my_adaptive_complex = my_toplex . complex ();
#if 0
  std::cout << "Checking complex...\n";
  for ( Adaptive_Complex::const_iterator it = my_adaptive_complex . begin (); it != my_adaptive_complex . end (); ++ it ) {
    std::cout << * it << " " << my_adaptive_complex . geometric_code ( (*it) . code ) << "\n";
  }
  for ( Adaptive_Complex::const_iterator it = my_adaptive_complex . begin (); it != my_adaptive_complex . end (); ++ it ) {
    Adaptive_Complex::Chain my_boundary = my_adaptive_complex . boundary ( it );
    typedef std::pair < Adaptive_Complex::const_iterator, long > term_type;
    BOOST_FOREACH ( term_type term, my_boundary ) {
      Adaptive_Complex::Chain my_coboundary = my_adaptive_complex . coboundary ( term . first );
      if ( my_coboundary . find ( it ) == my_coboundary . end () || my_coboundary . find ( it ) -> second != term . second ) std::cout << " Problem detected.\n";
      //else std::cout << term . second << " = " << coboundary . find ( it ) -> second << "\n";
      std::cout << "\nData:\n";
      std::cout << "     boundary of " << *it << " " << my_adaptive_complex . geometric_code ( (*it) . code ) << " = \n        ";
      std::cout << my_boundary << "\n";
      std::cout << "     coboundary of " << * term . first << " " << my_adaptive_complex . geometric_code ( (* term . first ) . code ) << " = \n        ";
      std::cout << my_coboundary << "\n";
    } /* BOOST_FOREACH */
  } /* for */
  for ( Adaptive_Complex::const_iterator it = my_adaptive_complex . begin (); it != my_adaptive_complex . end (); ++ it ) {
    Adaptive_Complex::Chain my_coboundary = my_adaptive_complex . coboundary ( it );
    typedef std::pair < Adaptive_Complex::const_iterator, long > term_type;
    BOOST_FOREACH ( term_type term, my_coboundary ) {
      Adaptive_Complex::Chain my_boundary = my_adaptive_complex . boundary ( term . first );
      if ( my_boundary . find ( it ) == my_boundary . end () || my_boundary . find ( it ) -> second != term . second ) {
        std::cout << "\nProblem detected:\n";
        std::cout << "     coboundary of " << *it << " " << my_adaptive_complex . geometric_code ( (*it) . code ) << " = \n        ";
        std::cout << my_coboundary << "\n";
        std::cout << "     boundary of " << * term . first << " " << my_adaptive_complex . geometric_code ( (* term . first ) . code ) << " = \n        ";
        std::cout << my_boundary << "\n";
      }
      //else std::cout << term . second << " = " << my_boundary . find ( it ) -> second << "\n";
    } /* BOOST_FOREACH */
  } /* for */
#endif
  stop_clock = clock ();
  std::cout << (float) ( stop_clock - start_clock ) / (float) CLOCKS_PER_SEC << " elapsed \n";

  utility::compute_example ( my_adaptive_complex );
	return 0;
} /* main */
