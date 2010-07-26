/* Test program for Adaptive_Complex
 * Shaun Harker 
 */

#include <iostream>

#define CHOMP_HEADER_ONLY_
#define TEST_PROGRAM
#include "complexes/Adaptive_Complex.h"
#include "algorithms/basic.h"

int main (int argc, char * const argv[]) {
  
  Adaptive_Complex my_adaptive_complex ( 2 );
  
  {
  std::vector < unsigned int > splitting ( 2 );
  
  splitting [ 0 ] = 0; splitting [ 1 ] = 2;
  my_adaptive_complex . Add_Full_Cube ( splitting );
  
  splitting [ 0 ] = 0; splitting [ 1 ] = 0;
  my_adaptive_complex . Add_Full_Cube ( splitting );
  
  splitting [ 0 ] = 0; splitting [ 1 ] = 1;
  my_adaptive_complex . Add_Full_Cube ( splitting );
   
  }
  {
  std::vector < unsigned int > splitting ( 1 );
    splitting [ 0 ] = 2;
    my_adaptive_complex . Add_Full_Cube ( splitting );
    
    
    splitting [ 0 ] = 1;
    my_adaptive_complex . Add_Full_Cube ( splitting );
    
    splitting [ 0 ] = 3;
    my_adaptive_complex . Add_Full_Cube ( splitting );
     
  }
  
  my_adaptive_complex . Finalize ();

  
  std::cout << "\n\nBoundary/Coboundary test:\n";
  for ( Adaptive_Complex::const_iterator it = my_adaptive_complex . begin (); it != my_adaptive_complex . end (); ++ it ) {
    std::cout << "bd ( " << *it << ": " << (* it ) . node << ", " << my_adaptive_complex . geometric_code ( (* it ) . code ) << " ) = " << my_adaptive_complex . boundary ( it ) << "\n";
    std::cout << "cbd ( " << * it  << ": "<< (* it ) . node << ", " << my_adaptive_complex . geometric_code ( (* it ) . code ) << " ) = " << my_adaptive_complex . coboundary ( it ) << "\n";  
    if ( it != my_adaptive_complex . find ( * it ) ) std::cout << " Problem with find. \n";
  }  
  
  
  std::cout << "\n\nIndex test:\n";
  
  my_adaptive_complex . index ();
  
  for ( unsigned int i = 0; i < my_adaptive_complex . size (); ++ i ) {
    std::cout << "lookup ( " << i << " ) = " << * my_adaptive_complex . lookup ( i ) << "\n";
    std::cout << "index ( " << * my_adaptive_complex . lookup ( i ) << " ) = " << my_adaptive_complex . index ( my_adaptive_complex . lookup ( i ) ) << "\n";
  }
  
   
  std::cout << "\n\nHomology test. \n";
  utility::compute_example ( my_adaptive_complex );
 
  return 0;
} /* main */
