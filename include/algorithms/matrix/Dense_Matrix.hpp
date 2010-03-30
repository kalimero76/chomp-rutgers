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
    The 'translation table' feature gives us the correspondence between row numbers and Elementary Cells 
    The matrix is the boundary matrix that yields (dimension - 1)-chains from (dimension)-chains       */

template < class Cell_Complex >
void Dense_Matrix_Boundary_Map ( typename Dense < typename Cell_Complex::Ring >::Matrix & output_matrix,
                                 std::vector < typename Cell_Complex::Cell > & translation_table,
                                 const Cell_Complex & complex, const unsigned int dimension ) {
    /* Initialize the matrix */
    if ( dimension == 0 ) {
        output_matrix = typename Dense < typename Cell_Complex::Ring >::Matrix ( 0, complex . cells [ dimension ] . size () );
        return;
    } else if ( dimension == complex . dimension + 1 ) {
        output_matrix = typename Dense < typename Cell_Complex::Ring >::Matrix ( complex . cells [ dimension - 1 ] . size (), 0 );
        return;
    } else
        output_matrix = typename Dense < typename Cell_Complex::Ring >::Matrix ( complex . cells [ dimension - 1 ] . size (),
                                                                        complex . cells [ dimension ] . size () );
    
    
    /* First we make the translation table. */
    translation_table . resize ( complex . cells [ dimension - 1 ] . size () + 1 );
    std::map < typename Cell_Complex::Cell, unsigned long >  inverse_table;
    /* scope */ { 
    unsigned long index = 0;
    for ( typename Cell_Complex::const_iterator cell = complex . cells [ dimension - 1 ] . begin (); 
    cell != complex . cells [ dimension - 1 ] . end (); ++ cell ) {
        ++ index;
        translation_table [ index ] = *cell; 
        inverse_table [ *cell ] = index;
    } /* for */
    
    } /* scope */
    
    /* Return if at top dimension + 1. */
    if ( dimension == complex . dimension + 1 ) return;
    
    /* Now we loop through all elementary chains of dimension "dimension". */
    unsigned long index = 0;
    for ( typename Cell_Complex::const_iterator right_cell = complex . cells [ dimension ] . begin (); 
    right_cell != complex . cells [ dimension ] . end (); ++ right_cell ) {
        ++ index;
        /* We find the boundary of the current elementary chain. */
        typename Cell_Complex::Chain boundary_chain; 
        complex . Boundary_Map ( boundary_chain, right_cell );
        /* We loop through the terms in the boundary we have found. */
        for ( typename Cell_Complex::Chain::iterator chain_term = boundary_chain . begin (); 
        chain_term != boundary_chain . end (); ++ chain_term ) 
            output_matrix ( inverse_table [ chain_term -> first ], index ) = chain_term -> second; 
    } /* for */
} /* Dense_Matrix_Boundary_Map */
