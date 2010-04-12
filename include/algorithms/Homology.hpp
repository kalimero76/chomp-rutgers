/*
 * Homology.hpp
 * CHomP
 * Shaun Harker
 *
 * This hpp file contains implementations for the homology interface.
 *
 */ 

#include <iostream>
#include <vector>   /* For vector<...> */
#include <utility>  /* For pair<...> */

#include "algorithms/matrix/Smith_Normal_Form.h" /* For Smith_Normal_Form(...) */
#include "algorithms/matrix/Sparse_Matrix.h" /* For Sparse_Matrix<...> */
#include "algorithms/matrix/Dense_Matrix.h"

#include "capd/matrixAlgorithms/intMatrixAlgorithms.hpp" /* for smithForm */

/* Compute Homology Groups via Smith Normal Form */
template < class Geometric_Complex_Template >
void Homology ( const Geometric_Complex_Template & the_complex ) {
	std::vector<int> Betti_output, minimal_number_of_generators_output;
	Homology ( Betti_output, minimal_number_of_generators_output, the_complex ); } /* endfunction */

/* Compute Homology Groups via Smith Normal Form */
template < class Geometric_Complex_Template >
void Homology ( std::vector<int> & Betti_output, std::vector<int> & minimal_number_of_generators_output, const Geometric_Complex_Template & the_complex ) {

	/* Step 1 
	 *		Produce matrices representing the boundary homomorphisms (aka boundary maps)
	 *		Compute their smith normal forms and store them in the appropriate place "smith_form_results"
	 */

	/* The output of Smith Normal Form is a vector of pairs of ring elements and their muliplicity */
	/* For example, 1 1 1 2 2 6 would yield a vector containing three elements: [ (1, 3), (2, 2), (6, 1) ] */

	typedef std::vector < std::pair < typename Geometric_Complex_Template::Ring, int > > smith_form_output_type;	
	std::vector < smith_form_output_type > smith_form_results ( the_complex . dimension + 1);

	for ( unsigned int dimension_index = 0;  dimension_index <= the_complex . dimension; dimension_index ++ ) {
		/* Compute the boundary map d_{dimension_index} : C_{dimension_index} -> C_{dimension_index - 1} */
		Sparse_Matrix < typename Geometric_Complex_Template::Ring > matrix;
		Sparse_Matrix_Boundary_Map ( matrix, the_complex, dimension_index );
		
		/* Compute the Smith Normal Form of the boundary homomorphism "matrix" *
		 * and store the answer in smith_form_results [ dimension_index ]      */
		Smith_Normal_Form ( smith_form_results [ dimension_index ], matrix ); }
	
	/* Step 2
	 *		Use SNF calculation to produce homology groups
	 */
	 
	/* First we need to know the size of the chain groups */
	std::vector < int > number_of_free_generators_of_chain_complex ( the_complex . dimension + 1);

	for ( unsigned int dimension_index = 0; dimension_index <= the_complex . dimension; dimension_index ++ ) 
		number_of_free_generators_of_chain_complex [ dimension_index ] = the_complex . cells [ dimension_index ] . size ();
	
	/* Next we need to know the rank of the boundary maps. This can be determined from the SNF. */
	std::vector < int > rank_of_boundary_homomorphism ( the_complex . dimension + 2 );
	
	rank_of_boundary_homomorphism [ the_complex . dimension + 1 ] = 0;
	for ( unsigned int dimension_index = 0; dimension_index <= the_complex . dimension; dimension_index ++ ) {
		rank_of_boundary_homomorphism [ dimension_index ] = 0;
		for ( unsigned int index = 0; index < smith_form_results [ dimension_index ] . size (); index ++ )
			if ( smith_form_results [ dimension_index ] [ index ] . first != 0 ) /* Thus, specialized ring classes should overload != */
				rank_of_boundary_homomorphism [ dimension_index ] += smith_form_results [ dimension_index ] [ index ] . second; }
		
		
	
	/* Now we are ready to compute the Betti numbers */
	
	std::vector < int > Betti_numbers ( the_complex . dimension + 1 );

	for ( unsigned int dimension_index = 0; dimension_index <= the_complex . dimension; dimension_index ++ ) 
		Betti_numbers [ dimension_index ] = 
			number_of_free_generators_of_chain_complex [ dimension_index ]
			- rank_of_boundary_homomorphism [ dimension_index ]
			- rank_of_boundary_homomorphism [ dimension_index + 1];

	Betti_output = Betti_numbers; // should maybe do a return value optimization by not making these different in the first place
	
	std::vector < int > minimal_number_of_generators = Betti_numbers;

	/* Step 3
	 *		Output the homology groups
	 */
	 
	 /* minor first_print problem if betti number is zero and there is torsion */
	 /* if group is trivial nothing is printed, that is also a problem */
	for ( unsigned int dimension_index = 0; dimension_index <= the_complex . dimension; dimension_index ++ ) {
		bool printed_betti = false;
		std::cout << "H_" << dimension_index << " =";
		if ( Betti_numbers [ dimension_index ] > 0 ) {
			std::cout << " Z^" << Betti_numbers [ dimension_index ];
			printed_betti = true; }
		bool first_torsion_subgroup = true;
		for ( unsigned int index = 0; index < smith_form_results [ dimension_index ] . size (); index ++ )
			if ( smith_form_results [ dimension_index ] [ index ] . first > 1 ) {
				for ( int multiplicity = 0; multiplicity < smith_form_results [ dimension_index ] [ index ] . second; multiplicity ++ ) {
					if ( first_torsion_subgroup ) {
						first_torsion_subgroup = false;
						if ( printed_betti ) std::cout << " + "; 
						std::cout << "Z_" << smith_form_results [ dimension_index ] [ index ] . first;}
					else std::cout << " + Z_" << smith_form_results [ dimension_index ] [ index ] . first; } /* for */
				minimal_number_of_generators [ dimension_index ] += smith_form_results [ dimension_index ] [ index ] . second; } /* if */
		if ( !printed_betti && first_torsion_subgroup ) std::cout << " 0";
		std::cout << "\n"; }
	minimal_number_of_generators_output = minimal_number_of_generators;
	return; } /* void Homology(...) */
	
/* Compute Homology Groups and also Homology generators */
/*

First, calculate the smith form of each boundary matrix, along with the transformation matrices
CAPD smithForm gives us B_in = Q*B_out*Rinv and t = number of non-zero diagonal entries

The torsion generators can be pulled directly out of the 'middle' columns of Q.
The betti number generators require a little work.

The right-most columns of R_k are a basis for the cycle subspace.
We need to know which cycles are neither boundaries nor weak boundaries.
If we perform the product Q_inv * z for some cycle z, then we see how to write z 
in terms of the 'image basis'. The right-most columns of Q, however, are not boundaries. If we 
project out all but the bottom components of Q_inv z, and then multiply by Q, we get a new cycle
which is equivalent in the sense that it has been deformed by the addition of boundaries. 
Thus, we become interested in the matrix we obtain by multiplying the bottom-most rows of Q_inv to 
the right-most rows of R. We need extract columns then from (bottom of Qinv) * (right of R), but
we need to be careful because the columns need not be linearly independent. Thus there is some
work to determine the rank r of this matrix (which is the betti number) and pull out exactly r
linearly independent columns. (It needs to be shown that any r linearly independent columns in this
matrix has the same span as the entire matrix itself.)

*/

template < class Cell_Complex >
std::vector < std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > > 
Homology_Generators ( const Cell_Complex & complex ) {
	std::vector < std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > > return_value ( complex . dimension + 1 );
	typedef typename Dense<typename Cell_Complex::Ring>::Matrix Matrix;
	/* Loop through Chain Groups */
	Matrix first_R, second_Q, second_Qinv, second_R, second_Rinv;
	int first_t, second_s, second_t;
	/* The d_{0} boundary matrix is zero, so its SNF has no non-zero elements on its diagonal. */
	first_t = 0;
	for ( unsigned int dimension_index = 0; dimension_index <= complex . dimension; ++ dimension_index ) {
		/* Compute the SNF of the new boundary matrix, d_{dimension_index} */
		std::vector < typename Cell_Complex::Cell > translation_table;
        Matrix boundary_matrix;
		Dense_Matrix_Boundary_Map ( boundary_matrix, translation_table, complex, dimension_index + 1 );
        
        if ( dimension_index < complex . dimension )
            capd::matrixAlgorithms::smithForm( boundary_matrix, second_Q, second_Qinv, second_R, second_Rinv, second_s, second_t);
        else {
            second_Qinv = second_Q = Matrix::Identity( complex . cells [ complex . dimension ] . size () );
            second_s = second_t = 0;
        }

		/* Obtain the relevant sub-matrix from second_Qinv */
		MatrixSlice<Matrix> image_slice ( second_Qinv, second_t + 1, second_Qinv . numberOfRows (), 1, second_Qinv . numberOfColumns () );
		Matrix image_cycles;
        
		if ( dimension_index > 0 ) {
			Matrix image ( image_slice );
			/* Obtain the relevant sub-matrix from first_R */
			MatrixSlice<Matrix> cycle_slice ( first_R, 1, first_R . numberOfRows (), first_t + 1, first_R . numberOfColumns () );
			Matrix cycle ( cycle_slice );
			/* REMARK: Had to copy the slices because CAPD doesn't support * on MatrixSlice class. */
			image_cycles = image * cycle;
        } else { /* Everything is a cycle */
			image_cycles = Matrix ( image_slice );
		}
                
		/* Put "image_cycles" into columnEchelon form by column operations. */
		Matrix S, Sinv; int l;
        S = Sinv = Matrix ( image_cycles . numberOfColumns (), image_cycles . numberOfColumns () ); // columnEchelon should do this.
        capd::matrixAlgorithms::columnEchelon ( image_cycles, S, Sinv, l );
        
        /* Now convert the answer into the correct space */
        MatrixSlice<Matrix> image_inv_slice ( second_Q, 1, second_Q . numberOfRows (), second_t + 1, second_Q . numberOfColumns () );
		Matrix image_inv ( image_inv_slice );
		Matrix betti_generators_matrix= image_inv * image_cycles;

		/* The generators are now sitting in the first n - t_1 - t_2 columns of betti_generators. */
		/* Prepare the output */
		unsigned int betti_number = second_Q . numberOfColumns () - first_t - second_t;
		unsigned int torsion_number = second_t - second_s;
		std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > & generators = return_value [ dimension_index ];
		generators . resize ( betti_number + torsion_number );
		using namespace capd::vectalg;

		/* Insert the betti generators */
        //std::cout << "The betti number is " << betti_number << ".\n";
		for ( unsigned int betti_index = 0; betti_index < betti_number; ++ betti_index ) {
			generators [ betti_index ] . second = 0;
			typename Cell_Complex::Chain & generator_chain = generators [ betti_index ] . first;
			ColumnVector < typename Cell_Complex::Ring, 0> generator_vector = betti_generators_matrix . column ( betti_index );
			unsigned long index = 0;
			for ( typename ColumnVector < typename Cell_Complex::Ring, 0> :: iterator entry = generator_vector . begin (); 
			entry != generator_vector . end (); ++ entry ) 
				generator_chain += std::pair < typename Cell_Complex::Cell, typename Cell_Complex::Ring > ( translation_table [ ++index ], *entry );
            //std::cout << "Betti Chain: " << generator_chain << "\n";
		}


		/* Insert the torsion generators */
		for ( unsigned int torsion_index = 0; torsion_index < torsion_number; ++ torsion_index ) {
			generators [ betti_number + torsion_index ] . second = boundary_matrix ( second_s + torsion_index + 1, second_s + torsion_index + 1);
			typename Cell_Complex::Chain & generator_chain = generators [ betti_number + torsion_index ] . first;
			ColumnVector < typename Cell_Complex::Ring, 0> generator_vector = second_Q . column ( second_s + torsion_index );
			unsigned long index = 0;
			for ( typename ColumnVector < typename Cell_Complex::Ring, 0> :: iterator entry = generator_vector . begin (); 
			entry != generator_vector . end (); ++ entry ) 
				generator_chain += std::pair < typename Cell_Complex::Cell, typename Cell_Complex::Ring > ( translation_table [ ++index ], *entry );
            //std::cout << "Torsion Chain: (order = " << generators [ betti_number + torsion_index ] . second  << ") " << generator_chain << "\n";

		}
		/* Store second_* into first_* */

		first_R = second_R;
		first_t = second_t;
		
	} /* for */
	return return_value; 
} /* void Homology(...) */
