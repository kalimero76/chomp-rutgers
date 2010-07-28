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

#include <stack>

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

  void alias_splitting_helper (std::list < std::list < GeoCell > > & answer, 
                               GeoCell & split_me,
                               const Adaptive_Complex & complex ) {
    
    //std::cout << "alias_splitting_helper ( " << split_me . node << " " << split_me . code << " )\n";
    /* The Cells are using geometric codes in this function */
    unsigned int b = split_me . code >> complex . dimension ();
    unsigned int c = split_me . code & complex . MASK; 
    unsigned int N = get_dimension ( b ); // count how many entries of b are 1
    unsigned int Npow2 = 1 << N;

    std::stack < Node * > work_stack;
    std::list < GeoCell > alias;
    work_stack . push ( split_me . node );
    while ( not work_stack . empty () ) {
      Node * work_node = work_stack . top ();
      work_stack . pop ();
      if ( work_node -> type == 1 ) {
        /* It is a leaf and we can split no more. */
        //std::cout << "  > outputting (" << work_node << " " << split_me . code << ")\n";
        alias . push_back ( GeoCell ( work_node, split_me . code ) );
        continue;
      } /* if */
      for ( unsigned int n = 0; n < Npow2; ++ n ) {
        unsigned int a = 0;
        unsigned int choice = n;
        unsigned int bit = 1;
        while ( bit < complex . MASK ) {
          if ( b & bit ) {
            /* b1_i != b2_i */
            if ( choice & 1 ) {
              a |= bit;
            } /* if */
            choice >>= 1;
          } else {
            a |= c & bit;
          } /* if-else */
          bit <<= 1;
        } /* for */
        
        /* Obtain split cell, add to stack */
        //std::cout << "  > Splitting into child " << a << " from node " << work_node << "\n";
        work_stack . push ( ( reinterpret_cast < std::vector < Node * > * > ( work_node -> data ) ) -> operator [] ( a ) );
      } /* for */
    } /* while */
    answer . push_back ( alias );
  } /* Adaptive_Complex_detail::alias_descent_helper */
  
  void alias_descent_helper (std::list < std::list < GeoCell > > & answer, 
                             const Division_History & history,
                             const Adaptive_Complex & complex ) {
    //std::cout << "alias_descent_helper: ";
    std::stack < std::pair <  Division_History::const_reverse_iterator, GeoCell > > work_stack;
    /* Initialize the stack with the first work item */ {
      GeoCell relative_state;
      relative_state . node = history . back () . node;
      relative_state . code = complex . MASK << complex . dimension (); /* 111...1000...0 */
      work_stack . push ( std::make_pair ( history . rbegin (), relative_state ) );
      //std::cout << " (" << relative_state . node << ", " << history . back () . code << ")\n";

    } /* scope */
    /* Create Cubes in which to Descend */
    while ( not work_stack . empty () ) {
      Division_History::const_reverse_iterator first_iterator, second_iterator;
      GeoCell relative_state;
      boost::tie ( first_iterator, relative_state ) = work_stack . top ();
      work_stack . pop ();
      second_iterator = first_iterator;
      ++ second_iterator;
      
      //std::cout << " alias_descent work loop: primary = (" << first_iterator -> node << ", " << first_iterator -> code << ")  relative = (" << relative_state . node << ", " << relative_state . code << ")\n";
      /* What if we cannot subdivide the primary state? */
      if ( second_iterator == history . rend () ) {
        //std::cout << "   cannot subdivide the primary state (calling splitting_helper) \n";
        GeoCell translated = relative_state;
        translated . code =  first_iterator -> code ^ ( ( ~ relative_state . code >> complex . dimension () ) & complex . MASK ); // side-by-side
        alias_splitting_helper ( answer, translated, complex );
        continue;
      } /* if */
      
      /* What if we cannot subdivide the relative state? */
      if ( relative_state . node -> type == 1 ) { 
        //std::cout << "   cannot subdivide the relative state\n";
        /* It is a leaf: 
         A "larger alias" which is singleton gets pushed onto the alias list */
        /* We must perform a check to see whether or not a1,b1,c1 is of "terminal dimension" (or instead is later reduced) 
         In fact this implies the codes will be the same */
        if ( ( history . front () . code >> complex . dimension () ) == ( first_iterator -> code  >> complex . dimension () ) ) {
          /* Question : does above check work without bitshift? */
          if ( ( history . front () . code ) != ( first_iterator -> code ) ) std::cout << "ANSWER TO QUESTION IS NO.\n"; // DBUG 
          /* Perform Side-by-Side Translation */
          GeoCell translated = relative_state;
          translated . code =  first_iterator -> code ^ ( ( ~ relative_state . code >> complex . dimension () ) & complex . MASK ); // side-by-side
          //std::cout << "outputting (unsubdividable relative state) (" << translated . node << ", " << translated . code << ") \n";
          std::list < GeoCell > alias;
          alias . push_back ( translated );
          answer . push_back ( alias );
        } /* if */
        continue;
      } /* if */
        
      /* Inheritance-Descent Rules */
      //unsigned int a1 = first_iterator -> node -> child_number;
      unsigned int b1 = first_iterator -> code >> complex . dimension ();
      unsigned int b2 = second_iterator -> code >> complex . dimension ();

      //unsigned int c1 = first_iterator -> code & complex . MASK;
      

      //unsigned int c2 = second_iterator -> code & complex . MASK;
      
      //unsigned int d = relative_state . node -> child_number;
      //unsigned int t = relative_state . code & complex . MASK;
      
      // unsigned int t2;
      
      unsigned int N = get_dimension ( b1 ^ b2 ); // count how many entries of b1 and b2 are different
      
      /* scope */ {
      unsigned int Npow2 = 1 << N;
      for ( unsigned int n = 0; n < Npow2; ++ n ) {
        unsigned int choice = n;
        unsigned int bit_choice = 0;
        unsigned int bit_choice2 = 0;
        unsigned int a2 = second_iterator -> node -> child_number;
        unsigned int s1 = relative_state . code >> complex . dimension ();
        unsigned int d2 = 0, s2 = 0;
        unsigned int diff = b1 ^ b2;
        unsigned int fixed = complex . MASK & ( a2 ^ ( ~ s1 ) );
        //std::cout << " BEGIN \n";
        //std::cout << " a2 = " << a2 << " and s1 = " << s1 << "\n";
        //std::cout << " choice  = " << choice << " and diff = " << diff  << " and fixed = " << (unsigned int ) ( fixed & complex . MASK ) << "\n";
        unsigned int bit = 1;
        while ( bit < complex . MASK ) {
          if ( diff & bit ) {
            /* b1_i != b2_i */
            if ( choice & 1 ) {
              bit_choice |= bit;
              if ( a2 & bit ) bit_choice2 |= bit;
            } else {
              if ( not ( a2 & bit ) ) bit_choice2 |= bit;
            } /* if-else */
            choice >>= 1;
          } /* if */
          bit <<= 1;
        } /* for */
        d2 = bit_choice | ( ( ~ diff ) & fixed );
        s2 = bit_choice2 | ( ( ~ diff ) & s1 );
        //std::cout << " bit_choice = " << bit_choice << " and bit_choice2 = " << bit_choice2 << "\n";
        //std::cout << "d2 = " << d2 << " and s2 = " << s2 << "\n";
        /* RELATIVE STATE SANITY CHECK: If s_i = 0 then (b_i = 0 and c_i = not t_i ) */
        if ( get_dimension ( s2 ) < complex . dimension () ) { // don't return self as alias
          /* We know from above it is not a leaf, so we can descend into children */
          Node * child_node = ( reinterpret_cast < std::vector < Node * > * > ( relative_state . node -> data ) ) -> operator [] ( d2 );
          //std::cout << " pushing onto descent_work_stack: (" << child_node << ", " << ( s2 << complex . dimension () ) << ") \n";
          work_stack . push ( std::make_pair ( second_iterator, GeoCell ( child_node, ( s2 << complex . dimension () ) /* | t2 */ ) ) );
        } /* if */ else { //std::cout << " ignoring self-answer\n"; 
        }
      } /* for */
      } /* scope */
    
    } /* while */
    
  } /* Adaptive_Complex_detail::alias_descent_helper */
  
  std::list < std::list < GeoCell > > 
  aliases (const GeoCell & initial_cell, const Adaptive_Complex & complex ) {
    //std::cout << "ALIASES ( " << initial_cell . node << " " << initial_cell . code << "\n";
    std::list < std::list < GeoCell > > return_value;
    /* Place input as first alias in return list */
    std::list < GeoCell > alias;
    alias . push_back ( initial_cell );
    return_value . push_back ( alias );
    //std::cout << "outputting (" << initial_cell . node << ", " << initial_cell . code << ") \n";

    Division_History history;
    /* Climb until reach root or geometry is enclosed */
    GeoCell cell = initial_cell;
    unsigned int old_dimension = get_dimension ( cell . code >> complex . dimension () );
    /* Determine event */
    unsigned int b = cell . code >> complex . dimension ();
    unsigned int c = cell . code & ( ( 1 << complex . dimension () ) - 1 );
    while ( 1 ) {
      /* Push onto history */
      history . push_back ( cell );
      //std::cout << " pushed " << cell . node << " " << cell . code << " onto history.\n";
      /* Check to see if descent should be performed:
       * if dimension increased, start a descent thread. */
      unsigned int dimension = get_dimension ( b );
      if ( dimension > old_dimension ) {
        /* Start a Descending Thread */
        alias_descent_helper ( return_value, history, complex );
      } /* if */
      old_dimension = dimension;
      
      /* Criterion to exit loop */
      if ( dimension == complex . dimension () || cell . node -> parent == NULL ) 
        break;

      /* Ascend to next GeoCell -- Parent Rule*/
      b = b | ( cell . node -> child_number ^ c ); // Parent Rule
      cell . node = cell . node -> parent;
      cell . code = ( b << complex . dimension () ) | c;
      
    } /* while */
    return return_value;
  } /* Adaptive_Complex_detail::aliases */

  
  /* Cube_Cells definitions */
  
  Cube_Cells::Cube_Cells ( unsigned int dimension ) {
    size_type N = 1;
    for ( unsigned int dim = 0; dim < dimension; ++ dim ) N *= 3;
    cell_data_ . resize ( N );
  } /* Adaptive_Complex_detail::Cube_Cells::Cube_Cells */

  const bool Cube_Cells::read ( size_type name ) const {
    return cell_data_ [ name ];
  } /* Adaptive_Complex_detail::Cube_Cells::read */
  
  void Cube_Cells::write ( size_type name, bool value ) {
    cell_data_ [ name ] = value;
  } /* Adaptive_Complex_detail::Cube_Cells::write */
  
  /* Node definitions */
  
  Node::Node ( Node * parent, size_type child_number ) : type(1), child_number(child_number), parent(parent), data(NULL) {
  } /* Adaptive_Complex_detail::Node::Node */

  Node::~Node ( void ) {
    if ( type == 0 ) {
      /* This is a node */
      BOOST_FOREACH ( Node * child, * reinterpret_cast < std::vector < Node * > * > ( data ) ) {
        delete child;
      } /* BOOST_FOREACH */
    } else {
      if ( data != NULL ) delete reinterpret_cast < Cube_Cells * > ( data );
    } /* if-else */
  } /* Adaptive_Complex_detail::Node::~Node */
  
  Node * become_next_leaf ( Node * & node ) {
    int which_child;
    //std::cout << "become_next_leaf " << node << "\n";
    if ( node -> parent != NULL ) {
      //std::cout << "   not the root\n";
      /* Ascending Phase */
      while ( 1 ) {
        which_child = node -> child_number;
        if ( node -> parent == NULL ) return node; // reached "end"
        /* Ascend. */
        node = node -> parent;
        //std::cout << "   ascended to " << node << "\n";
        /* If we are not the last child, enter descending phase*/
        if ( ( unsigned int ) ( which_child + 1 ) < reinterpret_cast < std::vector < Node * > * > ( node -> data ) -> size () )
          break;
      } /* while */
    } else {
      //std::cout << "   is the root\n";
      /* We are beginning at the root; skipped Ascending Phase */
      which_child = -1;
    } /* if-else */
    
    /* First Descent */
    node = reinterpret_cast < std::vector < Node * > * > ( node -> data ) -> operator [] ( which_child + 1 );
    //std::cout << "   descended to " << node << "\n";
    /* Descend */
    while ( node -> type == 0 ) {
      /* While we have not yet reached a leaf */
      node = reinterpret_cast < std::vector < Node * > * > ( node -> data ) -> operator [] ( 0 );
      //std::cout << "   descended to " << node << "\n";
    } /* while */
    //std::cout << "  returning " << node << "\n";

    return node;
  } /* become_next_leaf */
  
  GeoCell::GeoCell ( const Cell & cell, const Adaptive_Complex & complex ) {
    node = cell . node;
    code = complex . geometric_code ( cell . code );
  } /* Adaptive_Cubical_detail::GeoCell::GeoCell */
  
} /* namespace Adaptive_Cubical_detail */

std::ostream & operator << ( std::ostream & output_stream, const Adaptive_Cell & print_me ) {
  output_stream << "(" << print_me . node << ", " << print_me . code << ")";
  return output_stream;
} /* operator << for Cell */


/* * * * * * * * * * * * * * * * * * * *
 * Adaptive_const_iterator definitions *
 * * * * * * * * * * * * * * * * * * * */

/* Constructors */
Adaptive_const_iterator::Adaptive_const_iterator( void ) : container_(NULL) {
} /* Adaptive_const_iterator::Adaptive_const_iterator */

Adaptive_const_iterator::Adaptive_const_iterator( const Adaptive_Complex * const container ) : container_(container) {
} /* Adaptive_const_iterator::Adaptive_const_iterator */

Adaptive_const_iterator::Adaptive_const_iterator( const Adaptive_Complex * const container, const Cell & cell ) : container_(container), cell_(cell) { 
  dimension_ = Adaptive_Complex_detail::get_dimension ( container -> geometric_code ( cell . code ) >> container -> dimension () );
} /* Adaptive_const_iterator::Adaptive_const_iterator */

Adaptive_const_iterator::Adaptive_const_iterator( const Adaptive_Complex * const container, const Adaptive_Complex_detail::GeoCell & cell ) : container_(container) { 
  cell_ . node = cell . node;
  cell_ . code = container_ -> packing_code ( cell . code );
  dimension_ = Adaptive_Complex_detail::get_dimension ( cell . code >> container -> dimension () );
} /* Adaptive_const_iterator::Adaptive_const_iterator */

/* Iterator */

Adaptive_const_iterator & Adaptive_const_iterator::operator ++ ( void ) {
  //std::cout << "operator ++\n";
  while ( 1 ) {
    //std::cout << "node = " << cell_ . node << " code = " << cell_ . code << " dimension_ = " << dimension_ << " end = " << container () . packing_code_end ( dimension_ ) << "\n";
    ++ cell_ . code;
    if ( cell_ . code == container () . packing_code_end ( dimension_ ) ) {
      //std::cout << " Reached the end of the codes of dimension " << dimension_ << "\n";
      become_next_leaf ( cell_ . node );
      //std::cout << " node = " << cell_ . node << "\n";
      if ( cell_ . node == container () . root_ ) {
        //std::cout << "  Reached the end of the leaves \n";
        if ( dimension_ == container () . dimension () ) return *this; // returning end
        /* Obtain begin of next dimension */
        become_next_leaf ( cell_ . node ); 
        //std::cout << "  node = " << cell_ . node << "\n";
        ++ dimension_;
        //std::cout << "  dimension = " << dimension_ << "\n";
      } /* if */
      //std::cout << " begin  = " << container () . packing_code_begin ( dimension_ ) << "\n";
      cell_ . code = container () . packing_code_begin ( dimension_ );
    } /* if */
    if ( cell_ . node -> data != NULL && reinterpret_cast < Adaptive_Complex_detail::Cube_Cells * > ( cell_ . node -> data ) -> read ( cell_ . code ) ) return *this;
  } /* while */
} /* Adaptive_const_iterator::operator ++ */

const Adaptive_Complex::value_type & Adaptive_const_iterator::operator * ( void ) const {
	return cell_;
} /* Adaptive_const_iterator::operator * */

/* Relations */

bool Adaptive_const_iterator::operator != ( const const_iterator & right_hand_side ) const {
  if ( cell_ . code != right_hand_side . cell_ . code ) return true;
  if ( cell_ . node != right_hand_side . cell_ . node ) return true;
  return false;
} /* Adaptive_const_iterator::operator != */

bool Adaptive_const_iterator::operator == ( const const_iterator & right_hand_side ) const {
  if ( cell_ . code != right_hand_side . cell_ . code ) return false;
  if ( cell_ . node != right_hand_side . cell_ . node ) return false;
  return true;
} /* Adaptive_const_iterator::operator == */

bool Adaptive_const_iterator::operator < ( const Adaptive_const_iterator & right_hand_side ) const {
  if ( cell_ . code < right_hand_side . cell_ . code ) return true;
  if ( cell_ . code > right_hand_side . cell_ . code ) return false;
  return  ( cell_ . node < right_hand_side . cell_ . node );
} /* Adaptive_const_iterator::operator < */

/* Extras */

unsigned int Adaptive_const_iterator::dimension () const {
  return dimension_;
} /* Adaptive_const_iterator::dimension */

const Adaptive_Complex & Adaptive_const_iterator::container () const {
  return * container_;
} /* Adaptive_const_iterator::container */

/* * * * * * * * * * * * * * * * * 
 * Adaptive_Complex definitions  *
 * * * * * * * * * * * * * * * * */

/* Basic Container */

void Adaptive_Complex::erase ( const Adaptive_const_iterator & erase_me ) {
  bool bit = reinterpret_cast < Adaptive_Complex_detail::Cube_Cells * > ( ( * erase_me ) . node -> data ) -> read ( ( * erase_me ) . code );
  unsigned int dimension = erase_me . dimension_;
	if ( bit == true ) {
    reinterpret_cast < Adaptive_Complex_detail::Cube_Cells * > ( ( * erase_me ) . node -> data ) -> write ( ( * erase_me ) . code, false );
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
} /* Adaptive_Complex::erase */

Adaptive_const_iterator Adaptive_Complex::find ( const Adaptive_Complex::key_type & key ) const {
  return const_iterator ( this, key );
} /* Adaptive_Complex::find */

void Adaptive_Complex::clear ( void ) {
  
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
  
  if ( root_ != NULL ) delete root_;
	root_ = new Adaptive_Complex_detail::Node ( NULL, 0 );

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
  using namespace Adaptive_Complex_detail;
  Adaptive_Complex::Chain output;
	int coincidence_index = 1;
	/*Go thru all cells which are one dimension lower than input*/
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) {
    GeoCell cell ( *input, *this );
    if ( ( ( cell . code >> ( dimension_ + dimension_index ) ) & 1 ) == 1  ) { 
      /* The dimension is not reduced. */
      GeoCell lower_cell = cell;
      /* Produce Left Cell */
      lower_cell . code = cell . code & ( ~ ( 1 << ( dimension_ + dimension_index ) ) ) & ( ~ ( 1 << dimension_index ) );
      /* Process Left Cell */ {
        std::list < std::list < GeoCell > > left_boundary_aliases = aliases ( lower_cell, *this );
        /*Process the chain*/
        BOOST_FOREACH ( std::list < GeoCell > & alias, left_boundary_aliases ) {
          BOOST_FOREACH ( GeoCell & alias_cell, alias ) {
            if ( alias_cell . node -> data != NULL && reinterpret_cast < Cube_Cells * > ( alias_cell . node -> data ) 
                 -> read ( packing_code ( alias_cell . code ) ) )
              output . insert ( std::pair < const_iterator, Ring > ( const_iterator ( this, alias_cell ), - coincidence_index ) );
          } /* BOOST_FOREACH */
        } /* BOOST_FOREACH */
      } /* Left Cell */
      /* Produce Right Cell */
      lower_cell . code = lower_cell . code | ( 1 << dimension_index );
      /* Process Right Cell */ {
        std::list < std::list < GeoCell > > right_boundary_aliases = aliases ( lower_cell, *this );
        /*Process the chain*/
        BOOST_FOREACH ( std::list < GeoCell > & alias, right_boundary_aliases ) {
          BOOST_FOREACH ( GeoCell & alias_cell, alias ) {
            if ( alias_cell . node -> data != NULL && reinterpret_cast < Cube_Cells * > ( alias_cell . node -> data ) 
                 -> read ( packing_code ( alias_cell . code ) ) )
              output . insert ( std::pair < const_iterator, Ring > ( const_iterator ( this, alias_cell ), coincidence_index ) );
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
  //std::cout << "COBOUNDARY of " << * input << "\n\n";
  Adaptive_Complex::Chain output;
  using namespace Adaptive_Complex_detail;
	int coincidence_index = 1;
  GeoCell cell ( *input, *this );
  std::list < std::list < GeoCell > > cell_aliases = aliases ( cell, *this );
	/*Go thru all cells which are one dimension lower than input*/
	for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) {
    if ( ( ( cell . code >> ( dimension_ + dimension_index ) ) & 1 ) == 0  ) { 
      BOOST_FOREACH ( std::list < GeoCell > & alias, cell_aliases ) {
        BOOST_FOREACH ( GeoCell & alias_cell, alias ) {
          GeoCell coboundary_cell = alias_cell; 
          coboundary_cell . code = alias_cell . code | ( 1 << ( dimension_ + dimension_index ) );
          //std::cout << "alleged coboundary cell (geometric form): " << coboundary_cell . node << ", " << coboundary_cell . code << "\n";
          if ( coboundary_cell . node -> data != NULL && reinterpret_cast < Cube_Cells * > ( coboundary_cell . node -> data )
               -> read ( packing_code ( coboundary_cell . code ) ) ) {
            //std::cout << "verified coboundary cell (geometric form): " << coboundary_cell . node << ", " << coboundary_cell . code << "\n";
            output . insert ( std::pair < const_iterator, Ring > ( const_iterator ( this, coboundary_cell ), 
                                                                   alias_cell . code & ( 1 << dimension_index ) ? coincidence_index : - coincidence_index ) );
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

Adaptive_Complex::Adaptive_Complex ( void ) {
  
} /* Adaptive_Complex::Adaptive_Complex */

Adaptive_Complex::Adaptive_Complex ( unsigned int complex_dimension ) : dimension_(complex_dimension)  {
  /* Initialize Tree */
	root_ = new Adaptive_Complex_detail::Node ( NULL, 0 );
  //std::cout << "root_ = " << root_ << "\n";
  /* Initialize container variables */
  size_ . resize ( dimension_ + 1, 0 );
  total_size_ = 0;
  begin_ . resize ( dimension_ + 2 );
  /* Initialize MASK */
  MASK = ( 1 << dimension_ ) - 1;
  /* Initialize packing_code - geometric_code translation tables */
  size_type N = 1 << ( dimension_ << 1 ); // N = 4^dimension_
  size_type M = 1; for ( unsigned int dim = 0; dim < dimension_; ++ dim ) M *= 3; // M = 3^dimension
  
  //std::cout << " N = " << N << " M = " << M << "\n";
  
  geometric_code_ . resize ( M );
  packing_code_ . resize ( N );
  packing_code_begin_ . resize ( dimension_ + 2 );
  /* Produce a list of geometric codes, ordered as the packing codes are */
  std::list < size_type > code_list;
  std::map < unsigned int, std::list < size_type > :: iterator > position_map;
  for ( size_type gcode = 0; gcode < N; ++ gcode ) {
    unsigned int dim = Adaptive_Complex_detail::get_dimension ( gcode >> dimension_ );
    if ( gcode & ( gcode >> dimension_ ) ) continue;
    if ( position_map . lower_bound ( dim ) == position_map . end () ) {
      position_map [ dim ] = code_list . insert ( code_list . end (), gcode );
    } else {
      position_map [ dim ] = code_list . insert ( position_map . lower_bound ( dim ) -> second, gcode );
    } /* if-else */
  } /* for */
  /* Record the correspondence between "proper" geometric codes and packing codes */
  size_type index = 0;
  BOOST_FOREACH ( size_type gcode, code_list ) {
    geometric_code_ [ index ] = gcode;
    packing_code_ [ gcode ] = index;
    //std::cout << "packing_code_ [ " << gcode << " ] = " << index << "\n"; 
    ++ index;
  } /* BOOST_FOREACH */
  /* Extend the correspondence to "improper" geometric codes */
  for ( size_type gcode = 0; gcode < N; ++ gcode ) {
    packing_code_ [ gcode ] = packing_code_ [ ( ~ gcode >> dimension_ ) & gcode ];
  } /* for */
  /* Produce packing_code_begin_ */
  for ( unsigned int dim = 0; dim <= dimension_; ++ dim ) {
    packing_code_begin_ [ dim ] = packing_code_ [ * position_map . lower_bound ( dim ) -> second ];
  } /* for */
  packing_code_begin_ [ dimension_ + 1 ] = M;
} /* Adaptive_Complex::Adaptive_Complex */

/* Copy Constructor */
Adaptive_Complex::Adaptive_Complex ( const Adaptive_Complex & copy_me) {
  /* TODO */
}
/* Destructor */

Adaptive_Complex::~Adaptive_Complex ( void ) {
  if ( root_ != NULL ) delete root_;
} /* endfunction */

bool Adaptive_Complex::Add_Full_Cube ( std::vector < unsigned int > splitting) {
  //std::cout << "Add_Full_Cube \n";
  using namespace Adaptive_Complex_detail;
  Node * node = root_;
  BOOST_FOREACH ( unsigned int child_number, splitting ) {
 //   std::cout << "  child_number = " << child_number << "\n";
    if ( node -> type == 1 ) {
      /* The node is a leaf. */ 
      /* The node should no longer be a leaf. */
      if ( node -> data != NULL ) {
        //std::cout << "Warning, Adaptive_Complex::Add_Full_Cube being misused, returning without effect \n";
        return false;
      } /* if */
      node -> type = 0; // Now it is a node.
      node -> data = new std::vector<Node *> ( 1 << dimension_, ( Node * ) NULL );
      unsigned int child_index = 0;
      //std::cout << " Giving children to node " << node << "\n";
      BOOST_FOREACH ( Node * & child, * reinterpret_cast < std::vector<Node *> * > ( node -> data ) ) {
        //std::cout << "   child [ " << child_index << " ] = ";
        child = new Node ( node, child_index ++ );
        //std::cout << child << "\n";
      } /* BOOST_FOREACH */
      //std::cout << "child_index = " << child_index << "\n";
    } /* if */
    //std::cout << " node -> data = " << node -> data << " and child_number = " << child_number << "\n";
    node = reinterpret_cast < std::vector<Node *> * > ( node -> data ) -> operator [] ( child_number );
  } /* BOOST_FOREACH */
  node -> data = new Adaptive_Complex_detail::Cube_Cells ( dimension_ );
  return true;
} /* Adaptive_Complex::Add_Full_Cube */

namespace Adaptive_Complex_detail {

  void finalize_helper ( const GeoCell & input, const Adaptive_Complex & complex ) {
    //std::cout << "\n### finalize_helper : " << input . node << " " << input . code << "\n";
    /* Add the cell unless there is a conflict */
    std::list < std::list < GeoCell > > cell_aliases = aliases ( input, complex );
    
    /* Erase self-alias from answer */
    //cell_aliases . erase ( cell_aliases . begin () );
    
    /* Search for a conflict */
    bool conflict = false;
    BOOST_FOREACH ( std::list < GeoCell > & alias, cell_aliases ) {
      /* Check if any of the Cell's in this alias are "on" */
      BOOST_FOREACH ( GeoCell & cell, alias ) {
        if ( cell . node -> data != NULL && reinterpret_cast < Cube_Cells * > ( cell . node -> data ) 
            -> read ( complex . packing_code ( cell . code ) ) ) {
          conflict = true;
          break;
        } /* if */
      } /* BOOST_FOREACH */
      if ( conflict == true ) {
        BOOST_FOREACH ( GeoCell & cell, alias ) {
          if ( cell . node -> data != NULL && reinterpret_cast < Cube_Cells * > ( cell . node -> data ) 
              -> read ( complex . packing_code ( cell . code ) ) ) continue;
          finalize_helper ( cell, complex );
        } /* BOOST_FOREACH */
        return;
      } /* if */
    } /* BOOST_FOREACH */
    
    /* Write the Cell */
    if ( input . node -> data == NULL ) {
      input . node -> data = new Cube_Cells ( complex . dimension () );
      //std::cout << "Giving Cube_Cells at " << input . node -> data << " to leaf " << input . node << "\n";
    }
    //std::cout << "       Writing " << input . code << " to leaf " << input . node << "\n";
    reinterpret_cast < Cube_Cells * > ( input . node -> data ) -> write ( complex . packing_code ( input . code ), true );
    
    /* Recursive call to handle boundaries */
    for ( unsigned int dimension_index = 0; dimension_index < complex . dimension (); ++ dimension_index ) {
      GeoCell cell = input;
      if ( ( ( cell . code >> ( complex . dimension () + dimension_index ) ) & 1 ) == 1  ) { 
        /* The dimension is not reduced. */
        GeoCell lower_cell = cell;
        /* Produce Left Cell */
        lower_cell . code = cell . code & ( ~ ( 1 << ( complex . dimension () + dimension_index ) ) ) & ( ~ ( 1 << dimension_index ) );
        finalize_helper ( lower_cell, complex );
        /* Produce Right Cell */
        lower_cell . code = lower_cell . code | ( 1 << dimension_index );
        finalize_helper ( lower_cell, complex );
      } /* if */
    } /* for */
  } /* Adaptive_Complex_detail::finalize_helper */
  
} /* namespace Adaptive_Complex_detail */

void Adaptive_Complex::Finalize ( void ) {
  using namespace Adaptive_Complex_detail;
  /* Traversing the leaves of the tree and produce an ordered list of them,
     where a leaf precedes another leaf in the list whenever it represents a smaller
     cube */
  std::list < Node * > work_list;
  
  /* Produce the ordered list of leaves */ {
    std::map < int, std::list < Node * >::iterator > position_map;
    Node * leaf = root_;
    while ( become_next_leaf ( leaf ) != root_ ) {
      if ( leaf -> data == NULL ) continue;
      /* Calculate the depth of the leaf */
      Node * temp = leaf;
      int depth = 0;
      while ( ( temp = temp -> parent ) != root_ ) -- depth;      
      /* Insert leaf into list in proper position */
	//  std::cout << "Inserting leaf " << leaf << " into position_map at depth " << depth << "\n";
	  if ( position_map . lower_bound ( depth ) == position_map . end () ) {
        position_map [ depth ] = work_list . insert ( work_list . end (), leaf );
      } else {
		std::list < Node * > :: iterator placement = work_list . insert ( position_map . lower_bound ( depth ) -> second, leaf );
		position_map [ depth ] = placement;
		/* Question: Why doesn't position_map [ depth ] = work_list ... work directly? */
      } /* if-else */
    } /* while */
  } /* scope */
  
  /* Work through list */
  BOOST_FOREACH ( Node * leaf, work_list ) {
    finalize_helper ( GeoCell ( leaf, MASK << dimension_ ), *this );
  } /* BOOST_FOREACH */
  
  /* Initialize container variables */
  /* Initialize end_ */
  size_type end_code = 1;
  for ( unsigned int dim = 0; dim < dimension_; ++ dim ) end_code *= 3;
  Cell end_cell ( root_, end_code );
  end_ = const_iterator ( this, end_cell );
  /* Initialize begin_ */
  Node * leaf = root_;
  become_next_leaf ( leaf );
  Cell begin_cell ( leaf, 0 );
  begin_ [ 0 ] = const_iterator ( this, begin_cell );
  if ( begin_cell . node -> data == NULL || not reinterpret_cast < Adaptive_Complex_detail::Cube_Cells * > ( begin_cell . node -> data ) -> read ( begin_cell . code ) ) {
    ++ begin_ [ 0 ];
  } /* if */
  /* Initialize total_size_ */
  total_size_ = 0;
  /* Iterate through the complex to fill in begin_ and size_ */
  unsigned int dimension = 0;
  for ( const_iterator lookup = begin (); lookup != end (); ++ lookup ) {
    ++ total_size_;
    while ( dimension < lookup . dimension () ) {
      ++ dimension;
      begin_ [ dimension ] = lookup;
    }
    ++ size_ [ dimension ];
  } /* for */
  while ( dimension < dimension_ ) {
    ++ dimension;
    begin_ [ dimension ] = end_;
  } /* while */
  begin_ [ dimension_ + 1 ] = end_;
} /* Adaptive_Complex::Finalize */

Adaptive_Complex::size_type Adaptive_Complex::geometric_code ( const size_type pcode ) const {
  return geometric_code_ [ pcode ];
} /* Adaptive_Complex::geometric_code */

Adaptive_Complex::size_type Adaptive_Complex::packing_code ( const size_type gcode ) const {
  return packing_code_ [ gcode ];
} /* Adaptive_Complex::packing_code */

Adaptive_Complex::size_type Adaptive_Complex::packing_code_begin ( const unsigned int dimension ) const {
  return packing_code_begin_ [ dimension ];
} /* Adaptive_Complex::packing_code_begin */

Adaptive_Complex::size_type Adaptive_Complex::packing_code_end ( const unsigned int dimension ) const {
  return packing_code_begin_ [ dimension + 1 ];
} /* Adaptive_Complex::packing_code_end */


#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */
#endif
