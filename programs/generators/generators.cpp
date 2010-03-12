#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#define _CHOMP_HEADER_ONLY_
#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
#include "complexes/Morse_Complex.h"    /* for Morse_Complex */
#include "algorithms/Homology.h"		/* for function Homology(...)*/
#include "algorithms/matrix/Dense_Matrix.h"		/* for function Homology(...)*/

#include "capd/matrixAlgorithms/intMatrixAlgorithms.hpp"

void generator_example ( void ) {
    Cubical_Complex my_cubical_complex;
    std::vector < unsigned int > sizes ( 3, 10 );
    std::vector < unsigned int > cube_position ( 3, 5 );
    my_cubical_complex . Allocate_Bitmap ( sizes );
    my_cubical_complex . Add_Full_Cube ( cube_position );
    my_cubical_complex . Remove_Elementary_Chain ( my_cubical_complex . Chain_Groups[3] . begin () -> first );	
    Homology_Generators ( my_cubical_complex );
} /* generator_example */

int main (int argc, char * const argv[]) {
    generator_example ();
	return 0;
} /* main */
