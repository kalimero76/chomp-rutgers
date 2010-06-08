/* Test program for Map_Homology
 * Shaun Harker 
 */

#include <iostream>

#define CHOMP_HEADER_ONLY_
#include "toplexes/Adaptive_Cubical_Toplex.h"
#include "algorithms/Homology.h"			/* for function Homology(...)*/
#include "complexes/Adaptive_Complex.h"
#include "algorithms/Morse_Theory.h"

using namespace Adaptive_Cubical;

void subdivide_toplex ( Toplex & my_toplex ) {
  /* subdivide every top cell */ /* only for [0,1]^n */
  Geometric_Description bounds ( 2, Real ( 0 ), Real ( 1 ) );
  Toplex_Subset my_subset = my_toplex . cover ( bounds );
  for ( Toplex_Subset::const_iterator it = my_subset . begin (); it != my_subset . end (); ++ it )
    my_toplex . subdivide ( my_toplex . find ( *it ) );
} /* subdivide_toplex */


struct myMap {
  Geometric_Description operator () ( const Geometric_Description & rectangle ) {
    Geometric_Description return_value ( 2 );
    return_value . lower_bounds [ 0 ] = - rectangle . upper_bounds [ 0 ];
    return_value . upper_bounds [ 0 ] = - rectangle . lower_bounds [ 0 ];
    return_value . lower_bounds [ 1 ] =   rectangle . lower_bounds [ 1 ];
    return_value . upper_bounds [ 1 ] =   rectangle . upper_bounds [ 1 ];
    return return_value;
  }
};

int main ( int argc, char * argv [] ) {
  using namespace Adaptive_Cubical;
  Geometric_Description bounds ( 2, Real ( -1.0 ), Real ( 1.0 ) );
  Toplex my_toplex ( bounds );
  subdivide_complex ( my_complex );
  subdivide_complex ( my_complex );
  
  /* Cut out the middle */
  Geometric_Description square ( 2, Real ( -0.5 ), Real ( 0.5 ) );
  std::cout << "\nOuter cover of " << square << ":\n";
  Toplex_Subset children = my_toplex . cover ( square );
  
  for ( Toplex_Subset::const_iterator it = my_subset . begin (); it != my_subset . end (); ++ it )
    my_toplex . erase ( my_toplex . find ( *it ) );

  /* Map Homology */
  Map_Homology ( my_complex, my_complex, myMap () );
  
  return 0;
} /* main */
