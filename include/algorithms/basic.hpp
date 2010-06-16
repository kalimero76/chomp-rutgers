/*
 *  basic.hpp
 *  
 *
 *  Created by Shaun Harker on 4/29/10.
 *  Copyright 2010. All rights reserved.
 *
 */

template < class Complex >
typename Complex::Chain boundary ( const typename Complex::Chain & input, const Complex & complex ) {
  typename Complex::Chain return_value;
  for ( typename Complex::Chain::const_iterator term_iterator = input . begin (); 
        term_iterator != input . end (); ++ term_iterator ) {
    typename Complex::Chain summand = complex . boundary ( term_iterator -> first );
    summand *= term_iterator -> second;
    return_value += summand;
  } /* for */
  return return_value;
} /* boundary */

template < class Chain >
Chain boundary ( const Chain & input ) {
  Chain return_value ( input . container () );
  for ( typename Chain::const_iterator term_iterator = input . begin (); term_iterator != input . end (); ++ term_iterator ) {
    Chain summand = input . container () . boundary ( term_iterator -> first );
    summand *= term_iterator -> second;
    return_value += summand;
  }
  return return_value;
} /* boundary */

template < class Chain >
Chain coboundary ( const Chain & input ) {
  Chain return_value ( input . container () );
  for ( typename Chain::const_iterator term_iterator = input . begin (); term_iterator != input . end (); ++ term_iterator ) {
    Chain summand = input . container () . coboundary ( term_iterator -> first );
    summand *= term_iterator -> second;
    return_value += summand;
  }
  return return_value;
} /* coboundary */


/* * * * * * * * * * * * * * * * * * * * * * * *
 * TEST UTILITIES -- Common testing functions  *
 * * * * * * * * * * * * * * * * * * * * * * * */
#ifdef TEST_PROGRAM

#include "algorithms/Homology.h"		/* for function Homology(...)*/
#include "algorithms/Morse_Theory.h"		/* for morse::deep_reduction */
namespace utility {

template < class T >
void print_my_vector ( std::vector<T> print_me ) {
  for ( unsigned int index = 0; index < print_me . size (); ++ index ) std::cout << print_me [ index ] << " "; }

template < class T >
T vector_sum ( const std::vector<T> add_my_elements_together ) {
	T return_value(0);
	for ( unsigned int index = 0; index < add_my_elements_together . size (); ++ index ) 
		return_value += add_my_elements_together [ index ] ;
  return return_value; } /* endfunction */

  template < class T >
  T vector_prod ( const std::vector<T> multiply_my_elements_together ) {
    T return_value(1);
    for ( unsigned int index = 0; index < multiply_my_elements_together . size (); ++ index ) 
      return_value *= multiply_my_elements_together [ index ] ;
    return return_value; } /* endfunction */
  
template < class Cell_Complex >
void inspect_complex ( Cell_Complex & my_complex ) {
  std::cout << "Inspecting the complex!\n";
  std::cout << "The complex has dimension " << my_complex . dimension () << "\n";
  std::cout << "The complex has size " << my_complex . size () << "\n";
  std::cout << "begins at " << * my_complex . begin () << "\n";
  typename Cell_Complex::const_iterator it = my_complex . begin ();
  ++ it;
  std::cout << "next is " << * it << "\n";
  for ( typename Cell_Complex::const_iterator iter = my_complex . begin (); 
        iter != my_complex . end (); ++ iter ) {
    typename Cell_Complex::Chain my_chain = my_complex . boundary ( iter );
    typename Cell_Complex::Chain another_chain = my_complex . coboundary ( iter );
    std::cout << " bd ( " << *iter << ") = " << my_chain << "\n";
    std::cout << " cbd ( " << *iter << ") = " << another_chain << "\n";
  } /* for */
} /* inspect_complex */
  
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

} /* namespace utility */

#endif
