/* Test program for Map_Homology
 * Shaun Harker 
 */

#include <iostream>

#define CHOMP_HEADER_ONLY_
#define TEST_PROGRAM
#include "toplexes/Adaptive_Cubical_Toplex.h"
#include "algorithms/Homology.h"			/* for function Homology(...)*/
#include "complexes/Adaptive_Complex.h"
#include "algorithms/Morse_Theory.h"

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
    return_value . lower_bounds [ 0 ] = - rectangle . upper_bounds [ 0 ];
    return_value . upper_bounds [ 0 ] = - rectangle . lower_bounds [ 0 ];
    return_value . lower_bounds [ 1 ] =   rectangle . lower_bounds [ 1 ];
    return_value . upper_bounds [ 1 ] =   rectangle . upper_bounds [ 1 ];
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
  Geometric_Description bounds ( 2, Real ( -1.0 ), Real ( 1.0 ) );
  Toplex my_toplex ( bounds );

  inspect_toplex ( my_toplex );

  subdivide_toplex ( my_toplex );

  inspect_toplex ( my_toplex );

  subdivide_toplex ( my_toplex );
  
  inspect_toplex ( my_toplex );

  /* Cut out the middle */
  Geometric_Description square ( 2, Real ( -0.1 ), Real ( 0.1 ) );
  std::cout << "\nOuter cover of " << square << ":\n";
  Toplex_Subset my_subset = my_toplex . cover ( square );
  
  for ( Toplex_Subset::const_iterator it = my_subset . begin (); it != my_subset . end (); ++ it )
    my_toplex . erase ( my_toplex . find ( *it ) );

  inspect_toplex ( my_toplex );
  
  /* Map Homology */
  Map_Homology ( my_toplex, my_toplex, myMap () );
  
  return 0;
} /* main */
