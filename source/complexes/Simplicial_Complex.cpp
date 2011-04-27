/*
 *  Simplicial_Complex.cpp
 *  chomp-rutgers
 *
 *  Created by Shaun Harker on 4/26/11.
 *
 */

#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>

#include "complexes/Simplicial_Complex.h"
#include "algorithms/Morse_Theory.h"

/* * * * * * * * * * * * * * * * * * * *
 * Simplicial_const_iterator definitions  *
 * * * * * * * * * * * * * * * * * * * */

Simplicial_const_iterator::Simplicial_const_iterator ( void ) { 
} /* Simplicial_const_iterator::Simplicial_const_iterator */

Simplicial_const_iterator::Simplicial_const_iterator ( const Simplicial_Complex * const referral ) :
container_ ( referral ) {}

Simplicial_const_iterator::Simplicial_const_iterator (const Simplicial_Complex * const referral, 
                                                      const unsigned long & handle ) 
: container_ ( referral ), handle_ ( handle ) {}

Simplicial_Complex::value_type Simplicial_const_iterator::operator * ( void ) const {
  Simplex return_value;
  unsigned long temp_handle = handle_;
  while ( temp_handle != 0 ) {
    size_type v = container_ -> smallest_vertex_ . find ( temp_handle ) -> second;
    return_value . push_back ( v );
    temp_handle -= container_ -> vertex_hashes_ . find ( v ) -> second;
  }
	return return_value;
} /* Simplicial_const_iterator::operator * */

Simplicial_const_iterator & Simplicial_const_iterator::operator ++ ( void ) {
  unsigned int dim = container_ -> handle_to_fixed_index_ . find ( handle_ ) -> second . first;
  size_type fixed_index = container_ -> handle_to_fixed_index_ . find ( handle_ ) -> second . second;
  if ( container_ -> fixed_index_to_handle_ [ dim ] . size () == fixed_index + 1 ) {
    *this = container_ -> end ( dim );
  } else {
    handle_ = container_ -> fixed_index_to_handle_ [ dim ] [ fixed_index + 1 ];
  }
  return *this;
} /* Simplicial_const_iterator::operator ++ */

bool Simplicial_const_iterator::operator != ( const Simplicial_const_iterator & right_hand_side ) const {
  return handle_ != right_hand_side . handle_;
} /* Simplicial_const_iterator::operator != */

bool Simplicial_const_iterator::operator == ( const Simplicial_const_iterator & right_hand_side ) const {
  return handle_ == right_hand_side . handle_;
} /* Simplicial_const_iterator::operator == */

bool Simplicial_const_iterator::operator < ( const Simplicial_const_iterator & right_hand_side ) const {
  return handle_ < right_hand_side . handle_;
} /* Simplicial_const_iterator::operator < */

unsigned int Simplicial_const_iterator::dimension () const {
  return container_ -> handle_to_fixed_index_ . find ( handle_ ) -> second . first;
} /* Simplicial_const_iterator::dimension */

const Simplicial_Complex & Simplicial_const_iterator::container () const {
  return * container_;
} /* Simplicial_const_iterator::container */

std::ostream & operator << ( std::ostream & output_stream, const Simplex & print_me) {
  output_stream << "[";
  BOOST_FOREACH ( unsigned int v, print_me ) {
    output_stream << v << " ";
  }
  output_stream << "] ";
  return output_stream;
}

std::ostream & operator << ( std::ostream & output_stream, const Simplicial_const_iterator & print_me) {
  Simplex s = * print_me;
  output_stream << "[";
  BOOST_FOREACH ( Simplicial_const_iterator::size_type v, s ) {
    output_stream << v << " ";
  }
  output_stream << "] ";
  return output_stream;
} /* operator << */



/* * * * * * * * * * * * * * * *
 * Simplicial_Complex definitions *
 * * * * * * * * * * * * * * * */

std::pair<Simplicial_Complex::iterator, bool> Simplicial_Complex::insert ( const value_type & insert_me ) {
  if ( insert_me . empty () ) return std::pair < iterator, bool > ( end (), false );
  // read the vertices
  BOOST_FOREACH ( const size_type v, insert_me ) {
    if ( vertex_hashes_ . find ( v ) == vertex_hashes_ . end () ) {
      vertex_hashes_ [ v ] = (unsigned long) rand (); // low probability of having problems
    }
  }
  // generate handle for simplex
  unsigned long handle = simplex_to_handle_ ( insert_me );
  iterator it ( this, handle );
  if ( smallest_vertex_ . find ( handle ) != smallest_vertex_ . end () ) {
    return std::pair < iterator, bool > ( it, false );
  } else {
    // Store the smallest vertex
    smallest_vertex_ [ handle ] = insert_me [ 0 ];
    unsigned int d = insert_me . size () - 1;
    if ( fixed_index_to_handle_ . size () <= d ) {
      fixed_index_to_handle_ . resize ( d + 1 );
    }
    handle_to_fixed_index_ [ handle ] = std::pair<unsigned int, unsigned int> (d, size ( d ) );
    std::cout << " Inserting simplex " << insert_me << " as (" << d << ", " << size ( d ) << ")\n";
    fixed_index_to_handle_ [ d ] . push_back ( handle );                                                                            
    ++ total_size_;
    return std::pair < iterator, bool > ( it, true );
  }
} /* Simplicial_Complex::insert */

void Simplicial_Complex::erase ( const iterator & erase_me ) {
	//TODO
  std::cout << "Simplicial_Complex::erase is not implemented yet.\n";
} /* Simplicial_Complex::erase */

void Simplicial_Complex::clear ( void ) {

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
  
  vertex_hashes_ . clear ();
  smallest_vertex_ . clear ();
  handle_to_fixed_index_ . clear ();
  fixed_index_to_handle_ . clear ();
  total_size_ = 0;
} /* Simplicial_Complex::clear */

Simplicial_Complex::iterator Simplicial_Complex::find ( const Simplicial_Complex::key_type & key ) const {
  iterator it ( this, simplex_to_handle_ ( key ) );
  if ( handle_to_fixed_index_ . find ( it . handle_ ) == handle_to_fixed_index_ . end () )
    return end ();
  return it;
} /* Simplicial_Complex::find */

Simplicial_Complex::iterator Simplicial_Complex::begin ( void ) const {
	return begin ( 0 );
} /* Simplicial_Complex::begin */

Simplicial_Complex::iterator Simplicial_Complex::end ( void ) const {	
  return iterator ( this, 0 );
} /* Simplicial_Complex::end */

Simplicial_Complex::size_type Simplicial_Complex::size ( void ) const {
	return total_size_; 
} /* Simplicial_Complex::size */

Simplicial_Complex::iterator Simplicial_Complex::begin ( unsigned int dim ) const {
  while ( dim <= dimension () && fixed_index_to_handle_ [ dim ] . empty () ) {
    ++ dim;
  }
  if ( dim > dimension () ) return end ();
	return iterator ( this, fixed_index_to_handle_ [ dim ] [ 0 ] );
} /* Simplicial_Complex::begin */

Simplicial_Complex::iterator Simplicial_Complex::end ( unsigned int dim ) const {	
  return begin ( dim + 1 );
} /* Simplicial_Complex::end */

Simplicial_Complex::size_type Simplicial_Complex::size ( unsigned int dim ) const {
	return fixed_index_to_handle_ [ dim ] . size (); 
} /* Simplicial_Complex::size */

Simplicial_Complex::Chain Simplicial_Complex::boundary ( const const_iterator & lookup ) const {
  Chain output ( *this );
	Simplex s = * lookup;
  if ( s . size () == 1 ) return output;
  unsigned long bd_handle = lookup . handle_;
  Ring unit ( 1 );
  BOOST_FOREACH ( size_type v, s ) {
    bd_handle -= vertex_hashes_ . find ( v ) -> second;
    output . insert ( std::pair < const_iterator, Ring > ( iterator ( this, bd_handle ), unit ) );
    unit = - unit;
    bd_handle += vertex_hashes_ . find ( v ) -> second;
  }
  return output;
} /* Simplicial_Complex::boundary */

Simplicial_Complex::Chain Simplicial_Complex::coboundary ( const iterator & lookup ) const {
  Chain output ( *this );
	// TODO
  return output;  
} /* Simplicial_Complex::coboundary */

unsigned int Simplicial_Complex::dimension ( void ) const {
  return fixed_index_to_handle_. size () - 1;
} /* Simplicial_Complex::dimension */


/* Index Complex */

void Simplicial_Complex::index ( void ) {
  lookup_ . resize ( total_size_ + 1 );
  connection_ . resize ( total_size_, 0 );
  size_type indx = 0;
  for ( const_iterator lookup = begin (); lookup != end (); ++ lookup, ++ indx ) {
    index_ [ lookup ] = indx;
    lookup_ [ indx ] = lookup;
  } /* for */
  index_ [ end () ] = total_size_;
  lookup_ [ total_size_ ] = end ();
  index_begin_ . resize ( dimension () + 2, 0 );
  size_type sum = 0;
  for ( unsigned int dimension_index = 0; dimension_index <= dimension (); ++ dimension_index ) {
    sum += size ( dimension_index );
    index_begin_ [ dimension_index + 1 ] = sum;
  } /* for */
} /* Simplicial_Complex::index */

Simplicial_Complex::size_type Simplicial_Complex::index_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ];
} /* Simplicial_Complex::index_begin */

Simplicial_Complex::size_type Simplicial_Complex::index_end ( unsigned int dimension ) const {
  //std::cout << "*  dimension = " << dimension << "\n";  
  //std::cout << "   index_begin_ . size () = " << index_begin_ . size () << "\n";
  return index_begin_ [ dimension + 1 ];
} /* Simplicial_Complex::index_end */

Simplicial_Complex::size_type Simplicial_Complex::index ( const const_iterator & lookup ) const {
  return index_ . find ( lookup ) -> second;
} /* Simplicial_Complex::index */

Simplicial_Complex::size_type & Simplicial_Complex::index ( const const_iterator & lookup ) {
  return index_ [ lookup ];
} /* Simplicial_Complex::index */

std::vector < Simplicial_const_iterator > & Simplicial_Complex::lookup ( void ) {
  return lookup_;
} /* Simplicial_Complex::lookup */

const Simplicial_const_iterator & Simplicial_Complex::lookup ( size_type index ) const {
  return lookup_ [ index ];
} /* Simplicial_Complex::lookup */

Simplicial_const_iterator & Simplicial_Complex::lookup ( size_type index ) {
  return lookup_ [ index ];
} /* Simplicial_Complex::lookup */

/* Decomposable Complex */

std::vector < int > Simplicial_Complex::count_all_boundaries ( void ) const {
  std::vector < int > number_of_boundaries ( total_size_ );
  for ( size_type cell_index = 0; cell_index < total_size_; ++ cell_index ) {
    number_of_boundaries [ cell_index ] = boundary ( lookup ( cell_index ) ) . size ();
  } /* for */
  return number_of_boundaries;
} /* Simplicial_Complex::count_all_boundaries */
  
class push_functor {
public:
  push_functor ( const Simplicial_Complex & complex, std::vector < Simplicial_Complex::size_type > & output ) : complex(complex), output(output) {}
  void operator () ( std::pair < const Simplicial_const_iterator, Simplicial_Complex::Ring > & term ) {
    output . push_back ( complex . index ( term . first ) );
  }
  const Simplicial_Complex & complex;
  std::vector < Simplicial_Complex::size_type > & output;
};

void Simplicial_Complex::boundary ( std::vector < size_type > & output, const size_type cell_index ) const {
  output . clear ();
	Chain boundary_chain =  boundary ( lookup_ [ cell_index ] );
  for_each ( boundary_chain . begin (), boundary_chain . end (), push_functor ( *this, output ) );
} /* Simplicial_Complex::boundary */

void Simplicial_Complex::coboundary ( std::vector < size_type > & output, const size_type cell_index ) const {
  output . clear ();
  //std::cout << "cbd : cell_index = " << cell_index << " / " << total_size_ << "\n";
  Chain coboundary_chain =  coboundary ( lookup_ [ cell_index ] );
  for_each ( coboundary_chain . begin (), coboundary_chain . end (), push_functor ( *this, output ) );
} /* Simplicial_Complex::coboundary */

class push_functor_with_ring {
public:
  push_functor_with_ring ( const Simplicial_Complex & complex, std::vector < std::pair< Simplicial_Complex::size_type, Default_Ring > > & output ) : complex(complex), output(output) {}
  void operator () ( std::pair < const Simplicial_const_iterator, Simplicial_Complex::Ring > & term ) {
    output . push_back ( std::pair< Simplicial_Complex::size_type, Default_Ring > ( complex . index ( term . first ), term . second ) );
  }
  const Simplicial_Complex & complex;
  std::vector < std::pair< Simplicial_Complex::size_type, Default_Ring > > & output;
};

void Simplicial_Complex::boundary ( std::vector < std::pair< size_type, Default_Ring > > & output, const size_type cell_index ) const {
  output . clear ();
  Chain boundary_chain =  boundary ( lookup_ [ cell_index ] );
  for_each ( boundary_chain . begin (), boundary_chain . end (), push_functor_with_ring ( *this, output ) );
} /* Simplicial_Complex::boundary */

void Simplicial_Complex::coboundary ( std::vector < std::pair< size_type, Ring > > & output, const size_type cell_index ) const {
  output . clear ();
  Chain coboundary_chain =  coboundary ( lookup_ [ cell_index ] );
  for_each ( coboundary_chain . begin (), coboundary_chain . end (), push_functor_with_ring ( *this, output ) );
} /* Simplicial_Complex::coboundary */


void Simplicial_Complex::decompose ( void ) {
  index (); 
  king_count_ = morse::decompose ( *this );
} /*  Simplicial_Complex::decompose */

char Simplicial_Complex::type ( size_type index, unsigned int dimension ) const {
  if ( index < index_begin_ [ dimension ] + king_count_ [ dimension + 1 ] ) return 0; /* QUEEN */
  if ( index < index_begin_ [ dimension + 1 ] - king_count_ [ dimension ] ) return 1; /* ACE */
  return 2; /* KING */
} /* Simplicial_Complex::type */

Simplicial_Complex::size_type Simplicial_Complex::mate ( size_type queen_index, unsigned int dimension ) const {
  return index_begin_ [ dimension ] + index_begin_ [ dimension + 2 ] - queen_index - 1;
} /* Simplicial_Complex::mate */

const Simplicial_Complex::Ring & Simplicial_Complex::connection ( size_type queen_index ) const {
  return connection_ [ queen_index ];
} /* Simplicial_Complex::connection */

Simplicial_Complex::Ring & Simplicial_Complex::connection ( size_type queen_index ) {
  return connection_ [ queen_index ];
} /* Simplicial_Complex::connection */

Simplicial_Complex::size_type Simplicial_Complex::ace_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ] + king_count_ [ dimension + 1 ];
} /* Simplicial_Complex::ace_begin */

Simplicial_Complex::size_type Simplicial_Complex::ace_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ] - king_count_ [ dimension ];
} /* Simplicial_Complex::ace_end */

void Simplicial_Complex::Remove_Full_Simplex ( const std::vector<unsigned int> & simplex ) {
  // TODO
  std::cout << "WARNING: Remove_Full_Simplex not yet implemented.\n";
	return; 
} /* Simplicial_Complex::Remove_Full_Simplex */

void Simplicial_Complex::Add_Full_Simplex ( const Simplex & simplex ) {
  //std::cout << "Add_Full_Simplex\n"; 
  BOOST_FOREACH ( const size_type v, simplex ) {
    //std::cout << v << " ";
    if ( vertex_hashes_ . find ( v ) == vertex_hashes_ . end () ) {
      vertex_hashes_ [ v ] = rand ();
    }
  }
  //std::cout << "\n";
  iterator it = find ( simplex );
  //std::cout << "The iterator for the simplex has the handle " << it . handle_ << "\n";
  //std::cout << "The end iterator has the handle " << end () . handle_ << "\n";
  // If the simplex has already been added, return.
  if ( it != end () ) return;
  
  //std::cout << "Not already processed: inserting it.\n";
  // Otherwise, insert the simplex and recurse on its faces.
  insert ( simplex );
 
  //std::cout << "Recursing on the faces...\n";
  // Process the faces
  std::set< size_type > vertex_set ( simplex . begin (), simplex . end ());
  std::set< size_type > sending_set;
  BOOST_FOREACH ( const size_type v, simplex ) {
    sending_set = vertex_set;
    sending_set . erase ( v );
    Simplex send_me ( sending_set . begin (), sending_set . end () );
    //std::cout << "Recursing:\n";
    Add_Full_Simplex ( send_me );
  }
  return; 
} /* Simplicial_Complex::Add_Full_Simplex */

// constructors

Simplicial_Complex::Simplicial_Complex ( void ) {
  clear ();
}

Simplicial_Complex::Simplicial_Complex ( const char * FileName ) {
  Load_From_File ( FileName );
}


void Simplicial_Complex::Load_From_File ( const char * FileName ) {
  clear ();
	std::ifstream input_file ( FileName ); 
	if ( not input_file . good () ) {
		std::cout << "Simplicial_Complex::Load_From_File. Fatal Error. " 
    << FileName << " not found.\n";
		exit ( 1 ); 
  } /* if */  
  char c;
  std::string ascii_vertex;
  Simplex simplex;
  while ( input_file . good () ) {
    c = input_file . get ();
    if ( not input_file . good () ) break;
    //std::cout << "Load: read " << c << "\n";
    if ( c < '0' || c > '9') {
      if ( ascii_vertex . size () > 0 ) {
        //std::cout << "Load: Found a vertex: " << ascii_vertex << "\n";
        unsigned int vertex = atoi ( ascii_vertex . c_str () );
        ascii_vertex . clear ();
        //std::cout << "    becomes " << vertex << "\n";
        simplex . push_back ( vertex );
      }
    } else {
      ascii_vertex . push_back ( c );
    }
    if ( c == ']' ) {
      //std::cout << "Load: Found a simplex.\n";
      Add_Full_Simplex ( simplex );
      simplex . clear ();
    }
  }
  input_file . close ();
} /* Simplicial_Complex::Load_From_File */

// private functions

unsigned long Simplicial_Complex::simplex_to_handle_ ( const std::vector<unsigned int> & simplex ) const {
  unsigned long sum = 0;
  //std::cout << "simplex_to_handle_\n";
  BOOST_FOREACH ( const unsigned int v, simplex ) {
    //std::cout << "Looking at vertex " << v << "\n";
    // DEBUG
    //if ( vertex_hashes_ . find ( v ) == vertex_hashes_ . end () ) {
    //  std::cout << "Oh dear, it seems this vertex hasn't been registered. Prepare to crash. \n";
    //}
    sum += vertex_hashes_ . find ( v ) -> second;
  }
  return sum;
}

#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */
#endif
