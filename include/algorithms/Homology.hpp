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

#include "algorithms/Morse_Theory.h"

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
	std::vector < smith_form_output_type > smith_form_results ( the_complex . dimension () + 1);

	for ( unsigned int dimension_index = 0;  dimension_index <= the_complex . dimension (); dimension_index ++ ) {
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
	std::vector < int > number_of_free_generators_of_chain_complex ( the_complex . dimension () + 1);

	for ( unsigned int dimension_index = 0; dimension_index <= the_complex . dimension (); dimension_index ++ ) 
		number_of_free_generators_of_chain_complex [ dimension_index ] = the_complex . size ( dimension_index );
	
	/* Next we need to know the rank of the boundary maps. This can be determined from the SNF. */
	std::vector < int > rank_of_boundary_homomorphism ( the_complex . dimension () + 2 );
	
	rank_of_boundary_homomorphism [ the_complex . dimension () + 1 ] = 0;
	for ( unsigned int dimension_index = 0; dimension_index <= the_complex . dimension (); dimension_index ++ ) {
		rank_of_boundary_homomorphism [ dimension_index ] = 0;
		for ( unsigned int index = 0; index < smith_form_results [ dimension_index ] . size (); index ++ )
			if ( smith_form_results [ dimension_index ] [ index ] . first != 0 ) /* Thus, specialized ring classes should overload != */
				rank_of_boundary_homomorphism [ dimension_index ] += smith_form_results [ dimension_index ] [ index ] . second; }
		
		
	
	/* Now we are ready to compute the Betti numbers */
	
	std::vector < int > Betti_numbers ( the_complex . dimension () + 1 );

	for ( unsigned int dimension_index = 0; dimension_index <= the_complex . dimension (); dimension_index ++ ) 
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
	for ( unsigned int dimension_index = 0; dimension_index <= the_complex . dimension (); dimension_index ++ ) {
		bool printed_betti = false;
		std::cout << "H_" << dimension_index << " =";
		if ( Betti_numbers [ dimension_index ] > 0 ) {
			std::cout << " Z^" << Betti_numbers [ dimension_index ];
			printed_betti = true; 
    } /* if */
		bool first_torsion_subgroup = true;
    if ( dimension_index < the_complex . dimension () ) {
      for ( unsigned int index = 0; index < smith_form_results [ dimension_index + 1 ] . size (); ++ index ) {
        if ( smith_form_results [ dimension_index + 1 ] [ index ] . first > 1 ) {
          if ( first_torsion_subgroup ) {
            first_torsion_subgroup = false;
            if ( printed_betti ) std::cout << " + "; 
          } else {
            std::cout << " + ";
          } /* if-else */
          std::cout << "Z_" << smith_form_results [ dimension_index + 1 ] [ index ] . first;
          if ( smith_form_results [ dimension_index ] [ index ] . second > 1 ) 
            std::cout << "^" << smith_form_results [ dimension_index + 1 ] [ index ] . second;
          minimal_number_of_generators [ dimension_index ] += smith_form_results [ dimension_index + 1 ] [ index ] . second; 
        } /* if */
      } /* for */
    } /* if */
    if ( !printed_betti && first_torsion_subgroup ) std::cout << " 0";
    std::cout << "\n"; 
  } /* for */
  minimal_number_of_generators_output = minimal_number_of_generators;
	return; 
} /* void Homology(...) */
	
/* Compute Homology Groups and also Homology generators */
/*

First, calculate the smith form of each boundary matrix, along with the transformation matrices
CAPD smithForm gives us 
 B_in = Q*B_out*Rinv and 
 s = number of "1"'s in the diagonal
 t = number of non-zero diagonal entries

The trivial (boundary) generators are the left-most columns of Q
The torsion (weak boundary) generators are the middle columns of Q.
The betti number generators require a little work. The relevant information
 is involved in the Smith Normal Form decomposition of two consecutive boundary matrices.
 One also needs (R, Rinv) from the previous decomposition. Then the betti generators are given by
 a basis for the intersection of the spaces spanned by the right-most columns of R and the
 right-most columns of Q. 
 
This is because:
 a) The right-most columns of R are a basis for the cycle subspace.
 b) The right-most columns of Q are a basis for the non-boundary subspace. 

The matrix given by the bottom-most rows of Qinv (corresponding to the right-most columns of Q)
 is thus interesting because it gives a matrix which takes a chain, 'projects the boundary part out',
 and expresses it in the Q-basis.

In particular the product M=(bottom-Qinv)*(right-R) is important; to obtain the Betti generators
 we must determine the rank r of this matrix (which is the betti number) and obtain exactly r
 linearly independent column vectors which span the range of M. We then multiply these columns
 by right-Q to get back into the ambient space.
 
Proof:
 
 (It needs to be shown that any r linearly independent columns in this
matrix has the same span as the entire matrix itself.)

*/

template < class Cell_Complex >
std::vector < std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > > 
Homology_Generators_SNF ( const Cell_Complex & complex, bool trivial_generators ) {
	std::vector < std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > > return_value ( complex . dimension () + 1 );
	typedef typename Dense<typename Cell_Complex::Ring>::Matrix Matrix;

	/* Loop through Chain Groups */
	Matrix first_R, second_Q, second_Qinv, second_R, second_Rinv;
	int first_t, second_s, second_t;
	/* The d_{0} boundary matrix is zero, so its SNF has no non-zero elements on its diagonal. */
	first_t = 0;
	for ( unsigned int dimension_index = 0; dimension_index <= complex . dimension (); ++ dimension_index ) {
		/* Compute the SNF of the new boundary matrix, d_{dimension_index} */
		std::vector < typename Cell_Complex::const_iterator > translation_table;
    Matrix boundary_matrix;
		Dense_Matrix_Boundary_Map ( boundary_matrix, translation_table, complex, dimension_index + 1 );
    
    if ( dimension_index < complex . dimension () )
      capd::matrixAlgorithms::smithForm( boundary_matrix, second_Q, second_Qinv, second_R, second_Rinv, second_s, second_t);
    else {
      second_Qinv = second_Q = Matrix::Identity( complex . size ( complex . dimension () ) );
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
		const unsigned int betti_number = second_Q . numberOfColumns () - first_t - second_t;
		const unsigned int torsion_number = second_t - second_s;
    const unsigned int trivial_number = second_s;
    
		std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > & generators = return_value [ dimension_index ];
		if ( trivial_generators ) {
      generators . resize ( betti_number + torsion_number + trivial_number );
    } else {
      generators . resize ( betti_number + torsion_number );
    } /* if-else */
    
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
				generator_chain += std::pair < typename Cell_Complex::const_iterator, typename Cell_Complex::Ring > ( translation_table [ ++index ], *entry );
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
				generator_chain += std::pair < typename Cell_Complex::const_iterator, typename Cell_Complex::Ring > ( translation_table [ ++index ], *entry );
      //std::cout << "Torsion Chain: (order = " << generators [ betti_number + torsion_index ] . second  << ") " << generator_chain << "\n";
		}
    
    /* Insert the trivial generators */
    if ( trivial_generators )
		for ( unsigned int trivial_index = 0; trivial_index < trivial_number; ++ trivial_index ) {
			generators [ betti_number + torsion_number + trivial_index ] . second = 1;
			typename Cell_Complex::Chain & generator_chain = generators [  betti_number + torsion_number + trivial_index  ] . first;
			ColumnVector < typename Cell_Complex::Ring, 0> generator_vector = second_Q . column ( trivial_index );
			unsigned long index = 0;
			for ( typename ColumnVector < typename Cell_Complex::Ring, 0> :: iterator entry = generator_vector . begin (); 
           entry != generator_vector . end (); ++ entry ) 
				generator_chain += std::pair < typename Cell_Complex::const_iterator, typename Cell_Complex::Ring > ( translation_table [ ++index ], *entry );
      //std::cout << "Torsion Chain: (order = " << generators [ betti_number + torsion_index ] . second  << ") " << generator_chain << "\n";
		} /* for */
    
		/* Store second_* into first_* */
    
		first_R = second_R;
		first_t = second_t;
		
	} /* for */
	return return_value; 
} /* void Homology_Generators_SNF(...) */

template < class Cell_Complex >
std::vector < std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > > 
Homology_Generators_DMT ( Cell_Complex & complex ) {
  std::vector < std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > > 
    return_value;
  /* Perform Successive Morse Reductions to complex */
  std::cout << "Homology_Generators_DMT: calling reduction_tower \n";
  std::list<Morse_Complex> morse_tower = morse::reduction_tower ( complex ); 
  /* Get the Homology Generators on the deepest level */
  std::cout << "Homology_Generators_DMT: calling SNF \n";
  std::vector < std::vector < std::pair < typename Morse_Complex::Chain, unsigned int > > > 
    deep_generators = Homology_Generators_SNF ( morse_tower . back (), false );
  /* Lift the Homology Generators to the top level */
  std::cout << "Homology_Generators_DMT: lifting generators \n";
  return_value . resize ( complex . dimension () + 1 );
  for (unsigned int dimension_index = 0; 
       dimension_index <= complex . dimension ();
       ++ dimension_index) {
    return_value [ dimension_index ] . resize ( deep_generators [ dimension_index ] . size () );
    for (unsigned int generator_index = 0;
         generator_index < deep_generators [ dimension_index ] . size ();
         ++ generator_index) {
      return_value [ dimension_index ] [ generator_index ] = std::make_pair 
        (morse::phi_tower
         (deep_generators [ dimension_index ] [ generator_index ] . first,
                      morse_tower, complex),
         deep_generators [ dimension_index ] [ generator_index ] . second);
    } /* for */
  } /* for */
  return return_value;
} /* Homology_Generators_DMT */

namespace Homology_detail {
  
  /* Solve AX = B. warning -- writes over A
     assume A has linearly independent columns */
  template < class Matrix >
  Matrix matrix_solve ( Matrix & A, Matrix & B ) {
    
    Matrix Q, Qinv, R, Rinv;
    int s, t;
    capd::matrixAlgorithms::smithForm( A, Q, Qinv, R, Rinv, s, t);
    MatrixSlice<Matrix> Qinv_slice ( Qinv, 1, A . numberOfColumns (), 1, Qinv . numberOfColumns () );
    Matrix slice ( Qinv_slice );
    //std::cout << "smithForm: " << A << "\n";
    Matrix Y = slice * B;
    int n = Y . numberOfRows ();
    int m = Y . numberOfColumns ();
    
    for (int i = 1; i <= n; ++ i ) {
      for ( int j = 1; j <= m; ++ j ) {
        Y(i,j)=Y(i,j)/A(i,i);
      } /* for */
    } /* for */
    return R * Y;
  } /* matrix_solve */
  
} /* namespace Homology_detail */

#include <ctime>
#include "complexes/Graph_Complex.h"
#include "algorithms/basic.h"

/* Compute the homology of maps using "H_*(G) method" */
template < class Toplex, class Map >
void /* TODO */ Map_Homology ( const Toplex & X, const Toplex & Y, const Map & f ) {
  typedef typename Toplex::Complex Complex;
  typedef Graph_Complex < Toplex > Graph;
  
  /* Produce the graph complex */
  clock_t start, stop;
  start = clock ();
  Graph graph ( X, Y, f );
  stop = clock ();
  
  std::cout << "Graph Complex generated.\n";
  std::cout << "Elapsed time = " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";
  
  /* Find the homology generators of the graph */
  start = clock ();
  std::vector < std::vector < std::pair < typename Graph::Chain, unsigned int > > > 
    graph_generators = Homology_Generators_DMT ( graph );
  stop = clock ();
  std::cout << "Graph generators computed.\n";

  std::cout << "Elapsed time = " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";
  
  

  /* Get the morse reduction of the codomain */
  std::list<Morse_Complex> codomain_tower = morse::reduction_tower ( graph . codomain () );
  
  /* Find the homology generators in the morse reduction of the codomain */
  start = clock ();
  std::vector < std::vector < std::pair < typename Morse_Complex::Chain, unsigned int > > > 
    codomain_generators = Homology_Generators_SNF ( codomain_tower . back (), true );
  stop = clock ();
  std::cout << "Codomain generators computed.\n";
  std::cout << "Elapsed time = " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";

  /* Project the homology generators of the graph to the codomain */
  start = clock ();
  std::vector < std::vector < std::pair < typename Complex::Chain, unsigned int > > > 
    codomain_cycles ( graph_generators . size () );
  for ( unsigned int dimension_index = 0; 
        dimension_index < graph_generators . size (); 
        ++ dimension_index ) {
    codomain_cycles [ dimension_index ] . resize ( graph_generators [ dimension_index ] . size () );
    for ( unsigned int generator_index = 0; 
         generator_index < graph_generators [ dimension_index ]. size (); 
         ++ generator_index ) {
      //std::cout << "Graph generator: " << graph_generators [ dimension_index ] 
      //[ generator_index ] . first << "\n";
      codomain_cycles [ dimension_index ] 
                      [ generator_index ] . first = 
        graph . projectToCodomain ( graph_generators [ dimension_index ] 
                                                     [ generator_index ] . first );
      codomain_cycles [ dimension_index ] [ generator_index ] . second = 
        graph_generators [ dimension_index ] [ generator_index ] . second;
      //std::cout << "Projected to codomain: " << codomain_cycles [ dimension_index ] 
      //[ generator_index ] . first << "\n";
      //std::cout << "Projected to domain: " << graph . projectToDomain ( graph_generators [ dimension_index ] 
      //                           [ generator_index ] . first ) << "\n";

    } /* for */
  } /* for */
  stop = clock ();
  std::cout << "Generators projected to codomain.\n";
  std::cout << "Elapsed time = " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";

  /* Project the codomain cycles to the morse-reduced codomain */
  start = clock ();
  std::vector < std::vector < std::pair < typename Morse_Complex::Chain, unsigned int > > > 
  reduced_cycles ( graph_generators . size () );
  for ( unsigned int dimension_index = 0; 
       dimension_index < graph_generators . size (); 
       ++ dimension_index ) {
    reduced_cycles [ dimension_index ] . resize ( graph_generators [ dimension_index ] . size () );
    for ( unsigned int generator_index = 0; 
         generator_index < graph_generators [ dimension_index ]. size (); 
         ++ generator_index ) {
      reduced_cycles [ dimension_index ] [ generator_index ] . first = 
      morse::psi_tower (codomain_cycles [ dimension_index ] [ generator_index ] . first,
                        codomain_tower, graph . codomain () );
      reduced_cycles [ dimension_index ] [ generator_index ] . second = 
      codomain_cycles [ dimension_index ] [ generator_index ] . second;
     
    } /* for */
  } /* for */
  stop = clock ();
  std::cout << "Generators projected to codomain.\n";
  std::cout << "Elapsed time = " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";
  
  /* Write the reduced cycles in terms of the reduced codomain generators. */
  
  start = clock ();
  /* Re-express reduced_cycles in basis given by codomain_generators */
  /* Need to index the cells of the codomain */
  typedef typename Dense<typename Complex::Ring>::Matrix Matrix;
  for (unsigned int dimension_index = 0; 
       dimension_index < codomain_generators . size (); 
       ++ dimension_index ) {
    unsigned int trivial_count = 0;
    for (unsigned int codomain_generator_index = 0; 
         codomain_generator_index < codomain_generators [ dimension_index ] . size ();
         ++ codomain_generator_index ) {
      if (codomain_generators [ dimension_index ] [ codomain_generator_index ] . second == 1 ) {
        ++ trivial_count;
      } /* if */
    } /* for */
    /* Create cycle matrix */
    Matrix matC = chains_to_matrix < Morse_Complex > (reduced_cycles [ dimension_index ], 
                                                dimension_index, 
                                                codomain_tower . back () );
    Matrix matG = chains_to_matrix < Morse_Complex > (codomain_generators [ dimension_index ], 
                                                dimension_index, 
                                                codomain_tower . back () );
    std::cout << "Dimension " << dimension_index << ": \n";
    //std::cout << matC << "\n";
    //std::cout << matG << "\n";
    /* Solve for matX:  matG * matX = matC */
    Matrix matX = Homology_detail::matrix_solve ( matG, matC );
    MatrixSlice<Matrix> output_slice ( matX, 1, matX . numberOfRows () - trivial_count, 1, matX . numberOfColumns () );
    Matrix output ( output_slice );
    //std::cout << matX << "\n";
    //std::cout << trivial_count << "\n";
    std::cout << output << "\n";
  } /* for */
 
  stop = clock ();
  std::cout << "Algebra complete.\n";
  std::cout << "Elapsed time = " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";

  
  
  /* Return algebraic information */
  return /* TODO */;
} /* void Map_Homology(...) */
 
#if 0 

/* Compute the homology of maps using "preboundary method" */
template < class Toplex, class Map >
void Map_Homology_V2 ( const Toplex & X, const Toplex & Y, const Map & f ) {
  typedef Toplex::Complex Complex;
  Complex complex_X = X . complex ();
  Complex complex_Y = Y . complex ();
  Graph_Complex < Toplex, Map > graph ( X, Y, f );
  
  /* Step 1. Obtain a homology generator g in X. */
  /* Step 2. Find a cycle c in the graph which projects to g: \pi_X ( c ) = g */
  /*  a) Find a chain c which projects to g
   b) For i = 1 ... dim Y
   For each i-fiber F in the graph containing a term of dc, 
   c -= preboundary in F of \pi_F ( dc )
   end
   end
   Step 3. Project c into Y to obtain h = \pi_Y ( c )
   Step 4. Write h in terms of a generator basis for Y.
   */
  
  for ( unsigned int dimension_index = 1; dimension_index <= Y . dimension (); ++ dimension_index ) {
    
  } /* for */
} /* void Map_Homology(...) */
#endif
