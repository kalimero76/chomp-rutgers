#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

//#define MORSE_DEBUG
#define CHOMP_HEADER_ONLY_
#define TEST_PROGRAM
#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
#include "algorithms/basic.h"

void test_example ( void ) {
  Cubical_Complex my_cubical_complex;
  std::vector < unsigned int > sizes ( 2, 3 );
  my_cubical_complex . Allocate_Bitmap ( sizes );
  std::vector < unsigned int > cube_position ( 2 );
  
  
  for ( int i = 0; i < 3; ++ i ) {
    cube_position [ 0 ] = i;
    for ( int j = 0; j < 3; ++ j ) {
      cube_position [ 1 ] = j;
      my_cubical_complex . Add_Full_Cube ( cube_position );
    }
  }
  
  for ( int i = 0; i < 3; ++ i ) {
    cube_position [ 0 ] = i;
    for ( int j = 0; j < 3; ++ j ) {
      cube_position [ 1 ] = j;
      if ( i == 1 && j == 1 ) continue;
      my_cubical_complex . Remove_Full_Cube ( cube_position );
    }
  } 

  utility::compute_example ( my_cubical_complex );

} /* generator_example */

int main (int argc, char * const argv[]) {
  utility::do_preprocess = 0;
  test_example ();
	return 0;
} /* main */
