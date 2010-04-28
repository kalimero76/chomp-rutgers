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

/* * * * * * * * * * * * * * * * *
 * Cubical_Container definitions *
 * * * * * * * * * * * * * * * * */

unsigned int Cubical_Container::dimension ( void ) const {
  return dimension_;
} /* Cubical_Container::dimension */

Cubical_Container::size_type Cubical_Container::size ( unsigned int dimension ) const {
	return size_ [ dimension ]; 
} /* Cubical_Container::size */
	
Cubical_Container::iterator Cubical_Container::begin ( unsigned int dimension  ) const {
	return begin_ [ dimension ];
} /* Cubical_Container::begin */

Cubical_Container::iterator Cubical_Container::end ( unsigned int dimension ) const {	
  return end_ [ dimension ];
} /* Cubical_Container::end */

Cubical_Container::iterator Cubical_Container::find ( const Cubical_Container::key_type & key ) const {
  if ( bitmap_ [ key . data () ] == false ) return end_ [ dimension_ ];
	return const_iterator ( this, key . data (), key . dimension () );
} /* Cubical_Container::find */

void Cubical_Container::erase ( const iterator & erase_me ) {
	std::_Bit_reference bit_reference = bitmap_ [ erase_me . address_ ];
	if ( bit_reference == true ) {
		bit_reference = false;
    /* Update size_ */
    -- size_ [ erase_me . dimension_ ]; 
    /* Update begin_, end_ */
    if ( erase_me == begin_ [ erase_me . dimension_ ] ) {
      ++ begin_ [ erase_me . dimension_ ];
      if ( erase_me . dimension_  > 0 ) end_ [ erase_me . dimension_ - 1 ] = begin_ [ erase_me . dimension_ ];
    }
  } /* if */
  return; 
} /* Cubical_Container::erase */

std::pair<Cubical_Container::iterator, bool> Cubical_Container::insert ( const value_type & insert_me ) {
	std::_Bit_reference bit_reference = bitmap_ [ insert_me . data () ];
	if ( bit_reference == false ) { 
		bit_reference = true; 
    /* Update size_ */
		++ size_ [ insert_me . dimension () ]; 
    /* Update begin_, end_ */
    const_iterator iter = find ( insert_me );
    ++ iter;
    if ( iter == begin_ [ insert_me . dimension () ] ) {
      begin_ [ insert_me . dimension () ] = find ( insert_me );
      if ( insert_me . dimension ()  > 0 ) end_ [ insert_me . dimension () - 1 ] = begin_ [ insert_me . dimension () ];
    } /* if */
    return std::pair < iterator, bool > ( find ( insert_me ), true );
  } else {
    return std::pair < iterator, bool > ( find ( insert_me ), false ); 
  } /* if-else */
} /* Cubical_Container::insert */

Cubical_Container::Chain Cubical_Container::boundary ( const const_iterator & input ) const {
  Chain output;
	/* Because the name of an elementary chain in a cubical complex is its address, we already know where it is. */
	/* The task is to determine each of its neighbors, their 'sign', and construct the boundary chain.			 */
  
	if ( input . dimension_ == 0 ) return output; /* Boundary of a 0-dimensional object is trivial */
	const unsigned int boundary_dimension = input . dimension_ - 1;
	
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
	long address = input . address_;
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; work_bit <<= 1, ++ dimension_index ) {
		/* Can we demote this bit? If not, "continue". */
		if ( not ( address & work_bit) ) continue;
		sign = not sign;
		/* Alter address to refer to a boundary in the current full cube */
		address = address ^ work_bit;
		/* Insert the piece in the current full cube */
		if ( bitmap_ [ address ] )
			output . insert ( std::pair < const_iterator, Ring > ( const_iterator ( this, address,  boundary_dimension ), sign ? positive : negative ) );
		/* Insert the piece in the appropriate neighboring full cube */
		long offset_address = address + ( jump_values_ [ dimension_index ] << dimension_ );
		if ( bitmap_ [ offset_address ] )
			output . insert ( std::pair < const_iterator, Ring > ( const_iterator ( this, offset_address,  boundary_dimension ), sign ? negative : positive ) ); 
		/* Recover original address */
    address = address ^ work_bit; 
  } /* for */
  return output;  
} /* Cubical_Container::boundary */

Cubical_Container::Chain Cubical_Container::coboundary ( const iterator & input ) const {
  Chain output;
	/* Because the name of an elementary chain in a cubical complex is its address, we already know where it is. */
	/* The task is to determine each of its neighbors, their 'sign', and construct the coboundary chain.			 */
	
	if ( input . dimension_ == dimension_ ) return output;  /* Coboundary of a full-dimensional object is trivial */
	const unsigned int coboundary_dimension = input . dimension_ + 1;
	
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
	long address = input . address_;
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; work_bit <<= 1, ++ dimension_index ) {
		/* Can we promote this bit? If not, "continue". */
		if ( address & work_bit ) continue;
		sign = not sign; /* or should this be with the continue ? */
		address = address ^ work_bit;
		if ( bitmap_ [ address ] )
			output . insert ( std::pair < const_iterator, Ring > ( const_iterator ( this, address,  coboundary_dimension ), sign ? positive : negative  ) );
		long offset_address = address - ( jump_values_ [ dimension_index ] << dimension_ );
		if ( bitmap_ [ offset_address ] )
			output . insert ( std::pair < const_iterator, Ring > ( const_iterator ( this, offset_address,  coboundary_dimension ), sign ? negative : positive  ) ); 
    address = address ^ work_bit; 
  } /* for */
  return output;  
} /* Cubical_Container::coboundary */

/* * * * * * * * * * * * * * * * * * * *
 * Cubical_const_iterator definitions  *
 * * * * * * * * * * * * * * * * * * * */

Cubical_const_iterator::Cubical_const_iterator ( void ) : referral_(NULL), dimension_(0) { 
} /* Cubical_const_iterator::Cubical_const_iterator */

Cubical_const_iterator::Cubical_const_iterator ( const Cubical_Container * const referral, const unsigned long address, const unsigned int dimension ) : 
referral_(referral), address_(address), dimension_(dimension) {
} /* Cubical_const_iterator::Cubical_const_iterator */

Cubical_Container::value_type Cubical_const_iterator::operator * ( void ) const {
	return Cubical_Container::Cell ( address_, dimension_ ); 
} /* Cubical_const_iterator::operator * */

void Cubical_const_iterator::next_type ( void ) {
	const unsigned long maximum = 1 << referral_ -> dimension_ ; 
  unsigned long piece_number = address_ & ( maximum - 1 );
	while ( 1 ) {
		++ piece_number;
		if ( piece_number == maximum ) {
			if ( dimension_ == referral_ -> dimension_ ) { 
        /* There are no more types. Cycle to beginning. */
        dimension_ = 0; 
        piece_number = 0;
        break;
      } else {
        ++ dimension_;
        piece_number = 0;
      }
    } /* if */
		unsigned long temp = piece_number;
		unsigned int sum = 0;
		for ( unsigned int bit = 0; bit < referral_ -> dimension_; ++ bit ) {
			sum += temp % 2;
			temp = temp >> 1; 
    } /* for */
		if ( sum == dimension_ ) break; 
  } /* while */
  address_ = piece_number;
} /* Cubical_const_iterator::next_piece_number */

Cubical_const_iterator & Cubical_const_iterator::operator ++ ( void ) {
	const unsigned long hop_length = 1 << (referral_ -> dimension_);
  address_ += hop_length; 
	while ( 1 ) {
		/* Advance full_cube_number until we run out of cubes. */
		while ( address_ < referral_ -> bitmap_size_ ) {
			if ( referral_ -> bitmap_ [ address_ ] ) return *this ; 
			address_ += hop_length; 
    } /* while */
    /* Zero out the cube_number and get next piece */
		next_type (); 
		if ( address_ == 0 ) {
      /* Then we have reached end () */
      address_ = referral_ -> bitmap_size_;
      return *this;  
    } /* if */
  } /* while */ 
} /* Cubical_const_iterator::operator ++ */

bool Cubical_const_iterator::operator != ( const Cubical_const_iterator & right_hand_side ) const {
	if ( address_ != right_hand_side . address_ ) return true;
	return false; 
} /* Cubical_const_iterator::operator != */

bool Cubical_const_iterator::operator == ( const Cubical_const_iterator & right_hand_side ) const {
	if ( address_ != right_hand_side . address_ ) return false;
	return true; 
} /* Cubical_const_iterator::operator == */

bool Cubical_const_iterator::operator < ( const Cubical_const_iterator & right_hand_side ) const {
  return address_ < right_hand_side . address_;
} /* Cubical_const_iterator::operator < */

/* * * * * * * * * * * * * * * *
 * Cubical_Complex definitions *
 * * * * * * * * * * * * * * * */

void Cubical_Complex::Allocate_Bitmap ( const std::vector<unsigned int> & user_dimension_sizes ) {
	dimension_ = user_dimension_sizes . size ();
	dimension_sizes_ . resize ( dimension_, 0 );
	jump_values_ . resize ( dimension_, 0 );
  size_ . resize ( dimension_ + 1, 0 );
	unsigned long number_of_full_cubes = 1;
	for ( unsigned long index = 0; index < dimension_; ++ index ) { 
		dimension_sizes_ [ index ] = ( 1 + user_dimension_sizes [ index ] );
		jump_values_ [ index ] = number_of_full_cubes;
		number_of_full_cubes *= ( 1 + user_dimension_sizes [ index ] ); 
  } /* for */
	bitmap_ . clear ();
  bitmap_ . resize ( number_of_full_cubes << dimension_, false );
  bitmap_size_ = bitmap_ . size ();
  const_iterator end_iter ( this, bitmap_size_, 0 );
  begin_ . resize ( dimension_ + 1, end_iter );
  end_ . resize ( dimension_ + 1, end_iter );
} /* Cubical_Complex::Allocate_Bitmap */

void Cubical_Complex::Add_Full_Cube ( const std::vector<unsigned int> & cube_coordinates ) {
	std::vector<unsigned int> neighbor_coordinates( dimension_, 0);
	/* Calculate the number of the read cube */
	long full_cube_number = 0;
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) 
		full_cube_number += jump_values_ [dimension_index] * ( (long) cube_coordinates [dimension_index] );
	/* Insert the pieces from all neighboring cubes */
	for ( int neighbor_index = 0; neighbor_index < ( 1 << dimension_ ); ++ neighbor_index ) {
		neighbor_coordinates = cube_coordinates;
		int temp = neighbor_index;
		long offset = 0;
		bool flag = false;
		for ( unsigned int dimension_index = 0;  dimension_index < dimension_ ; ++ dimension_index ) {
			if ( temp % 2 == 0 ) {
				neighbor_coordinates [ dimension_index ] ++;
				offset += jump_values_ [ dimension_index ]; 
      } /* if */
			temp = temp >> 1; 
			if ( neighbor_coordinates [ dimension_index ] == dimension_sizes_ [ dimension_index ] ) flag = true; 
    } /* for */
		if ( flag ) continue;
		/* this is inefficient */
		for( int piece_index = 0; piece_index < 1 << dimension_; piece_index ++ ) 
			if ( ( piece_index & ~neighbor_index ) == 0)  /* clever bitwise test */  {
        /* Figure out the dimension of the cell */
        unsigned int cell_dimension = 0;
        for ( unsigned int bit_index = 0; bit_index < dimension_; ++ bit_index ) {
          if ( piece_index & ( 1 << bit_index ) ) ++ cell_dimension; 
        } /* for */
        /* insert the cell */
				insert ( Cell ( ( ( full_cube_number + offset ) << dimension_ ) + piece_index , cell_dimension ) ); 
      } /* if */
  } /* for */
	return; 
} /* Cubical_Complex::Add_Full_Cube */
	
void Cubical_Complex::Load_From_File ( const char * FileName ) {
	char text_buffer[512];
	char *ptr;
	std::ifstream input_file ( FileName ); 
	if ( not input_file . good () ) {
		std::cout << "Cubical_Complex::Load_From_File. Fatal Error. " 
			<< FileName << " not found.\n";
		exit ( 1 ); } /* if */
	int index = 0; 
	/* Find first line of text with a "(" */
	while ( not input_file . eof () ) {
		input_file . getline ( text_buffer, 512, '\n' );
		index = 0;
		while ( text_buffer[index] != '(' && text_buffer[index] != 0 ) index++; 
		if ( text_buffer [ index ] == 0 ) continue; 
		else break; 
  } /* while */
	/* Obtain dimension from first line of text */
	/*	(Count number of commas and add one -- that is dimension.) */
	dimension_ = 1;
	index = 0;
	while ( text_buffer [ index ] != 0 ) {
		if( text_buffer [ index ] == ',' ) dimension_ = dimension_ + 1;
		index ++ ; 
  } /* while */
	std::vector<unsigned int> user_dimension_sizes (dimension_, 0);
	/* Obtain sizes from the first line of text */
	index = 0; while ( text_buffer[index] != '(') index++; index ++; 
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; dimension_index ++ ) {
		ptr = text_buffer + index;
		while ( text_buffer[index] != ',' && text_buffer[index] != ')') index++;
		text_buffer[index] = 0; index++;
		user_dimension_sizes[dimension_index] = atoi(ptr) + 3; 
  } /* for */
	/* Allocate */
	Allocate_Bitmap ( user_dimension_sizes );
	/* Now scan through every line of text and read in full cubes */
	std::vector<unsigned int> cube_coordinates( dimension_, 0);
	std::vector<unsigned int> neighbor_coordinates( dimension_, 0);
	while ( not input_file . eof () ) {
		input_file . getline ( text_buffer, 512, '\n' );
		index = 0; 
		while ( text_buffer[index] != '(' && text_buffer[index] != 0 ) index++; 
		if ( text_buffer [ index ] == 0 ) continue;
		++ index; 
		/* Read the coordinates of the cube from the line */
		for ( unsigned int dimension_index = 0; dimension_index < dimension_; dimension_index ++ ) {
			ptr = text_buffer + index;
			while ( text_buffer[index] != ',' && text_buffer[index] != ')') index++;
			text_buffer[index] = 0; 
      index++;
      cube_coordinates[dimension_index] = atoi(ptr) + 1 ; 
    } /* for */
		/* Now Add the Cube to the complex. */
		Add_Full_Cube ( cube_coordinates ); 
	} /* while */
	/* We are done reading. Close the file. */
	input_file . close ();
} /* Cubical_Complex::Load_From_File */

#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */
#endif
