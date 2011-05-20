/* Test program for Relative_Map_Homology
 * Shaun Harker 
 */

#include <iostream>

#define RGVISUALIZE_DEBUG
#define VISUALISE_DEBUG
#define CHOMP_HEADER_ONLY_
#define TEST_PROGRAM


#include "toplexes/Adaptive_Cubical_Toplex.h"
#include "complexes/Adaptive_Complex.h"
#include "algorithms/Morse_Theory.h"
#include "algorithms/Homology.h"			/* for function Homology(...)*/

#include "visualization.cpp"



using namespace Adaptive_Cubical;

void subdivide_toplex ( Toplex & my_toplex ) {
  /* subdivide every top cell */ /* only for [-1,1]^2 */
  Geometric_Description bounds ( 2, Real ( -1.0 ), Real ( 1.0 ) );
  Toplex_Subset my_subset = my_toplex . cover ( bounds );
  for ( Toplex_Subset::const_iterator it = my_subset . begin (); it != my_subset . end (); ++ it )
    my_toplex . subdivide ( my_toplex . find ( *it ) );
} /* subdivide_toplex */


struct myMap {
  Geometric_Description operator () ( const Geometric_Description & rectangle ) const {
    Geometric_Description return_value ( 2 );
    return_value . lower_bounds [ 0 ] = rectangle . lower_bounds [ 0 ] / 2.0f;
    return_value . upper_bounds [ 0 ] = rectangle . upper_bounds [ 0 ] / 2.0f;
    return_value . lower_bounds [ 1 ] = rectangle . lower_bounds [ 1 ] * 2.0f;
    return_value . upper_bounds [ 1 ] = rectangle . upper_bounds [ 1 ] * 2.0f;
    return return_value;
  }
};

void inspect_toplex ( const Toplex & my_toplex ) {
  std::cout << "Inspecting.\n";
  for ( Toplex::const_iterator it = my_toplex . begin (); it != my_toplex . end (); ++ it )
    std::cout << "top cell " << * it << " has geometry " << my_toplex . geometry ( it ) << "\n";
} /* inspect_toplex */

int main ( int argc, char * argv [] ) {
  using namespace Adaptive_Cubical;
  
  /* Produce toplex "T" */
  std::cout << "Producing Toplex.\n";
  Geometric_Description bounds ( 2, Real ( -1.0 ), Real ( 1.0 ) );
  Toplex T ( bounds );
  subdivide_toplex ( T );
  subdivide_toplex ( T );
  subdivide_toplex ( T );
  subdivide_toplex ( T );
  subdivide_toplex ( T );
  //subdivide_toplex ( T );
  //subdivide_toplex ( T );
  //subdivide_toplex ( T );
  /*
  Adaptive_Complex complex = T . complex ();
  ComplexVisualization < Adaptive_Complex > cv ( "adaptive complex" );
  cv . drawComplex ( complex, 100 );
  
  // wait a minute here
  char c;
  std::cin >> c;
   */
  /* Produce morse set "S" */
  std::cout << "Producing Morse set (known apriori)\n";
  Geometric_Description square ( 2, Real ( -0.4 ), Real ( 0.4 ) );
  Toplex::Subset S = T . cover ( square );
  
  /* Produce continuous map "f" */
  myMap f;
  
  /* Call Conley Index */
  std::cout << "Calling Conley Index. \n";
  Conley_Index_t conley_index;
  Conley_Index ( &conley_index, T, S, f );
  std::cout << "Computed Conley Index. \n";
  
  return 0;
} /* main */
