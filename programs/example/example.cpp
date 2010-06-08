#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#define CHOMP_HEADER_ONLY_
#define TEST_PROGRAM
#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
#include "algorithms/basic.h"

using namespace utility;

/* Declarations */
void generate_random_cubical_complex ( Cubical_Complex & my_cubical_complex, std::vector < unsigned int > & dimension_sizes, float probability );
void save_cubical_complex ( Cubical_Complex & my_cubical_complex, const char * file_name );

void generate_random_cubical_complex ( Cubical_Complex & my_cubical_complex, std::vector < unsigned int > & dimension_sizes, float probability ) {
	my_cubical_complex . Allocate_Bitmap ( dimension_sizes );
	/* Discount the double wrapping of cubes */
	std::vector < unsigned int > actual_sizes ( dimension_sizes );
	for ( unsigned int index = 0; index < actual_sizes . size (); ++ index ) actual_sizes [ index ] -= 4;
	int max_count = (int) ( (float) vector_prod ( actual_sizes ) * probability );
	for ( int count = 0; count < max_count; ++ count ) {
		std::vector < unsigned int > cube_coordinates ( dimension_sizes . size () );
		for ( unsigned int dimension_index = 0; dimension_index < my_cubical_complex . dimension (); ++ dimension_index ) 
			cube_coordinates [ dimension_index ] = 2 + rand() % (dimension_sizes [ dimension_index ] - 4);
		my_cubical_complex . Add_Full_Cube ( cube_coordinates ); 
  } /* for */ 
} /* generate_random_cubical_complex */

/* assume given a 'full' cubical complex, save it in simple (#, #, ... #) format */
/* TODO: no longer need to put in sizes, now this causes a goofy thing */
void save_cubical_complex ( Cubical_Complex & my_cubical_complex, const char * file_name ) {
	std::ofstream output_file ( file_name ); 
	const unsigned int dimension = my_cubical_complex . dimension ();
	output_file << "(";
	for ( unsigned int dimension_index = 0; dimension_index < dimension; dimension_index ++ ) {
		output_file << my_cubical_complex . dimension_sizes () [ dimension_index ]; 
		if ( dimension_index + 1 != dimension ) output_file << ", "; }
	output_file << ")\n";
	for ( Cubical_Complex::const_iterator full_cube = my_cubical_complex . begin ( dimension ) ; 
	full_cube != my_cubical_complex . end ( dimension ); ++ full_cube ) {
		unsigned long name = (*full_cube) . data () >> dimension;
		output_file << "(";
		for ( unsigned int dimension_index = 0; dimension_index < dimension; dimension_index ++ ) {
			unsigned long current_dimension_size = my_cubical_complex . dimension_sizes () [ dimension_index ];
			output_file << name % current_dimension_size;
			if ( dimension_index + 1 != dimension ) output_file << ", ";
			name = name / current_dimension_size; 
    } /* for */
		output_file << ")\n"; 
  } /* for */
	output_file . close (); 
} /* save_cubical_complex */

compute_results cubical_example (  int dimension, int width, float probability, bool save_it = false ) {
	/* Generate Random Cubical Complex */
	std::cout << "\n---- Random Complex of dimension " << dimension << ", width " << width << " ----\n";
	std::vector < unsigned int > dimension_sizes (  dimension, width );
	Cubical_Complex my_cubical_complex; 
	std::cout << "  Generating Random Cubical Complex... \n";
	generate_random_cubical_complex ( my_cubical_complex, dimension_sizes, probability );
	std::cout << "  Random Cubical Complex Generated \n";
  if ( save_it ) save_cubical_complex ( my_cubical_complex, "complex.cub" );
  /*
  clock_t start, stop;
  start = clock ();
  my_cubical_complex . index ();
  stop = clock ();
  std::cout << " Indexing time = " << (float ) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";
  start = clock ();
  std::vector < std::pair < unsigned long, long > > chain;
  for ( unsigned long index = 0; index < my_cubical_complex . size (); ++ index ) {
    my_cubical_complex . boundary ( chain, index );
  };
  stop = clock ();
  std::cout << " Sweeping time = " << (float ) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";
   */
  return utility::compute_example ( my_cubical_complex );
} /* cubical_example */

void run_tests ( const char * filename, int dimension, float probability, int Number, int stepsize ) {
	std::vector< compute_results > results ( Number );
	for ( int index = 0; index < Number; ++index ) {
		results [ index ] = cubical_example ( dimension, 8 + stepsize*index, probability); 
	} /* for */
	std::ofstream output_file ( filename );
	for ( int index = 0; index < Number; ++index ) {
		output_file << " data{" << dimension << "}(" << index + 1 << ", 1) = " << results [ index ] . original_size << ";\n"; 
		output_file << " data{" << dimension << "}(" << index + 1 << ", 2) = " << results [ index ] . morse_size << ";\n"; 
		output_file << " data{" << dimension << "}(" << index + 1 << ", 3) = " << results [ index ] . homology_size << ";\n";
		output_file << " data{" << dimension << "}(" << index + 1 << ", 4) = " << results [ index ] . total_time << ";\n"; 
		output_file << " data{" << dimension << "}(" << index + 1 << ", 5) = " << results [ index ] . morse_time << ";\n"; 
		output_file << " data{" << dimension << "}(" << index + 1 << ", 6) = " << results [ index ] . smith_time << ";\n"; 
	} /* for */
	output_file.close();
} /* run_tests */


int main (int argc, char * const argv[]) {

  if ( argc > 1 ) { 
  if ( argc < 3 ) {
    std::cout << "give arguments: dimension, width\n";
    return 0;
  }
  
  do_preprocess = atoi(argv[3]);
  
  cubical_example ( atoi(argv[1]), atoi(argv[2]), .2, true );
  } else {
    
  do_preprocess = true;
	/* Run 2D tests */
	run_tests( "random_cubical_stats_2d.m", 2, .2, 40, 100);
	/* Run 3D tests */
	run_tests( "random_cubical_stats_3d.m", 3, .2, 25, 10);
	
	/* Run 4D tests */
	run_tests( "random_cubical_stats_4d.m", 4, .1, 21, 2);

	/* Run 5D tests */
	run_tests( "random_cubical_stats_5d.m", 5, .1, 13, 1);
  }
	
return 0;
}
