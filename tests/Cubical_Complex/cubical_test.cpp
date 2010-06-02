#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

//#define MORSE_DEBUG
#define CHOMP_HEADER_ONLY_
#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
#include "algorithms/Morse_Theory.h"
#include "algorithms/Homology.h"		/* for function Homology(...)*/

/* Declarations */
class compute_results;
template < class Cell_Complex > void inspect ( const Cell_Complex & my_complex );
template < class Cell_Complex > void AKQ_inspect ( const Cell_Complex & my_complex );

template < class Cell_Complex_Template > compute_results compute_example ( Cell_Complex_Template & my_complex );
template < class T > void print_my_vector ( std::vector<T> print_me );
template < class T > T vector_sum ( const std::vector<T> add_my_elements_together );

int do_preprocess;

/* Definitions */
class compute_results {
public:
	unsigned long original_size;
	unsigned long morse_size;
	unsigned long homology_size;
	float total_time;
	float morse_time;
	float smith_time;
};

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

  compute_example ( my_cubical_complex );
  
  
} /* generator_example */

int main (int argc, char * const argv[]) {
  do_preprocess = 0;
    test_example ();
	return 0;
} /* main */


template < class T >
void print_my_vector ( std::vector<T> print_me ) {
for ( unsigned int index = 0; index < print_me . size (); ++ index ) std::cout << print_me [ index ] << " "; }

template < class T >
T vector_sum ( const std::vector<T> add_my_elements_together ) {
	T return_value(0);
	for ( unsigned int index = 0; index < add_my_elements_together . size (); ++ index ) 
		return_value += add_my_elements_together [ index ] ;
return return_value; } /* endfunction */

template < class Cell_Complex >
void inspect ( const Cell_Complex & my_complex ) {
  std::cout << "Inspecting the complex.\n";
  std::cout << "The complex has dimension " << my_complex . dimension () << "\n";
  std::cout << "The complex has size " << my_complex . size () << "\n";
  for ( typename Cell_Complex::const_iterator iter = my_complex . begin (); iter != my_complex . end (); ++ iter ) {
    std::cout << " bd ( " << *iter << ") = " << my_complex . boundary ( iter ) << "\n";
    std::cout << " cbd ( " << *iter << ") = " << my_complex . coboundary ( iter ) << "\n";
  } /* for */
} /* inspect */

    
template < class Cell_Complex >
void AKQ_inspect ( const Cell_Complex & my_complex ) {
  std::cout << "AKQ-Inspecting the complex.\n";
  std::cout << "The complex has dimension " << my_complex . dimension () << "\n";
  std::cout << "The complex has size " << my_complex . size () << "\n";
  for ( unsigned int dimension_index = 0; dimension_index <= my_complex . dimension (); ++ dimension_index ) {
  for ( unsigned long index = my_complex . index_begin ( dimension_index ); 
        index != my_complex . index_end ( dimension_index ); ++ index ) {
    std::cout << " cell ( " << index << ") = " << * my_complex . lookup ( index ) << "\n";
    std::cout << " type ( " << index << ") = " << ( int ) my_complex . type ( index, dimension_index ) << "\n";
    std::cout << " mate ( " << index << ") = " << my_complex . mate ( index, dimension_index ) << "\n";
  } /* for */
  } /* for */
} /* AKQ-inspect */



template < class Cell_Complex_Template >
compute_results compute_example ( Cell_Complex_Template & my_complex ) {
	clock_t start_clock, stop_clock, total_time_start, total_time_stop;
	float total_time, morse_time, homology_time;
  unsigned long original_size;
  std::cout << " Size = " << (original_size = my_complex . size ()) << "\n";
  std::cout << " Preprocessing complex... ";
  total_time_start = start_clock = clock ();
  if ( do_preprocess ) my_complex . preprocess ();
  stop_clock = clock ();
  std::cout << (float) ( stop_clock - start_clock ) / (float) CLOCKS_PER_SEC << " elapsed \n";
  std::cout << " Size = " << my_complex . size () << "\n";
  std::cout << "  Original Sizes (by increasing dimension): ";
	for ( unsigned int dim = 0; dim <= my_complex . dimension (); dim ++ ) 
		std::cout << my_complex . size ( dim ) << " ";  
	std::cout << "\n";
	/* Create Morse Complex */
	std::cout << "  Using morse theory to reduce ... \n";
	start_clock = clock ();
  Morse_Complex my_morse_complex = morse::deep_reduction ( my_complex );
	std::cout << " ... morse reduction completed. \n";
	stop_clock = clock (); 
	std::cout << (float) ( stop_clock - start_clock ) / (float) CLOCKS_PER_SEC << " so far elapsed \n";
	morse_time = (float) ( stop_clock - start_clock ) / (float) CLOCKS_PER_SEC;
	/* Tell me interesting things about the Morse Complex */
	
	std::cout << "  Morse Sizes (by increasing dimension): ";
	for ( unsigned int dim = 0; dim <= my_morse_complex . dimension (); dim ++ ) 
		std::cout << my_morse_complex . size ( dim ) << " "; 
	std::cout << "\n";
  
  //inspect ( my_morse_complex );
  
	/* Compute the homology */
	std::vector<int> Betti_Numbers, Minimal_Number_of_Generators;
	std::cout << "  Computing Homology ... \n";
	start_clock = clock ();
	Homology < Morse_Complex > ( Betti_Numbers, Minimal_Number_of_Generators, my_morse_complex );
	stop_clock = clock ();
	std::cout << "  Homology computed. "; 
	std::cout << (float) ( stop_clock - start_clock ) / (float) CLOCKS_PER_SEC << " elapsed \n";
	homology_time = (float) ( stop_clock - start_clock ) / (float) CLOCKS_PER_SEC;
	std::cout << "  Betti Numbers: "; print_my_vector ( Betti_Numbers ); 
	std::cout << "\n";
	total_time_stop = clock ();
	total_time = (float) ( total_time_stop - total_time_start) / (float) CLOCKS_PER_SEC ;
	std::cout << "  STATISTICS \n";
	std::cout << "  ---------- \n";
	std::cout << "  Total time: " << total_time << " seconds.\n";
	std::cout << "  Cells in original complex: " << original_size << "\n";
	std::cout << "  Cells in Morse complex: " << my_morse_complex . size () << "\n";
	std::cout << "  Homology size: " << (float)vector_sum(Minimal_Number_of_Generators) << "\n";
	std::cout << "  Compression factor = " << 100.0 - 100.0 * (float) my_morse_complex . size () / original_size << "\%\n";
	std::cout << "  Morse / Homology = " << (float)my_morse_complex . size () / (float)vector_sum(Minimal_Number_of_Generators) << "\n";
	std::cout << "  Cells per second:" << (float) original_size / total_time << "\n"; 
	std::cout << "  Cells per second of Morse time:" << (float) original_size / morse_time << "\n"; 
	std::cout << "  Cells per second of Smith time:" << (float) original_size / homology_time << "\n"; 
	std::cout << "  ----------\n";
	compute_results return_value;
	return_value . original_size =  original_size;
	return_value . morse_size =  my_morse_complex . size ();
	return_value . homology_size = vector_sum(Minimal_Number_of_Generators) ;
	return_value . total_time =  total_time;
	return_value . morse_time =  morse_time;
	return_value . smith_time =  homology_time;
  return return_value;
} /* compute_example */