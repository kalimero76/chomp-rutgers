#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#define CHOMP_HEADER_ONLY_
#define TEST_PROGRAM
#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
#include "algorithms/basic.h"

int main (int argc, char * const argv[]) {

  if ( argc != 2 ) {
    std::cout << "usage ./chomp filename\n " <<
      " filename is cubical complex file, which consists of a list \n" <<
      " of many lines of the form \n" <<
      "      ( n1, n2, ..., nd ) \n" <<
      "  where d is the dimension of the complex \n" <<
      "  and the ni's are the coordinates of the individual cube. Example:\n" <<
      " (0, 0, 1, 0) \n" <<
      " (10, 13, 2, 3) \n" << " ... two cubes of a four dimensional cubical complex\n";
    return 0;
  }
  /* Load the cubical_complex from file */
  Cubical_Complex my_cubical_complex;
  std::cout << "Loading from file " << argv [ 1 ] << "\n";
  my_cubical_complex . Load_From_File ( argv [ 1 ] );
  std::cout << "Loaded cubical complex.\n";
  /* Compute homology and states using DMT */
  utility::compute_example ( my_cubical_complex );   
  return 0;
}
