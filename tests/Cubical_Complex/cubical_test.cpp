#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

//#define MORSE_DEBUG
#define CHOMP_HEADER_ONLY_
#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
#include "algorithms/Homology.h"		/* for function Homology(...)*/

template < class Cell_Complex >
void inspect ( Cell_Complex & my_complex ) {
  std::cout << "Inspecting the complex.\n";
  std::cout << "The complex has dimension " << my_complex . dimension () << "\n";
  std::cout << "The complex has size " << my_complex . size () << "\n";
  for ( typename Cell_Complex::const_iterator iter = my_complex . begin (); iter != my_complex . end (); ++ iter ) {
    typename Cell_Complex::Chain my_chain, another_chain;
    my_complex . Boundary_Map ( my_chain, iter );
    my_complex . Coboundary_Map ( another_chain, iter );
    std::cout << " bd ( " << *iter << ") = " << my_complex . Boundary_Map ( my_chain, iter ) << "\n";
    std::cout << " cbd ( " << *iter << ") = " << my_complex . Coboundary_Map ( another_chain, iter ) << "\n";
  } /* for */
} /* inspect */

void test_example ( void ) {
  Cubical_Complex my_cubical_complex;
  std::vector < unsigned int > sizes ( 2, 10 );
  std::vector < unsigned int > cube_position ( 2, 5 );
  my_cubical_complex . Allocate_Bitmap ( sizes );
  my_cubical_complex . Add_Full_Cube ( cube_position );
  //my_cubical_complex . erase ( my_cubical_complex . begin ( 2 ) );	
  inspect ( my_cubical_complex );
} /* generator_example */

int main (int argc, char * const argv[]) {
    test_example ();
	return 0;
} /* main */
