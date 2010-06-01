/*
 *  Abstract_Complex.hpp
 *  
 *  Created by Shaun Harker on 10/6/09.
 *
 */

#include "algorithms/Morse_Theory.h" /* for morse::decompose */

namespace std {
  /* This is technically illegal, since it is a function overload and not
   a specialization. But there is no other choice, since the standard doesn't allow
   partial specializations for functions. (As of May 8th, 2010) */
  template < class Cell_Type >
  void swap ( Abstract_Complex < Cell_Type > & a, Abstract_Complex < Cell_Type > & b ) {
    a . swap ( b );
  } /* swap */
  
} /* namespace std */


/* * * * * * * * * * * * * * * *
 * Abstract_Chain definitions  *
 * * * * * * * * * * * * * * * */

template < class Cell_Type >
Abstract_Chain<Cell_Type>::Abstract_Chain ( void  ) {
}

template < class Cell_Type >
Abstract_Chain<Cell_Type>::Abstract_Chain ( const complex_type & container ) 
: Chain_Archetype < std::map < Abstract_const_iterator<Cell_Type>, Default_Ring > > ( container ) {
}

/* * * * * * * * * * * * * * * * *
 * Abstract_Complex definitions  *
 * * * * * * * * * * * * * * * * */

template < class Cell_Type >
std::pair<typename Abstract_Complex<Cell_Type>::iterator, bool> Abstract_Complex<Cell_Type>::insert ( const value_type & insert_me ) {
  std::pair<typename std::set<Cell_Type>::iterator, bool> insert_value = cells_ . insert ( insert_me );
  std::pair<typename Abstract_Complex<Cell_Type>::iterator, bool> return_value ( typename Abstract_Complex<Cell_Type>::iterator ( this, insert_value . first, insert_me . dimension () ), insert_value . second );
  unsigned int dimension = insert_me . dimension ();
  if ( return_value . second ) { /* It really was inserted, wasn't already there. */
    /* Increment size */
    ++ size_ [ dimension ];
    ++ total_size_;
    /* Update begin_ */
    const_iterator next = return_value . first;
    ++ next;
    while ( begin_ [ dimension ] == next ) {
      begin_ [ dimension ] = return_value . first;
      if ( dimension == 0 ) break;
      -- dimension;
    } /* if */
  } /* if */
  return return_value;
} /* Abstract_Complex<Cell_Type>::insert */

template < class Cell_Type >
void Abstract_Complex<Cell_Type>::erase ( iterator erase_me ) {
  /* TODO : NOT YET IMPLEMENTED. LOOK AT COBOUNDARY, BOUNDARY, FIX THE INFO, THEN ERASE. */
} /* Abstract_Complex<Cell_Type>::erase */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::iterator Abstract_Complex<Cell_Type>::find ( const key_type & find_me ) {
  typename Abstract_Complex<Cell_Type>::iterator return_value ( this, cells_ . find ( find_me ), find_me . dimension () );
  if ( return_value . data_ == cells_ . end () ) return end_;
  return return_value;
} /* Abstract_Complex<Cell_Type>::find */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::const_iterator Abstract_Complex<Cell_Type>::find ( const key_type & find_me ) const {
  typename Abstract_Complex<Cell_Type>::iterator return_value ( this, cells_ . find ( find_me ), find_me . dimension () );
  if ( return_value . data_ == cells_ . end () ) return end_;
  return return_value;
} /* Abstract_Complex<Cell_Type>::find */

template < class Cell_Type > 
typename Abstract_Complex<Cell_Type>::iterator Abstract_Complex<Cell_Type>::begin ( void ) const {
	return begin_ [ 0 ];  
} /* Abstract_Complex<Cell_Type>::begin */

template < class Cell_Type > 
typename Abstract_Complex<Cell_Type>::iterator Abstract_Complex<Cell_Type>::end ( void ) const {
  return end_;
} /* Abstract_Complex<Cell_Type>::end */

template < class Cell_Type > 
typename Abstract_Complex<Cell_Type>::size_type Abstract_Complex<Cell_Type>::size ( void ) const {
	return total_size_; 
} /* Abstract_Complex<Cell_Type>::size */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::iterator Abstract_Complex<Cell_Type>::begin ( unsigned int dimension ) const {
  return begin_ [ dimension ];
} /* Abstract_Complex<Cell_Type>::begin */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::iterator Abstract_Complex<Cell_Type>::end ( unsigned int dimension ) const {
  return begin_ [ dimension + 1 ];
} /* Abstract_Complex<Cell_Type>::end */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::size_type Abstract_Complex<Cell_Type>::size ( unsigned int dimension ) const {
  return size_ [ dimension ];
} /* Abstract_Complex<Cell_Type>::size */

template < class Cell_Type >
Abstract_Chain<Cell_Type> & Abstract_Complex<Cell_Type>::boundary ( const iterator & input ) {
  return boundary_ [ *input ];
} /* Abstract_Complex<Cell_Type>::boundary */

template < class Cell_Type >
const Abstract_Chain<Cell_Type> & Abstract_Complex<Cell_Type>::boundary ( const const_iterator & input ) const {
  return boundary_ . find ( *input ) -> second;
} /* Abstract_Complex<Cell_Type>::boundary */

template < class Cell_Type >
Abstract_Chain<Cell_Type> & Abstract_Complex<Cell_Type>::coboundary ( const iterator & input ) {
  return coboundary_ [ *input ];
} /* Abstract_Complex<Cell_Type>::coboundary */

template < class Cell_Type >
const Abstract_Chain<Cell_Type> & Abstract_Complex<Cell_Type>::coboundary ( const const_iterator & input ) const {
  return coboundary_ . find ( *input ) -> second;
} /* Abstract_Complex<Cell_Type>::coboundary */

template < class Cell_Type >
unsigned int Abstract_Complex<Cell_Type>::dimension ( void ) const {
  return dimension_;
} /* Abstract_Complex<Cell_Type>::dimension */

template < class Cell_Type >
void Abstract_Complex<Cell_Type>::index ( void ) {
  lookup_ . resize ( size () + 1 );
  connection_ . resize ( size (), 0 );
  size_type cell_index = 0;
  for ( const_iterator cell_iterator = begin (); cell_iterator != end (); ++ cell_iterator, ++ cell_index ) {
    index_ [ cell_iterator ] = cell_index;
    //if ( find ( * cell_iterator ) != cell_iterator ) std::cout << "trouble in river city \n";
    //std::cout << cell_iterator . dimension_ << " ?= " << find ( * cell_iterator ) . dimension_ << "\n";
    //std::cout << "index_ [ " << * cell_iterator << "] = " << index_ [ cell_iterator ] << " = " << cell_index << " => " << * find ( * cell_iterator ) << "\n";
    lookup_ [ cell_index ] = cell_iterator;
    //std::cout << "index_ [ " << * lookup_ [ cell_index ] << "] = " << cell_index << 
    //" = " << index ( find ( * lookup_ [ cell_index ] ) ) << "\n";
  } /* for */
  index_ [ end_ ] = total_size_;
  lookup_ [ total_size_ ] = end_;
  index_begin_ . resize ( dimension_ + 2, 0 );
  size_type sum = 0;
  for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) {
    sum += size_ [ dimension_index ];
    index_begin_ [ dimension_index + 1 ] = sum;
  } /* for */
  /* DEBUG */
  //for ( size_type cell_index = 0; cell_index < total_size_; ++ cell_index ) {
  //  std::cout << "index_ [ " << * lookup_ [ cell_index ] << "] = " << cell_index << 
  //  " = " << index ( find ( * lookup_ [ cell_index ] ) ) << "\n";
  //}
  //std::cout << "\n";
} /* Abstract_Complex<Cell_Type>::index */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::size_type Abstract_Complex<Cell_Type>::index_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ]; 
} /* Abstract_Complex<Cell_Type>::index_begin */ 

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::size_type Abstract_Complex<Cell_Type>::index_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ];
} /* Abstract_Complex<Cell_Type>::index_end */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::size_type Abstract_Complex<Cell_Type>::index ( const const_iterator & lookup ) const {
  return index_ . find ( lookup ) -> second;
} /* Abstract_Complex<Cell_Type>::index */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::size_type & Abstract_Complex<Cell_Type>::index ( const const_iterator & lookup ) {
  return index_ [ lookup ];
} /* Abstract_Complex<Cell_Type>::index */

template < class Cell_Type >
std::vector < Abstract_const_iterator<Cell_Type> > & Abstract_Complex<Cell_Type>::lookup ( void ) {
  return lookup_;
} /* Abstract_Complex<Cell_Type>::lookup */

template < class Cell_Type >
const Abstract_const_iterator<Cell_Type> & Abstract_Complex<Cell_Type>::lookup ( size_type index ) const {
  return lookup_ [ index ];
} /* Abstract_Complex<Cell_Type>::lookup */

template < class Cell_Type >
Abstract_const_iterator<Cell_Type> & Abstract_Complex<Cell_Type>::lookup ( size_type index ) {
  return lookup_ [ index ];
} /* Abstract_Complex<Cell_Type>::lookup */

template < class Cell_Type >
std::vector < int > Abstract_Complex<Cell_Type>::count_all_boundaries ( void ) const {
  std::vector < int > number_of_boundaries ( total_size_ );
  for ( const_iterator cell_iterator = begin_ [ 0 ]; cell_iterator != end_; ++ cell_iterator )
    number_of_boundaries [ index ( cell_iterator ) ] = boundary ( cell_iterator ) . size ();
  return number_of_boundaries;
} /* Abstract_Complex<Cell_Type>::count_all_boundaries */

template < class Cell_Type >
void Abstract_Complex<Cell_Type>::boundary ( std::vector < size_type > & output, const size_type input ) const {
  output . clear ();
  Chain boundary_chain = boundary ( lookup_ [ input ] );
  for ( typename Abstract_Chain<Cell_Type>::const_iterator term_iterator = boundary_chain . begin (); 
        term_iterator != boundary_chain . end (); ++ term_iterator ) {
    output . push_back ( index ( term_iterator -> first ) );
  }
} /* Abstract_Complex<Cell_Type>::boundary */

template < class Cell_Type >
void Abstract_Complex<Cell_Type>::coboundary ( std::vector < size_type > & output, const size_type input ) const {
  output . clear ();
  Chain coboundary_chain = coboundary ( lookup_ [ input ] );
  for ( typename Abstract_Chain<Cell_Type>::const_iterator term_iterator = coboundary_chain . begin (); 
        term_iterator != coboundary_chain . end (); ++ term_iterator ) {
    output . push_back ( index ( term_iterator -> first ) );
  }
} /* Abstract_Complex<Cell_Type>::coboundary */

template < class Cell_Type >
void Abstract_Complex<Cell_Type>::boundary ( std::vector < std::pair< size_type, Ring > > & output, const size_type input ) const {
  output . clear ();
  Chain boundary_chain = boundary ( lookup_ [ input ] );
  //std::cout << " bd ( " << input << ") = " << boundary_chain << "\n";
  for ( typename Abstract_Chain<Cell_Type>::const_iterator term_iterator = boundary_chain . begin (); 
       term_iterator != boundary_chain. end (); ++ term_iterator ) {
    output . push_back ( std::pair < size_type, Ring > ( index ( term_iterator -> first ), 
                                                             term_iterator -> second ) );
    //std::cout << * term_iterator -> first << "\n";
    //std::cout << index ( term_iterator -> first ) << "\n ";
    //std::cout << index ( find ( * term_iterator -> first ) ) << " ?\n";
  }

} /* Abstract_Complex<Cell_Type>::boundary */

template < class Cell_Type >
void Abstract_Complex<Cell_Type>::coboundary ( std::vector < std::pair< size_type, Ring > > & output, const size_type input ) const {
  output . clear ();
  Chain coboundary_chain = coboundary ( lookup_ [ input ] );
  for ( typename Abstract_Chain<Cell_Type>::const_iterator term_iterator = coboundary_chain . begin (); 
       term_iterator != coboundary_chain . end (); ++ term_iterator ) {
    output . push_back ( std::pair < size_type, Ring > ( index ( term_iterator -> first ), 
                                                             term_iterator -> second ) );
  }
} /* Abstract_Complex<Cell_Type>::coboundary */

template < class Cell_Type >
void Abstract_Complex<Cell_Type>::decompose ( void ) {
  index (); 
  king_count_ = morse::decompose ( *this );
} /*  Abstract_Complex<Cell_Type>::decompose */

template < class Cell_Type >
char Abstract_Complex<Cell_Type>::type ( size_type index, unsigned int dimension ) const {
  if ( index < index_begin_ [ dimension ] + king_count_ [ dimension + 1 ] ) return 0; /* QUEEN */
  if ( index < index_begin_ [ dimension + 1 ] - king_count_ [ dimension ] ) return 1; /* ACE */
  return 2; /* KING */
} /* Abstract_Complex<Cell_Type>::type */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::size_type Abstract_Complex<Cell_Type>::mate ( size_type queen_index, unsigned int dimension ) const {
  return index_begin_ [ dimension ] + index_begin_ [ dimension + 2 ] - queen_index - 1;
} /* Abstract_Complex<Cell_Type>::mate */

template < class Cell_Type >
const typename Abstract_Complex<Cell_Type>::Ring & Abstract_Complex<Cell_Type>::connection ( size_type queen_index ) const {
  return connection_ [ queen_index ];
} /* Abstract_Complex<Cell_Type>::connection */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::Ring & Abstract_Complex<Cell_Type>::connection ( size_type queen_index ) {
  return connection_ [ queen_index ];
} /* Abstract_Complex<Cell_Type>::connection */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::size_type Abstract_Complex<Cell_Type>::ace_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ] + king_count_ [ dimension + 1 ];
} /* Abstract_Complex<Cell_Type>::ace_begin */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::size_type Abstract_Complex<Cell_Type>::ace_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ] - king_count_ [ dimension ];
} /* Abstract_Complex<Cell_Type>::ace_end */

template < class Cell_Type > 
void Abstract_Complex<Cell_Type>::generate_coboundary_information ( void ) {
  for ( const_iterator cell_iterator = begin (); cell_iterator != end (); ++ cell_iterator)
    coboundary_ [ * cell_iterator ]; /* Put in empty chains! */
  for ( const_iterator cell_iterator = begin (); cell_iterator != end (); ++ cell_iterator) { 
    for ( typename Abstract_Chain<Cell_Type>::const_iterator term_iterator = boundary_ [ * cell_iterator ] . begin (); 
         term_iterator != boundary_ [ * cell_iterator ] . end (); ++ term_iterator ) {
      coboundary_ [ * term_iterator -> first ] += std::pair < const_iterator, Ring > ( cell_iterator, 
                                                         term_iterator -> second );
    } /* for */
  } /* for */
} /* Abstract_Complex<Cell_Type>::generate_coboundary_information */

template < class Cell_Type >
Abstract_Complex<Cell_Type>::Abstract_Complex ( void ) {
} /* Abstract_Complex<Cell_Type>::Abstract_Complex */

template < class Cell_Type >
Abstract_Complex<Cell_Type>::Abstract_Complex ( const Abstract_Complex & copy_me ) {
  /* Shallow copies that work */
  cells_ = copy_me . cells_;
  dimension_ = copy_me . dimension_;
  size_ = copy_me . size_;
  total_size_ = copy_me . total_size_;
  boundary_ = copy_me . boundary_;
  coboundary_ = copy_me . coboundary_;
  index_begin_ = copy_me . index_begin_;
  connection_ = copy_me . connection_;
  king_count_ = copy_me . king_count_;
  /* Copy begin_, end_ */
  end_ = const_iterator ( this, cells_ . end (), dimension_ + 1 );
  begin_ . resize ( dimension_ + 2, end_ );
  for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) { 
    if ( copy_me . begin_ [ dimension_index ] == copy_me . end_ ) break;
    begin_ [ dimension_index ] = find ( * copy_me . begin_ [ dimension_index ] );
  } /* for */
  /* Copy the indexing information if it exists */
  if ( not copy_me . index_ . empty () ) {
    lookup_ . resize ( total_size_ + 1 );
    const_iterator copy_iterator = copy_me . begin ();
    for ( const_iterator cell_iterator = begin (); cell_iterator != end (); ++ cell_iterator, ++ copy_iterator ) { 
      size_type cell_index = index_ [ copy_iterator ];
      index_ [ cell_iterator ] = cell_index;
      lookup_ [ cell_index ] = cell_iterator;
    } /* for */
    index_ [ end_ ] = total_size_;
    lookup_ [ total_size_ ] = end_;
  } /* if */
} /* Abstract_Complex<Cell_Type>::Abstract_Complex */

template < class Cell_Type >
Abstract_Complex<Cell_Type>::Abstract_Complex ( unsigned int dimension ) : dimension_(dimension) {
  end_ = const_iterator ( this, cells_ . end (), dimension_ + 1 );
  begin_ . resize ( dimension_ + 2, end_ );
  size_ . resize ( dimension_ + 1, 0 );
  total_size_ = 0;
} /* Abstract_Complex<Cell_Type>::Abstract_Complex */

template < class Cell_Type >
void Abstract_Complex<Cell_Type>::constructor ( unsigned int dimension ) {
  dimension_ = dimension;
  end_ = const_iterator ( this, typename std::set<Cell>::const_iterator (), dimension_ + 1 );
  /* Remark. since cells_ . end () is changing around when we insert things, we use the big dimension.
   This is too bad, since this means the comparisons need to check the dimension as well. */
  begin_ . resize ( dimension_ + 2, end_ );
  size_ . resize ( dimension_ + 1, 0 );
  total_size_ = 0;
} /* Abstract_Complex<Cell_Type>::constructor */

template < class Cell_Type >
typename Abstract_Complex<Cell_Type>::size_type Abstract_Complex<Cell_Type>::mass ( void ) {
  size_type sum = 0;
  for ( const_iterator cell_iterator = begin (); cell_iterator != end (); ++ cell_iterator) { 
    sum += boundary ( cell_iterator ) . size ();
  } /* for */
  return sum;
} /* Abstract_Complex<Cell_Type>::mass */

template < class Cell_Type >
void Abstract_Complex<Cell_Type>::swap ( Abstract_Complex < Cell_Type > & b ) {
  std::swap ( cells_, b . cells_ );
  std::swap ( begin_, b . begin_ );
  std::swap ( end_, b . end_ );
  std::swap ( size_, b. size_ );
  std::swap ( total_size_, b . total_size_);
  std::swap ( boundary_, b . boundary_ );
  std::swap ( coboundary_, b . coboundary_ );
  std::swap ( dimension_, b . dimension_ );
  std::swap ( index_, b . index_ );
  std::swap ( lookup_, b . lookup_ );
  std::swap ( index_begin_, b . index_begin_ );
  std::swap ( connection_, b . connection_ );
  std::swap ( king_count_, b . king_count_);
} /* Abstract_Complex<Cell_Type>::swap */

/* * * * * * * * * * * * * * * * * * * *
 * Abstract_const_iterator definitions  *
 * * * * * * * * * * * * * * * * * * * */

template < class Cell_Type > 
Abstract_const_iterator<Cell_Type>::Abstract_const_iterator ( void ) : container_(NULL), dimension_(0) { 
} /* Abstract_const_iterator::Abstract_const_iterator */

template < class Cell_Type > 
Abstract_const_iterator<Cell_Type>::Abstract_const_iterator ( const Abstract_Complex<Cell_Type> * const container, typename std::set<Cell_Type>::const_iterator data, const unsigned int dimension ) : 
container_(container), data_(data), dimension_(dimension) {
} /* Abstract_const_iterator::Abstract_const_iterator */

template < class Cell_Type > 
Cell_Type Abstract_const_iterator<Cell_Type>::operator * ( void ) const {
	return * data_; 
} /* Abstract_const_iterator::operator * */

template < class Cell_Type > 
const Cell_Type * Abstract_const_iterator<Cell_Type>::operator -> ( void ) const {
  return data_ . operator -> ();
} /* Abstract_const_iterator operator -> */

template < class Cell_Type > 
Abstract_const_iterator<Cell_Type> & Abstract_const_iterator<Cell_Type>::operator ++ ( void ) {
	++ data_;
  if ( data_ == container_ -> cells_ . end () ) {
    //std::cout << "reached end.\n";
    return *this = container_ -> end_;
  }
  while ( dimension_ < container_ -> dimension_ && data_ == container_ -> begin_ [ dimension_ + 1 ] . data_ ) ++ dimension_;
  return *this;
} /* Abstract_const_iterator::operator ++ */

template < class Cell_Type > 
bool Abstract_const_iterator<Cell_Type>::operator != ( const Abstract_const_iterator & right_hand_side ) const {
	if ( dimension_ != right_hand_side . dimension_ ) return true;
  return data_ != right_hand_side . data_;
} /* Abstract_const_iterator::operator != */

template < class Cell_Type > 
bool Abstract_const_iterator<Cell_Type>::operator == ( const Abstract_const_iterator & right_hand_side ) const {
  if ( dimension_ != right_hand_side . dimension_ ) return false;
  return data_ == right_hand_side . data_;
} /* Abstract_const_iterator::operator == */

template < class Cell_Type > 
bool Abstract_const_iterator<Cell_Type>::operator < ( const Abstract_const_iterator & right_hand_side ) const {
  if ( dimension_ == right_hand_side . dimension_ ) 
    return * reinterpret_cast < const unsigned long * > ( & data_ ) < 
    * reinterpret_cast < const unsigned long * > ( & right_hand_side . data_ );
  else
    return dimension_ < right_hand_side . dimension_;
} /* Abstract_const_iterator::operator < */

template < class Cell_Type > 
unsigned int Abstract_const_iterator<Cell_Type>::dimension () const {
  return dimension_;
} /* Abstract_const_iterator::dimension */

template < class Cell_Type > 
const Abstract_Complex<Cell_Type> & Abstract_const_iterator<Cell_Type>::container () const {
  return container_;
} /* Abstract_const_iterator::container */
