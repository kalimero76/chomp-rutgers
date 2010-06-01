#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#define CHOMP_HEADER_ONLY_
#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
//#include "complexes/Vector_Complex.h"	/* for class Vector_Complex */
#include "algorithms/Homology.h"		/* for function Homology(...)*/


/* Declarations */
void generate_random_cubical_complex ( Cubical_Complex & my_cubical_complex, std::vector < unsigned int > & dimension_sizes, float probability );
void save_cubical_complex ( Cubical_Complex & my_cubical_complex, const char * file_name );
class compute_results;
template < class Cell_Complex_Template > compute_results compute_example ( Cell_Complex_Template & my_complex );

template < class T >
void print_my_vector ( std::vector<T> print_me ) {
	for ( unsigned int index = 0; index < print_me . size (); ++ index ) std::cout << print_me [ index ] << " "; }

template < class T >
T vector_prod ( const std::vector<T> multiply_my_elements_together ) {
	T return_value(1);
	for ( unsigned int index = 0; index < multiply_my_elements_together . size (); ++ index ) 
		return_value *= multiply_my_elements_together [ index ]; 
	return return_value; } /* endfunction */

template < class T >
T vector_sum ( const std::vector<T> add_my_elements_together ) {
	T return_value(0);
	for ( unsigned int index = 0; index < add_my_elements_together . size (); ++ index ) 
		return_value += add_my_elements_together [ index ] ;
	return return_value; } /* endfunction */

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
	
class compute_results {
public:
	unsigned long original_size;
	unsigned long morse_size;
	unsigned long homology_size;
	float total_time;
	float morse_time;
	float smith_time;
};

int do_preprocess;

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
  unsigned long size;
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
  return compute_example ( my_cubical_complex );
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
	/* Manifold Example */
	//manifold_example ( 8 );
	
	//imperfect_product_example ();
	
return 0;
}

#if 0

void manifold_example (  unsigned int dimension  ) {
  
	std::cout << "\n---- Manifold Example: T^d ----\n";
	/* Generate a Vector_Complex representation of S^1 */
	std::vector<unsigned int> sizes(2);
	sizes[0] = 3; sizes[1] = 3;
	std::vector<unsigned int> coordinates(2);
	coordinates[0] = 1; coordinates[1] = 1;
	Cubical_Complex my_cubical_complex;
	my_cubical_complex . Allocate_Bitmap ( sizes );
	my_cubical_complex . Add_Full_Cube ( coordinates );
	my_cubical_complex . Remove_Cell ( * my_cubical_complex . cells [ 2 ] . begin () );
  std::cout << " Creating S^1. \n";
	Vector_Complex my_vector_complex ( my_cubical_complex );
  std::cout << " Created S^1! \n";
  
	/* Now S^1 is in my_vector_complex. */
	
	/* Compute on Tori */
	Vector_Complex Torus = my_vector_complex;
	for ( unsigned int dimension_index = 2; dimension_index <= dimension; dimension_index ++ ) {
		std::cout << "  Producing a " << dimension_index << "-torus. \n";
		Vector_Complex New_Torus; 
		New_Torus . Product_Complex < Vector_Complex, Vector_Complex > ( my_vector_complex, Torus );
		Torus = New_Torus; 
		New_Torus = Vector_Complex ();
		/* Give Information */
		std::cout << "  --- " << dimension_index << "-Torus ---\n";
		compute_example ( Torus );
	} /* for */
  
	return;
}

void imperfect_product_example ( void ) {
	std::cout << "\n---- Imperfect Product Example ----\n";
	/* Generate Random Cubical Complex */
	unsigned int dimension = 4;
	unsigned int width = 15;
	float probability = .2;
	std::vector < unsigned int > dimension_sizes (  dimension, width );
	Cubical_Complex my_random_complex; 
	generate_random_cubical_complex ( my_random_complex, dimension_sizes, probability );
	
	/* Compute on it. */
  compute_example ( my_random_complex );
  
	/* Generate a Vector_Complex representation of S^1 */
	std::vector<unsigned int> sizes(2);
	sizes[0] = 3; sizes[1] = 3;
	std::vector<unsigned int> coordinates(2);
	coordinates[0] = 1; coordinates[1] = 1;
	Cubical_Complex my_cubical_complex;
	my_cubical_complex . Allocate_Bitmap ( sizes );
	my_cubical_complex . Add_Full_Cube ( coordinates );
	my_cubical_complex . Remove_Cell ( * my_cubical_complex . cells [ 2 ] . begin () );
	Vector_Complex my_vector_complex ( my_cubical_complex );
	
	Vector_Complex my_product_complex;
	my_product_complex . Product_Complex < Cubical_Complex, Vector_Complex > ( my_random_complex, my_vector_complex ) ; 
	
	compute_example ( my_product_complex );
	return;
}

#endif
