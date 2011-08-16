/* Subcomplex.hpp
 * CHomP project
 * Shaun Harker 3/31/10
 */

#include "algorithms/Morse_Theory.h"

/* * * * * * * * * * * * * *
 * Subcomplex definitions  *
 * * * * * * * * * * * * * */

template < class Cell_Complex > 
std::pair<typename Subcomplex<Cell_Complex>::iterator, bool> Subcomplex<Cell_Complex>::insert ( const value_type & insert_me, bool update ) {
  std::pair < Subcomplex_const_iterator<Cell_Complex>, bool > 
    return_value ( Subcomplex_const_iterator<Cell_Complex> ( this, 
                                                             insert_me . data (), 
                                                             insert_me . dimension () ), 
                   not bitmap_ [ insert_me . data () ] );
  if ( return_value . second ) {
    bitmap_ [ insert_me . data () ] = true;
    unsigned int dimension = insert_me . dimension ();
    if ( update ) {
      /* Update size_ */
      ++ size_ [ dimension ];
      ++ total_size_;
      /* Update begin_ */
      const_iterator iter = return_value . first;
      ++ iter;
      if ( iter == begin_ [ dimension ] ) {
        begin_ [ dimension ] = return_value . first;
      } /* if */
    }
  } /* if */
  return return_value;
} /* Subcomplex<>::insert */

template < class Cell_Complex > 
void Subcomplex<Cell_Complex>::erase ( const iterator & erase_me, bool update ) {
  if ( update ) {
    unsigned int dimension = erase_me . dimension ();
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
      }
    }
  }
  bitmap_ [ erase_me . data_ ] = false;
} /* Subcomplex<>::erase */

template < class Cell_Complex > 
void Subcomplex<Cell_Complex>::erase ( const Cell & erase_me, bool update ) {
  const_iterator it = find ( erase_me );
  if ( it == end () ) return; // or maybe we should complain
  erase ( it, update );
} /* Subcomplex<>::erase */

template < class Cell_Complex > 
void Subcomplex<Cell_Complex>::clear ( void ) {
  for ( unsigned int i = 0; i < bitmap_size_; ++ i )
    bitmap_ [ i ] = false;
} /* Subcomplex<>::clear */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::iterator Subcomplex<Cell_Complex>::find ( const key_type & find_me ) const {
  if ( bitmap_ [ find_me . data () ] == false ) return end_;
  return typename Subcomplex<Cell_Complex>::iterator ( this, find_me . data (), find_me . dimension () );
} /* Subcomplex<>::find */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::iterator Subcomplex<Cell_Complex>::begin ( void ) const {
	return begin_ [ 0 ];  
} /* Subcomplex<>::begin */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::iterator Subcomplex<Cell_Complex>::end ( void ) const {
  return end_;
} /* Subcomplex<>::end */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::size_type Subcomplex<Cell_Complex>::size ( void ) const {
	size_type return_value = 0;
	for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) 
		return_value += size_ [ dimension_index ];
	return return_value; 
} /* Subcomplex<>::size */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::iterator Subcomplex<Cell_Complex>::begin ( unsigned int dimension ) const {
  return begin_ [ dimension ];
} /* Subcomplex<>::begin */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::iterator Subcomplex<Cell_Complex>::end ( unsigned int dimension ) const {
  return begin_ [ dimension + 1 ];
} /* Subcomplex<>::end */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::size_type Subcomplex<Cell_Complex>::size ( unsigned int dimension ) const {
  return size_ [ dimension ];
} /* Subcomplex<>::size */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::Chain Subcomplex<Cell_Complex>::boundary ( const const_iterator & input ) const {
  typename Cell_Complex::Chain super_boundary = super_complex_ -> boundary ( super_complex_ -> lookup ( input . data_ ) );
  return project ( super_boundary );
} /* Subcomplex<>::boundary */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::Chain Subcomplex<Cell_Complex>::coboundary ( const const_iterator & input ) const {
  typename Cell_Complex::Chain super_coboundary = super_complex_ -> coboundary ( super_complex_ -> lookup ( input . data_ ) );
  return project ( super_coboundary );
} /* Subcomplex<>::coboundary */

template < class Cell_Complex > 
unsigned int Subcomplex<Cell_Complex>::dimension ( void ) const {
  return dimension_;
} /* Subcomplex<>::dimension */


/* Index Complex */

template < class Cell_Complex >
void Subcomplex<Cell_Complex>::
index ( void ) {
  lookup_ . resize ( size () + 1 );
  connection_ . resize ( size (), 0 );
  size_type cell_index = 0;
  for ( const_iterator cell_iterator = begin (); cell_iterator != end (); ++ cell_iterator, ++ cell_index ) {
    index_ [ cell_iterator ] = cell_index;
    lookup_ [ cell_index ] = cell_iterator;
  } /* for */
  index_ [ end_ ] = total_size_;
  lookup_ [ total_size_ ] = end_;
  index_begin_ . resize ( dimension_ + 2, 0 );
  size_type sum = 0;
  for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) {
    sum += size_ [ dimension_index ];
    index_begin_ [ dimension_index + 1 ] = sum;
  } /* for */
} /* Subcomplex<Cell_Complex>::index */

template < class Cell_Complex >
typename Subcomplex<Cell_Complex>::size_type 
Subcomplex<Cell_Complex>::
index_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ]; 
} /* Subcomplex<Cell_Complex>::index_begin */ 

template < class Cell_Complex >
typename Subcomplex<Cell_Complex>::size_type 
Subcomplex<Cell_Complex>::
index_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ];
} /* Subcomplex<Cell_Complex>::index_end */

template < class Cell_Complex >
typename Subcomplex<Cell_Complex>::size_type 
Subcomplex<Cell_Complex>::
index ( const const_iterator & lookup ) const {
  return index_ . find ( lookup ) -> second;
} /* Subcomplex<Cell_Complex>::index */

template < class Cell_Complex >
typename Subcomplex<Cell_Complex>::size_type & 
Subcomplex<Cell_Complex>::
index ( const const_iterator & lookup ) {
  return index_ [ lookup ];
} /* Subcomplex<Cell_Complex>::index */

template < class Cell_Complex >
std::vector < Subcomplex_const_iterator<Cell_Complex> > & 
Subcomplex<Cell_Complex>::
lookup ( void ) {
  return lookup_;
} /* Subcomplex<Cell_Complex>::lookup */

template < class Cell_Complex >
const Subcomplex_const_iterator<Cell_Complex> & 
Subcomplex<Cell_Complex>::
lookup ( size_type index ) const {
  return lookup_ [ index ];
} /* Subcomplex<Cell_Complex>::lookup */

template < class Cell_Complex >
Subcomplex_const_iterator<Cell_Complex> & 
Subcomplex<Cell_Complex>::
lookup ( size_type index ) {
  return lookup_ [ index ];
} /* Subcomplex<Cell_Complex>::lookup */

template < class Cell_Complex >
std::vector < int > 
Subcomplex<Cell_Complex>::
count_all_boundaries ( void ) const {
  std::vector < int > number_of_boundaries ( total_size_ );
  for ( const_iterator cell_iterator = begin (); cell_iterator != end_; ++ cell_iterator )
    number_of_boundaries [ index ( cell_iterator ) ] = boundary ( cell_iterator ) . size ();
  return number_of_boundaries;
} /* Subcomplex<Cell_Complex>::count_all_boundaries */

template < class Cell_Complex >
void Subcomplex<Cell_Complex>::
boundary ( std::vector < size_type > & output, const size_type input ) const {
  output . clear ();
  Chain boundary_chain = boundary ( lookup_ [ input ] );
  for ( typename Chain::const_iterator term_iterator = boundary_chain . begin (); 
       term_iterator != boundary_chain . end (); ++ term_iterator ) {
    output . push_back ( index ( term_iterator -> first ) );
  } /* for */
} /* Subcomplex<Cell_Complex>::boundary */

template < class Cell_Complex >
void Subcomplex<Cell_Complex>::
coboundary ( std::vector < size_type > & output, const size_type input ) const {
  output . clear ();
  Chain coboundary_chain = coboundary ( lookup_ [ input ] );
  for ( typename Chain::const_iterator term_iterator = coboundary_chain . begin (); 
       term_iterator != coboundary_chain . end (); ++ term_iterator ) {
    output . push_back ( index ( term_iterator -> first ) );
  } /* for */
} /* Subcomplex<Cell_Complex>::coboundary */

template < class Cell_Complex >
void Subcomplex<Cell_Complex>::
boundary ( std::vector < std::pair< size_type, Ring > > & output, const size_type input ) const {
  output . clear ();
  Chain boundary_chain = boundary ( lookup_ [ input ] );
  for ( typename Chain::const_iterator term_iterator = boundary_chain . begin (); 
       term_iterator != boundary_chain. end (); ++ term_iterator ) {
    output . push_back ( std::pair < size_type, Ring > ( index ( term_iterator -> first ), 
                                                        term_iterator -> second ) );
  } /* for */
} /* Subcomplex<Cell_Complex>::boundary */

template < class Cell_Complex >
void Subcomplex<Cell_Complex>::
coboundary ( std::vector < std::pair< size_type, Ring > > & output, const size_type input ) const {
  output . clear ();
  Chain coboundary_chain = coboundary ( lookup_ [ input ] );
  for ( typename Chain::const_iterator term_iterator = coboundary_chain . begin (); 
       term_iterator != coboundary_chain . end (); ++ term_iterator ) {
    output . push_back ( std::pair < size_type, Ring > ( index ( term_iterator -> first ), 
                                                        term_iterator -> second ) );
  } /* for */
} /* Subcomplex<Cell_Complex>::coboundary */

/* Decomposable Complex */

template < class Cell_Complex >
void Subcomplex<Cell_Complex>::
decompose ( void ) {
  index (); 
  king_count_ = morse::decompose ( *this );
} /*  Subcomplex<Cell_Complex>::decompose */

template < class Cell_Complex >
char Subcomplex<Cell_Complex>::
type ( size_type index, unsigned int dimension ) const {
  if ( index < index_begin_ [ dimension ] + king_count_ [ dimension + 1 ] ) return 0; /* QUEEN */
  if ( index < index_begin_ [ dimension + 1 ] - king_count_ [ dimension ] ) return 1; /* ACE */
  return 2; /* KING */
} /* Subcomplex<Cell_Complex>::type */

template < class Cell_Complex >
typename Subcomplex<Cell_Complex>::size_type 
Subcomplex<Cell_Complex>::
mate ( size_type queen_index, unsigned int dimension ) const {
  return index_begin_ [ dimension ] + index_begin_ [ dimension + 2 ] - queen_index - 1;
} /* Subcomplex<Cell_Complex>::mate */

template < class Cell_Complex >
const typename Subcomplex<Cell_Complex>::Ring & 
Subcomplex<Cell_Complex>::
connection ( size_type queen_index ) const {
  return connection_ [ queen_index ];
} /* Subcomplex<Cell_Complex>::connection */

template < class Cell_Complex >
typename Subcomplex<Cell_Complex>::Ring & 
Subcomplex<Cell_Complex>::
connection ( size_type queen_index ) {
  return connection_ [ queen_index ];
} /* Subcomplex<Cell_Complex>::connection */

template < class Cell_Complex >
typename Subcomplex<Cell_Complex>::size_type 
Subcomplex<Cell_Complex>::
ace_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ] + king_count_ [ dimension + 1 ];
} /* Subcomplex<Cell_Complex>::ace_begin */

template < class Cell_Complex >
typename Subcomplex<Cell_Complex>::size_type 
Subcomplex<Cell_Complex>::
ace_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ] - king_count_ [ dimension ];
} /* Subcomplex<Cell_Complex>::ace_end */

template < class Cell_Complex > 
Subcomplex<Cell_Complex>::
Subcomplex ( void ) {
} /* Subcomplex<>::Subcomplex */

template < class Cell_Complex > 
Subcomplex<Cell_Complex>::
Subcomplex ( const Cell_Complex * super_complex ) : super_complex_(super_complex) {
  dimension_ = super_complex -> dimension ();
  bitmap_size_ = super_complex_ -> size ();
  end_ = const_iterator ( this, bitmap_size_ , dimension_ );
  begin_ . resize ( dimension_ + 2, end_ );
  size_ . resize ( dimension_ + 1, 0 );
  total_size_ = 0;
  bitmap_ . resize ( bitmap_size_, false );
} /* Subcomplex<>::Subcomplex */

template < class Cell_Complex > 
void Subcomplex<Cell_Complex>::
construct ( const Cell_Complex * super_complex ) {
  super_complex_ = super_complex;
  dimension_ = super_complex -> dimension ();
  bitmap_size_ = super_complex_ -> size ();
  end_ = const_iterator ( this, bitmap_size_ , dimension_ );
  begin_ . resize ( dimension_ + 2, end_ );
  size_ . resize ( dimension_ + 1, 0 );
  total_size_ = 0;
  bitmap_ . resize ( bitmap_size_, false );
} /* Subcomplex<>::Subcomplex */

template < class Cell_Complex > 
void Subcomplex<Cell_Complex>::finalize ( void ) {
  /* Gives correct values to total_size_, begin_, and size_ */
  unsigned int dimension = 0;
  size_ [ 0 ] = 0;
  total_size_ = 0;
  begin_ [ 0 ] = const_iterator ( this, 0, 0 );
  if ( not bitmap_ [ 0 ] ) {
    ++ begin_ [ 0 ];
  } /* if */
  for ( const_iterator lookup = begin (); lookup != end (); ++ lookup ) { 
    // warning: ++ doesn't work properly if begin is incorrect; it gets the dimension wrong
    while ( super_complex_ -> index_begin ( dimension + 1 ) <= lookup . data () ) {
      ++ dimension;
      lookup . dimension_ = dimension; // because ++ didn't increment dimension
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
  index ();
} /* Subcomplex<>::finalize */

#if 0
// OLD constructor, used to make full subcomplex rather than empty one
// and also used const ref instead of pointer
template < class Cell_Complex > 
Subcomplex<Cell_Complex>::
Subcomplex ( const Cell_Complex & super_complex ) : super_complex_(super_complex) {
  dimension_ = super_complex . dimension ();
  bitmap_size_ = super_complex_ -> size ();
  end_ = const_iterator ( this, bitmap_size_ , dimension_ );
  begin_ . resize ( dimension_ + 2, end_ );
  size_ . resize ( dimension_ + 1, 0 );
  total_size_ = 0;
  bitmap_ . resize ( bitmap_size_, true );
  /* Load in begin_, size_, total_size_ data. */
  for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) {
    begin_ [ dimension_index ] = Subcomplex_const_iterator < Cell_Complex > 
      ( this, super_complex_ -> index ( super_complex_ -> begin ( dimension_index ) ),
       super_complex_ -> begin ( dimension_index ) . dimension () );
    total_size_ += size_ [ dimension_index ] = super_complex_ -> size ( dimension_index );
  }
} /* Subcomplex<>::Subcomplex */
#endif
template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::Chain Subcomplex<Cell_Complex>::project ( const typename Cell_Complex::Chain & project_me ) const {
  Chain return_value;
  //unsigned int dimension = project_me . dimension (); TODO make this work instead of calling dimension on all the terms
  for ( typename Cell_Complex::Chain::const_iterator chain_term = project_me . begin (); chain_term != project_me . end (); ++ chain_term ) {
    unsigned long indx = super_complex_ -> index ( chain_term -> first );
    if ( bitmap_ [ indx ] ) return_value += 
      typename Chain::Chain_Term ( Subcomplex_const_iterator<Cell_Complex> ( this, indx, 
                                                               chain_term -> first . dimension () ), 
                                   chain_term -> second );
  } /* for */
  return return_value;
} /* Subcomplex<>::project */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::Chain Subcomplex<Cell_Complex>::project ( const Chain & project_me ) const {
  Chain return_value;
  //unsigned int dimension = project_me . dimension (); TODO make this work instead of calling dimension on all the terms
  for ( typename Chain::const_iterator chain_term = project_me . begin (); chain_term != project_me . end (); ++ chain_term ) {
    unsigned long indx = chain_term -> first . data ();
    if ( bitmap_ [ indx ] ) return_value += 
      typename Chain::Chain_Term ( Subcomplex_const_iterator<Cell_Complex> ( this, indx, 
                                                                            chain_term -> first . dimension () ), 
                                  chain_term -> second );
  } /* for */
  return return_value;
} /* Subcomplex<>::project */

template < class Cell_Complex > 
typename Cell_Complex::const_iterator Subcomplex<Cell_Complex>::include ( const Cell & include_me ) const {
  return super_complex_ -> lookup ( include_me . data () ); 
}

template < class Cell_Complex > 
typename Cell_Complex::const_iterator Subcomplex<Cell_Complex>::include ( const const_iterator & include_me ) const {
  return super_complex_ -> lookup ( include_me . data_ );
} /* Subcomplex<>::include */

template < class Cell_Complex > 
typename Cell_Complex::Chain Subcomplex<Cell_Complex>::include ( const Chain & include_me ) const {
  typename Cell_Complex::Chain return_value;
  for ( typename Chain::const_iterator chain_term = include_me . begin (); chain_term != include_me . end (); ++ chain_term )
    return_value += typename Cell_Complex::Chain::Chain_Term 
      ( super_complex_ -> lookup ( chain_term -> first . data_ ), chain_term -> second );
  return return_value;
} /* Subcomplex<>::include */

/* * * * * * * * * * * * * * * * * * * *
 * Subcomplex_const_iterator definitions  *
 * * * * * * * * * * * * * * * * * * * */

template < class Cell_Complex > 
Subcomplex_const_iterator<Cell_Complex>::Subcomplex_const_iterator ( void ) : container_(NULL), dimension_(0) { 
} /* Subcomplex_const_iterator::Subcomplex_const_iterator */

template < class Cell_Complex > 
Subcomplex_const_iterator<Cell_Complex>::Subcomplex_const_iterator ( const Subcomplex<Cell_Complex> * const container, const unsigned long data ) : 
container_(container), data_(data) {
  /* We have to determine the dimension. */
  dimension_ = 0;
  while ( dimension_ < container_ -> dimension_ && 
          data_ >= container_ -> begin_ [ dimension_ + 1 ] . data_ ) 
    ++ dimension_;
} /* Subcomplex_const_iterator::Subcomplex_const_iterator */

template < class Cell_Complex > 
Subcomplex_const_iterator<Cell_Complex>::Subcomplex_const_iterator ( const Subcomplex<Cell_Complex> * const container, const unsigned long data, const unsigned int dimension ) : 
container_(container), data_(data), dimension_(dimension) {
} /* Subcomplex_const_iterator::Subcomplex_const_iterator */

template < class Cell_Complex > 
typename Subcomplex<Cell_Complex>::value_type Subcomplex_const_iterator<Cell_Complex>::operator * ( void ) const {
	return Default_Cell ( data_, dimension_ ); 
} /* Subcomplex_const_iterator::operator * */

template < class Cell_Complex > 
Subcomplex_const_iterator<Cell_Complex> & Subcomplex_const_iterator<Cell_Complex>::operator ++ ( void ) {
	++ data_;
  while ( data_ < container_ -> bitmap_size_ && container_ -> bitmap_ [ data_ ] == false ) ++ data_;
  while ( dimension_ < container_ -> dimension_ && data_ >= container_ -> begin_ [ dimension_ + 1 ] . data_ ) ++ dimension_; 
  return *this;
} /* Subcomplex_const_iterator::operator ++ */

template < class Cell_Complex > 
bool Subcomplex_const_iterator<Cell_Complex>::operator != ( const Subcomplex_const_iterator & right_hand_side ) const {
	return data_ != right_hand_side . data_;
} /* Subcomplex_const_iterator::operator != */

template < class Cell_Complex > 
bool Subcomplex_const_iterator<Cell_Complex>::operator == ( const Subcomplex_const_iterator & right_hand_side ) const {
	return data_ == right_hand_side . data_;
} /* Subcomplex_const_iterator::operator == */

template < class Cell_Complex > 
bool Subcomplex_const_iterator<Cell_Complex>::operator < ( const Subcomplex_const_iterator & right_hand_side ) const {
  if ( dimension_ == right_hand_side . dimension_ ) 
    return data_ < right_hand_side . data_;
  else
    return dimension_ < right_hand_side . dimension_;
} /* Subcomplex_const_iterator::operator < */

template < class Cell_Complex > 
unsigned int Subcomplex_const_iterator<Cell_Complex>::dimension () const {
  return dimension_;
} /* Subcomplex_const_iterator::dimension */

template < class Cell_Complex > 
const Subcomplex<Cell_Complex> & Subcomplex_const_iterator<Cell_Complex>::container () const {
  return container_;
} /* Subcomplex_const_iterator::container */

template < class Cell_Complex > 
unsigned long Subcomplex_const_iterator<Cell_Complex>::data () const {
  return data_;
} /* Subcomplex_const_iterator::data */

template < class Cell_Complex >
std::ostream & operator << ( std::ostream & output_stream, const Subcomplex_const_iterator<Cell_Complex> & print_me) {
  output_stream << * print_me;
  return output_stream;
} /* operator << */
