#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#define CHOMP_HEADER_ONLY_
#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
#include "algorithms/Homology.h"		/* for function Homology(...)*/

void generator_example ( void ) {
    Cubical_Complex my_cubical_complex;
    std::vector < unsigned int > sizes ( 2, 3 );
    std::vector < unsigned int > cube_position ( 2, 1 );
    my_cubical_complex . Allocate_Bitmap ( sizes );
    my_cubical_complex . Add_Full_Cube ( cube_position );
    my_cubical_complex . erase ( my_cubical_complex . begin ( 2 ) );

    /* Obtain Generators*/
    std::vector < std::vector < std::pair < Cubical_Complex::Chain, unsigned int > > > generators = 
      Homology_Generators ( my_cubical_complex );
    
    /* Print them out. */
    for ( unsigned int dimension_index = 0; dimension_index < generators . size (); ++ dimension_index ) {
      std::cout << "\nDimension " << dimension_index << "\n";
      for ( unsigned int generator_index = 0; generator_index < generators [ dimension_index ] . size (); ++ generator_index ) {
	if ( generators [ dimension_index ] [ generator_index ] . second == 0 ) 
	  std::cout << "  Betti generator: ";
	else
	  std::cout << "  Torsion generator (" << generators [ dimension_index ] [ generator_index ] . second << "): ";
	std::cout << generators [ dimension_index ] [ generator_index ] . first << "\n";
      } /* for */
    } /* for */

    Cubical_Complex::Chain my_chain = generators [ 1 ] [ 0 ] . first;

    for ( Cubical_Complex::Chain::const_iterator it = my_chain . begin (); 
          it != my_chain . end (); ++ it ) {
      std::cout << "Term = " << (*it) . first << " " << (*it) . second << "\n";
      std::cout << "Cell = " << * ( (*it).first ) << "\n";
    }

    
} /* generator_example */

int main (int argc, char * const argv[]) {
    generator_example ();
	return 0;
} /* main */
