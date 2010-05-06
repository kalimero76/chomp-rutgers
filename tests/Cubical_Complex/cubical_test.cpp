#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

//#define MORSE_DEBUG
#define CHOMP_HEADER_ONLY_
#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
#include "algorithms/Morse_Theory.h"
#include "algorithms/Homology.h"		/* for function Homology(...)*/

template < class Cell_Complex >
void inspect ( Cell_Complex & my_complex ) {
  std::cout << "Inspecting the complex.\n";
  std::cout << "The complex has dimension " << my_complex . dimension () << "\n";
  std::cout << "The complex has size " << my_complex . size () << "\n";
  for ( typename Cell_Complex::const_iterator iter = my_complex . begin (); iter != my_complex . end (); ++ iter ) {
    typename Cell_Complex::Chain my_chain = my_complex . boundary ( iter );
    typename Cell_Complex::Chain another_chain = my_complex . coboundary ( iter );
    std::cout << " bd ( " << *iter << ") = " << my_chain << "\n";
    std::cout << " cbd ( " << *iter << ") = " << another_chain << "\n";
  } /* for */
} /* inspect */

template < class Cell_Complex >
void index_inspect ( Cell_Complex & my_complex ) {
  std::cout << "Inspecting the indices.\n";
  for ( typename Cell_Complex::const_iterator iter = my_complex . begin (); iter != my_complex . end (); ++ iter ) {
    std::cout << " index ( " << *iter << ") = " << (int) my_complex . index ( iter ) << "\n";
    std::cout << " type ( " << *iter << ") = " << (int) my_complex . type ( my_complex . index ( iter ), iter . dimension () ) << "\n";
    if ( my_complex . type ( my_complex . index ( iter ), iter . dimension () ) == 0 ) 
      std::cout << "This is a queen, and its mate is " << my_complex . mate ( my_complex . index ( iter ), iter . dimension () ) << "\n";
  } /* for */
  for ( unsigned int dimension_index = 0; dimension_index <= my_complex . dimension (); ++ dimension_index ) {
    std::cout << "Listing the " << dimension_index << " dimensional aces:\n";
    unsigned long start = my_complex . ace_begin ( dimension_index );
    unsigned long stop = my_complex . ace_end ( dimension_index );
    std::cout << "   ( index range is " << start << " to " << stop << ") \n";
    for ( unsigned long ace_index = start; ace_index < stop; ++ ace_index ) {
      std::cout << ace_index << " = " << * my_complex . lookup ( ace_index ) << "\n";
    } /* for */
  } /* for */
  std::cout << "inspection complete \n";
} /* inspect_decomposition */

void test_example ( void ) {
  Cubical_Complex my_cubical_complex;
  std::vector < unsigned int > sizes ( 2, 10 );
  std::vector < unsigned int > cube_position ( 2, 5 );
  std::vector < unsigned int > cube_position2 ( 2, 6 );

  my_cubical_complex . Allocate_Bitmap ( sizes );
  my_cubical_complex . Add_Full_Cube ( cube_position );
  my_cubical_complex . Add_Full_Cube ( cube_position2 );
  my_cubical_complex . erase ( my_cubical_complex . begin ( 2 ) );	
  
  inspect ( my_cubical_complex );
  
  my_cubical_complex . decompose ();

  index_inspect ( my_cubical_complex );
  
  Abstract_Complex < Default_Cell > my_abstract_complex 
    = morse::reduction ( my_cubical_complex );
  
  inspect ( my_abstract_complex );
} /* generator_example */

int main (int argc, char * const argv[]) {
    test_example ();
	return 0;
} /* main */
