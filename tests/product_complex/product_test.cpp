#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

//#define MORSE_DEBUG
#define CHOMP_HEADER_ONLY_
#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
#include "complexes/Morse_Complex.h"	/* for class Cubical_Complex */
#include "complexes/Product_Complex.h"    /* for Morse_Complex */
#include "complexes/Subcomplex.h"    /* for Morse_Complex */
#include "algorithms/Homology.h"		/* for function Homology(...)*/

template < class Cell_Complex >
void inspect ( Cell_Complex & my_complex ) {
  for ( unsigned int dimension_index = 0; dimension_index <= my_complex . dimension; ++ dimension_index ) {
    std::cout << "Inspecting dimension " << dimension_index << "\n";
    for ( typename Cell_Complex::const_iterator iter = my_complex . cells [ dimension_index ] . begin ();
         iter != my_complex . cells [ dimension_index ] . end (); ++ iter ) {
      typename Cell_Complex::Chain my_chain, another_chain;
      my_complex . Boundary_Map ( my_chain, iter );
      my_complex . Coboundary_Map ( another_chain, iter );
      my_chain += another_chain;
      another_chain += my_chain;
      //std::cout << " bd ( " << *iter << ") = " << my_complex . Boundary_Map ( my_chain, iter ) << "\n";
      //std::cout << " cbd ( " << *iter << ") = " << my_complex . Coboundary_Map ( another_chain, iter ) << "\n";
    } /* for */
  } /* for */
} /* inspect */

void test_example ( void ) {
  Cubical_Complex my_cubical_complex;
  std::vector < unsigned int > sizes ( 2, 10 );
  std::vector < unsigned int > cube_position ( 2, 5 );
  my_cubical_complex . Allocate_Bitmap ( sizes );
  my_cubical_complex . Add_Full_Cube ( cube_position );
  my_cubical_complex . Remove_Cell ( * my_cubical_complex . cells[2] . begin () );	
  
  /* Assertion: my_cubical_complex contains a square (homeomorphic to S^1) */
  
  typedef Product_Complex < Cubical_Complex, Cubical_Complex > two_type;
  typedef Product_Complex < two_type, two_type > four_type;
  typedef Product_Complex < four_type, four_type > eight_type;

  two_type my_two_torus  ( my_cubical_complex, my_cubical_complex );
  four_type my_four_torus ( my_two_torus, my_two_torus );
  eight_type my_eight_torus ( my_four_torus, my_four_torus );
  clock_t start, stop;
  start = clock ();
  inspect ( my_four_torus );
  stop = clock ();
  std::cout << "Iteration time: " << ( float ) (stop - start ) / (float) CLOCKS_PER_SEC << "\n";
  typedef Subcomplex < eight_type > final_type;
  std::cout << "Generating subcomplex:\n";
  final_type my_subcomplex ( my_eight_torus );
  Morse_Complex < final_type > my_morse_complex ( my_subcomplex );
  
	/* Generators of the Morse complex */
	std::vector < std::vector < std::pair < final_type::Chain, unsigned int > > > generators = 
	Homology_Generators ( my_morse_complex );
    
	/* Get them in the original format */
	for ( unsigned int dimension_index = 0; dimension_index < generators . size (); ++ dimension_index ) {
		std::cout << "\nDimension " << dimension_index << "\n";
		for ( unsigned int generator_index = 0; generator_index < generators [ dimension_index ] . size (); ++ generator_index ) {
			if ( generators [ dimension_index ] [ generator_index ] . second == 0 ) 
				std::cout << "  Betti generator: ";
			else
				std::cout << "  Torsion generator (" << generators [ dimension_index ] [ generator_index ] . second << "): ";
			final_type::Chain my_chain;
			my_morse_complex . Chain_Correspondence_Algorithm ( my_chain, generators [ dimension_index ] [ generator_index ] . first );
			std::cout << my_chain << "\n";
		} /* for */
	} /* for */
	
} /* generator_example */

int main (int argc, char * const argv[]) {
    test_example ();
	return 0;
} /* main */
