/* Test program for Adaptive_Cubical_Toplex
 * Shaun Harker 
 */

#include <iostream>

#define CHOMP_HEADER_ONLY_
#include "toplexes/Adaptive_Cubical_Toplex.h"
#include "algorithms/Homology.h"			/* for function Homology(...)*/
#include "complexes/Adaptive_Complex.h"
#include "algorithms/Morse_Theory.h"

using namespace Adaptive_Cubical;

/* Declarations */
class compute_results;
template < class Cell_Complex_Template > compute_results compute_example ( Cell_Complex_Template & my_complex );
template < class T > void print_my_vector ( std::vector<T> print_me );
template < class T > T vector_sum ( const std::vector<T> add_my_elements_together );
template < class Cell_Complex > void inspect_complex ( Cell_Complex & my_complex );

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

void subdivide_toplex ( Toplex & my_toplex ) {
  /* subdivide every top cell */ /* only for [0,1]^n */
  Geometric_Description bounds ( 2, Real ( 0 ), Real ( 1 ) );
  Toplex_Subset my_subset = my_toplex . cover ( bounds );
  for ( Toplex_Subset::const_iterator it = my_subset . begin (); it != my_subset . end (); ++ it )
    my_toplex . subdivide ( my_toplex . find ( *it ) );
};

void inspect_toplex ( const Toplex & my_toplex ) {
  for ( Toplex::const_iterator it = my_toplex . begin (); it != my_toplex . end (); ++ it )
    std::cout << "top cell " << * it << " has geometry " << my_toplex . geometry ( it ) << "\n";
} /* inspect_toplex */

void inspect_subset ( const Toplex & my_toplex, const Toplex_Subset & my_subset ) {
  for ( Toplex_Subset::const_iterator it = my_subset . begin (); it != my_subset . end (); ++ it )
    std::cout << "top cell " << * it << " has geometry " << my_toplex . geometry ( my_toplex . find ( *it ) ) << "\n";
} /* inspect_toplex */

int main (int argc, char * const argv[]) {
  /* Initialize Toplex */
  std::cout << "\nInitializing:\n";
  Geometric_Description bounds ( 2, Real ( 0 ), Real ( 1 ) );
  Toplex my_toplex ( bounds );
  
  /* Inspection: */
  std::cout << "\nInspection:\n";
  inspect_toplex ( my_toplex );
  
  /* Subdivide toplex */
  std::cout << "\nSubdividing " << * my_toplex . begin () << "\n";
  Toplex_Subset children = my_toplex . subdivide ( my_toplex . begin () );
  
  /* Inspection: */
  std::cout << "\nInspection:\n";
  inspect_toplex ( my_toplex );
  
  /* Children spawned: */
  std::cout << "\nInspection of children of last subdivided cell:\n";
  inspect_subset ( my_toplex, children );

  /* Subdivide toplex */
  std::cout << "\nSubdividing " << * children . begin () << "\n";
  children = my_toplex . subdivide ( my_toplex . find ( * children . begin () ) );
  
  /* Inspection: */
  std::cout << "\nInspection:\n";
  inspect_toplex ( my_toplex );
  
  /* Children spawned: */
  std::cout << "\nInspection of children of last subdivided cell:\n";
  inspect_subset ( my_toplex, children );

  /* Outer cover: */
  Geometric_Description square ( 2, Real ( .4 ), Real ( .6 ) );
  std::cout << "\nOuter cover of " << square << ":\n";
  children = my_toplex . cover ( square );
  inspect_subset ( my_toplex, children );
  
  
  /* erasure */
  Geometric_Description rectangle ( 2, Real ( .4 ), Real ( .45 ) );
  std::cout << "\nOuter cover of " << rectangle << ":\n";
  children = my_toplex . cover ( rectangle );
  inspect_subset ( my_toplex, children );
  std::cout << "\nErasing " << * children . begin () << "\n";
  my_toplex . erase ( my_toplex . find ( * children . begin () ) );
  
  
  /* Inspection: */
  std::cout << "\nInspection:\n";
  inspect_toplex ( my_toplex );
  
  /* Outer cover: */
  std::cout << "\nOuter cover of " << square << ":\n";
  children = my_toplex . cover ( square );
  inspect_subset ( my_toplex, children );
  
  subdivide_toplex ( my_toplex );
  subdivide_toplex ( my_toplex );
  subdivide_toplex ( my_toplex );
  subdivide_toplex ( my_toplex );
  subdivide_toplex ( my_toplex );
  subdivide_toplex ( my_toplex );
  subdivide_toplex ( my_toplex );
  subdivide_toplex ( my_toplex );
  subdivide_toplex ( my_toplex );

  /* Complex */
  children = my_toplex . cover ( bounds );
  clock_t start_clock, stop_clock;
  std::cout << "Initializing complex...\n";
  start_clock = clock ();
  Adaptive_Complex my_adaptive_complex = my_toplex . complex ( children );
  stop_clock = clock ();
  std::cout << (float) ( stop_clock - start_clock ) / (float) CLOCKS_PER_SEC << " elapsed \n";

  //inspect_complex ( my_adaptive_complex );
  compute_example ( my_adaptive_complex );
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
void inspect_complex ( Cell_Complex & my_complex ) {
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

template < class Cell_Complex_Template >
compute_results compute_example ( Cell_Complex_Template & my_complex ) {
	clock_t start_clock, stop_clock, total_time_start, total_time_stop;
	float total_time, morse_time, homology_time;
  unsigned long original_size;
  std::cout << " Size = " << (original_size = my_complex . size ()) << "\n";
  std::cout << "  Original Sizes (by increasing dimension): ";
	for ( unsigned int dim = 0; dim <= my_complex . dimension (); dim ++ ) 
		std::cout << my_complex . size ( dim ) << " ";  
	std::cout << "\n";
	/* Create Morse Complex */
	std::cout << "  Using morse theory to reduce ... \n";
	total_time_start = start_clock = clock ();
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