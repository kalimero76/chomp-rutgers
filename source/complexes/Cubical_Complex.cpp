/*
 *  Cubical_Complex.cpp
 *  srh_chomp
 *
 *  Created by Shaun Harker on 9/14/09.
 *
 */

#include <iostream>
#include <fstream>

#include "complexes/Cubical_Complex.h"
	
/* 
 * Cubical_Container::const_iterator implementations 
 */


Cubical_Container::const_iterator::const_iterator( void ) : referral(NULL) { }

Cubical_Container::const_iterator::const_iterator( const Cubical_Container * const referral) : referral(referral) {
	full_cube_number = 0;
	piece_number = 0;
	if ( referral -> cube_dimension > 0 ) next_piece_number (); }
	
Cubical_Container::const_iterator::const_iterator( const Cubical_Container * const referral, const unsigned long full_cube_number, const unsigned long piece_number) : 
referral(referral), full_cube_number(full_cube_number), piece_number(piece_number) {}
	
const Cubical_Container::value_type & Cubical_Container::const_iterator::operator * ( void ) const {
	dereference_value . first . name = piece_number + (full_cube_number << referral -> space_dimension);
	dereference_value . first . dimension = referral -> cube_dimension; 
	dereference_value . second = true; 
	return dereference_value; } /* endfunction */

const Cubical_Container::value_type * Cubical_Container::const_iterator::operator -> ( void ) const { 
	return &**this; } /* address of overloaded * of builtin * of this */

void Cubical_Container::const_iterator::next_piece_number ( void ) {
	const unsigned long MAX_piece_number = 1 << referral -> space_dimension ; 
	while ( 1 ) {
		piece_number ++ ;
		if ( piece_number >= MAX_piece_number ) {
			piece_number = MAX_piece_number;
			return; }
		unsigned long temp = piece_number;
		unsigned int sum = 0;
		for ( unsigned int bit = 0; bit < referral -> space_dimension; ++ bit ) {
			sum += temp % 2;
			temp = temp >> 1; }
		if ( sum == referral -> cube_dimension ) {
			return; } } 
}

Cubical_Container::const_iterator & Cubical_Container::const_iterator::operator ++ ( void ) {
	const unsigned long number_of_cubes = referral -> number_of_cubes; 
	const unsigned long hop_length = 1 << (referral -> space_dimension);
	full_cube_number ++;
	while ( 1 ) {
		unsigned long address = piece_number + hop_length * full_cube_number;
		/* Advance full_cube_number until we run out of cubes. */
		while ( full_cube_number < number_of_cubes ) {
			if ( (*(referral -> bitmap)) [ address ] ) return *this ; 
			full_cube_number ++ ;
			address += hop_length; } /* while */
		next_piece_number (); 
		if ( piece_number == hop_length) return *this;  
		full_cube_number = 0; } /* while */ } /* endfunction */

bool Cubical_Container::const_iterator::operator != ( const const_iterator & right_hand_side ) const {
	if ( piece_number != right_hand_side . piece_number ) return true;
	if ( full_cube_number != right_hand_side . full_cube_number ) return true;
	return false; }
	
bool Cubical_Container::const_iterator::operator == ( const const_iterator & right_hand_side ) const {
	if ( piece_number == right_hand_side . piece_number ) return true;
	if ( full_cube_number == right_hand_side . full_cube_number ) return true;
	return false; }
	
/*
 * Cubical_Container implementations 
 */

Cubical_Container::size_type Cubical_Container::size ( void ) const {
	return remembered_size; } /* size */
	
Cubical_Container::const_iterator Cubical_Container::begin ( void ) const {
	const unsigned long address_size = number_of_cubes << space_dimension;
	if ( first_address >= address_size ) return end ();
	const_iterator return_value ( this );
	return_value . full_cube_number = first_address >> space_dimension;
	return_value . piece_number = first_address ^ ( return_value . full_cube_number << space_dimension );
	if ( (*bitmap)[ first_address ] == false ) ++ return_value;
	first_address = ( return_value . full_cube_number << space_dimension ) + return_value . piece_number;
	return return_value; }

Cubical_Container::const_iterator Cubical_Container::end ( void ) const {	
	const_iterator return_value ( this );
	return_value . full_cube_number = number_of_cubes;
	return_value . piece_number = 1 << space_dimension;
	return return_value; }

Cubical_Container::const_iterator Cubical_Container::find ( const Cubical_Container::key_type & key ) const {
	/* key_type is Elementary_Chain, and we store the bitmap address in the name of the elementary chain, so find is easy. */
	unsigned long mask = ( 1 << space_dimension ) - 1;
	return const_iterator ( this, key . name >> space_dimension, key . name & mask); }

/*
 * Cubical_Complex Implementations
 */

/* Constructor */
Cubical_Complex::Cubical_Complex ( void ) {
	data_allocated = false; } /* endfunction */

/* Copy Constructor */
Cubical_Complex::Cubical_Complex ( const Cubical_Complex & copy_me) {
	/* Shallow copy. */
	is_a_full_complex = copy_me . is_a_full_complex;
	dimension = copy_me . dimension;
	Chain_Groups = copy_me . Chain_Groups;
	data_allocated = copy_me . data_allocated;
	
	/* If deep copy isn't necessary, quit now. */
	if ( copy_me . data_allocated == false ) return;
	
	/* New storage for deep copy. */
	std::vector<unsigned int> * dimension_sizes = new std::vector<unsigned int>; 
	std::vector<unsigned long> * jump_values = new std::vector<unsigned long>;
	std::vector<bool> * bitmap = new std::vector<bool>;
	
	/* Deep Copy the important data. */
	*dimension_sizes = * ( Chain_Groups [ 0 ] . dimension_sizes );
	*jump_values = * ( Chain_Groups [ 0 ] . jump_values );
	*bitmap = * ( Chain_Groups [ 0 ] . bitmap );
	
	/* Make pointers to copies. */
	for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) {
		Chain_Groups [ dimension_index ] . dimension_sizes = dimension_sizes;
		Chain_Groups [ dimension_index ] . jump_values = jump_values;
		Chain_Groups [ dimension_index ] . bitmap = bitmap; } /* for */ } /* endfunction */

/* Deconstructor */
Cubical_Complex::~Cubical_Complex ( void ) { 
	delete Chain_Groups [ 0 ] . dimension_sizes;
	delete Chain_Groups [ 0 ] . jump_values;
	delete Chain_Groups [ 0 ] . bitmap; } /* endfunction */

/* Allocate_Bitmap */
void Cubical_Complex::Allocate_Bitmap ( const std::vector<unsigned int> & user_dimension_sizes ) {
	this -> dimension = user_dimension_sizes . size ();
	std::vector<unsigned int> * dimension_sizes = new std::vector<unsigned int> (this -> dimension, 0);
	std::vector<unsigned long> * jump_values = new std::vector<unsigned long> (this -> dimension, 0);
	unsigned long number_of_full_cubes = 1;
	for ( unsigned long index = 0; index < this -> dimension; ++ index ) { 
		( *dimension_sizes ) [ index ] = user_dimension_sizes [ index ];
		( *jump_values ) [ index ] = number_of_full_cubes;
		number_of_full_cubes *= user_dimension_sizes [ index ]; } /* for */
		
	std::vector<bool> * bitmap = new std::vector<bool> ( number_of_full_cubes << this -> dimension, false );
	Chain_Groups . resize ( this -> dimension + 1);
	for ( unsigned long index = 0; index <= this -> dimension; ++ index ) {
		Chain_Groups [ index ] . space_dimension = this -> dimension;
		Chain_Groups [ index ] . cube_dimension = index;
		Chain_Groups [ index ] . number_of_cubes = number_of_full_cubes;
		Chain_Groups [ index ] . dimension_sizes = dimension_sizes;
		Chain_Groups [ index ] . jump_values =  jump_values;
		Chain_Groups [ index ] . bitmap = bitmap; 
		Chain_Groups [ index ] . remembered_size = 0;
		Chain_Groups [ index ] . first_address = number_of_full_cubes << ( this -> dimension ); } /* for */
	data_allocated = true; } /* endfunction */
	
/* Insert_Elementary_Cell */

void Cubical_Complex::Insert_Elementary_Cell ( const unsigned long address ) {
	std::_Bit_reference place_reference = Chain_Groups [0] . bitmap -> operator [] ( address );
	if ( place_reference != true) { 
		place_reference = true; 
		unsigned int cell_dimension = 0;
		for ( unsigned int dimension_index = 0;  dimension_index < this -> dimension ; ++ dimension_index ) 
			if ( address & ( 1 << dimension_index ) ) ++ cell_dimension;
		++ Chain_Groups[ cell_dimension ] . remembered_size;
		if ( Chain_Groups [ cell_dimension ] . first_address > address ) Chain_Groups [ cell_dimension ] . first_address = address; 
    } /* if */ 
} /* Cubical_Complex::Insert_Elementary_Cell */

/* Add_Full_Cube */
/* pretty inefficient */
void Cubical_Complex::Add_Full_Cube ( const std::vector<unsigned int> & cube_coordinates ) {
	std::vector<unsigned int> neighbor_coordinates( this -> dimension, 0);

	const std::vector<unsigned int> * dimension_sizes = Chain_Groups [0] . dimension_sizes;
	const std::vector<unsigned long> * jump_values = Chain_Groups [0] . jump_values;

	/* Calculate the number of the read cube */
	long full_cube_number = 0;
	for ( unsigned int dimension_index = 0; dimension_index < this -> dimension; ++ dimension_index ) 
		full_cube_number += ( *jump_values ) [dimension_index] * ( (long) cube_coordinates [dimension_index] );
		
		
	/* Insert the pieces from all neighboring cubes */
	for ( int neighbor_index = 0; neighbor_index < ( 1 << this -> dimension ); ++ neighbor_index ) {
		neighbor_coordinates = cube_coordinates;
		int temp = neighbor_index;
		long offset = 0;
		bool flag = false;
		for ( unsigned int dimension_index = 0;  dimension_index < this -> dimension ; ++ dimension_index ) {
			if ( temp % 2 == 0 ) {
				neighbor_coordinates [ dimension_index ] ++;
				offset += ( *jump_values ) [ dimension_index ]; }
			temp = temp >> 1; 
			if ( neighbor_coordinates [ dimension_index ] == ( *dimension_sizes ) [ dimension_index ] ) flag = true; }
		if ( flag ) continue;
		/* this is inefficient */
		for( int piece_index = 0; piece_index < 1 << this -> dimension; piece_index ++ ) 
			if ( ( piece_index & ~neighbor_index ) == 0)  /* clever bitwise test */ 
				Insert_Elementary_Cell ( ( ( full_cube_number + offset ) << this -> dimension ) + piece_index ); } /* for */
	return; } /* endfunction */

/* Load_From_File --- cubical format with full cubes.
	(n1, n2, ... ), all non-negative integers. */
	
void Cubical_Complex::Load_From_File ( const char * FileName ) {
	/* Notice the [this ->] to access from the base class. This seems like a bug to me, but the gcc people claim it's righteous */
	/* http://gcc.gnu.org/onlinedocs/gcc/Name-lookup.html#Name-lookup */
	char text_buffer[512];
	char *ptr;
	std::ifstream input_file ( FileName ); 
	
	if ( not input_file . good () ) {
		std::cout << "Cubical_Complex::Load_From_File. Fatal Error. " 
			<< FileName << " not found.\n";
		exit ( 1 ); } /* if */
		
	if ( data_allocated == true ) {
		delete Chain_Groups [ 0 ] . dimension_sizes;
		delete Chain_Groups [ 0 ] . jump_values;
		delete Chain_Groups [ 0 ] . bitmap; } /* if */
		
	data_allocated = true;
	
	int index = 0; 
	/* Find first line of text with a ( */
	while ( not input_file . eof () ) {
		input_file . getline ( text_buffer, 512, '\n' );
		index = 0;
		while ( text_buffer[index] != '(' && text_buffer[index] != 0 ) index++; 
		if ( text_buffer [ index ] == 0 ) continue; 
		else break; }
		
	
	/* Obtain dimension from first line of text */
	/*	(Count number of commas and add one -- that is dimension.) */
	this -> dimension = 1;
	index = 0;
	while ( text_buffer [ index ] != 0 ) {
		if( text_buffer [ index ] == ',' ) this -> dimension = this -> dimension + 1;
		index ++ ; }
		
	std::vector<unsigned int> user_dimension_sizes (this -> dimension, 0);
	
	/* Obtain sizes from the first line of text */
	index = 0; while ( text_buffer[index] != '(') index++; index ++; 
	for ( unsigned int dimension_index = 0; dimension_index < this -> dimension; dimension_index ++ ) {
		ptr = text_buffer + index;
		while ( text_buffer[index] != ',' && text_buffer[index] != ')') index++;
		text_buffer[index] = 0; index++;
		user_dimension_sizes[dimension_index] = atoi(ptr) + 3; }
	
	/* Allocate */
	Allocate_Bitmap ( user_dimension_sizes );
	
	/* Now scan through every line of text and read in full cubes */
	std::vector<unsigned int> cube_coordinates( this -> dimension, 0);
	std::vector<unsigned int> neighbor_coordinates( this -> dimension, 0);
	
	while ( not input_file . eof () ) {
		input_file . getline ( text_buffer, 512, '\n' );
		index = 0; 
		while ( text_buffer[index] != '(' && text_buffer[index] != 0 ) index++; 
		if ( text_buffer [ index ] == 0 ) continue;
		index ++; 
		
		/* Read the coordinates of the cube from the line */
		for ( unsigned int dimension_index = 0; dimension_index < this -> dimension; dimension_index ++ ) {
			ptr = text_buffer + index;
			while ( text_buffer[index] != ',' && text_buffer[index] != ')') index++;
			text_buffer[index] = 0; index++;
			 cube_coordinates[dimension_index] = atoi(ptr) + 1 ; }
			 
		/* Now Add the Cube to the complex. */
			 
		Add_Full_Cube ( cube_coordinates );
			
	} /* while */
	
	/* We are done reading. Close the file. */
	input_file . close ();
	
	/* Since we read full cubes from a .cub file, this is a full complex */
	is_a_full_complex = true;
}


/* WARNING. VERIFY THE SIGNS */
Cubical_Complex::Chain & Cubical_Complex::Boundary_Map ( Cubical_Complex::Chain & output, const Cubical_Complex::Container::const_iterator & input) const {
	/* Because the name of an elementary chain in a cubical complex is its address, we already know where it is. */
	/* The task is to determine each of its neighbors, their 'sign', and construct the boundary chain.			 */

	if ( input -> first . dimension == 0 ) return output; /* Boundary of a 0-dimensional object is trivial */
	const unsigned int boundary_dimension = input -> first . dimension - 1;
	
	/* Consider the bit-representation of piece_number. For three-dimensional space, it is three bits
	 *    101, perhaps. This corresponds to a square piece. Its boundaries come in 2 varieties:
	 *	  with piece_number 001 and with piece_number 100. In general the set of piece numbers can be arrived at
	 *    by considering the 'single bit demotions'. 1101 has single bit demotions of 1100, 1001, and 0101, for example.
	 * Corresponding to each single bit demotion are two boundary pieces. One has the same full_cube_number as the original
	 *    and gets a sign of -1. The other has a full_cube_number that is offset by the "jump_values" corresponding to the bit position.
	 * Of course we have to check these 'alleged' boundary pieces to see if they actually exist first!
	 * Now we are in a position to give the algorithm: 
	 */
	 
	long work_bit = 1;
	Ring positive = ( Ring ) 1; /* Ring must be able to cast 1 to get its multiplicative identity */
	Ring negative = - positive; /* Ring must overload unary "-" operator for additive inverse */
	bool sign = false;
	long address = input -> first . name;
	for ( unsigned int dimension_index = 0; dimension_index < dimension; work_bit <<= 1, dimension_index++ ) {
		/* Can we demote this bit? If not, "continue". */
		if (  not ( address & work_bit) ) continue;
		sign = not sign;
		/* Alter address to refer to a boundary in the current full cube */
		address = address ^ work_bit;
		/* Insert the piece in the current full cube */
		if ( Chain_Groups [ boundary_dimension ] . bitmap -> operator [] ( address ) )
			output . insert ( std::pair < Elementary_Chain, Ring > ( Elementary_Chain ( address,  boundary_dimension ), sign ? positive : negative ) );
		/* Insert the piece in the appropriate neighboring full cube */
		long offset_address = address + ( Chain_Groups [ boundary_dimension ] . jump_values -> operator [] (dimension_index) << dimension );
		if ( Chain_Groups [ boundary_dimension ] . bitmap -> operator [] ( offset_address ) )
			output . insert ( std::pair < Elementary_Chain, Ring > ( Elementary_Chain ( offset_address,  boundary_dimension ), sign ? negative : positive ) ); 
		/* Recover original address */
		address = address ^ work_bit; } /* for */
	return output;  } /* endfunction */


/* Warning: check the signs */
Cubical_Complex::Chain & Cubical_Complex::Coboundary_Map ( Cubical_Complex::Chain & output, const Cubical_Complex::Container::const_iterator & input ) const {
	/* Because the name of an elementary chain in a cubical complex is its address, we already know where it is. */
	/* The task is to determine each of its neighbors, their 'sign', and construct the coboundary chain.			 */
	
	
	if ( input -> first . dimension == this -> dimension ) return output;  /* Coboundary of a full-dimensional object is trivial */
	const unsigned int coboundary_dimension = input -> first . dimension + 1;
	
	/* Consider the bit-representation of piece_number. For three-dimensional space, it is three bits
	 *    001, perhaps. This corresponds to an edge. Its boundaries come in 4 varieties:
	 *	  with piece_number 101 and with piece_number 011. In general the set of piece numbers can be arrived at
	 *    by considering the 'single bit promotions'. 0101 has single bit promotions of 0111 and 1101, for example.
	 * Corresponding to each single bit promotion are two coboundary pieces. One has the same full_cube_number as the original
	 *    and gets a sign of -1. The other has a full_cube_number that is offset by NEGATIVE of the "jump_values" corresponding to the bit position.
	 * Of course we have to check these 'alleged' coboundary pieces to see if they actually exist first!
	 * Now we are in a position to give the algorithm: 
	 */
	 
	long work_bit = 1;
	Ring positive = ( Ring ) 1; /* Ring must be able to cast 1 to get its multiplicative identity */
	Ring negative = - positive; /* Ring must overload unary "-" operator for additive inverse */
	bool sign = false;
	long address = input -> first . name;
	for ( unsigned int dimension_index = 0; dimension_index < dimension; work_bit <<= 1, dimension_index++ ) {
		/* Can we promote this bit? If not, "continue". */
		if ( address & work_bit ) continue;
		sign = not sign; /* or should this be with the continue ? */
		address = address ^ work_bit;
		if ( Chain_Groups [ coboundary_dimension ] . bitmap -> operator [] ( address ) )
			output . insert ( std::pair < Elementary_Chain, Ring > ( Elementary_Chain ( address,  coboundary_dimension ), sign ? positive : negative  ) );
		long offset_address = address - ( Chain_Groups [ coboundary_dimension ] . jump_values -> operator [] (dimension_index) << dimension );
		if ( Chain_Groups [ coboundary_dimension ] . bitmap -> operator [] ( offset_address ) )
			output . insert ( std::pair < Elementary_Chain, Ring > ( Elementary_Chain ( offset_address,  coboundary_dimension ), sign ? negative : positive  ) ); 
		address = address ^ work_bit; } /* for */
	return output;  } /* endfunction */


void Cubical_Complex::Remove_Elementary_Chain ( const Cubical_Complex::Elementary_Chain & input) {
	std::_Bit_reference bit_reference = Chain_Groups [ input . dimension ] . bitmap -> operator [] ( input . name );
	if ( bit_reference == true ) {
		bit_reference = false;
		-- Chain_Groups [ input . dimension ] . remembered_size; } /* if */
	return; }



