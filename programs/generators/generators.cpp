#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#define _CHOMP_HEADER_ONLY_
#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
#include "complexes/Morse_Complex.h"    /* for Morse_Complex */
#include "algorithms/Homology.h"		/* for function Homology(...)*/

void generator_example ( void ) {
    Cubical_Complex my_cubical_complex;
    std::vector < unsigned int > sizes ( 3, 10 );
    std::vector < unsigned int > cube_position ( 3, 5 );
    my_cubical_complex . Allocate_Bitmap ( sizes );
    my_cubical_complex . Add_Full_Cube ( cube_position );
    my_cubical_complex . Remove_Cell ( * my_cubical_complex . cells[3] . begin () );	
	
	/* Now "my_cubical_complex" has something homeomorphic to S^2 */
	
	/* We make a morse complex. */
	Morse_Complex < Cubical_Complex > my_morse_complex ( my_cubical_complex );
    
	/* Generators of the Morse complex */
	std::vector < std::vector < std::pair < Cubical_Complex::Chain, unsigned int > > > generators = 
	Homology_Generators ( my_morse_complex );

	/* Get them in the original format */
	for ( unsigned int dimension_index = 0; dimension_index < generators . size (); ++ dimension_index ) {
		std::cout << "\nDimension " << dimension_index << "\n";
		for ( unsigned int generator_index = 0; generator_index < generators [ dimension_index ] . size (); ++ generator_index ) {
			if ( generators [ dimension_index ] [ generator_index ] . second == 0 ) 
				std::cout << "  Betti generator: ";
			else
				std::cout << "  Torsion generator (" << generators [ dimension_index ] [ generator_index ] . second << "): ";
			Cubical_Complex::Chain my_chain;
			my_morse_complex . Chain_Correspondence_Algorithm ( my_chain, generators [ dimension_index ] [ generator_index ] . first );
			std::cout << my_chain << "\n";
		} /* for */
	} /* for */
	
} /* generator_example */

int main (int argc, char * const argv[]) {
    generator_example ();
	return 0;
} /* main */
