#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

//#define MORSE_DEBUG
#define CHOMP_HEADER_ONLY_
#define TEST_PROGRAM
#include "complexes/Simplicial_Complex.h"	/* for class Simplicial_Complex */
#include "complexes/Abstract_Complex.h"	/* for class Simplicial_Complex */
#include "archetypes/Chain_Archetype.h" /* For Default_Cell */
#include "algorithms/basic.h"
#include <ctime>

clock_t start_clock, stop_clock;

Abstract_Complex<> Simplicial_to_Abstract ( Simplicial_Complex & simplicial ) {  
  std::cout << "Converting to abstract complex.\n";
  std::cout << "Size of simplicial complex = " << simplicial . size () << "\n";
  Abstract_Complex<>  my_abstract ( simplicial . dimension () );
  // Insert The Cells
  for ( Simplicial_Complex::iterator it = simplicial . begin (); it != simplicial . end (); ++ it ) {
    //std::cout << "Inserting " << it << " into abstract complex.\n";
    my_abstract . insert ( Default_Cell ( it . handle_, it . dimension () ) );
  }
  // Create the Boundary Information
  for ( Simplicial_Complex::iterator it = simplicial . begin (); it != simplicial . end (); ++ it ) {
    Default_Cell cell ( it . handle_, it . dimension () );
    Simplicial_Complex::Chain s_chain = simplicial . boundary ( it );
    //std::cout << "bd ( " << it << " ) = " << s_chain << "\n";
    Abstract_Complex<>::Chain a_chain;
    typedef std::pair < Simplicial_Complex::iterator, long > S_Term;
    BOOST_FOREACH ( const S_Term & term, s_chain ) {
      Simplicial_Complex::iterator bd_it = term  . first;
      Default_Cell bd_cell ( bd_it . handle_, bd_it . dimension () );
      Abstract_Complex<>::const_iterator a_iter = my_abstract . find ( bd_cell);
      std::pair < Abstract_Complex<>::iterator, long > new_term ( a_iter, term . second );
      a_chain . insert ( new_term );
    }
    my_abstract . boundary ( my_abstract . find ( cell ) ) = a_chain;
  }
  // Create Coboundary information
  my_abstract . generate_coboundary_information ();
  // return
  return my_abstract;
}

void test_example ( char * filename ) {
  start_clock = clock ();
  Simplicial_Complex my_simplicial_complex ( filename );
  Abstract_Complex<> my_abstract = Simplicial_to_Abstract ( my_simplicial_complex );
  my_simplicial_complex . clear ();
  utility::compute_example ( my_abstract );
  stop_clock = clock ();
  std::cout << (float) ( stop_clock - start_clock ) / (float) CLOCKS_PER_SEC << " total time elapsed \n";

} /* generator_example */

int main (int argc, char * const argv[]) {
  utility::do_preprocess = 0;
  if ( argc != 2 ) {
    std::cout << "supply a filename\n";
  } else {
    test_example ( argv [ 1 ] );
  }
	return 0;
} /* main */
