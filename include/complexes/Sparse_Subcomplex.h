/*
 *  Sparse_Subcomplex.h
 *  
 *  Shaun Harker
 *  6/7/10.
 *
 */

#ifndef CHOMP_SPARSE_SUBCOMPLEX_
#define CHOMP_SPARSE_SUBCOMPLEX_

#include <vector>
#include <map>

#include "boost/unordered_set.hpp"

#include "boost/functional/hash.hpp"
#include "archetypes/Chain_Archetype.h"

/**************************************************************************
 *                     SPARSE SUBCOMPLEXES                                *
 **************************************************************************/

/* Forward Declarations */

template < class Cell_Complex > class Sparse_Subcomplex_Chain;
template < class Cell_Complex > class Sparse_Subcomplex;
template < class Cell_Complex > class Sparse_Subcomplex_const_iterator;

/* * * * * * * * * * * * * * * * *
 * class Sparse_Subcomplex_Chain *
 * * * * * * * * * * * * * * * * */

template < class Cell_Complex >
class Sparse_Subcomplex_Chain : 
public Chain_Archetype < std::map < Sparse_Subcomplex_const_iterator < Cell_Complex >, 
                         typename Cell_Complex::Ring > > {};

/* * * * * * * * * * * * * *
 * class Sparse_Subcomplex *
 * * * * * * * * * * * * * */

template < class Cell_Complex >
class Sparse_Subcomplex {
public:
	/* typedefs */	 
	typedef Sparse_Subcomplex_Chain<Cell_Complex> Chain;
  typedef typename Cell_Complex::const_iterator Cell;
	typedef typename Cell_Complex::Ring Ring;
	typedef unsigned int size_type;
	typedef Cell key_type;
	typedef Cell value_type;
  typedef Sparse_Subcomplex_const_iterator<Cell_Complex> const_iterator;
  typedef const_iterator iterator;
  /* Basic Container */
  std::pair<iterator, bool> insert ( const value_type & insert_me );
  void erase ( const iterator & erase_me );
  void clear ( void );
  iterator find ( const key_type & find_me ) const;
  iterator begin ( void ) const;
  iterator end ( void ) const;
  size_type size ( void ) const;
  /* Cell Complex */
  iterator begin ( unsigned int dimension ) const;
  iterator end ( unsigned int dimension ) const;
  size_type size ( unsigned int dimension ) const;
  Chain boundary ( const const_iterator & input ) const;
  Chain coboundary ( const const_iterator & input ) const;
  unsigned int dimension ( void ) const;
  /* Index Complex */
  void index ( void );
  size_type index_begin ( unsigned int dimension ) const;
  size_type index_end ( unsigned int dimension ) const;
  size_type index ( const const_iterator & lookup ) const;
  size_type & index ( const const_iterator & lookup );
  std::vector < const_iterator > & lookup ( void );
  const const_iterator & lookup ( size_type index ) const;
  const_iterator & lookup ( size_type index );
  std::vector < int > count_all_boundaries ( void ) const;
  void boundary ( std::vector < size_type > & output, const size_type index ) const;
  void coboundary ( std::vector < size_type > & output, const size_type index ) const;
  void boundary ( std::vector < std::pair <size_type, Ring > > & output, const size_type input ) const;
  void coboundary ( std::vector < std::pair <size_type, Ring > > & output, const size_type input ) const;
  /* Decomposable Complex */
  void decompose ( void );
  char type ( size_type index, unsigned int dimension ) const;
  size_type mate ( size_type queen_index, unsigned int dimension ) const;
  const Ring & connection ( size_type queen_index ) const;
  Ring & connection ( size_type queen_index );  
  size_type ace_begin ( unsigned int dimension ) const;
  size_type ace_end ( unsigned int dimension ) const;  
  /* Sparse Subcomplex */
  Sparse_Subcomplex ( void );
  Sparse_Subcomplex ( const Cell_Complex & super_complex );
  void construct ( const Cell_Complex & super_complex );
  Chain project ( const typename Cell_Complex::Chain & project_me ) const;
  typename Cell_Complex::const_iterator include ( const const_iterator & include_me ) const;
  typename Cell_Complex::Chain include ( const Chain & include_me ) const;

protected:
  friend class Sparse_Subcomplex_const_iterator<Cell_Complex>;
  /* Cell Complex */
  size_type total_size_;
  const_iterator end_;
  std::vector<size_type> size_;
  unsigned int dimension_; 
  /* Index Complex */
  std::map < const_iterator, size_type > index_;
  std::vector < const_iterator > lookup_;
  std::vector < size_type > index_begin_;
  /* Decomposable Complex */
  std::vector<Ring> connection_;
  std::vector< int > boundary_count_;
  bool boundary_count_available_;
  std::vector<size_type> king_count_;
  /* Sparse Subcomplex */
  //todo:: make this unordered_map and use it to store the index_ data?
  std::vector < boost::unordered_set < Cell, boost::hash<Cell> > > data_;
  const Cell_Complex * super_complex_;
};

/* * * * * * * * * * * * * * * * * * * * * *
 * class Sparse_Subcomplex_const_iterator  * 
 * * * * * * * * * * * * * * * * * * * * * */

template < class Cell_Complex >
class Sparse_Subcomplex_const_iterator {
public:
  typedef Sparse_Subcomplex<Cell_Complex> complex_type;
  Sparse_Subcomplex_const_iterator ( void );
  Sparse_Subcomplex_const_iterator ( const complex_type * const container, 
                                     const typename boost::unordered_set < typename Cell_Complex::const_iterator, 
                                     boost::hash < typename Cell_Complex::const_iterator > >
                                    ::const_iterator data,
                             const unsigned int dimension ); 
  Sparse_Subcomplex_const_iterator & operator ++ ( void );
  bool operator != ( const Sparse_Subcomplex_const_iterator & right_hand_side ) const;
  bool operator == ( const Sparse_Subcomplex_const_iterator & right_hand_side ) const;
  bool operator < ( const Sparse_Subcomplex_const_iterator & right_hand_side ) const;
  typename Sparse_Subcomplex<Cell_Complex>::value_type operator * ( void ) const; 
  unsigned int dimension () const;
  const complex_type & container () const;
private:
  friend class Sparse_Subcomplex<Cell_Complex>;
  const complex_type * container_;
  typename boost::unordered_set < typename Cell_Complex::const_iterator, boost::hash < typename Cell_Complex::const_iterator > >::const_iterator data_;
  unsigned int dimension_;
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Sparse_Subcomplex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Sparse_Subcomplex.cpp"
#endif

#endif
