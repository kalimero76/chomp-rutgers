/*
 *  Cubical_Complex.cpp
 *  srh_chomp
 *
 *  Created by Shaun Harker on 9/14/09.
 *
 */

#include <iostream>
#include <fstream>
#include <algorithm>

#include "complexes/Cubical_Complex.h"
#include "algorithms/Morse_Theory.h"

/* * * * * * * * * * * * * * * * * * * *
 * Cubical_const_iterator definitions  *
 * * * * * * * * * * * * * * * * * * * */

Cubical_const_iterator::Cubical_const_iterator ( void ) : container_(NULL), dimension_(0) { 
} /* Cubical_const_iterator::Cubical_const_iterator */

Cubical_const_iterator::Cubical_const_iterator ( const Cubical_Complex * const container, const unsigned long address, const unsigned int dimension ) : 
container_(container), address_(address), dimension_(dimension) {
} /* Cubical_const_iterator::Cubical_const_iterator */

Cubical_Complex::value_type Cubical_const_iterator::operator * ( void ) const {
	return Cubical_Complex::Cell ( address_, dimension_ ); 
} /* Cubical_const_iterator::operator * */

Cubical_const_iterator & Cubical_const_iterator::operator ++ ( void ) {
	const unsigned long hop_length = 1 << (container_ -> dimension_);
  address_ += hop_length; 
	while ( 1 ) {
		/* Advance full_cube_number until we run out of cubes. */
		while ( address_ < container_ -> bitmap_size_ ) {
			if ( container_ -> bitmap_ [ address_ ] ) return *this ; 
			address_ += hop_length; 
    } /* while */
    /* Zero out the cube_number and get next piece */
		next_type (); 
		if ( *this == container_ -> end_ ) return *this;  
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
  if ( dimension_ == right_hand_side . dimension_ ) return address_ < right_hand_side . address_;
  return dimension_ < right_hand_side . dimension_;
} /* Cubical_const_iterator::operator < */

unsigned int Cubical_const_iterator::dimension () const {
  return dimension_;
} /* Cubical_const_iterator::dimension */

const Cubical_Complex & Cubical_const_iterator::container () const {
  return * container_;
} /* Cubical_const_iterator::container */

void Cubical_const_iterator::next_type ( void ) {
  unsigned long piece = address_ & container_ -> mask_;
  unsigned long next_index = container_ -> types_inv_ [ piece ];
  if ( next_index == container_ -> mask_ ) {
    *this = container_ -> end_;
  } else {
    address_ = container_ -> types_ [ next_index + 1 ];
    dimension_ = container_ -> type_dims_ [ next_index + 1 ];
  } /* if-else */
} /* Cubical_const_iterator::next_type */

std::ostream & operator << ( std::ostream & output_stream, const Cubical_const_iterator & print_me) {
  output_stream << * print_me;
  return output_stream;
} /* operator << */

/* * * * * * * * * * * * * * * *
 * Cubical_Complex definitions *
 * * * * * * * * * * * * * * * */

std::pair<Cubical_Complex::iterator, bool> Cubical_Complex::insert ( const value_type & insert_me ) {
	std::_Bit_reference bit_reference = bitmap_ [ insert_me . data () ];
	if ( bit_reference == false ) { 
		bit_reference = true; 
    /* Update size_ */
		++ size_ [ insert_me . dimension () ]; 
    ++ total_size_;
    /* Update begin_ */
    const_iterator iter = find ( insert_me );
    ++ iter;
    if ( iter == begin_ [ insert_me . dimension () ] ) {
      begin_ [ insert_me . dimension () ] = find ( insert_me );
    } /* if */
    return std::pair < iterator, bool > ( find ( insert_me ), true );
  } else {
    return std::pair < iterator, bool > ( find ( insert_me ), false ); 
  } /* if-else */
} /* Cubical_Complex::insert */

void Cubical_Complex::erase ( const iterator & erase_me ) {
	std::_Bit_reference bit_reference = bitmap_ [ erase_me . address_ ];
  unsigned int dimension = erase_me . dimension_;
	if ( bit_reference == true ) {
		bit_reference = false;
    /* Update size_ */
    -- size_ [ dimension ]; 
    -- total_size_;
    /* Update begin_ */
    if ( erase_me == begin_ [ dimension ] ) {
      ++ begin_ [ dimension ];
      while ( dimension > 0 ) {
        if ( begin_ [ dimension - 1 ] == erase_me ) begin_ [ dimension - 1 ] = begin_ [ dimension ];
        else break;
        -- dimension;
      } /* while */
    } /* if */
  } /* if */
  return; 
} /* Cubical_Complex::erase */

Cubical_Complex::iterator Cubical_Complex::find ( const Cubical_Complex::key_type & key ) const {
  if ( bitmap_ [ key . data () ] == false ) return end_;
	return const_iterator ( this, key . data (), key . dimension () );
} /* Cubical_Complex::find */

Cubical_Complex::iterator Cubical_Complex::begin ( void ) const {
	return begin_ [ 0 ];
} /* Cubical_Complex::begin */

Cubical_Complex::iterator Cubical_Complex::end ( void ) const {	
  return end_;
} /* Cubical_Complex::end */

Cubical_Complex::size_type Cubical_Complex::size ( void ) const {
	return total_size_; 
} /* Cubical_Complex::size */

Cubical_Complex::iterator Cubical_Complex::begin ( unsigned int dimension  ) const {
	return begin_ [ dimension ];
} /* Cubical_Complex::begin */

Cubical_Complex::iterator Cubical_Complex::end ( unsigned int dimension ) const {	
  return begin_ [ dimension + 1 ];
} /* Cubical_Complex::end */

Cubical_Complex::size_type Cubical_Complex::size ( unsigned int dimension ) const {
	return size_ [ dimension ]; 
} /* Cubical_Complex::size */

Cubical_Complex::Chain Cubical_Complex::boundary ( const const_iterator & lookup ) const {
  Chain output ( *this );
	/* Because the name of an elementary chain in a cubical complex is its address, we already know where it is. */
	/* The task is to determine each of its neighbors, their 'sign', and construct the boundary chain.			 */
  
	if ( lookup . dimension_ == 0 ) return output; /* Boundary of a 0-dimensional object is trivial */
	const unsigned int boundary_dimension = lookup . dimension_ - 1;
	
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
	long address = lookup . address_;
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
} /* Cubical_Complex::boundary */

Cubical_Complex::Chain Cubical_Complex::coboundary ( const iterator & lookup ) const {
  Chain output ( *this );
	/* Because the name of an elementary chain in a cubical complex is its address, we already know where it is. */
	/* The task is to determine each of its neighbors, their 'sign', and construct the coboundary chain.			 */
	
	if ( lookup . dimension_ == dimension_ ) return output;  /* Coboundary of a full-dimensional object is trivial */
	const unsigned int coboundary_dimension = lookup . dimension_ + 1;
	
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
	long address = lookup . address_;
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
} /* Cubical_Complex::coboundary */

unsigned int Cubical_Complex::dimension ( void ) const {
  return dimension_;
} /* Cubical_Complex::dimension */

void Cubical_Complex::index ( void ) {
  std::cout << " size / bitmap_size = " << (float) total_size_ / (float) bitmap_size_ << "\n";
  index_ . resize ( bitmap_size_ + 1 );
  lookup_ . resize ( total_size_ + 1 );
  connection_ . resize ( total_size_, 0 );
  unsigned long indx = 0;
  for ( const_iterator lookup = begin (); lookup != end (); ++ lookup, ++ indx ) { 
    index_ [ lookup . address_ ] = indx;
    lookup_ [ indx ] = lookup;
  } /* for */
  index_ [ end_ . address_ ] = total_size_;
  lookup_ [ total_size_ ] = end_;
  index_begin_ . resize ( dimension_ + 2, 0 );
  unsigned long sum = 0;
  for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) {
    sum += size_ [ dimension_index ];
    index_begin_ [ dimension_index + 1 ] = sum;
  } /* for */
} /* Cubical_Complex::index */

unsigned long Cubical_Complex::index_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ]; 
} /* Cubical_Complex::index_begin */ 

unsigned long Cubical_Complex::index_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ];
} /* Cubical_Complex::index_end */

unsigned long Cubical_Complex::index ( const const_iterator & lookup ) const {
  return index_ [ lookup . address_ ];
} /* Cubical_Complex::index */

unsigned long & Cubical_Complex::index ( const const_iterator & lookup ) {
  return index_ [ lookup . address_ ];
} /* Cubical_Complex::index */

std::vector < Cubical_const_iterator > & Cubical_Complex::lookup ( void ) {
  return lookup_;
} /* Cubical_Complex::lookup */

const Cubical_const_iterator & Cubical_Complex::lookup ( unsigned long index ) const {
  return lookup_ [ index ];
} /* Cubical_Complex::lookup */

Cubical_const_iterator & Cubical_Complex::lookup ( unsigned long index ) {
  return lookup_ [ index ];
} /* Cubical_Complex::lookup */

namespace Cubical_detail {
  
  class set_functor {
  public:
    set_functor ( const int value ) : value ( value ) {}
    void operator () ( int & a ) const { a = value; }
  private:
    int value;
  };
  
} /* namespace Cubical_detail */

std::vector < int > Cubical_Complex::count_all_boundaries ( void ) const {
  std::vector < int > number_of_boundaries ( total_size_ );
  /* TODO: don't assume full complex */

  for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) {
    for_each ( number_of_boundaries . begin () + index ( begin_ [ dimension_index ] ),
               number_of_boundaries . begin () + index ( begin_ [ dimension_index + 1 ] ), 
               Cubical_detail::set_functor ( 2 * dimension_index ) );
  } /* for */
  return number_of_boundaries;
} /* Cubical_Complex::count_all_boundaries */

void Cubical_Complex::boundary ( std::vector < unsigned long > & output, const unsigned long index ) const {
  output . clear ();
	const_iterator cell_iterator = lookup ( index );
	if ( cell_iterator . dimension_ == 0 ) return;  /* Boundary of a zero-dimensional object is trivial */
	long work_bit = 1;
	long address = cell_iterator . address_;
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; work_bit <<= 1, ++ dimension_index ) {
		/* Can we demote this bit? If not, "continue". */
		if ( not ( address & work_bit) ) continue;
		address = address ^ work_bit;
		if ( bitmap_ [ address ] )
			output . push_back ( index_ [ address ] );
		long offset_address = address + ( jump_values_ [ dimension_index ] << dimension_ );
		if ( bitmap_ [ offset_address ] )
			output . push_back ( index_ [ offset_address ] );
    address = address ^ work_bit; 
  } /* for */
} /* Cubical_Complex::boundary_index_list */

void Cubical_Complex::coboundary ( std::vector < unsigned long > & output, const unsigned long index ) const {
  output . clear ();
	const_iterator cell_iterator = lookup ( index );
	if ( cell_iterator . dimension_ == dimension_ ) return;  /* Coboundary of a full-dimensional object is trivial */
	long work_bit = 1;
	long address = cell_iterator . address_;
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; work_bit <<= 1, ++ dimension_index ) {
		/* Can we promote this bit? If not, "continue". */
		if ( address & work_bit ) continue;
		address = address ^ work_bit;
		if ( bitmap_ [ address ] )
			output . push_back ( index_ [ address ] );
		long offset_address = address - ( jump_values_ [ dimension_index ] << dimension_ );
		if ( bitmap_ [ offset_address ] )
			output . push_back ( index_ [ offset_address ] );
    address = address ^ work_bit; 
  } /* for */
} /* Cubical_Complex::coboundary_index_list */

void Cubical_Complex::boundary ( std::vector < std::pair< unsigned long, Default_Ring > > & output, const unsigned long input ) const {
  output . clear ();
  const_iterator cell_iterator = lookup ( input );
	if ( cell_iterator . dimension_ == 0 ) return; /* Boundary of a 0-dimensional object is trivial */
	long work_bit = 1;
	Ring positive = ( Ring ) 1; /* Ring must be able to cast 1 to get its multiplicative identity */
	Ring negative = - positive; /* Ring must overload unary "-" operator for additive inverse */
	bool sign = false;
	long address = cell_iterator . address_;
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; work_bit <<= 1, ++ dimension_index ) {
		/* Can we demote this bit? If not, "continue". */
		if ( not ( address & work_bit) ) continue;
		sign = not sign;
		/* Alter address to refer to a boundary in the current full cube */
		address = address ^ work_bit;
		/* Insert the piece in the current full cube */
		if ( bitmap_ [ address ] )
			output . push_back ( std::pair<unsigned long, Ring> ( index_ [ address ], sign ? positive : negative ) );
		/* Insert the piece in the appropriate neighboring full cube */
		long offset_address = address + ( jump_values_ [ dimension_index ] << dimension_ );
		if ( bitmap_ [ offset_address ] )
			output . push_back ( std::pair<unsigned long, Ring> ( index_ [ offset_address ], sign ? negative : positive ) );
		/* Recover original address */
    address = address ^ work_bit; 
  } /* for */
  return;  
} /* Cubical_Complex::boundary */

void Cubical_Complex::coboundary ( std::vector < std::pair< unsigned long, Ring > > & output, const unsigned long input ) const {
  output . clear ();
  const_iterator cell_iterator = lookup ( input );
	if ( cell_iterator . dimension_ == dimension_ ) return; /* Coboundary of a full dimensional object is trivial */
	long work_bit = 1;
	Ring positive = ( Ring ) 1; /* Ring must be able to cast 1 to get its multiplicative identity */
	Ring negative = - positive; /* Ring must overload unary "-" operator for additive inverse */
	bool sign = false;
	long address = cell_iterator . address_;
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; work_bit <<= 1, ++ dimension_index ) {
		/* Can we promote this bit? If not, "continue". */
		if ( address & work_bit ) continue;
		sign = not sign;
		/* Alter address to refer to a boundary in the current full cube */
		address = address ^ work_bit;
		/* Insert the piece in the current full cube */
		if ( bitmap_ [ address ] )
			output . push_back ( std::pair<unsigned long, Ring> ( index_ [ address ], sign ? positive : negative ) );
		/* Insert the piece in the appropriate neighboring full cube */
		long offset_address = address - ( jump_values_ [ dimension_index ] << dimension_ );
		if ( bitmap_ [ offset_address ] )
			output . push_back ( std::pair<unsigned long, Ring> ( index_ [ offset_address ], sign ? negative : positive ) );
		/* Recover original address */
    address = address ^ work_bit; 
  } /* for */
  return; 
} /* Cubical_Complex::coboundary */

void Cubical_Complex::decompose ( void ) {
  index (); 
  king_count_ = morse::decompose ( *this );
} /*  Cubical_Complex::decompose */

char Cubical_Complex::type ( unsigned long index, unsigned int dimension ) const {
  if ( index < index_begin_ [ dimension ] + king_count_ [ dimension + 1 ] ) return 0; /* QUEEN */
  if ( index < index_begin_ [ dimension + 1 ] - king_count_ [ dimension ] ) return 1; /* ACE */
  return 2; /* KING */
} /* Cubical_Complex::type */

unsigned long Cubical_Complex::mate ( unsigned long queen_index, unsigned int dimension ) const {
  return index_begin_ [ dimension ] + index_begin_ [ dimension + 2 ] - queen_index - 1;
} /* Cubical_Complex::mate */

const Cubical_Complex::Ring & Cubical_Complex::connection ( unsigned long queen_index ) const {
  return connection_ [ queen_index ];
} /* Cubical_Complex::connection */

Cubical_Complex::Ring & Cubical_Complex::connection ( unsigned long queen_index ) {
  return connection_ [ queen_index ];
} /* Cubical_Complex::connection */

unsigned long Cubical_Complex::ace_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ] + king_count_ [ dimension + 1 ];
} /* Cubical_Complex::ace_begin */

unsigned long Cubical_Complex::ace_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ] - king_count_ [ dimension ];
} /* Cubical_Complex::ace_end */

namespace Cubical_detail {
  
  void initialize_types ( unsigned int dimension_, std::vector < unsigned long > & types_, std::vector < unsigned long > & types_inv_, std::vector < unsigned int > & type_dims_ ) {
    const unsigned long maximum = 1 << dimension_;
    types_ . clear ();
    types_inv_ . clear ();
    type_dims_ . clear ();
    types_ . resize ( maximum );
    types_inv_ . resize ( maximum );
    type_dims_ . resize ( maximum );

    unsigned long piece = 0;
    unsigned int dim = 0;
    for ( unsigned int type_index = 0; type_index < maximum; ++ type_index ) {
      while ( 1 ) {
        if ( piece == maximum ) {
          ++ dim;
          piece = 0;
        } /* if */
        unsigned int sum = 0;
        for ( unsigned int bit = 1; bit < maximum; bit <<= 1 ) if ( piece & bit ) ++ sum;
        if ( sum == dim ) break; /* Got it! */
        ++ piece;
      } /* while */
      types_ [ type_index ] = piece;
      types_inv_ [ piece ] = type_index;
      type_dims_ [ type_index ] = dim;
      ++ piece;
    } /* for */
  } /* Cubical_detail::initialize_types */
  
} /* namespace Cubical_detail */

void Cubical_Complex::Allocate_Bitmap ( const std::vector<unsigned int> & user_dimension_sizes ) {
	dimension_ = user_dimension_sizes . size ();
  mask_ = ( 1 << dimension_ ) - 1;
	dimension_sizes_ . resize ( dimension_, 0 );
	jump_values_ . resize ( dimension_, 0 );
  size_ . resize ( dimension_ + 1, 0 );
  total_size_ = 0;
	unsigned long number_of_full_cubes = 1;
	for ( unsigned long index = 0; index < dimension_; ++ index ) { 
		dimension_sizes_ [ index ] = ( 1 + user_dimension_sizes [ index ] );
		jump_values_ [ index ] = number_of_full_cubes;
		number_of_full_cubes *= ( 1 + user_dimension_sizes [ index ] ); 
  } /* for */
	bitmap_ . clear ();
  bitmap_ . resize ( number_of_full_cubes << dimension_, false );
  bitmap_size_ = bitmap_ . size ();
  end_ = const_iterator ( this, bitmap_size_, 0 );
  begin_ . resize ( dimension_ + 2, end_);
  Cubical_detail::initialize_types ( dimension_, types_, types_inv_, type_dims_ );

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

const std::vector<unsigned int> & Cubical_Complex::dimension_sizes ( void ) const {
  return dimension_sizes_;
} /* Cubical_Complex::dimension_sizes */

#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */
#endif
