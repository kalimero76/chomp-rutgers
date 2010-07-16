/*
 *  Adaptive_Complex.cpp
 *
 *
 *  Created by Miro Kramar 2010/01/10.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "complexes/Adaptive_Complex.h"
#include "algorithms/Morse_Theory.h"

#include <boost/foreach.hpp>
#include "boost/tuple/tuple.hpp"


namespace Adaptive_Complex_detail {
  
  unsigned int get_dimension ( unsigned int cell_type ) {
    unsigned int return_value = 0;
    while ( cell_type ) {
      if ( cell_type & 1 ) ++ return_value;
      cell_type >>= 1;
    } /* while */
    return return_value;
  } /* Adaptive_Complex_detail::get_dimension */

  void alias_splitting_helper (std::list < std::list < Cell_Name > > & answer, 
                               Cell_Name & split_me,
                               unsigned int full_dimension ) {
    unsigned int b = split_me . cell_code >> full_dimension;
    unsigned int c = split_me . cell_code & MASK; 
    unsigned int N = get_dimension ( b ); // count how many entries of b are 1
    unsigned int Npow2 = 1 << N;

    std::stack < Node * > work_stack;
    std::list < Cell_Name > alias;
    work_stack . push ( split_me . node );
    while ( not work_stack . empty () ) {
      Node * work_node = work_stack . top ();
      work_stack . pop ();
      if ( work_node -> type == 1 ) {
        alias . push_back ( Cell_Name ( work_node, split_me . cell_code ) );
        continue;
      } /* if */
      unsigned int a = 0;
      for ( unsigned int n = 0; n < Npow2; ++ n ) {
        unsigned int choice = n;
        unsigned int diff = b;
        unsigned int fixed = c;
        for ( unsigned int i = 0; i < full_dimension; ++ full_dimension ) {
          if ( diff & 1 ) {
            /* b1_i != b2_i */
            a <<= 1;
            a |= ( choice & 1 );
            choice >>= 1;
          } else {
            a <<= 1;
            a |= ( fixed & 1 );
          } /* if-else */
          fixed >>= 1;
          diff >>= 1;
        } /* for */
        /* Obtain split cell, add to stack */
        work_stack . push_back ( ( reinterpret_cast < std::vector < Node * > * > ( work_node -> data ) ) -> operator [] ( a ) );
      } /* for */
    } /* while */
    answer . push_back ( alias );
  } /* Adaptive_Complex_detail::alias_descent_helper */
  
  void alias_descent_helper (std::list < std::list < Cell_Name > > & answer, 
                             Division_History & history,
                             unsigned int full_dimension ) {
    unsigned int MASK = 1 << full_dimension - 1;
    std::stack < std::pair <  Division_History::const_reverse_iterator, Cell_Name > > work_stack;
    /* Initialize the stack with the first work item */ {
      Cell_Name relative_state;
      relative_state . node = history . back () . node;
      relative_state . cell_code = MASK << full_dimension; /* 111...1000...0 */
      work_stack . push ( std::make_pair ( history . rbegin (), relative_state ) );
    }
    /* Create Cubes in which to Descend */
    while ( not work_stack . empty () ) {
      Division_History::const_reverse_iterator first_iterator, second_iterator;
      Cell_Name relative_state;
      boost::tie ( first_iterator, relative_state ) = work_stack . top ();
      work_stack . pop ();
      second_iterator = first_iterator;
      ++ second_iterator;
      
      /* What if we cannot subdivide the primary state? */
      if ( second_iterator == history . rend () ) {
        /* TODO: figure out what to do here */
        Cell_Name translated = relative_state;
        translated . cell_code =  first_iterator -> cell_code ^ ( ( ~ relative_state . cell_code >> full_dimension ) & MASK ); // side-by-side
        alias_splitting_helper ( answer, translated, full_dimension );
        continue;
      } /* if */
      
      /* What if we cannot subdivide the relative state? */
      if ( relative_state . node -> type == 1 ) { 
        /* It is a leaf: 
         A "larger alias" which is singleton gets pushed onto the alias list */
        /* We must perform a check to see whether or not a1,b1,c1 is of "terminal dimension" (or instead is later reduced) 
         In fact this implies the cell_codes will be the same */
        if ( history . front () . cell_code == first_iterator -> cell_code ) {
          /* Perform Side-by-Side Translation */
          Cell_Name translated = relative_state;
          translated . cell_code =  first_iterator -> cell_code ^ ( ( ~ relative_state . cell_code >> full_dimension ) & MASK ); // side-by-side
          std::list < Cell_Name > alias;
          alias . push_back ( translated );
          answer . push_back ( alias );
        } /* if */
        continue;
      } /* if */
        
      /* Inheritance-Descent Rules */
      unsigned int a1 = first_iterator -> node -> child_number;
      unsigned int b1 = first_iterator -> cell_code >> full_dimension;
      unsigned int c1 = first_iterator -> cell_code & MASK;
      
      unsigned int a2 = second_iterator -> node -> child_number;
      unsigned int b2 = second_iterator -> cell_code >> full_dimension;
      unsigned int c2 = second_iterator -> cell_code & MASK;
      
      unsigned int d = relative_state . node -> child_number;
      unsigned int s = relative_state . cell_code >> full_dimension;
      //unsigned int t = relative_state . cell_code & MASK;
      
      unsigned int d2, s2, t2;
      
      unsigned int N = get_dimension ( b1 ^ b2 ); // count how many entries of b1 and b2 are different
      
      /* scope */ {
      unsigned int Npow2 = 1 << N;
      for ( unsigned int n = 0; n < Npow2; ++ n ) {
        unsigned int choice = n;
        unsigned int diff = b1 ^ b2;
        unsigned int fixed = a2 ^ ( ~ s );
        for ( unsigned int i = 0; i < full_dimension; ++ full_dimension ) {
          if ( diff & 1 ) {
            /* b1_i != b2_i */
            d2 <<= 1;
            d2 |= ( choice & 1 );
            choice >>= 1;
          } else {
            d2 <<= 1;
            d2 |= ( fixed & 1 );
          } /* if-else */
          fixed >>= 1;
          diff >>= 1;
        } /* for */
        s2 = d2 ^ ( ~ a2 );
        //t2 = a2 & ( ~ d2 );
        /* RELATIVE STATE SANITY CHECK: If s_i = 0 then (b_i = 0 and c_i = not t_i ) */
        if ( get_dimension ( s2 ) < full_dimension ) { // don't return self as alias
          /* We know from above it is not a leaf, so we can descend into children */
          Node * child_node = ( reinterpret_cast < std::vector < Node * > * > ( relative_state . node -> data ) ) -> operator [] ( d2 );
          work_stack . push ( std::make_pair ( second_iterator, Cell_Name ( child_node, ( s2 << full_dimension ) /* | t2 */ ) ) );
        } /* if */
      } /* for */
      } /* scope */
    
    } /* while */
    
  } /* Adaptive_Complex_detail::alias_descent_helper */
  
  std::list < std::list < Cell_Name > > aliases (const Cell_Name & initial_cell_name, 
                                                 unsigned int full_dimension ) {
    std::list < std::list < Cell_Name > > return_value;
    /* Place input as first alias in return list */
    std::list < Cell_Name > alias;
    alias . push_back ( initial_cell_name );
    return_value . push_back ( alias );
    
    Division_History history;
    /* Climb until reach root or geometry is enclosed */
    Cell_Name cell_name = initial_cell_name;
    unsigned int old_dimension = get_dimension ( cell_name . cell_code >> full_dimension );
    /* Determine event */
    unsigned int b = cell_name . cell_code >> full_dimension;
    unsigned int c = cell_name . cell_code & ( ( 1 << full_dimension ) - 1 );
    while ( 1 ) {
      /* Push onto history */
      history . push_back ( cell_name );
      
      /* Check to see if descent should be performed:
       * if dimension increased, start a descent thread. */
      unsigned int dimension = get_dimension ( b );
      if ( dimension > old_dimension ) {
        /* Start a Descending Thread */
        alias_descent_helper ( return_value, history, full_dimension );
      } /* if */
      old_dimension = dimension;
      
      /* Criterion to exit loop */
      if ( dimension == full_dimension || cell_name . node -> parent == NULL ) 
        break;

      /* Ascend to next Cell_Name -- Parent Rule*/
      cell_name . node = cell_name . node -> parent;
      b = b | ( cell_name . node -> child_number ^ c ); // Parent Rule
      c = c & ( ~ b );
      cell_name . cell_code = ( b << full_dimension ) | c;
      
    } /* while */
  } /* Adaptive_Complex_detail::aliases */

} /* namespace Adaptive_Cubical_Detail */



/*
 * Adaptive_const_iterator implementation
 */


Adaptive_const_iterator::Adaptive_const_iterator( void ) : referral(NULL) { }

Adaptive_const_iterator::Adaptive_const_iterator( const   Adaptive_Complex * const referral ) : referral(referral) { }

const Adaptive_Complex::value_type & Adaptive_const_iterator::operator * ( void ) const {
	dereference_value . data () = piece_iterator -> first + (full_cube_number << (referral -> dimension_ << 1 ) );
	dereference_value . dimension () = dimension_;
	return dereference_value;
} /* Adaptive_const_iterator::operator * */

const Adaptive_Complex::value_type * Adaptive_const_iterator::operator -> ( void ) const {
	return &**this;
} /* address of overloaded * of builtin * of this */

Adaptive_const_iterator & Adaptive_const_iterator::operator ++ ( void ) {

	if( piece_iterator !=referral->adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ dimension_ ].end() )
	    if ( ++piece_iterator !=referral->adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ dimension_ ].end() ) {
			return *this;
	}
	while ( 1 ) {
		++ full_cube_number;
		if ( full_cube_number == referral->adaptive_tree->leaf_lookup.size() ) {
      if ( dimension_ < referral -> dimension_ ) {
        ++ dimension_;
        full_cube_number = 0;
        //std::cout << "increasing dimension!\n";
      } else {
        /* Reached end_ */
        return *this = referral -> end_;
      } /* if-else */
		}
    //std::cout << "full cube number = " << full_cube_number << "\n";
    //std::cout << "cells.size = " << referral->adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells.size() << "\n";
    //std::cout << "dimension = " << dimension_ << "\n";
    //std::cout << "referral= " << & referral -> adaptive_tree -> leaf_lookup [ full_cube_number ] -> elementary_cells << "\n";

		if ( ( referral->adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells.size() > 0 )  &&
				( referral->adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ dimension_ ].size() > 0) ){
			piece_iterator = referral->adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ dimension_ ].begin();
			return *this;
		}
	}
} /* Adaptive_const_iterator::operator ++ */

bool Adaptive_const_iterator::operator != ( const const_iterator & right_hand_side ) const {
	if( full_cube_number != right_hand_side.full_cube_number ) return true;
	//in the case we are at the end
	//if( full_cube_number == referral->adaptive_tree->leaf_lookup.size() ) return false;
	if( piece_iterator != right_hand_side.piece_iterator ) return true;
	return false;
}

bool Adaptive_const_iterator::operator == ( const const_iterator & right_hand_side ) const {
	if( full_cube_number != right_hand_side.full_cube_number ) return false;
	//in the case we are at the end
	//if( full_cube_number == referral->adaptive_tree->leaf_lookup.size() ) return true;
	if( piece_iterator != right_hand_side.piece_iterator ) return false;
	return true;}

bool Adaptive_const_iterator::operator < ( const Adaptive_const_iterator & right_hand_side ) const {
  if ( dimension_ == right_hand_side . dimension_ ) {
    if ( full_cube_number == right_hand_side . full_cube_number )
      return * reinterpret_cast < const unsigned long * > ( & piece_iterator ) <
      * reinterpret_cast < const unsigned long * > ( & right_hand_side . piece_iterator );
    return full_cube_number < right_hand_side . full_cube_number;
  }
  return dimension_ < right_hand_side . dimension_;
} /* Adaptive_const_iterator::operator < */

unsigned int Adaptive_const_iterator::dimension () const {
  return dimension_;
} /* Adaptive_const_iterator::dimension */

const Adaptive_Complex & Adaptive_const_iterator::container () const {
  return * referral;
} /* Adaptive_const_iterator::container */


/*/////////////////////////////////////////////////////////////////
* Adaptive_complex implementation /////////////////////////////////
*//////////////////////////////////////////////////////////////////

/* Basic Container */

void Adaptive_Complex::erase ( const Adaptive_const_iterator & input) {
	unsigned int full_cube_number;
	unsigned int cell_ID;
  Adaptive_Cell cell = *input;
	full_cube_number = cell . data () >> ( 2 * dimension_ );
	cell_ID = cell . data () ^ ( full_cube_number << (2 * dimension_ ) );

	unsigned int size = adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ cell . dimension() ].size();
	adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ cell . dimension() ].erase(cell_ID);
	if( size > adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ cell . dimension() ].size() ) {
		-- size_ [ cell . dimension () ];
    -- total_size_;
  } /* if */

  /* TODO: fix begin_ */

} /* Adaptive_Complex::erase */

Adaptive_const_iterator Adaptive_Complex::find ( const Adaptive_Complex::key_type & key ) const {
	const_iterator return_value ( this );
	int cell_ID;
	return_value.full_cube_number = key.data () >> ( 2 * dimension_ );
	cell_ID = key.data () ^ ( return_value.full_cube_number  << ( 2 * dimension_ ) );
	return_value.piece_iterator = adaptive_tree->leaf_lookup [ return_value.full_cube_number ] ->elementary_cells[ key . dimension () ].find(cell_ID);
  return_value.dimension_ = key . dimension ();
	return return_value;
	//TODO if cell is not present it should return end;
}

void Adaptive_Complex::clear ( void ) {
  delete adaptive_tree;
  adaptive_tree = NULL;
  begin_ . clear ();
  begin_ . reserve ( 0 );
  index_ . clear ();
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
}

Adaptive_Complex::iterator Adaptive_Complex::begin ( void ) const {
	return begin_ [ 0 ];
} /* Adaptive_Complex::begin */

Adaptive_Complex::iterator Adaptive_Complex::end ( void ) const {
  return end_;
} /* Adaptive_Complex::end */

Adaptive_Complex::size_type Adaptive_Complex::size ( void ) const {
	return total_size_;
} /* Adaptive_Complex::size */

/* Cell Complex */

Adaptive_Complex::iterator Adaptive_Complex::begin ( unsigned int dimension  ) const {
	return begin_ [ dimension ];
} /* Adaptive_Complex::begin */

Adaptive_Complex::iterator Adaptive_Complex::end ( unsigned int dimension ) const {
  return begin_ [ dimension + 1 ];
} /* Adaptive_Complex::end */

Adaptive_Complex::size_type Adaptive_Complex::size ( unsigned int dimension ) const {
	return size_ [ dimension ];
} /* Adaptive_Complex::size */

Adaptive_Complex::Chain Adaptive_Complex::boundary ( const Adaptive_const_iterator & input ) const {
  //std::cout << "Adaptive_Complex::boundary\n";
  //std::cout << "Finding boundary of " << *input << "\n";
  using Adaptive_Complex_detail;
  Adaptive_Complex::Chain output;
	int coincidence_index = 1;
	/*Go thru all cells which are one dimension lower than input*/
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) {
    Cell_Name cell = * input;
    if ( ( ( cell . cell_code >> ( dimension_ + dimension_index ) ) & 1 ) == 1  ) { 
      /* The dimension is not reduced. */
      Cell_Name lower_cell = cell;
      /* Produce Left Cell */
      lower_cell . cell_code = cell . cell_code & ( not ( 1 << ( dimension_ + dimension_index ) ) ) & ( not ( 1 << dimension_index ) );
      /* Process Left Cell */ {
        std::list < std::list < Cell_Name > > left_boundary_aliases = aliases ( lower_cell );
        /*Process the chain*/
        BOOST_FOREACH ( std::list < Cell_Name > & alias, left_boundary_aliases ) {
          BOOST_FOREACH ( Cell_Name & alias_cell, alias ) {
            if ( reinterpret_cast < Cube_Cells * > ( alias_cell . node -> data ) ) -> read ( alias_cell . cell_code ) )
              output . insert ( std::pair < const_iterator, Ring > ( const_iterator ( alias_cell ), - coincidence_index ) );
          } /* BOOST_FOREACH */
        } /* BOOST_FOREACH */
      } /* Left Cell */
      /* Produce Right Cell */
      lower_cell . cell_code = lower_cell . cell_code | ( 1 << dimension_index );
      /* Process Right Cell */ {
        std::list < std::list < Cell_Name > > right_boundary_aliases = aliases ( lower_cell );
        /*Process the chain*/
        BOOST_FOREACH ( std::list < Cell_Name > & alias, right_boundary_aliases ) {
          BOOST_FOREACH ( Cell_Name & alias_cell, alias ) {
            if ( reinterpret_cast < Cube_Cells * > ( alias_cell . node -> data ) ) -> read ( alias_cell . cell_code ) )
              output . insert ( std::pair < const_iterator, Ring > ( const_iterator ( alias_cell ), coincidence_index ) );
          } /* BOOST_FOREACH */
        } /* BOOST_FOREACH */
			} /* Right Cell */
      /* Negate the sign of the coincidence index. */
			coincidence_index = -coincidence_index;
    } /* if */
	} /* for */
	return output;
} /* Adaptive_Complex::boundary */

Adaptive_Complex::Chain Adaptive_Complex::coboundary ( const Adaptive_const_iterator & input) const {
  Adaptive_Complex::Chain output;
  using Adaptive_Complex_detail;
  Adaptive_Complex::Chain output;
	int coincidence_index = 1;
  Cell_Name cell = * input;
  std::list < std::list < Cell_Name > > cell_aliases = aliases ( cell );
	/*Go thru all cells which are one dimension lower than input*/
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) {
    if ( ( ( cell . cell_code >> ( dimension_ + dimension_index ) ) & 1 ) == 0  ) { 
      BOOST_FOREACH ( std::list < Cell_Name > & alias, cell_aliases ) {
        BOOST_FOREACH ( Cell_Name & alias_cell, alias ) {
          Cell_Name coboundary_cell = alias_cell; 
          coboundary_cell . cell_code = alias_cell . cell_code | ( 1 << ( dimension_ + dimension_index ) );
          if ( reinterpret_cast < Cube_Cells * > ( coboundary_cell . node -> data ) ) -> read ( coboundary_cell . cell_code ) ) {
            int sign = alias_cell . cell_code & ( 1 << dimension_index ) ? coincidence_index : - coincidence_index;
            output . insert ( std::pair < const_iterator, Ring > ( const_iterator ( coboundary_cell ), sign ) );
          } /* if */
        } /* BOOST_FOREACH */
      } /* BOOST_FOREACH */
    } else {
      coincidence_index = - coincidence_index; 
    } /* if-else */
	} /* for */
  
	return output;
} /* Adaptive_Complex::coboundary */

unsigned int Adaptive_Complex::dimension ( void ) const {
  return dimension_;
} /* Adaptive_Complex::dimension */

/* Index Complex */

void Adaptive_Complex::index ( void ) {
  lookup_ . resize ( total_size_ + 1 );
  connection_ . resize ( total_size_, 0 );
  size_type indx = 0;
  for ( const_iterator lookup = begin (); lookup != end (); ++ lookup, ++ indx ) {
    index_ [ lookup ] = indx;
    lookup_ [ indx ] = lookup;
  } /* for */
  index_ [ end_ ] = total_size_;
  lookup_ [ total_size_ ] = end_;
  index_begin_ . resize ( dimension_ + 2, 0 );
  size_type sum = 0;
  for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) {
    sum += size_ [ dimension_index ];
    index_begin_ [ dimension_index + 1 ] = sum;
  } /* for */
} /* Adaptive_Complex::index */

Adaptive_Complex::size_type Adaptive_Complex::index_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ];
} /* Adaptive_Complex::index_begin */

Adaptive_Complex::size_type Adaptive_Complex::index_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ];
} /* Adaptive_Complex::index_end */

Adaptive_Complex::size_type Adaptive_Complex::index ( const const_iterator & lookup ) const {
  return index_ . find ( lookup ) -> second;
} /* Adaptive_Complex::index */

Adaptive_Complex::size_type & Adaptive_Complex::index ( const const_iterator & lookup ) {
  return index_ [ lookup ];
} /* Adaptive_Complex::index */

std::vector < Adaptive_const_iterator > & Adaptive_Complex::lookup ( void ) {
  return lookup_;
} /* Adaptive_Complex::lookup */

const Adaptive_const_iterator & Adaptive_Complex::lookup ( size_type index ) const {
  return lookup_ [ index ];
} /* Adaptive_Complex::lookup */

Adaptive_const_iterator & Adaptive_Complex::lookup ( size_type index ) {
  return lookup_ [ index ];
} /* Adaptive_Complex::lookup */

/* Decomposable Complex */

std::vector < int > Adaptive_Complex::count_all_boundaries ( void ) const {
  std::vector < int > number_of_boundaries ( total_size_ );
  for ( unsigned long cell_index = 0; cell_index < total_size_; ++ cell_index ) {
    number_of_boundaries [ cell_index ] = boundary ( lookup_ [ cell_index ] ) . size ();
  } /* for */
  return number_of_boundaries;
} /* Adaptive_Complex::count_all_boundaries */

class push_functor {
public:
  push_functor ( const Adaptive_Complex & complex, std::vector < Adaptive_Complex::size_type > & output ) : complex(complex), output(output) {}
  void operator () ( std::pair < const Adaptive_const_iterator, Adaptive_Complex::Ring > & term ) {
    output . push_back ( complex . index ( term . first ) );
  }
  const Adaptive_Complex & complex;
  std::vector < Adaptive_Complex::size_type > & output;
};

void Adaptive_Complex::boundary ( std::vector < size_type > & output, const size_type cell_index ) const {
  output . clear ();
	Chain boundary_chain =  boundary ( lookup_ [ cell_index ] );
  for_each ( boundary_chain . begin (), boundary_chain . end (), push_functor ( *this, output ) );
} /* Adaptive_Complex::boundary */

void Adaptive_Complex::coboundary ( std::vector < size_type > & output, const size_type cell_index ) const {
  output . clear ();
  Chain coboundary_chain =  coboundary ( lookup_ [ cell_index ] );
  for_each ( coboundary_chain . begin (), coboundary_chain . end (), push_functor ( *this, output ) );
} /* Adaptive_Complex::coboundary */

class push_functor_with_ring {
public:
  push_functor_with_ring ( const Adaptive_Complex & complex, std::vector < std::pair< Adaptive_Complex::size_type, Default_Ring > > & output ) : complex(complex), output(output) {}
  void operator () ( std::pair < const Adaptive_const_iterator, Adaptive_Complex::Ring > & term ) {
    output . push_back ( std::pair< Adaptive_Complex::size_type, Default_Ring > ( complex . index ( term . first ), term . second ) );
  }
  const Adaptive_Complex & complex;
  std::vector < std::pair< Adaptive_Complex::size_type, Default_Ring > > & output;
};

void Adaptive_Complex::boundary ( std::vector < std::pair< size_type, Default_Ring > > & output, const size_type cell_index ) const {
  output . clear ();
  Chain boundary_chain =  boundary ( lookup_ [ cell_index ] );
  for_each ( boundary_chain . begin (), boundary_chain . end (), push_functor_with_ring ( *this, output ) );
} /* Adaptive_Complex::boundary */

void Adaptive_Complex::coboundary ( std::vector < std::pair< size_type, Ring > > & output, const size_type cell_index ) const {
  output . clear ();
  Chain coboundary_chain =  coboundary ( lookup_ [ cell_index ] );
  for_each ( coboundary_chain . begin (), coboundary_chain . end (), push_functor_with_ring ( *this, output ) );
} /* Adaptive_Complex::coboundary */

void Adaptive_Complex::decompose ( void ) {
  index ();
  king_count_ = morse::decompose ( *this );
} /*  Adaptive_Complex::decompose */

char Adaptive_Complex::type ( size_type index, unsigned int dimension ) const {
  if ( index < index_begin_ [ dimension ] + king_count_ [ dimension + 1 ] ) return 0; /* QUEEN */
  if ( index < index_begin_ [ dimension + 1 ] - king_count_ [ dimension ] ) return 1; /* ACE */
  return 2; /* KING */
} /* Adaptive_Complex::type */

Adaptive_Complex::size_type Adaptive_Complex::mate ( size_type queen_index, unsigned int dimension ) const {
  return index_begin_ [ dimension ] + index_begin_ [ dimension + 2 ] - queen_index - 1;
} /* Adaptive_Complex::mate */

const Adaptive_Complex::Ring & Adaptive_Complex::connection ( size_type queen_index ) const {
  return connection_ [ queen_index ];
} /* Adaptive_Complex::connection */

Adaptive_Complex::Ring & Adaptive_Complex::connection ( size_type queen_index ) {
  return connection_ [ queen_index ];
} /* Adaptive_Complex::connection */

Adaptive_Complex::size_type Adaptive_Complex::ace_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ] + king_count_ [ dimension + 1 ];
} /* Adaptive_Complex::ace_begin */

Adaptive_Complex::size_type Adaptive_Complex::ace_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ] - king_count_ [ dimension ];
} /* Adaptive_Complex::ace_end */

/* Adaptive_Complex */

bool Adaptive_Complex::Add_Full_Cube( std::vector < std::vector <bool> > splitting){
	return adaptive_tree->Add_Full_Cube( splitting );
}
void Adaptive_Complex::Finalize(){
	adaptive_tree->Finalize();
  /* Gives correct values to begin_*/
  /* Set end_ */
  end_ . referral = this;
  end_ . full_cube_number = adaptive_tree->leaf_lookup . size ();
  /* Find begin [ 0 ] */ /* TODO: this fails if there are no vertices */
  begin_ [ 0 ] = end_;
  for ( unsigned long temp_full_cube_number = 0; temp_full_cube_number < adaptive_tree -> leaf_lookup . size (); ++ temp_full_cube_number ) {
    if ( ( adaptive_tree->leaf_lookup [ temp_full_cube_number ]->elementary_cells.size() > 0 )  &&
        ( adaptive_tree->leaf_lookup [ temp_full_cube_number ]->elementary_cells[ 0 ].size() > 0) ){
      begin_ [ 0 ] . full_cube_number = temp_full_cube_number;
      begin_ [ 0 ] . piece_iterator = adaptive_tree->leaf_lookup [ temp_full_cube_number ]->elementary_cells[ 0 ].begin();
      begin_ [ 0 ] . dimension_ = 0;
      begin_ [ 0 ] . referral = this;
      break;
    } /* if */
  } /* for */
  /* Iterate through the complex to fill in begin_ */
  unsigned int dimension = 0;
  for ( const_iterator lookup = begin (); lookup != end (); ++ lookup ) {
    while ( dimension < lookup . dimension () ) {
      ++ dimension;
      begin_ [ dimension ] = lookup;
    }
  } /* for */
  while ( dimension < dimension_ ) {
    ++ dimension;
    begin_ [ dimension ] = end_;
  } /* while */
  begin_ [ dimension_ + 1 ] = end_;
} /* Adaptive_Complex::Finalize */

Adaptive_Complex::Adaptive_Complex ( void ) {

} /* Adaptive_Complex::Adaptive_Complex */

Adaptive_Complex::Adaptive_Complex ( unsigned int complex_dimension ) : dimension_(complex_dimension)  {
	adaptive_tree = new Adaptive_Tree ( this );
  size_ . resize ( dimension_ + 1, 0 );
  total_size_ = 0;
  begin_ . resize ( dimension_ + 2 );
} /* Adaptive_Complex::Adaptive_Complex */

/* Copy Constructor */
Adaptive_Complex::Adaptive_Complex ( const Adaptive_Complex & copy_me) {
	dimension_ = copy_me.dimension_;
	adaptive_tree = new Adaptive_Tree ( this );

  total_size_ = copy_me . total_size_;
  size_ = copy_me . size_;

	adaptive_tree->leaf_lookup.resize( copy_me.adaptive_tree->leaf_lookup.size() );

	/*reconstruct the tree*/
	typedef Adaptive_Tree::Node Node;
	Node * current_node_in_old_tree;
	Node * current_node_in_new_tree;

	std::vector< Node * > old_tree_nodes_stack;
	std::vector< Node * > new_tree_nodes_stack;

	new_tree_nodes_stack.push_back( &adaptive_tree->tree_root );
	old_tree_nodes_stack.push_back( &copy_me.adaptive_tree->tree_root );
	while( new_tree_nodes_stack.size() > 0 ){
		current_node_in_old_tree = old_tree_nodes_stack.back();
		old_tree_nodes_stack.pop_back();
		current_node_in_new_tree = new_tree_nodes_stack.back();
		new_tree_nodes_stack.pop_back();

		current_node_in_new_tree->splitting_dimension = current_node_in_old_tree->splitting_dimension;
		if( current_node_in_new_tree->splitting_dimension == 0){/* it's a leaf */
			current_node_in_new_tree->leaf_number = current_node_in_old_tree->leaf_number;
			current_node_in_new_tree->left_child = NULL;
			current_node_in_new_tree->right_child = NULL;
			adaptive_tree->leaf_lookup[ current_node_in_new_tree->leaf_number ] = current_node_in_new_tree;
			if( current_node_in_old_tree->elementary_cells.size() != 0 ){
				current_node_in_new_tree->elementary_cells = current_node_in_old_tree->elementary_cells;
			}
		}
		else{
			Node * new_node_1 = new Adaptive_Tree::Node;
			Node * new_node_2 = new Adaptive_Tree::Node;
			new_node_1->parent = current_node_in_new_tree;
			current_node_in_new_tree->left_child = new_node_1;
			new_tree_nodes_stack.push_back( new_node_1 );
			old_tree_nodes_stack.push_back( current_node_in_old_tree->left_child );
			new_node_2->parent = current_node_in_new_tree;
			current_node_in_new_tree->right_child = new_node_2;
			new_tree_nodes_stack.push_back( new_node_2 );
			old_tree_nodes_stack.push_back( current_node_in_old_tree->right_child );
		}
	}
  adaptive_tree -> finalized = copy_me . adaptive_tree -> finalized;
  Finalize ();
}
/* Destructor */
Adaptive_Complex::~Adaptive_Complex ( void ) {
  if ( adaptive_tree != NULL ) delete adaptive_tree;
} /* endfunction */

double Adaptive_Complex::Coordinates_Of_Cell ( const Cell & input,  std::vector< double > & coordinates){
	unsigned int full_cube_number;

	Adaptive_Tree::Node * new_node = NULL;
	Adaptive_Tree::Node * old_node = NULL;
	std::vector<int> acsending_path;

	full_cube_number = input.data () >> ( 2 * dimension_ );
	new_node = adaptive_tree->leaf_lookup [ full_cube_number ];
	while(  new_node->parent != NULL ){
		//climbing up
		old_node = new_node;
		new_node = new_node->parent;
		//We arrived from left
		if( new_node->left_child == old_node)
			acsending_path.push_back( 0 );
		//we arived from right
		else
			acsending_path.push_back( 1 );
	}

	coordinates.clear();
	coordinates.resize( dimension_ );
	unsigned int split_count = 1;
	unsigned int splitting_dimension = 0;
	while( acsending_path.size() > 0 ){
		if( acsending_path.back() == 1 )
			coordinates[ splitting_dimension ] += (double)1 / (double) ( 1 << split_count );

		++splitting_dimension;
		splitting_dimension = splitting_dimension % dimension_;
		if( splitting_dimension == 0 )
			++split_count;
		acsending_path.pop_back();
	}
	return split_count;
}

std::vector<Adaptive_Complex::Cell> & Adaptive_Complex::Find_Elementary_Cell( std::vector<Adaptive_Complex::Cell> & output, const Adaptive_Complex::Cell & input) const {

	unsigned int full_cube_number;
	unsigned int cell_ID;
	unsigned int neighbour_cube_number;
	unsigned int neighbour_cell_ID;
	std::vector< Adaptive_Tree::Descend_Info > possible_owners;

	output.clear();

	full_cube_number = input.data () >> ( 2 * dimension_ );
	cell_ID = input.data () - ( full_cube_number << (2 * dimension_ ) );

	//if cell is present in the cube with ID full_cube_number
	std::map < int, bool > elementary_cells = adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ input.dimension() ];
	if( elementary_cells.find( cell_ID) != elementary_cells.end() ){
		output.push_back( input );
		return output;
	}

	Find_Possible_Owners( possible_owners, input, false);
	/* Go thru all possible owners and add the cells */
	std::vector< Adaptive_Tree::Descend_Info >::iterator it;
	for( it = possible_owners.begin(); it != possible_owners.end(); ++it){
		neighbour_cube_number = it->current_node->leaf_number;
		neighbour_cell_ID = 0;
		for(unsigned int dimension_index = 0; dimension_index < dimension_; ++dimension_index){
			if( ( ( cell_ID >> ( dimension_index + dimension_ ) ) % 2 ) == 1 ){/*if it is a reduced dimension*/
				neighbour_cell_ID +=1 << ( dimension_index + dimension_ );
				if( ( ( cell_ID >>  dimension_index   ) % 2 ) == 1 ){/*if cell is on the right side*/
					if( it->relative_position_table[ dimension_index ].back_max_forward_min != 0 ) /* cubes are not on the opposite sides (same side)*/
						neighbour_cell_ID += ( 1 << dimension_index );
				}
				else /*if cell is on the left side*/
					if( it->relative_position_table[ dimension_index ].back_min_forward_max == 0 ) /* cubes are on the opposite sides */
						neighbour_cell_ID +=1 << dimension_index;
			}
			if( it->current_node->elementary_cells.size() > input.dimension() )
				if( it->current_node->elementary_cells[ input.dimension() ].find( neighbour_cell_ID ) != it->current_node->elementary_cells[ input.dimension() ].end() )
					output.push_back( Cell ( ( neighbour_cube_number << ( 2 * dimension_ ) ) + neighbour_cell_ID, input.dimension() ) );
		} /* for */
	}
	return output;
} /* Adaptive_Complex::Find_Elementary_Cell */

std::vector< Adaptive_Tree::Descend_Info > & Adaptive_Complex::Find_Possible_Owners( std::vector< Adaptive_Tree::Descend_Info > & possible_owners, const Adaptive_Complex::Cell & input, bool to_all_neighbours ) const {
	unsigned int full_cube_number;
	unsigned int cell_ID;
	/* Bit 1 at dimension_ + splitting_dimension - 1 means that a node with the splitting_dimension is supposed to be viseted from left
	 * Bit 1 at splitting_dimension - 1 means that a node with the splitting_dimension is supposed to be viseted from right
	 */
	int nodes_to_be_visited = 0;
	int nodes_visited = 0;

	Adaptive_Tree::Node * new_node = NULL;
	Adaptive_Tree::Node * old_node = NULL;
	std::vector<int> acsending_path;
	Adaptive_Tree::Descend_Info descend_info;

	full_cube_number = input.data () >> ( 2 * dimension_ );
	cell_ID = input.data () - ( full_cube_number << (2 * dimension_ ) );
	new_node = adaptive_tree->leaf_lookup [ full_cube_number ];

	//prepare nodes_to_be_visited
	for(unsigned int dimension_index = 0; dimension_index < dimension_; ++dimension_index){
		if( ( cell_ID >> ( dimension_index + dimension_ ) ) % 2 != 0 )/* reduced dimension */
			if( ( cell_ID >> dimension_index ) % 2 != 0 ) /* reduced to the right : suppose to be visited from left*/
				nodes_to_be_visited |= ( 1 << ( dimension_index +  dimension_ ) );
			else /* reduced to the left : suppose to be visited from the right */
				nodes_to_be_visited |= ( 1 << dimension_index  );
	}
	while( ( new_node->parent != NULL ) && ( nodes_to_be_visited != nodes_visited ) ){
			//climbing up
			old_node = new_node;
			new_node = new_node->parent;
      /* Did we arrive from the left? */
			if( new_node->left_child == old_node)
			{
        //We arrived from left
				//if we never arived from left before and its a splitting dimension visited from the left child
				if( (  ( nodes_visited >> ( dimension_ + new_node->splitting_dimension - 1 ) ) % 2 == 0 ) &&
						( ( nodes_to_be_visited >> ( dimension_ + new_node->splitting_dimension - 1 ) ) % 2 != 0 ) ){
					// update visited_nodes and start descending
					nodes_visited |= ( 1 << ( dimension_ + new_node->splitting_dimension - 1 ) );
					descend_info.current_node = new_node->right_child;
					descend_info.back_before_forward = true;
					descend_info.forward_is_smaller = false;
					descend_info.ascending_path = acsending_path;
					descend_info.relative_position_table.clear();
					descend_info.relative_position_table.resize(dimension_);
					descend_info.relative_position_table[ new_node->splitting_dimension - 1 ].Set_To_Opposite( true );
          //std::cout << descend_info . current_node << " --> Descending A ";
          //if ( descend_info . current_node -> right_child != NULL )
          //  std::cout << descend_info . current_node -> right_child -> leaf_number << " - ";


					Descend_To_Possible_Owners( possible_owners, descend_info, nodes_to_be_visited/*nodes_visited*/,  to_all_neighbours );
          //for ( unsigned int s = 0; s < possible_owners . size (); ++ s ) std::cout << possible_owners [ s ] . current_node -> leaf_number << ", ";
          //std::cout << "\n";
				}
				acsending_path.push_back( 0 );
			} else { //we arived from right
        //std::cout << "arrived from right \n";
				//if we never arived from right before and its a pliting dimension visited from the right child
				if( ( ( nodes_visited >> ( new_node->splitting_dimension - 1 ) ) % 2 == 0 ) &&
						( ( nodes_to_be_visited >> ( new_node->splitting_dimension - 1 ) ) % 2 != 0 ) ){
					// update visited_nodes and start descending
					nodes_visited |= 1 << ( new_node->splitting_dimension - 1 );
					descend_info.current_node = new_node->left_child;
					descend_info.back_before_forward = false;
					descend_info.forward_is_smaller = false;
					descend_info.ascending_path = acsending_path;
					descend_info.relative_position_table.clear();
					descend_info.relative_position_table.resize(dimension_);
					descend_info.relative_position_table[ new_node->splitting_dimension - 1 ].Set_To_Opposite( false );
          //std::cout << descend_info . current_node << " --> Descending B ";
					Descend_To_Possible_Owners( possible_owners, descend_info, nodes_to_be_visited/*nodes_visited*/, to_all_neighbours  );
          //for ( unsigned int s = 0; s < possible_owners . size (); ++ s ) std::cout << possible_owners [ s ] . current_node -> leaf_number << ", ";
          //std::cout << "\n";
				}
				acsending_path.push_back( 1 );
			}
		}

	return possible_owners;
} /* Adaptive_Complex::Find_Possible_Owners */


std::vector< Adaptive_Tree::Descend_Info > & Adaptive_Complex::Descend_To_Possible_Owners( std::vector< Adaptive_Tree::Descend_Info > & possible_owners, Adaptive_Tree::Descend_Info descend_info, int splitting_nodes, bool to_all_neighbours ) const{
	std::vector<Adaptive_Tree::Descend_Info> descend_info_stack;
	Adaptive_Tree::Descend_Info current_descend;

	descend_info_stack.push_back(descend_info);
	while( descend_info_stack.size() > 0 ){
		current_descend = descend_info_stack.back();
		descend_info_stack.pop_back();
    //std::cout << "* current node = " << current_descend.current_node << "\n";
		while(  current_descend.current_node->splitting_dimension != 0  ){/*Node in the descending path is not a leaf*/
			if( current_descend.ascending_path.size() > 0 ){ /*Ascending path is not empty*/
        //std::cout << " Ascending path is not empty \n ";

				//if cubes are on the opposite sides in this dimension we have to take the opposite child as we came from in ascending_path
				if( current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_min_forward_max == 0 ||
					current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_max_forward_min == 0 ){
					if( current_descend.ascending_path.back() == 0 ) {
            //std::cout << "right1 ";
						current_descend.current_node = current_descend.current_node->right_child;
					} else {
            //std::cout << "left1 ";
						current_descend.current_node = current_descend.current_node->left_child;
          }
					current_descend.ascending_path.pop_back();
				}
				//current_descend.ascending_path is not empty so if the cubes are not on the opposite sides then they have a common side in this dimension
				else{
					//if the node is a spliiting node (node with reduced dimension visited from the right child )
					if( ( current_descend.ascending_path.back() == 0 && (splitting_nodes >> (dimension_ + current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 ) ||
					  ( current_descend.ascending_path.back() == 1 && (splitting_nodes >> ( current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 )	)
					{
            //std::cout << "node is a splitting node. ";
						bool last_split = true;
						//check if there is another split in the same dimension and the same direction as the one for the current_node
						if ( current_descend.ascending_path.size() > dimension_ )
							for( unsigned int split =  dimension_ - current_descend.current_node->splitting_dimension;    split < ( current_descend.ascending_path.size() - dimension_ + 1); split +=  dimension_ )
								if( current_descend.ascending_path[ split ] == current_descend.ascending_path[ current_descend.ascending_path.size() - 1 ] )
									last_split = false;
						//for the last splitting we have to check also the opposite path
						if( last_split ){
              //std::cout << "last_split ";
							Adaptive_Tree::Descend_Info copy_current_descend;
							copy_current_descend.current_node = current_descend.current_node;
							copy_current_descend.relative_position_table = current_descend.relative_position_table;
							copy_current_descend.back_before_forward = current_descend.back_before_forward;
							copy_current_descend.ascending_path = current_descend.ascending_path;
							copy_current_descend.forward_is_smaller = current_descend.forward_is_smaller;
							if( copy_current_descend.ascending_path.back() == 0 ){
                //std::cout << "right2 ";
								copy_current_descend.relative_position_table[ copy_current_descend.current_node->splitting_dimension - 1 ].Set_To_Opposite( true );
								copy_current_descend.current_node = copy_current_descend.current_node->right_child;
							}
							else{
                //std::cout << "left2 ";
								copy_current_descend.relative_position_table[ copy_current_descend.current_node->splitting_dimension - 1 ].Set_To_Opposite( false );
								copy_current_descend.current_node = copy_current_descend.current_node->left_child;
							}
							copy_current_descend.ascending_path.pop_back();
							//add the copy to the stack
							descend_info_stack.push_back( copy_current_descend );
						}
					}
					//This path is taken in both case cubes on the same side and it is not the last spliting in this dimension and this direction (it is the last splitting)
					if( current_descend.ascending_path.back() == 0 ) {
            //std::cout << "left3 ";
						current_descend.current_node = current_descend.current_node->left_child;
					} else {
            //std::cout << "right3 ";
						current_descend.current_node = current_descend.current_node->right_child;
          }
					current_descend.ascending_path.pop_back();
				}
			}
			else{/* Ascending path is empty */
        //std::cout << " Ascending path is empty \n ";

				current_descend.forward_is_smaller = true;
				if( ( splitting_nodes >> (dimension_ + current_descend.current_node->splitting_dimension - 1 )) % 2 == 1  ||
				    (splitting_nodes >> ( current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 	){/*if the node is a spliiting node*/

					if( current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_max_forward_min == 0 )/*Forward cube on the right side*/ {
						current_descend.current_node = current_descend.current_node->left_child;
            //std::cout << "left4. ";
          } else if( current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_min_forward_max == 0 )/*Forward cube on the left side*/ {
            //std::cout << "right4. ";
						current_descend.current_node = current_descend.current_node->right_child;
          } else {/* cubes are touching in an interval*/
            //std::cout << "cubes are touching in an interval. ";
						if( ( splitting_nodes >> (dimension_ + current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 ){/*cell is on the right*/
              //std::cout << "right5. ";
							current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_min_forward_min = -1;
							current_descend.current_node = current_descend.current_node->right_child;
						} else if( ( splitting_nodes >> ( current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 ){/*cell is on the left*/
              //std::cout << "left5. ";
							current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_max_forward_max = 1;
							current_descend.current_node = current_descend.current_node->left_child;
						}
					}
				} else {/* node is not a splitting node*/
					/*Add the right cube to the stack*/
					Adaptive_Tree::Descend_Info copy_current_descend;
					copy_current_descend.current_node = current_descend.current_node->right_child;;
					copy_current_descend.relative_position_table = current_descend.relative_position_table;
					copy_current_descend.back_before_forward = current_descend.back_before_forward;
					copy_current_descend.ascending_path = current_descend.ascending_path;
					copy_current_descend.forward_is_smaller = current_descend.forward_is_smaller;
					copy_current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_min_forward_min = -1;
					descend_info_stack.push_back( copy_current_descend );
					/*continue with the left cube */
					current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_max_forward_max = 1;
					current_descend.current_node = current_descend.current_node->left_child;
          //std::cout << "left6. ";
				}
			}
		} /* while */
		/* Cube is not smaller but is the first one in the lexicographical order */
		if( ( current_descend.ascending_path.size() == 0 ) && ( !descend_info.back_before_forward ) )
			current_descend.forward_is_smaller = true;
		/* if we searhc for all the neighbours */
		if( to_all_neighbours )
			current_descend.forward_is_smaller = true;
		if( current_descend.forward_is_smaller ) {
      //std::cout << "pushing " << current_descend . current_node -> leaf_number << ". ";
			possible_owners.push_back( current_descend );
    } else {
      //std::cout << "not pushing " << current_descend . current_node -> leaf_number << ".";
    }
	}
	return possible_owners;
} /* Adaptive_Complex::Descend_To_Possible_Owners */

#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */
#endif
