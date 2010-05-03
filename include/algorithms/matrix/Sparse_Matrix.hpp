/*
 * Sparse_Matrix.hpp
 * CHomP
 * Shaun Harker
 *
 * This hpp file contains implementations for Sparse Matrices.
 *
 */ 

#include "Sparse_Matrix.h"
#include <utility> /* For pair<...> in set_entry */
#include <iostream> /* for cout in debug_info*/

Matrix_Indices::Matrix_Indices ( long row, long column ) : row(row), column(column) { }

bool Matrix_Indices::operator < ( const Matrix_Indices & right_hand_side) const {
	if ( row < right_hand_side . row ) return true;
	if ( row > right_hand_side . row ) return false;
	if ( column < right_hand_side . column ) return true;
	return false; }
	
template < class Ring_Template >
Sparse_Matrix<Ring_Template>::Sparse_Matrix ( void ) : number_of_rows(0), number_of_columns(0) { }

template < class Ring_Template >
void Sparse_Matrix<Ring_Template>::set_entry ( const long row , const long column, const Ring_Template element ) {
	if ( element != 0 ) {
		(this -> operator []) ( Matrix_Indices ( row, column ) ) = element; 
		if ( row >= number_of_rows ) number_of_rows = row + 1;
		if ( column >= number_of_columns) number_of_columns = column + 1; }
	else (this -> erase) ( Matrix_Indices ( row, column ) ); 
} /* set_entry */
	
template < class Ring_Template >
Ring_Template Sparse_Matrix<Ring_Template>::get_entry ( const long row, const long column ) const {
	const Matrix_Indices key ( row, column );
	if ( (this -> count) ( key ) > 0 ) return  ( this -> find ) ( key ) -> second; 
	else return Ring_Template ( 0 ); /* The Ring class should have a constructor which will make its representation of 0 from this */ 
} /* get_entry */

template < class Ring_Template >
Sparse_Matrix<Ring_Template> Sparse_Matrix<Ring_Template>::compress ( void ) const {
	/* This function takes the sparse matrix "*this" and returns a matrix such that every row and column has a non-zero entry which is obtained via
	 * deleting zero rows and columns.  */
	long row_number = 0;
	long column_number = 0;
	Sparse_Matrix<Ring_Template> return_value;
	std::map<long, long> row_numbers;
	std::map<long, long> column_numbers;
	/* Make translation tables that map name to number */
	for ( typename Sparse_Matrix<Ring_Template>::const_iterator index = this -> begin ();
	index != this -> end (); ++ index ) {
		/* Get the row number. If name has not been stored, then store it and allocate a new row number. Otherwise, pull the row number from the lookup table.*/
		long current_row_number;
		long current_row_name = index -> first . row;
		if ( row_numbers . count ( current_row_name ) == 0 ) row_numbers . insert ( std::pair<long, long> ( current_row_name, current_row_number = row_number ++ ) );
		else current_row_number = row_numbers . find ( current_row_name ) -> second;
		/* Get the column number. If name has not been stored, then store it and allocate a new column number. Otherwise, pull the column number from the lookup table.*/
		long current_column_number;
		long current_column_name = index -> first . column;
		if ( column_numbers . count ( current_column_name ) == 0 ) column_numbers . insert ( std::pair<long, long> ( current_column_name, current_column_number = column_number ++ ) );
		else current_column_number = column_numbers . find ( current_column_name ) -> second;
		/* Store the entry into the compressed matrix */
		return_value . set_entry ( current_row_number, current_column_number, index -> second ); } /* for */
	return return_value; 
} /* compress */
	
template < class Ring_Template > void
Sparse_Matrix<Ring_Template>::debug_info ( void ) const {
	std::cout << " The matrix is " << number_of_rows << " by " << number_of_columns << "\n";
	std::cout << " Its entries are: \n ";
	for ( typename Sparse_Matrix<Ring_Template>::const_iterator iter = this -> begin (); iter != this -> end (); iter ++ )
		std::cout << " (" << (*iter) . first . row << ", " << (*iter) . first . column << "; " << (*iter) . second << ")";
	std::cout << "\n"; 
} /* debug_info */

/* Friend Functions */

template < class Ring_Template > 
Sparse_Matrix<Ring_Template> operator * ( const Sparse_Matrix<Ring_Template>  & left_hand_side, const Sparse_Matrix<Ring_Template>  & right_hand_side) {
	/* HORRIBLE, NON-USABLE IMPLEMENTATION PURELY FOR DEBUGGING PURPOSES */
	std::cout << " Warning: Sparse_Matrix operator * ( Sparse_Matrix &, Sparse_Matrix & ) : \n     This implementation of matrix multiplication is for debugging purposes ONLY. It is superbly non-optimal. \n";
	//if ( left_hand_side . number_of_columns != right_hand_side . number_of_rows ) {
		std::cout << " message: Sparse_Matrix operator * ( Sparse_Matrix &, Sparse_Matrix & ) : multiplying " << left_hand_side . number_of_rows << "x" << 
			left_hand_side . number_of_columns << " matrix to " << right_hand_side . number_of_rows << "x" << right_hand_side . number_of_columns << " matrix. \n";
	//	exit (-1); 
	//}
	Sparse_Matrix<Ring_Template> return_value;
	for ( typename Sparse_Matrix<Ring_Template>::const_iterator left_iterator = left_hand_side . begin ();
	left_iterator != left_hand_side . end (); left_iterator ++ ) 
		for ( typename Sparse_Matrix<Ring_Template>::const_iterator right_iterator = right_hand_side . begin ();
		right_iterator != right_hand_side . end (); right_iterator ++ ) 
			if ( left_iterator -> first . column == right_iterator -> first . row ) {
			/*
				printf(" (%li, %li; %li) (%li, %li, %li) ---> (%li, %li, %li) becomes (%li, %li, %li) \n", 
					left_iterator -> first . row, left_iterator -> first . column, left_iterator -> second,
					right_iterator -> first . row, right_iterator -> first . column, right_iterator -> second, 
					left_iterator -> first . row, right_iterator -> first . column, 
					return_value . get_entry ( left_iterator -> first . row,  right_iterator -> first . column ) ,
					left_iterator -> first . row, right_iterator -> first . column, 
					left_iterator -> second  * right_iterator -> second + 
					return_value . get_entry ( left_iterator -> first . row,  right_iterator -> first . column ) ); */
					
				return_value . set_entry ( left_iterator -> first . row, right_iterator -> first . column, left_iterator -> second  * right_iterator -> second
					+ return_value . get_entry ( left_iterator -> first . row,  right_iterator -> first . column ) );

			} /* if */
	
	std::cout << return_value << "\n";
	return return_value; 
} /* operator * */

template < class Ring_Template > 
std::ostream & operator << ( std::ostream & output_stream, const Sparse_Matrix<Ring_Template> & print_me) {
	for ( typename Sparse_Matrix<Ring_Template>::const_iterator index = print_me . begin ();
	index != print_me . end (); ++ index ) output_stream << "(" << index -> first . row << ", " << index -> first . column << "; " << index -> second << ") ";
	return output_stream; } 
/* operator << */

/* This function produces a "Sparse_Matrix" to represent the boundary map of the appropriate dimension in the complex. */
/* TODO: this only will work for Default_Cell */
template < class Cell_Complex >
void Sparse_Matrix_Boundary_Map ( Sparse_Matrix < typename Cell_Complex::Ring > & output_matrix, 
                                  const Cell_Complex & complex, const unsigned int dimension ) {
	/* Clear the output, just in case. */
	output_matrix . clear ();
    /* Check dimension to see whether or not it is within bounds. */
	if ( dimension > complex . dimension () || dimension < 0 ) return;
	/* Now we loop through all elementary chains of dimension "map_dimension". */
	for ( typename Cell_Complex::const_iterator group_iterator = complex . begin ( dimension ); 
	group_iterator != complex . end ( dimension ); ++ group_iterator ) {
    unsigned long group_index = complex . index ( group_iterator );
		/* We find the boundary of the current elementary chain. */
		typename Cell_Complex::Chain boundary_chain = complex . boundary ( group_iterator );
		/* We loop through the terms in the boundary we have found. */
		for ( typename Cell_Complex::Chain::iterator chain_iterator = boundary_chain . begin (); 
		chain_iterator != boundary_chain . end (); ++ chain_iterator ) 
			output_matrix . set_entry ( complex . index ( chain_iterator -> first )  , group_index, chain_iterator -> second ); 
  } /* for */
} /* Sparse_Matrix_Boundary_Map */
