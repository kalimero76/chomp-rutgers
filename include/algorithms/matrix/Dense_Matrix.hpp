/*
 * Dense_Matrix.hpp
 * CHomP
 * Shaun Harker
 *
 * This hpp file contains implementations for Dense Matrices.
 *
 */ 

#include <map>

/* This function produces a "Dense_Matrix" to represent one of the boundary matrices of a cell complex.
    The 'translation table' feature gives us the correspondence between row numbers and Cells 
    The matrix is the boundary matrix that yields (dimension - 1)-chains from (dimension)-chains       */

template < class Cell_Complex >
void Dense_Matrix_Boundary_Map ( typename Dense < typename Cell_Complex::Ring >::Matrix & output_matrix,
                                 std::vector < typename Cell_Complex::const_iterator > & translation_table,
                                 const Cell_Complex & complex, const unsigned int dimension ) {
    /* Initialize the matrix */
    if ( dimension == 0 ) {
        output_matrix = typename Dense < typename Cell_Complex::Ring >::Matrix 
          ( 0, complex . size ( dimension ) );
        return;
    } else if ( dimension == complex . dimension () + 1 ) {
        output_matrix = typename Dense < typename Cell_Complex::Ring >::Matrix 
          ( complex . size ( dimension - 1 ), 0 );
        return;
    } else
        output_matrix = typename Dense < typename Cell_Complex::Ring >::Matrix 
          ( complex . size ( dimension - 1 ),
            complex . size ( dimension ) );
    
    
    /* First we make the translation table. */
    translation_table . resize ( complex . size ( dimension - 1 ) + 1 );
    std::map < typename Cell_Complex::const_iterator, unsigned long >  inverse_table;
    /* scope */ { 
    unsigned long index = 0;
    for ( typename Cell_Complex::const_iterator cell_iterator = complex . begin ( dimension - 1 ); 
    cell_iterator != complex . end ( dimension - 1 ); ++ cell_iterator ) {
        ++ index;
        translation_table [ index ] = cell_iterator; 
        inverse_table [ cell_iterator ] = index;
    } /* for */
    
    } /* scope */
    
    /* Return if at top dimension + 1. (Because bd = 0) */
    if ( dimension == complex . dimension () + 1 ) return;
    
    /* Now we loop through all "dimension"-cells and fetch boundary information */
    unsigned long index = 0;
    for ( typename Cell_Complex::const_iterator right_cell_iter = complex . begin ( dimension ); 
    right_cell_iter != complex . end ( dimension ); ++ right_cell_iter ) {
        ++ index;
        /* We find the boundary of the current elementary chain. */
      std::cout << "The boundary of " << (* right_cell_iter) << "\n";
        typename Cell_Complex::Chain boundary_chain = complex . boundary ( right_cell_iter );
        /* We loop through the terms in the boundary we have found. */
        for ( typename Cell_Complex::Chain::iterator chain_term = boundary_chain . begin (); 
        chain_term != boundary_chain . end (); ++ chain_term ) 
            output_matrix ( inverse_table [ chain_term -> first ], index ) = chain_term -> second; 
    } /* for */
} /* Dense_Matrix_Boundary_Map */
