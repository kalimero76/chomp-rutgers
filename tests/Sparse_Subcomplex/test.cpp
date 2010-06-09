/* Test program for Sparse_Subcomplex
 * Shaun Harker 
 */

#include <iostream>

#define CHOMP_HEADER_ONLY_
#define TEST_PROGRAM
#include "complexes/Cubical_Complex.h"
#include "complexes/Sparse_Subcomplex.h"
#include "algorithms/basic.h"

int main (int argc, char * const argv[]) {

  /* Make a simple Cubical Complex with 3x3 full 2D cubes */
  Cubical_Complex my_cubical_complex;
  std::vector < unsigned int > sizes ( 2, 3 );
  my_cubical_complex . Allocate_Bitmap ( sizes );
  std::vector < unsigned int > cube_position ( 2 );
  for ( int i = 0; i < 3; ++ i ) {
    cube_position [ 0 ] = i;
    for ( int j = 0; j < 3; ++ j ) {
      cube_position [ 1 ] = j;
      my_cubical_complex . Add_Full_Cube ( cube_position );
    } /* for */
  } /* for */
  
  /* Make a Sparse_Subcomplex */
  Sparse_Subcomplex < Cubical_Complex > my_sparse_subcomplex ( my_cubical_complex );
  
  std::cout << "Inspecting subcomplex:\n";
  utility::inspect_complex ( my_sparse_subcomplex );
  
  /* Add some cells to the sparse subcomplex */
  std::cout << "Adding cells...\n";
  for ( Cubical_Complex::const_iterator it = my_cubical_complex . begin (); 
       it != my_cubical_complex . end (); ++ it ) {
    my_sparse_subcomplex . insert ( it );
  } /* for */
 
  std::cout << "Inspecting subcomplex:\n";
  utility::inspect_complex ( my_sparse_subcomplex );

  utility::compute_example ( my_sparse_subcomplex );
	return 0;
} /* main */
