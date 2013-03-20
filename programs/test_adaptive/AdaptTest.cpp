//============================================================================
// Name        : AdaptTest.cpp
// Author      : Miro Kramar and Shaun Harker
//============================================================================

#define CHOMP_HEADER_ONLY_
#define TEST_PROGRAM
#include <iostream>
#include <vector>
#include <time.h>

#include "complexes/Adaptive_Complex.h"
#include "algorithms/basic.h"

int main() {
	Adaptive_Complex my_adaptive_complex(2);
  {
  std::vector<std::vector<bool> > split ( 2, std::vector<bool> ( 2, false ) );
  
	split[0][0] = false;
	split[0][1] = false;
  split[1][0] = false;
	split[1][1] = false;
	my_adaptive_complex.Add_Full_Cube(split);
  
  split[0][0] = false;
	split[0][1] = false;
  split[1][0] = true;
	split[1][1] = false;
	my_adaptive_complex.Add_Full_Cube(split);
  
  split[0][0] = false;
	split[0][1] = false;
  split[1][0] = false;
	split[1][1] = true;
	my_adaptive_complex.Add_Full_Cube(split);
  
  split[0][0] = false;
	split[0][1] = false;
  split[1][0] = false;
	split[1][1] = true;
	my_adaptive_complex.Add_Full_Cube(split);
  }
  {
  std::vector<std::vector<bool> > split ( 1, std::vector<bool> ( 2, false ) );
  split[0][0] = true;
	split[0][1] = false;
	my_adaptive_complex.Add_Full_Cube(split);
  
  split[0][0] = false;
  split[0][1] = true;
  my_adaptive_complex.Add_Full_Cube(split);
  
  split[0][0] = true;
  split[0][1] = true;
  my_adaptive_complex.Add_Full_Cube(split);
    
  }
	my_adaptive_complex.Finalize();

  utility::inspect_complex ( my_adaptive_complex );
  
	std::vector<int> Betti_Numbers, Minimal_Number_of_Generators;
	my_adaptive_complex . index ();
  Homology ( Betti_Numbers, Minimal_Number_of_Generators, my_adaptive_complex );
  utility::inspect_complex ( my_adaptive_complex );
  
  utility::do_preprocess = true;
  utility::compute_example ( my_adaptive_complex ); 
	return 0;
}
