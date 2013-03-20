#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

//#define MORSE_DEBUG
#define CHOMP_HEADER_ONLY_
#define TEST_PROGRAM
#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
#include "complexes/Product_Complex.h"    /* for Morse_Complex */
#include "algorithms/basic.h"		/* for function Homology(...)*/

void test_example ( void ) {
  Cubical_Complex my_cubical_complex;
  std::vector < unsigned int > sizes ( 2, 10 );
  std::vector < unsigned int > cube_position ( 2, 5 );
  my_cubical_complex . Allocate_Bitmap ( sizes );
  my_cubical_complex . Add_Full_Cube ( cube_position );
  my_cubical_complex . erase ( my_cubical_complex . begin ( 2 ) );	
  
  /* Assertion: my_cubical_complex contains a square (homeomorphic to S^1) */
  
  typedef Product_Complex < Cubical_Complex, Cubical_Complex > two_type;
  typedef Product_Complex < two_type, two_type > four_type;
  typedef Product_Complex < four_type, four_type > eight_type;

  two_type my_two_torus ( my_cubical_complex, my_cubical_complex );
  four_type my_four_torus ( my_two_torus, my_two_torus );
  eight_type my_eight_torus ( my_four_torus, my_four_torus );

  utility::compute_example ( my_eight_torus );

} /* example */

int main (int argc, char * const argv[]) {
    test_example ();
	return 0;
} /* main */
