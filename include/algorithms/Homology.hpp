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
#include <algorithm>
#include <iterator>
#include <ctime>

#include "algorithms/basic.h" /* for verify_complex.h and matrix_solve */
#include "algorithms/matrix/Smith_Normal_Form.h" /* For Smith_Normal_Form(...) */
#include "algorithms/matrix/Sparse_Matrix.h" /* For Sparse_Matrix<...> */
#include "algorithms/Morse_Theory.h"
#include "complexes/Graph_Complex.h"
#include "complexes/Relative_Graph_Complex.h"
#include "tools/visualization.h"  //for debug

// weird -- cannot put visualization before Graph Complex in includes! X11 has some funny business.

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
		Smith_Normal_Form ( smith_form_results [ dimension_index ], matrix ); 
  }
	
  //DEBUG
  //for ( unsigned int dimension_index = 0; dimension_index <= the_complex . dimension (); dimension_index ++ ) {
	//	for ( unsigned int index = 0; index < smith_form_results [ dimension_index ] . size (); index ++ ) {
  //		if ( smith_form_results [ dimension_index ] [ index ] . first != 0 ) {/* Thus, specialized ring classes should overload != */
	//			std::cout << "(" << dimension_index << "; " << index << ": " << 
  //      smith_form_results [ dimension_index ] [ index ] . first << ", " << smith_form_results [ dimension_index ] [ index ] . second << ")\n"; 
  //    }
  //  }
  //}
  
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
        if ( abs ( smith_form_results [ dimension_index + 1 ] [ index ] . first ) > 1 ) {
          if ( first_torsion_subgroup ) {
            first_torsion_subgroup = false;
            if ( printed_betti ) std::cout << " + "; 
          } else {
            std::cout << " + ";
          } /* if-else */
          std::cout << "Z_" << abs ( smith_form_results [ dimension_index + 1 ] [ index ] . first );
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
  //std::cout << "Homology_Generators_SNF\n";
  //verify_complex ( complex );
	std::vector < std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > > return_value ( complex . dimension () + 1 );
	typedef typename Cell_Complex::Ring Ring;
  typedef Sparse_Matrix<Ring> Matrix;
	/* Loop through Chain Groups */
  Matrix first_Vinv, second_U, second_Uinv, second_V, second_Vinv;
  int first_t, second_s, second_t;
	/* The d_{0} boundary matrix is zero, so its SNF has no non-zero elements on its diagonal. */
	first_t = 0;
	for ( unsigned int dimension_index = 0; dimension_index <= complex . dimension (); ++ dimension_index ) {
		/* Compute the SNF of the new boundary matrix, d_{dimension_index} */
    //std::cout << "\n\n\nCalculating at dimension levels (" << dimension_index << " - " << dimension_index + 1 << "):\n";
    Matrix smith_diagonal, boundary_matrix;
    Sparse_Matrix_Boundary_Map ( boundary_matrix, complex, dimension_index + 1 );
    
    //DEBUG
    //std::cout << " D_" << dimension_index + 1 << " = \n";
    //print_matrix ( boundary_matrix ); std::cout << "\n";
    //std::cout << "     " << boundary_matrix . number_of_rows () << " x " << boundary_matrix . number_of_columns () << "\n";
    
    SmithNormalForm( &second_U, &second_Uinv, &second_V, &second_Vinv, &smith_diagonal, boundary_matrix);
    second_s = 0;
    for ( int i = 0; i < std::min(smith_diagonal . number_of_rows (), smith_diagonal . number_of_columns ()); ++ i ) {
      if ( smith_diagonal . read ( i, i ) == Ring(1) ) {
        /* WARNING, only works for integers, not general ring, in general need to check if unit */
        ++ second_s; 
      }
    }
    second_t = std::min(smith_diagonal . number_of_rows (), smith_diagonal . number_of_columns ());
    for ( int i = 0; i < std::min(smith_diagonal . number_of_rows (), smith_diagonal . number_of_columns ()); ++ i ) {
      if ( smith_diagonal . read ( i, i ) == Ring(0) ) {
        second_t = i; 
        break;
      }
    }
    
    
    /* Here is the current method of calculation.
       We want to obtain the generators.
       The torsion generators can be read right off from the middle columns of U_2.
       To obtain the betti generators, here is a possible solution:
           1 Get the bottom rows of U^{-1}_2. Call it A. 
           2 Get the the rightmost columns of V^{-1}_1. Call it B.
           3 Get the rightmost rows of U_2, call it C.
           4 Form the product P = CAB.
           5 Find the smith normal form UDV of P. Claim: D is a projection.
       Now the betti generators are the non-zero columns of U corresponding to non-zero entries in D
     */
		/* Obtain the relevant sub-matrix from second_Qinv */
		Matrix A, B, C, U, Uinv, V, Vinv, D, P, G;

    if ( dimension_index == 0 ) {
      typename Matrix::Index n = complex . size ( 0 );
      C . resize ( n, n );
      for ( typename Matrix::Index i = 0; i < n; ++ i ) C . write ( i, i, Ring ( 1 ) );
    } else {
      Submatrix ( & C, 
                 0, first_Vinv . number_of_rows () - 1, 
                 first_t, first_Vinv . number_of_columns () - 1,
                 first_Vinv );
    }
    Submatrix ( & A, 
               second_t, second_U . number_of_rows () - 1, 
               0, second_U . number_of_columns () - 1,
               second_U );
    Submatrix ( & B, 
               second_t, second_Uinv . number_of_rows () - 1, 
               0, second_Uinv . number_of_columns () - 1,
               second_Uinv );
    P = A * (B * C);
    SmithNormalForm( &U, &Uinv, &V, &Vinv, &D, P);
    G = U * D;
    
    //std::cout << "Here is U_2 = ";
    //print_matrix ( second_U); std::cout << "\n";
    
    //std::cout << "The betti generators are in the following matrix:\n";
    //print_matrix ( G );
    //std::cout << "\n";
		/* Prepare the output */
		const unsigned int betti_number = second_U . number_of_rows () - first_t - second_t;
		const unsigned int torsion_number = second_t - second_s;
    const unsigned int trivial_number = second_s;
    //std::cout << " betti number = " << betti_number << ", torsion number = " << torsion_number << ", trivial number = " << trivial_number << "\n";
		std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > & generators = return_value [ dimension_index ];
		if ( trivial_generators ) {
      generators . resize ( betti_number + torsion_number + trivial_number );
    } else {
      generators . resize ( betti_number + torsion_number );
    } /* if-else */
    
    typedef typename Cell_Complex::size_type size_type;
    /* Insert the betti generators */
    //std::cout << "The betti number is " << betti_number << ".\n";
    //sane_matrix ( G );
    unsigned int betti_index = 0;
		for (typename Matrix::size_type column_number = 0; 
        column_number < G . number_of_columns (); ++ column_number ) {
      if ( G . column_size ( column_number ) == 0 ) continue;
			generators [ betti_index ] . second = 0;
			typename Cell_Complex::Chain & generator_chain = generators [ betti_index ] . first;
      ++ betti_index;
      size_type index = complex . index_begin ( dimension_index );
      //std::cout << "inspecting column " << column_number << "\n";
      //std::cout << "starting at index " << index << "\n";
			for (typename Matrix::Index entry = G . column_begin ( column_number ); 
           entry != G . end (); G . column_advance ( entry ) ) {
        typename Matrix::Index j = G . row ( entry );
        //std::cout << "   found an entry at (" << j << ") --> " << complex . lookup ( j + index ) << "\n";
				generator_chain += std::make_pair ( complex . lookup ( j + index ), G . read ( entry ) );
      } /* for */
      //std::cout << "Betti Chain: " << generator_chain << "\n";
		}
    
    
		/* Insert the torsion generators */
		for ( unsigned int torsion_index = 0; torsion_index < torsion_number; ++ torsion_index ) {
			generators [ betti_number + torsion_index ] . second = 
        smith_diagonal . read ( second_s + torsion_index + 1, second_s + torsion_index + 1);
			typename Cell_Complex::Chain & generator_chain = generators [ betti_number + torsion_index ] . first;
			size_type index = complex . index_begin ( dimension_index );
      for (typename Matrix::Index entry = second_U . column_begin ( second_s + torsion_index ); 
           entry != G . end (); G . column_advance ( entry ) ) {
        typename Matrix::Index j = G . row ( entry );
        generator_chain += std::make_pair ( complex . lookup ( j + index ), G . read ( entry ) );
      } /* for */
      //std::cout << "Torsion Chain: (order = " << generator_chain << "\n";
		}
    
    /* Insert the trivial generators */
    if ( trivial_generators )
      for ( unsigned int trivial_index = 0; trivial_index < trivial_number; ++ trivial_index ) {
        generators [ betti_number + torsion_number + trivial_index ] . second = 1;
        typename Cell_Complex::Chain & generator_chain = generators [ betti_number + torsion_number + trivial_index ] . first;
        size_type index = complex . index_begin ( dimension_index );
        for (typename Matrix::Index entry = second_U . column_begin ( trivial_index ); 
             entry != G . end (); G . column_advance ( entry ) ) {
          typename Matrix::Index j = G . row ( entry );
          generator_chain += std::make_pair ( complex . lookup ( j + index ), G . read ( entry ) );
        } /* for */
        //std::cout << "trivial Chain: (order = " << generator_chain << "\n";
      }
    
		/* Store second_* into first_* */
    
		first_Vinv = second_Vinv;
		first_t = second_t;
		
	} /* for */
  //std::cout << "Returning from HGSNF\n";
	return return_value; 
} /* void Homology_Generators_SNF(...) */

template < class Cell_Complex >
std::vector < std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > > 
Homology_Generators_DMT ( Cell_Complex & complex ) {
  std::vector < std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > > 
    return_value;
  /* Perform Successive Morse Reductions to complex */
  //std::cout << "Homology_Generators_DMT: calling reduction_tower \n";
  std::list<Morse_Complex> morse_tower = morse::reduction_tower ( complex ); 
  /* Get the Homology Generators on the deepest level */
  //std::cout << "Homology_Generators_DMT: calling SNF \n";
  std::vector < std::vector < std::pair < typename Morse_Complex::Chain, unsigned int > > > 
    deep_generators = Homology_Generators_SNF ( morse_tower . back (), false );
  
  //DEBUG
  
  /*
  std::cout << "deep_generators . size () = " << deep_generators . size () << "\n";
  for ( unsigned int i = 0; i < deep_generators . size (); ++ i ) {
    std::cout << "deep_generators [ " << i << " ] . size () = " << deep_generators [ i ] . size () << "\n";
    
    std::cout << "Dimension " << i << "\n";
    for ( unsigned 
         int j = 0; j < deep_generators [ i ] . size (); ++ j ) {
      std::cout << "   ->" << deep_generators [ i ] [ j ] . first << "\n";
    }
  }
  */
  
  /* Lift the Homology Generators to the top level */
  //std::cout << "Homology_Generators_DMT: lifting generators \n";
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
      // DEBUG
      //std::cout << " At dimension_index = " << dimension_index << ", there is a generator " << return_value [ dimension_index ] [ generator_index ] . first << "\n";
    } /* for */
  } /* for */
  return return_value;
} /* Homology_Generators_DMT */

template < class Cell_Complex >
std::vector < std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > > 
Homology_Generators ( Cell_Complex & complex ) {
  return Homology_Generators_DMT ( complex );
}

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
  std::cout << "Calling Homology_Generators_DMT...\n";
  std::vector < std::vector < std::pair < typename Graph::Chain, unsigned int > > > 
    graph_generators = Homology_Generators_DMT ( graph );
  stop = clock ();
  for ( unsigned int a = 0; a < graph_generators . size (); ++ a ) std::cout << 
    graph_generators [ a ] . size () << " ";
  std::cout << "Graph generators computed.\n";

  std::cout << "Elapsed time = " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";
  
  

  /* Get the morse reduction of the codomain */
  std::list<Morse_Complex> codomain_tower = morse::reduction_tower ( graph . codomain () );
  
  /* Find the homology generators in the morse reduction of the codomain */
  start = clock ();
  std::cout << "Calling Homology_Generators_SNF...\n";
  std::vector < std::vector < std::pair < typename Morse_Complex::Chain, unsigned int > > > 
    codomain_generators = Homology_Generators_SNF ( codomain_tower . back (), true );
  stop = clock ();
  for ( unsigned int a = 0; a < codomain_generators . size (); ++ a ) std::cout << 
    codomain_generators [ a ] . size () << " ";
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
  typedef typename Complex::Ring Ring;
  typedef Sparse_Matrix<Ring> Matrix;
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
    Matrix matC = SparseMatrixRepresentation < Morse_Complex > (reduced_cycles [ dimension_index ], 
                                                dimension_index, 
                                                codomain_tower . back () );
    Matrix matG = SparseMatrixRepresentation < Morse_Complex > (codomain_generators [ dimension_index ], 
                                                dimension_index, 
                                                codomain_tower . back () );
    std::cout << "Dimension " << dimension_index << ": \n";
    //std::cout << matC << "\n";
    //std::cout << matG << "\n";
    /* Solve for matX:  matG * matX = matC */
    Matrix matX = matrix_solve ( matG, matC );
    Matrix output;
    Submatrix ( & output, 
               0, matX . number_of_rows () - trivial_count - 1, 
               0, matX . number_of_columns () - 1 );
    
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



/* Given a single toplex, with subsets X, A, Y, B, and a combinatorial map F : X -> Y which restricts
   to F : A -> B, compute the relative homology of F. */
template < class Toplex, class Subset, class Combinatorial_Map >
void /* TODO */ Relative_Map_Homology (const Toplex & toplex, 
                                       const Subset X, 
                                       const Subset A,
                                       const Subset Y, 
                                       const Subset B,
                                       const Combinatorial_Map & F ) {
  /* Method.
   1. Construct the Relative Graph G
     Definition of relative graph now follows: Define F_A : A -> B as the restriction of F to A.
     The graph of F_A is a regular subcomplex of the graph of F. 
     We define the relative graph as Gr(G) = Gr(F) \ Gr(F_A)
   2. Lift the Homology Generators of (X, A) to cycles in G and Project these cycles to (Y, B)
   3. Construct Morse Tower for (Y, B) and send the cycles via psi down the morse tower
   4. Compute the generators of M_N(Y, B)
   5. Express the cycles in terms of the codomain generators.
  */
  typedef typename Relative_Graph_Complex < Toplex, Combinatorial_Map >::Relative_Complex Complex;
  typedef Relative_Graph_Complex < Toplex, Combinatorial_Map > Graph;
  
  /* Produce the graph complex */
  std::cout << "RMH: Generating Graph Complex...\n";
  clock_t start0, start, stop;
  start = start0 = clock ();
  Graph graph ( toplex, X, A, Y, B, F );
  stop = clock ();
  
  
  std::cout << "Graph Complex generated.\n";
  std::cout << "Elapsed time = " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";
  
  //DEBUG
  
  //std::cout << "Inspecting (X, A) complex...\n";
  //utility::inspect_complex ( graph . domain () );
  //std::cout << "Complex inspected.\n";
  
  
  // DEBUG
  start = clock ();
  std::cout << "Computing Domain Generators, lifting to Graph, and projecting to Codomain.\n";
  
  /* Find the homology generators of the domain */
  std::vector < std::vector < std::pair < typename Complex::Chain, unsigned int > > > 
    domain_generators = Homology_Generators_DMT ( graph . domain () );


  /* Lift the homology generators from the domain to the graph and project them to the codomain */
  std::vector < std::vector < std::pair < typename Complex::Chain, unsigned int > > > codomain_cycles ( domain_generators . size () );
  for ( unsigned int dimension_index = 0; dimension_index < domain_generators . size (); ++ dimension_index ) {
//#define VISUALIZE_DEBUG
#ifdef VISUALIZE_DEBUG
    for ( unsigned int generator_index = 0; generator_index < domain_generators [ dimension_index ] . size (); ++ generator_index ) {
      // First let's take a look at the domain generators.
      Adaptive_Complex & complex = * graph . full_domain_;
      ComplexVisualization < Adaptive_Complex > * cv = new ComplexVisualization < Adaptive_Complex > ( "Starting: here is the domain generator." );
      cv -> drawRelativeComplex ( complex, graph . domain (), 0, 100 );
      Adaptive_Complex::Chain my_chain = graph . domain () . include ( domain_generators [ dimension_index ] [ generator_index ] . first );
      cv -> drawChain ( complex, my_chain, 200 );
      // explore a minute here
      cv -> explore ();
      delete cv;
    }
#endif
    codomain_cycles [ dimension_index ] . resize ( domain_generators [ dimension_index ] . size () );
    //std::cout << " dimension_index = " << dimension_index << "\n";
    for ( unsigned int generator_index = 0; generator_index < domain_generators [ dimension_index ] . size (); ++ generator_index ) {
      codomain_cycles [ dimension_index ] [ generator_index ] = 
      std::make_pair ( graph . projectToCodomain ( graph . cycleLift ( domain_generators [ dimension_index ] [ generator_index ] . first ) ),
                       domain_generators [ dimension_index ] [ generator_index ] . second );
      //std::cout << "  generator_index = " << generator_index << "\n";
      //std::cout << "   Lifting the domain-cycle " << domain_generators [ dimension_index ] [ generator_index ] . first << "\n";
      
#ifdef VISUALIZE_DEBUG
      /*
      {
      Adaptive_Complex & complex = * graph . full_domain_;
      ComplexVisualization < Adaptive_Complex > * cv = new ComplexVisualization < Adaptive_Complex > ( "domain projection of cycle lift");
      cv -> drawRelativeComplex ( complex, graph . domain (), 0, 100 );
      typename Complex::Chain p_chain = graph . projectToDomain ( graph . cycleLift ( domain_generators [ dimension_index ] [ generator_index ] . first ) );
      Adaptive_Complex::Chain my_chain = graph . domain () . include ( p_chain );
        std::cout << "DEBUG: lifted domain-cycle should project right back to itself: " << p_chain << "\n";
      cv -> drawChain ( complex, my_chain, 200 );
      // explore a minute here
      char c;
      std::cin >> c;
      delete cv;
      }
       */
#endif
      //std::cout << "   Projecting the graph-cycle "; 
      //graph . printChain ( graph . cycleLift ( domain_generators [ dimension_index ] [ generator_index ] . first ) );
      //std::cout << "\n";
      //std::cout << "   Storing codomain-cycle " << codomain_cycles [ dimension_index ] [ generator_index ] . first << "\n";
      
#ifdef VISUALIZE_DEBUG
      Adaptive_Complex & complex = * graph . full_codomain_;
      ComplexVisualization < Adaptive_Complex > * cv = new ComplexVisualization < Adaptive_Complex > ( "Finished: Here is the codomain cycle!");
      cv -> drawRelativeComplex ( complex, graph . codomain (), 0, 100 );
      Adaptive_Complex::Chain my_chain = graph . codomain () . include ( codomain_cycles [ dimension_index ] [ generator_index ] . first );
      cv -> drawChain ( complex, my_chain, 200 );
      // explore a minute here
      cv -> explore ();
      delete cv;
#endif
    } /* for */
  } /* for */


  stop = clock ();
  std::cout << "Generators projected to codomain.\n";
  std::cout << "Elapsed time = " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";
  
  /* Get the morse reduction of the codomain */
  std::cout << "Computing Morse Tower of codomain.\n";
  start = clock ();
  std::list<Morse_Complex> codomain_tower = morse::reduction_tower ( graph . codomain () );
  stop = clock ();
  std::cout << "Elapsed time = " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";

  //DEBUG
  start = clock ();
  std::cout << "Homology generators of morse reduced codomain...\n";
  /* Find the homology generators in the morse reduction of the codomain */
  std::vector < std::vector < std::pair < typename Morse_Complex::Chain, unsigned int > > > 
  codomain_generators = Homology_Generators_SNF ( codomain_tower . back (), true );
  
  //DEBUG
  stop = clock ();
  //for ( unsigned int a = 0; a < codomain_generators . size (); ++ a ) std::cout << 
  //  codomain_generators [ a ] . size () << " ";
  std::cout << "Codomain generators computed.\n";
  std::cout << "Elapsed time = " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";
  

  
  /* Project the codomain cycles to the morse-reduced codomain */
  start = clock ();
  std::vector < std::vector < std::pair < typename Morse_Complex::Chain, unsigned int > > > 
  reduced_cycles ( codomain_cycles . size () );
  for ( unsigned int dimension_index = 0; 
       dimension_index < codomain_cycles . size (); 
       ++ dimension_index ) {
    reduced_cycles [ dimension_index ] . resize ( codomain_cycles [ dimension_index ] . size () );
    for ( unsigned int generator_index = 0; 
         generator_index < codomain_cycles [ dimension_index ]. size (); 
         ++ generator_index ) {
      reduced_cycles [ dimension_index ] [ generator_index ] . first = 
      morse::psi_tower (codomain_cycles [ dimension_index ] [ generator_index ] . first,
                        codomain_tower, graph . codomain () );
      reduced_cycles [ dimension_index ] [ generator_index ] . second = 
      codomain_cycles [ dimension_index ] [ generator_index ] . second;
      
    } /* for */
  } /* for */
  stop = clock ();
  std::cout << "Generators projected to morse reduced codomain.\n";
  std::cout << "Elapsed time = " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";
  
  /* Write the reduced cycles in terms of the reduced codomain generators. */
  
  start = clock ();
  /* Re-express reduced_cycles in basis given by codomain_generators */
  /* Need to index the cells of the codomain */
  typedef typename Complex::Ring Ring;
  typedef Sparse_Matrix<Ring> Matrix;
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
    Matrix matC = SparseMatrixRepresentation < Morse_Complex > (reduced_cycles [ dimension_index ], 
                                                      dimension_index, 
                                                      codomain_tower . back () );
    Matrix matG = SparseMatrixRepresentation < Morse_Complex > (codomain_generators [ dimension_index ], 
                                                      dimension_index, 
                                                      codomain_tower . back () );
    std::cout << "Dimension " << dimension_index << ": \n";
    //std::cout << "matC"; print_matrix ( matC ); std::cout << "\n";
    //std::cout << "matG"; print_matrix ( matG ); std::cout << "\n";
    /* Solve for matX:  matG * matX = matC */
    Matrix matX = matrix_solve ( matG, matC );
    Matrix output;
    Submatrix ( & output, 
               0, matX . number_of_rows () - trivial_count - 1, 
               0, matX . number_of_columns () - 1,
               matX );
    
    //std::cout << trivial_count << "\n";
    std::cout << "output"; print_matrix ( output ); std::cout << "\n";
  } /* for */
  
  stop = clock ();
  std::cout << "Algebraic reexpression of projected cycles in terms of generators complete.\n";
  std::cout << "Elapsed time = " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";

  std::cout << "Relative Homology time = " << (float) ( stop - start0 ) / (float) CLOCKS_PER_SEC << "\n";
  
  
  
  /* Return algebraic information */
  return /* TODO */;
  
} /* Relative_Map_Homology */

/* CONLEY INDEX */

// note:: assuming Map::mapped_type == Subset, so that needs to be fixed
template < class Toplex, class Subset, class Map > 
void Conley_Index ( Conley_Index_t * output,
              const Toplex & toplex, 
              const Subset & S,
              /* const */ Map & f ) {
  /* Method.
  1. Let F be the combinatorial map on "toplex" that "f" induces
  2. Generate a combinatorial index pair (X, A)
     A combinatorial index pair satisfies the following:
      a) F(X\A) \subset X
      b) F(A) \cap X \subset A
     Since S is a Path-SCC (Combinatorial Morse set) in "toplex" with respect to F,
     the following is a combinatorial index pair:
     X = S \cup F(S), A = F(S) \ S.
     proof: We show (a). Observe that X \ A = S, so F(X\A) = F(S) \subset X trivially. Hence (a). 
            We show (b). First we make the following observation from the definition of Path-SCC:
            Since S is a combinatorial Morse set, (t \in F(S) and s \in F(t) \cap S) implies t \in S.
            Suppose t \in A, s \in F(t), and s \in X. We show s \in A. Assume, to the contrary, that s \notin A.
            Then s \in X \ A = S. We then have t \in A \subset F(S), and s \in F(t) \cap S. Thus t \in S, by
            the observation. Yet t \in A \cap S = \emptyset is impossible! Hence (b).
   
  3. Generate the restricted combinatorial map G : (X, A) -> (X, A) via the formula
     G = F\vert_A \cap A
     That is, for each x \in X, define G(x) := F(x) \cap A.
     A theorem gives us F_* = G_*.
  4. Return the Relative Homology G_* of G
  */
  
  typedef typename Toplex::Top_Cell Cell;
  typedef std::map < Cell, Subset > Combinatorial_Map;
  

  
  clock_t start, start0, stop;
  std::cout << "Conley Index. Preparing computation...\n";
  start0 = start = clock ();
  /* Construct G on S, and also X and S in hash set forms called X_cells and S_cells */
  // note: All cells in X may be obtained as images of cells in S.
  typedef std::unordered_set<Cell> CellDictionary;
  CellDictionary X_cells;
  CellDictionary S_cells;
  Combinatorial_Map G;
  
  std::insert_iterator<CellDictionary> Xc_ii ( X_cells, X_cells . begin () );
  BOOST_FOREACH ( Cell cell, S ) {
    S_cells . insert ( cell );
    Subset & image = G [ cell ] = Subset ();
    std::insert_iterator<Subset> image_ii ( image, image . end () );
    toplex . cover ( image_ii, f ( toplex . geometry ( toplex . find ( cell ) ) ) );
    std::copy ( image . begin (), image . end (), Xc_ii );
  } /* boost_foreach */
  
  /* Construct X and A */
  // note: The cells in A are those in X not in S.
  Subset X, A;
  std::insert_iterator<Subset> X_ii ( X, X . begin () );
  std::insert_iterator<Subset> A_ii ( A, A . begin () );
  BOOST_FOREACH ( Cell cell, X_cells ) {
    * X_ii ++ = cell;
    if ( S_cells . count ( cell ) == 0 ) {
      * A_ii ++ = cell;
      
    }
  } /* boost_foreach */ 
  
  /* Compute G for domain cells in A */
  // note: we restrict the ranges to A 
  BOOST_FOREACH ( Cell cell, A ) {
    Subset image;
    std::insert_iterator<Subset> image_ii ( image, image . end () );
    toplex . cover ( image_ii, f ( toplex . geometry ( toplex . find ( cell ) ) ) );
    G [ cell ] = Subset ();
    std::insert_iterator<Subset> G_ii ( G [ cell ], G [ cell ] . begin () );
    BOOST_FOREACH ( Cell image_cell, image ) {
      if ( X_cells . count ( image_cell ) != 0 ) * G_ii ++ = image_cell; // better to look in A_cells, but i didn't make it
    }
    
  } /* boost_foreach */  
  
  /* DEBUG CONLEY INDEX */
  /*
  std::cout << "Geometry of X\n";
  BOOST_FOREACH ( Cell cell, X ) {
    std::cout << cell << " = " << toplex . geometry ( cell ) << "\n";
  }
  
  std::cout << "Geometry of A\n";
  BOOST_FOREACH ( Cell cell, A ) {
    std::cout << cell << " = " << toplex . geometry ( cell ) << "\n";
  }

  */
  stop = clock ();
  std::cout << "Conley Index computation prepared as relative map homology problem.\n";
  std::cout << "Elapsed time = " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";

  start = clock ();
  std::cout << "Conley_Index: calling Relative_Map_Homology.\n";
  Relative_Map_Homology ( toplex, X, A, X, A, G );
  stop = clock ();
  
  std::cout << "Conley Index computed. Total time = " << (float) ( stop - start0 ) / (float) CLOCKS_PER_SEC << "\n";
  std::cout << "Number of cells in X = " << X . size () << "\n Cells per second = " << (float) X . size () * (float) CLOCKS_PER_SEC / (float) ( stop - start0 )  << "\n";
  
  return;
} /* void Conley_Index(...) */

