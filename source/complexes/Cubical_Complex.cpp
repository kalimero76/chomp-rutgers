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

Cubical_const_iterator::Cubical_const_iterator ( void ) { 
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
  unsigned int dimension = insert_me . dimension ();
	if ( bit_reference == false ) { 
		bit_reference = true; 
    iterator cell_iterator = find ( insert_me );
    /* Update size_ */
		++ size_ [ dimension ]; 
    ++ total_size_;
    /* Update begin_ */
    iterator next = cell_iterator;
    ++ next; // PROBLEM: This is unbearably slow. TODO
    //while
    while ( begin_ [ dimension ] == next ) {
      begin_ [ dimension ] = cell_iterator;
      if ( dimension == 0 ) break;
      -- dimension;
    } /* while */
    return std::pair < iterator, bool > ( cell_iterator, true );
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

void Cubical_Complex::clear ( void ) {
  bitmap_ . clear ();
  bitmap_ . reserve ( 0 );
  begin_ . clear ();
  begin_ . reserve ( 0 );
  index_ . clear ();
  index_ . reserve ( 0 );
  lookup_ . clear ();
  lookup_ . reserve ( 0 );
  index_begin_ . clear ();
  index_begin_ . reserve ( 0 );
  connection_ . clear ();
  connection_ . reserve ( 0 );
  boundary_count_ . clear ();
  boundary_count_ . reserve ( 0 );
  king_count_ . clear ();
  king_count_ . reserve ( 0 );
  dimension_sizes_ . clear ();
  dimension_sizes_ . reserve ( 0 );
  jump_values_ . clear ();
  jump_values_ . reserve ( 0 );
  types_ . clear ();
  types_ . reserve ( 0 );
  types_inv_ . clear ();
  types_inv_ . reserve ( 0 );
  type_dims_ . clear ();
  type_dims_ . reserve ( 0 );
} /* Cubical_Complex::clear */

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
  /* Also fixes begin_ and size_ from preprocess, which might break it */
  index_ . resize ( bitmap_size_ + 1 );
  lookup_ . resize ( total_size_ + 1 );
  connection_ . resize ( total_size_, 0 );
  size_type indx = 0;
  unsigned int dimension = 0;
  size_ [ 0 ] = 0;
  total_size_ = 0;
  begin_ [ 0 ] = const_iterator ( this, 0, 0 );
  if ( not bitmap_ [ 0 ] ) {
    ++ begin_ [ 0 ];
  } /* if */
  for ( const_iterator lookup = begin (); lookup != end (); ++ lookup, ++ indx ) { 
    while ( dimension < lookup . dimension () ) {
      ++ dimension;
      size_ [ dimension ] = 0;
      begin_ [ dimension ] = lookup;
    }
    ++ size_ [ dimension ];
    ++ total_size_;
    index_ [ lookup . address_ ] = indx;
    lookup_ [ indx ] = lookup;
  } /* for */
  while ( dimension < dimension_ ) {
    ++ dimension;
    size_ [ dimension ] = 0;
    begin_ [ dimension ] = end_;
  } /* while */
  index_ [ end_ . address_ ] = total_size_;
  lookup_ [ total_size_ ] = end_;
  index_begin_ . resize ( dimension_ + 2, 0 );
  size_type sum = 0;
  for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) {
    sum += size_ [ dimension_index ];
    index_begin_ [ dimension_index + 1 ] = sum;
  } /* for */
} /* Cubical_Complex::index */

Cubical_Complex::size_type Cubical_Complex::index_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ]; 
} /* Cubical_Complex::index_begin */ 

Cubical_Complex::size_type Cubical_Complex::index_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ];
} /* Cubical_Complex::index_end */

Cubical_Complex::size_type Cubical_Complex::index ( const const_iterator & lookup ) const {
  return index_ [ lookup . address_ ];
} /* Cubical_Complex::index */

Cubical_Complex::size_type & Cubical_Complex::index ( const const_iterator & lookup ) {
  return index_ [ lookup . address_ ];
} /* Cubical_Complex::index */

std::vector < Cubical_const_iterator > & Cubical_Complex::lookup ( void ) {
  return lookup_;
} /* Cubical_Complex::lookup */

const Cubical_const_iterator & Cubical_Complex::lookup ( size_type index ) const {
  return lookup_ [ index ];
} /* Cubical_Complex::lookup */

Cubical_const_iterator & Cubical_Complex::lookup ( size_type index ) {
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
  if ( closed_complex_ ) {
    for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) {
      for_each ( number_of_boundaries . begin () + index ( begin_ [ dimension_index ] ),
                number_of_boundaries . begin () + index ( begin_ [ dimension_index + 1 ] ), 
                Cubical_detail::set_functor ( 2 * dimension_index ) );
    } /* for */
  } else {
    if ( boundary_count_available_ ) {
      for ( size_type cell_index = 0; cell_index < total_size_; ++ cell_index ) {
        number_of_boundaries [ cell_index ] = boundary_count_ [ lookup_ [ cell_index ] . address_ ];
      } /* for */
    } else {
      for ( size_type cell_index = 0; cell_index < total_size_; ++ cell_index ) {
        number_of_boundaries [ cell_index ] = boundary ( lookup ( cell_index ) ) . size ();
      } /* for */
    } /* if-else */
  } /* if-else */
  return number_of_boundaries;
} /* Cubical_Complex::count_all_boundaries */
  
void Cubical_Complex::boundary ( std::vector < size_type > & output, const size_type index ) const {
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
} /* Cubical_Complex::boundary */

void Cubical_Complex::coboundary ( std::vector < size_type > & output, const size_type index ) const {
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
} /* Cubical_Complex::coboundary */

void Cubical_Complex::boundary ( std::vector < std::pair< size_type, Default_Ring > > & output, const size_type input ) const {
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
			output . push_back ( std::pair<size_type, Ring> ( index_ [ address ], sign ? positive : negative ) );
		/* Insert the piece in the appropriate neighboring full cube */
		long offset_address = address + ( jump_values_ [ dimension_index ] << dimension_ );
		if ( bitmap_ [ offset_address ] )
			output . push_back ( std::pair<size_type, Ring> ( index_ [ offset_address ], sign ? negative : positive ) );
		/* Recover original address */
    address = address ^ work_bit; 
  } /* for */
  return;  
} /* Cubical_Complex::boundary */

void Cubical_Complex::coboundary ( std::vector < std::pair< size_type, Ring > > & output, const size_type input ) const {
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
			output . push_back ( std::pair<size_type, Ring> ( index_ [ address ], sign ? positive : negative ) );
		/* Insert the piece in the appropriate neighboring full cube */
		long offset_address = address - ( jump_values_ [ dimension_index ] << dimension_ );
		if ( bitmap_ [ offset_address ] )
			output . push_back ( std::pair<size_type, Ring> ( index_ [ offset_address ], sign ? negative : positive ) );
		/* Recover original address */
    address = address ^ work_bit; 
  } /* for */
  return; 
} /* Cubical_Complex::coboundary */

void Cubical_Complex::decompose ( void ) {
  index (); 
  king_count_ = morse::decompose ( *this );
} /*  Cubical_Complex::decompose */

char Cubical_Complex::type ( size_type index, unsigned int dimension ) const {
  if ( index < index_begin_ [ dimension ] + king_count_ [ dimension + 1 ] ) return 0; /* QUEEN */
  if ( index < index_begin_ [ dimension + 1 ] - king_count_ [ dimension ] ) return 1; /* ACE */
  return 2; /* KING */
} /* Cubical_Complex::type */

Cubical_Complex::size_type Cubical_Complex::mate ( size_type queen_index, unsigned int dimension ) const {
  return index_begin_ [ dimension ] + index_begin_ [ dimension + 2 ] - queen_index - 1;
} /* Cubical_Complex::mate */

const Cubical_Complex::Ring & Cubical_Complex::connection ( size_type queen_index ) const {
  return connection_ [ queen_index ];
} /* Cubical_Complex::connection */

Cubical_Complex::Ring & Cubical_Complex::connection ( size_type queen_index ) {
  return connection_ [ queen_index ];
} /* Cubical_Complex::connection */

Cubical_Complex::size_type Cubical_Complex::ace_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ] + king_count_ [ dimension + 1 ];
} /* Cubical_Complex::ace_begin */

Cubical_Complex::size_type Cubical_Complex::ace_end ( unsigned int dimension ) const {
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
      type_dims_ [ type_index ] = dim;   // monotone
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
  closed_complex_ = true;
  boundary_count_available_ = false;
	unsigned long number_of_full_cubes = 1;
	for ( unsigned long index = 0; index < dimension_; ++ index ) { 
		dimension_sizes_ [ index ] = ( 2 + user_dimension_sizes [ index ] );
		jump_values_ [ index ] = number_of_full_cubes;
		number_of_full_cubes *= ( 2 + user_dimension_sizes [ index ] ); 
    /* +2 for lower-left and upper-right buffers */
  } /* for */
	bitmap_ . clear ();
  bitmap_ . resize ( number_of_full_cubes << dimension_, false );
  bitmap_size_ = bitmap_ . size ();
  end_ = const_iterator ( this, bitmap_size_, 0 );
  begin_ . resize ( dimension_ + 2, end_);
  Cubical_detail::initialize_types ( dimension_, types_, types_inv_, type_dims_ );
  //std::cout << "Allocate_Bitmap finished\n";
} /* Cubical_Complex::Allocate_Bitmap */

void Cubical_Complex::finalize ( void ) {
  /* Gives correct values to total_size_, begin_, and size_ */
  unsigned int dimension = 0;
  size_ [ 0 ] = 0;
  total_size_ = 0;
  begin_ [ 0 ] = const_iterator ( this, 0, 0 );
  if ( not bitmap_ [ 0 ] ) {
    ++ begin_ [ 0 ];
  } /* if */
  for ( const_iterator lookup = begin (); lookup != end (); ++ lookup ) { 
    while ( dimension < lookup . dimension () ) {
      ++ dimension;
      size_ [ dimension ] = 0;
      begin_ [ dimension ] = lookup;
    }
    ++ size_ [ dimension ];
    ++ total_size_;
  } /* for */
  while ( dimension < dimension_ ) {
    ++ dimension;
    size_ [ dimension ] = 0;
    begin_ [ dimension ] = end_;
  } /* while */
} /* Cubical_Complex::finalize */

void Cubical_Complex::Remove_Full_Cube ( const std::vector<unsigned int> & cube_coordinates ) {
  std::vector<unsigned int> neighbor_coordinates( dimension_, 0);
	/* Calculate the number of the read cube */
	long full_cube_number = 0;
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) 
		full_cube_number += jump_values_ [dimension_index] * ( (long) cube_coordinates [ dimension_index ] + 1 ); // lower-left buffer +1
	/* Erase the pieces from all neighboring cubes */
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
        /* erase the cell */
        erase ( iterator ( this, ( ( full_cube_number + offset ) << dimension_ ) + piece_index , cell_dimension ) ); 
      } /* if */
  } /* for */
  //std::cout << " . \n";
  closed_complex_ = false;
	return; 
} /* Cubical_Complex::Remove_Full_Cube */

void Cubical_Complex::Add_Full_Cube ( const std::vector<unsigned int> & cube_coordinates, bool update ) {
	std::vector<unsigned int> neighbor_coordinates( dimension_, 0);
	/* Calculate the number of the read cube */
	long full_cube_number = 0;
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) 
		full_cube_number += jump_values_ [dimension_index] * ( (long) cube_coordinates [dimension_index] + 1 ); // lower-left buffer +1
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
        if ( update ) {
          insert ( Cell ( ( ( full_cube_number + offset ) << dimension_ ) + piece_index , cell_dimension ) ); 
        } else {
          bitmap_ [ ( ( full_cube_number + offset ) << dimension_ ) + piece_index ] = true;
        }
      } /* if */
  } /* for */
  //std::cout << " . \n";
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
  
  std::vector<int> min_entry (dimension_, 0);
  std::vector<int> max_entry (dimension_, 0);
	std::vector<unsigned int> user_dimension_sizes (dimension_, 0);
	
  
  /* Obtain sizes from the first line of text */ 
  // .cubx
	/* 
  index = 0; while ( text_buffer[index] != '(') index++; index ++; 
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; dimension_index ++ ) {
		ptr = text_buffer + index;
		while ( text_buffer[index] != ',' && text_buffer[index] != ')') index++;
		text_buffer[index] = 0; index++;
		user_dimension_sizes[dimension_index] = atoi(ptr) + 3; 
  } 
  */ /* for */
  
  /* Return to beginning of file */
  input_file . clear ();
  input_file . seekg ( 0, std::ios::beg );
  
  /* Now scan through every line of text and read in full cubes */ {
	std::vector<int> cube_coordinates( dimension_, 0);
	while ( not input_file . eof () ) {
		input_file . getline ( text_buffer, 512, '\n' );
		index = 0; 
		while ( text_buffer[index] != '(' && text_buffer[index] != 0 ) index++; 
		if ( text_buffer [ index ] == 0 ) continue;
		++ index; 
		/* Read the coordinates of the cube from the line */
		for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) {
			ptr = text_buffer + index;
			while ( text_buffer[index] != ',' && text_buffer[index] != ')') index++;
			text_buffer[index] = 0; 
      index++;
      cube_coordinates[dimension_index] = atoi(ptr); 
    } /* for */
		/* Update min_entry and max_entry */
		for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) {
      if ( cube_coordinates [ dimension_index ] < min_entry [ dimension_index ] )
        min_entry [ dimension_index ] = cube_coordinates [ dimension_index ];
      if ( cube_coordinates [ dimension_index ] > max_entry [ dimension_index ] )
        max_entry [ dimension_index ] = cube_coordinates [ dimension_index ];     
    } /* for */
	} /* while */
  } /* scope */
  
  for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) {
    user_dimension_sizes [ dimension_index ] = max_entry [ dimension_index ] - min_entry [ dimension_index ] + 1;
  } /* for */
  
	/* Allocate */
	Allocate_Bitmap ( user_dimension_sizes );
	
  /* Return to beginning of file */
  input_file . clear ();
  input_file . seekg ( 0, std::ios::beg );

  /* Now scan through every line of text and read in full cubes */
  std::vector<unsigned int> cube_coordinates( dimension_, 0);
	while ( not input_file . eof () ) {
		input_file . getline ( text_buffer, 512, '\n' );
		index = 0; 
		while ( text_buffer[index] != '(' && text_buffer[index] != 0 ) index++; 
		if ( text_buffer [ index ] == 0 ) continue;
		++ index; 
		/* Read the coordinates of the cube from the line */
		for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) {
			ptr = text_buffer + index;
			while ( text_buffer[index] != ',' && text_buffer[index] != ')') index++;
			text_buffer[index] = 0; 
      index++;
      cube_coordinates[dimension_index] = atoi(ptr) - min_entry[dimension_index]; 

    } /* for */
		/* Now Add the Cube to the complex. */
		Add_Full_Cube ( cube_coordinates, false ); 
	} /* while */
  finalize ();
	/* We are done reading. Close the file. */
	input_file . close ();
} /* Cubical_Complex::Load_From_File */

const std::vector<unsigned int> & Cubical_Complex::dimension_sizes ( void ) const {
  return dimension_sizes_;
} /* Cubical_Complex::dimension_sizes */

void Cubical_Complex::native_boundary ( std::vector < unsigned long > & output, unsigned long address ) const {
  output . clear ();
  long work_bit = 1;
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; work_bit <<= 1, ++ dimension_index ) {
		/* Can we demote this bit? If not, "continue". */
		if ( not ( address & work_bit) ) continue;
		address = address ^ work_bit;
		if ( bitmap_ [ address ] )
			output . push_back ( address );
		long offset_address = address + ( jump_values_ [ dimension_index ] << dimension_ );
		if ( bitmap_ [ offset_address ] )
			output . push_back ( offset_address );
    address = address ^ work_bit; 
  } /* for */
} /* Cubical_Complex::native_boundary */

void Cubical_Complex::native_coboundary ( std::vector < unsigned long > & output, unsigned long address ) const {
  output . clear ();
  long work_bit = 1;
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; work_bit <<= 1, ++ dimension_index ) {
		/* Can we promote this bit? If not, "continue". */
		if ( address & work_bit ) continue;
		address = address ^ work_bit;
		if ( bitmap_ [ address ] )
			output . push_back ( address );
		long offset_address = address - ( jump_values_ [ dimension_index ] << dimension_ );
		if ( bitmap_ [ offset_address ] )
			output . push_back ( offset_address );
    address = address ^ work_bit; 
  } /* for */
} /* Cubical_Complex::native_coboundary */

namespace Cubical_detail {
  class functor {
  public:
    functor ( std::vector < int > & number_of_coboundaries,
             unsigned long * queen_queue,
             unsigned long & queen_end ) :
    number_of_coboundaries ( number_of_coboundaries ),
    queen_queue ( queen_queue ),
    queen_end ( queen_end ) {}
    void operator () ( unsigned long address ) {
      if ( -- number_of_coboundaries [ address ] == 1 ) queen_queue [ queen_end ++ ] = address;
    }
  private:
    std::vector < int > & number_of_coboundaries;
    unsigned long * queen_queue;
    unsigned long & queen_end;
  };
  
} /* namespace Cubical_detail */

void Cubical_Complex::preprocess ( void ) {
  reductions ();
  coreductions ();
} /* Cubical_Complex::preprocess */

#define PRESWEEP
void Cubical_Complex::reductions ( void ) {
  /* Free face collapses */
  using namespace Cubical_detail;
  unsigned long * queen_queue  = new unsigned long [ total_size_ ];
  unsigned long queen_begin = 0;
  unsigned long queen_end = 0;
  std::vector < int > number_of_coboundaries ( bitmap_size_, 0 );
  functor process_boundary ( number_of_coboundaries, queen_queue, queen_end );
  typedef std::vector < unsigned long > Address_List;
  Address_List boundary;
  Address_List coboundary;
  unsigned long address = 0;
#ifdef PRESWEEP
  /* * * * * * * * *
   * Sweep Routine *
   * * * * * * * * */
  while ( address < bitmap_size_ ) {
    if ( bitmap_ [ address ] ) {
      native_coboundary ( coboundary, address );
      number_of_coboundaries [ address ] = coboundary . size ();
      if ( number_of_coboundaries [ address ] == 1 ) {
        queen_queue [ queen_end ++ ] = address;
      } /* if */
    } /* if */
    ++ address;
  } /* while */
#endif
  /* * * * * * * 
   * MAIN LOOP *
   * * * * * * */
  
#ifndef PRESWEEP
  while ( address < bitmap_size_ || queen_begin < queen_end ) {
    
    /* * * * * * * * *
     * Sweep Routine *
     * * * * * * * * */
    while ( address < bitmap_size_ ) {
      if ( bitmap_ [ address ] ) {
        native_coboundary ( coboundary, address );
        number_of_coboundaries [ address ] = coboundary . size ();
        if ( number_of_coboundaries [ address ] == 1 ) {
          queen_queue [ queen_end ++ ] = address;
          break;
        } /* if */
      } /* if */
      ++ address;
    } /* while */
    ++ address;
#endif
    /* * * * * * * * *
     * Queue Routine *
     * * * * * * * * */
    while ( queen_begin < queen_end ) {
      /* Pick a Queen. */
      while ( queen_begin < queen_end ) {
        /* This suffices to guarantee the queen and king are both in-complex */
        if ( number_of_coboundaries [ queen_queue [ queen_begin ] ] == 1 ) break; 
        ++ queen_begin;
      } /* while */
      if ( queen_begin == queen_end ) break;
      const unsigned long & queen_address = queen_queue [ queen_begin ];
      ++ queen_begin;
      /* Get the king_index */ 
      native_coboundary ( coboundary, queen_address );
      unsigned long king_address = coboundary [ 0 ];
      /* Boundary processing of King and Queen. */
      native_boundary ( boundary, king_address );
      for_each ( boundary . begin (), boundary . end (), process_boundary );
      native_boundary ( boundary, queen_address );
      for_each ( boundary . begin (), boundary . end (), process_boundary );
      /* Remove the King and Queen from copy_complex. */
      /* Breaking the rules -- begin_, size_ are broken. The subsequent call to index () fixes them. */
      bitmap_ [ king_address ] = false;
      bitmap_ [ queen_address ] = false;
      total_size_ -= 2;
    } /* while */
#ifndef PRESWEEP
  } /* while */
#endif
  std::cout << "Reductions complete -- size is now " << total_size_ << "\n";
  delete queen_queue;
} /* Cubical_Complex::reductions */

void Cubical_Complex::coreductions ( void ) {
  /* Free face collapses */
  using namespace Cubical_detail;
  unsigned long * king_queue  = new unsigned long [ total_size_ ];
  unsigned long king_begin = 0;
  unsigned long king_end = 0;
  std::list < unsigned long > vertices;
  std::vector < int > & number_of_boundaries = boundary_count_; 
  boundary_count_ . resize ( bitmap_size_ );
  functor process_coboundary ( number_of_boundaries, king_queue, king_end );
  typedef std::vector < unsigned long > Address_List;
  Address_List boundary;
  Address_List coboundary;
  unsigned long address = 0;

  if ( closed_complex_ == false ) {
    std::cout << "TODO: Cubical_Complex::coreductions for non-closed complexes. Exiting.\n";
    exit ( -1 );
  }
  
  /* Set up number_of_boundaries */
  for ( address = 0; address < bitmap_size_; ++ address ) {
    number_of_boundaries [ address ] = 2 * type_dims_ [ types_inv_ [ address & mask_ ] ];
  } /* for */
  boundary_count_available_ = true;
  
  /* * * * * * * 
   * MAIN LOOP *
   * * * * * * */
  
  address = 0;
  unsigned long hop = 1 << dimension_;
  while ( address < bitmap_size_ || king_begin < king_end ) {
    
    /* * * * * * * * *
     * Sweep Routine *
     * * * * * * * * */
    while ( address < bitmap_size_ ) {
      if ( bitmap_ [ address ] ) {
        /* * * * * * * * * *
         * Vertex Routine  *
         * * * * * * * * * */
        /* Boundary processing of Vertex. */
        native_coboundary ( coboundary, address );
        for_each ( coboundary . begin (), coboundary . end (), process_coboundary );
        bitmap_ [ address ] = false;
        vertices . push_back ( address );
        break;
      } /* if */
      address += hop;
    } /* while */
    address += hop;

    /* * * * * * * * *
     * Queue Routine *
     * * * * * * * * */
    while ( king_begin < king_end ) {
      /* Pick a Queen. */
      while ( king_begin < king_end ) {
        /* This suffices to guarantee the queen and king are both in-complex */
        if ( number_of_boundaries [ king_queue [ king_begin ] ] == 1 ) break; 
        ++ king_begin;
      } /* while */
      if ( king_begin == king_end ) break;
      const unsigned long & king_address = king_queue [ king_begin ];
      ++ king_begin;
      /* Get the king_index */ 
      native_boundary ( boundary, king_address );
      unsigned long queen_address = boundary [ 0 ];
      /* Boundary processing of King and Queen. */
      native_coboundary ( coboundary, king_address );
      for_each ( coboundary . begin (), coboundary . end (), process_coboundary );
      native_coboundary ( coboundary, queen_address );
      for_each ( coboundary . begin (), coboundary . end (), process_coboundary );
      /* Remove the King and Queen from copy_complex. */
      /* Breaking the rules -- begin_, size_ are broken. The subsequent call to index () fixes them. */
      bitmap_ [ king_address ] = false;
      bitmap_ [ queen_address ] = false;
      total_size_ -= 2;
    } /* while */
  } /* while */
  /* * * * * * * * * * *
   * Replace vertices  *
   * * * * * * * * * * */
  for ( std::list<unsigned long>::const_iterator vertex_iterator = vertices . begin ();
       vertex_iterator != vertices . end (); ++ vertex_iterator ) {
    bitmap_ [ * vertex_iterator ] = true;
  }
  std::cout << "Coreductions complete -- size is now " << total_size_ << "\n";
  delete king_queue;
  closed_complex_ = false; // typically
} /* Cubical_Complex::coreductions */

#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */
#endif
