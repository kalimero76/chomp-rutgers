#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#define CHOMP_HEADER_ONLY_
#define TEST_PROGRAM
#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
#include "algorithms/basic.h"

int main (int argc, char * const argv[]) {

  if ( argc < 3 ) {
    std::cout << "usage ./chomp filename i\n filename is cubical complex file \n n is 1 or 0 (indicates whether or not to do preprocessing) \n";
    exit(-1);
  }
  std::cout << argc << "\n";
  /* Use command line argument to determine whether or not to do preprocessing */
  utility::do_preprocess = atoi(argv [ 2 ]);
  /* Load the cubical_complex from file */
  Cubical_Complex my_cubical_complex;
  std::cout << "Loading from file " << argv [ 1 ] << "\n";
  my_cubical_complex . Load_From_File ( argv [ 1 ] );
  std::cout << "Loaded cubical complex.\n";
  /* Compute homology and states using DMT */
  utility::compute_example ( my_cubical_complex );   
  return 0;
}
