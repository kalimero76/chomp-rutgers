//============================================================================
// Name        : AdaptTest.cpp
// Author      : Miro Kramar
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#define CHOMP_HEADER_ONLY_

#include <iostream>


#include <time.h>

#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
#include "algorithms/Homology.h"			/* for function Homology(...)*/
#include "complexes/Adaptive_Complex.h"
#include "complexes/Morse_Complex.h"
#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for Cell_Complex */

typedef Default_Chain::Cell Cell;
typedef Default_Chain Chain;
typedef Default_Chain::Ring Ring;

using namespace std;


int main() {
	Adaptive_Complex my_adaptive_complex(4);
	vector<vector<bool> > split;

	split.resize(1);
	split[0].resize(4);




	split[0][0] = false;
	split[0][1] = false;
	split[0][2] = false;
	split[0][3] = false;
	//split[0][4] = 1;
	my_adaptive_complex.Add_Full_Cube(split);
	my_adaptive_complex.Finalize();
	Adaptive_Complex copy_adaptive_complex = my_adaptive_complex;



	std::vector<int> Betti_Numbers, Minimal_Number_of_Generators;
	Homology <   Adaptive_Complex  > ( Betti_Numbers, Minimal_Number_of_Generators, my_adaptive_complex );

	Morse_Complex < Adaptive_Complex > my_morse_complex ( my_adaptive_complex );
	Homology <   Morse_Complex < Adaptive_Complex > > ( Betti_Numbers, Minimal_Number_of_Generators, my_morse_complex );

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}



