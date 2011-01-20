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
    The matrix is the boundary matrix that yields (dimension - 1)-chains from (dimension)-chains      
 The indexing uses the "Index Complex" concept. */

template < class Cell_Complex >
void Dense_Matrix_Boundary_Map ( typename Dense < typename Cell_Complex::Ring >::Matrix & output_matrix,
                                const Cell_Complex & complex, const unsigned int dimension ) {
  typedef typename Cell_Complex::size_type size_type;
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
  
  /* Return if at top dimension + 1. (Because bd = 0) */
  if ( dimension == complex . dimension () + 1 ) return;
  
  typedef std::vector < std::pair < size_type, typename Cell_Complex::Ring > > Index_Chain;
  
  size_type low_offset, high_offset;
  high_offset = complex . index_begin ( dimension );
  low_offset = complex . index_begin ( dimension - 1 );
  /* Now we loop through all "dimension"-cells and fetch boundary information */
  for ( typename Cell_Complex::size_type right_cell_index = complex . index_begin ( dimension ); 
       right_cell_index != complex . index_end ( dimension ); ++ right_cell_index ) {
    /* We find the boundary of the current elementary chain. */
    Index_Chain boundary_chain;
    complex . boundary ( boundary_chain, right_cell_index );
    /* We loop through the terms in the boundary we have found. */
    for ( typename Index_Chain::iterator chain_term = boundary_chain . begin (); 
         chain_term != boundary_chain . end (); ++ chain_term ) {
      //std::cout << "writing to output_matrix(" << chain_term -> first - low_offset  << " " << 
      //                               right_cell_index - high_offset << ")m\n";
      //std::cout << " = " << chain_term -> second << "\n";
      output_matrix ( 1 + chain_term -> first - low_offset, 1 + right_cell_index - high_offset ) = chain_term -> second; 
    } /* for */
  } /* for */
} /* Dense_Matrix_Boundary_Map */


template < class Cell_Complex >
typename Dense < typename Cell_Complex::Ring >::Matrix chains_to_matrix ( const std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > & input, const unsigned int dimension, const Cell_Complex & complex ) {
  typedef typename Dense < typename Cell_Complex::Ring >::Matrix Matrix;
  Matrix return_value ( complex . size ( dimension ), input . size () );
  const unsigned int offset = complex . index_begin ( dimension );
  for ( unsigned int index = 0; index < input . size (); ++ index ) {
    for ( typename Cell_Complex::Chain::const_iterator term_iterator = input [ index ] . first . begin (); 
         term_iterator != input [ index ] . first . end (); ++ term_iterator ) {
      return_value ( 1 + complex . index ( term_iterator -> first ) - offset, 1 + index ) = 
      term_iterator -> second;
      //std::cout << "M(" << complex . index ( term_iterator -> first ) - offset << ", " << index << ") = " << 
      // term_iterator -> second << ".\n";
    } /* for */
  } /* for */
  
  return return_value;
} /* chains_to_matrix */
