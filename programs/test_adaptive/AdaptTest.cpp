//============================================================================
// Name        : AdaptTest.cpp
// Author      : Miro Kramar
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#define _CHOMP_HEADER_ONLY_

#include <iostream>


#include <time.h>

#include "complexes/Cubical_Complex.h"	/* for class Cubical_Complex */
#include "algorithms/Homology.h"			/* for function Homology(...)*/
#include "complexes/Adaptive_Complex.h"
#include "complexes/Morse_Complex.h"
#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for Cell_Complex */

typedef Default_Chain::Elementary_Chain Elementary_Chain;
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

/*
	split.resize(1);
	split[0].resize(2);

	split[0][0] = 0;
	split[0][1] = 0;
	my_adaptive_complex.Add_Full_Cube(split);

	split.resize(2);
	split[0].resize(2);
	split[1].resize(2);

	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < 2; ++j)
		{
			if ( i != 0 || j != 0){
				split[0][0] = i;
				split[0][1] = j;
				for(int k = 0; k < 2; ++k)
					for(int l = 0; l < 2; ++l)
						if( i == k || j == l){
							split[1][0] = k;
							split[1][1] = l;
							my_adaptive_complex.Add_Full_Cube(split);
					}
			}
		}
*/

/*
	Chain chain_out;

	for ( unsigned int dimension_index = 0; dimension_index <= my_adaptive_complex . dimension; ++ dimension_index ){
		std::cout << "Number of cells of dim " << dimension_index << " is: " << my_adaptive_complex.Chain_Groups [ dimension_index ].size() << "\n";
		for ( Adaptive_Complex::Container::const_iterator my_iterator = my_adaptive_complex . Chain_Groups [ dimension_index ] . begin ();
		my_iterator != my_adaptive_complex . Chain_Groups [ dimension_index ] . end (); ++ my_iterator ) {
			std::cout << "\n d" << my_iterator -> first << " = " << my_adaptive_complex . Boundary_Map ( chain_out, my_iterator ) << ".";
		}
	}


	for ( unsigned int dimension_index = 0; dimension_index <= copy_adaptive_complex . dimension; ++ dimension_index ){
		std::cout << "Number of cells of dim " << dimension_index << " is: " << copy_adaptive_complex.Chain_Groups [ dimension_index ].size() << "\n";
		for ( Adaptive_Complex::Container::const_iterator my_iterator = copy_adaptive_complex . Chain_Groups [ dimension_index ] . begin ();
		my_iterator != copy_adaptive_complex . Chain_Groups [ dimension_index ] . end (); ++ my_iterator ) {
			std::cout << "\n" << "Cube " <<  ( my_iterator -> first.name >> (2 * copy_adaptive_complex . dimension ) ) ;
			std::cout << "\n d" << my_iterator -> first << " = " << copy_adaptive_complex . Coboundary_Map ( chain_out, my_iterator ) << ".";
		}
	}

*/

	std::vector<int> Betti_Numbers, Minimal_Number_of_Generators;
	Homology <   Adaptive_Complex  > ( Betti_Numbers, Minimal_Number_of_Generators, my_adaptive_complex );

	Morse_Complex < Adaptive_Complex > my_morse_complex ( my_adaptive_complex );
	Homology <   Morse_Complex < Adaptive_Complex > > ( Betti_Numbers, Minimal_Number_of_Generators, my_morse_complex );

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}


/*
 * int cube;
	int  side;
	int temp;
	long ring;
	Chain chain_out;
	std::vector<double> coordinates;
	for ( unsigned int dimension_index = 0; dimension_index <= my_adaptive_complex . dimension; ++ dimension_index ){
		std::cout << "Number of cells of dim " << dimension_index << " is: " << my_adaptive_complex.Chain_Groups [ dimension_index ].size() << "\n";
		for ( Adaptive_Complex::Container::const_iterator my_iterator = my_adaptive_complex . Chain_Groups [ dimension_index ] . begin ();
		my_iterator != my_adaptive_complex . Chain_Groups [ dimension_index ] . end (); ++ my_iterator ) {
			cube = ( ( my_iterator -> first.name ) >> (2 * my_adaptive_complex . dimension) );
			side = my_iterator -> first.name - ( cube << (2 * my_adaptive_complex . dimension) );
			std::cout << "Looking at the elementary cell " << my_iterator -> first <<  " Cube " << cube <<  " Side ";
			my_adaptive_complex.Coordinates_Of_Elementary_Chain(my_iterator -> first, coordinates);
			std::cout << "Cube coordinates " << "\n";
			for(int i = 0; i < coordinates.size(); ++i )
				std::cout << "Dim " << i << " : " <<  coordinates[i] << "\n";

			temp = side >> my_adaptive_complex . dimension;
			for(unsigned int i = 0; i < my_adaptive_complex.dimension; ++i){
				if( temp % 2 != 0 )
				{
					std::cout << "1";
				}
				else
					std::cout << "0";
				temp = temp >> 1;
			}
			std::cout << "||";
			temp = side;
			for(unsigned int i = 0; i < my_adaptive_complex.dimension; ++i){
				if( temp % 2 != 0 )
				{
					std::cout << "1";
				}
				else
					std::cout << "0";
				temp = temp >> 1;
			}

			std::cout << "\n";
			std::cout << "bundary \n";
			my_adaptive_complex.Boundary_Map(chain_out, my_iterator);
			for(std::map < Default_Elementary_Chain, Default_Ring >::iterator chain_it = chain_out.begin(); chain_it != chain_out.end(); ++chain_it){
				cube = ( ( chain_it -> first.name ) >> (2 * my_adaptive_complex . dimension) );
				side = chain_it -> first.name - ( cube << (2 * my_adaptive_complex . dimension) );
				ring = chain_it -> second;
				std::cout << "Ring : " << ring << " Cube : " << cube << "\n";
				temp = side >> my_adaptive_complex . dimension;
				for(unsigned int i = 0; i < my_adaptive_complex.dimension; ++i){
					if( temp % 2 != 0 )
					{
						std::cout << "1";
					}
					else
						std::cout << "0";
					temp = temp >> 1;
				}
				std::cout << "||";
				temp = side;
				for(unsigned int i = 0; i < my_adaptive_complex.dimension; ++i){
					if( temp % 2 != 0 )
					{
						std::cout << "1";
					}
					else
						std::cout << "0";
					temp = temp >> 1;
				}
				std::cout << "\n";
			}

		}
	}

	//Chain chain_out;

	//TestComplex.Find_Elementary_Cell( chain_out, chain);
	//cout << chain_out.size()<< "\n";
	//map<Elementary_Chain, Ring >::iterator it;
	//it = chain_out.begin();
	//cout << "Cahin " <<  (*it).first << " Ring " << (*it).second << "\n";

	unsigned int dimension_index;
	dimension_index = 1;
	for ( Adaptive_Complex::Container::const_iterator my_iterator = TestComplex . Chain_Groups [ dimension_index ] . begin ();
			my_iterator != TestComplex . Chain_Groups [ dimension_index ] . end (); ++ my_iterator ) {
			TestComplex.Boundary_Map(chain_out, my_iterator);
			cout << chain_out.size() << "\n";
	}
 */
