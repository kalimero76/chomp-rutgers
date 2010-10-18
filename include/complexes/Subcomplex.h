/*
 *  Subcomplex.h
 *  
 *  Shaun Harker
 *  3/31/10.
 *
 */

#ifndef CHOMP_SUBCOMPLEX_
#define CHOMP_SUBCOMPLEX_

#include <set> /* for map */
#include "archetypes/Chain_Archetype.h" /* for Default_Cell, Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for class Cell_Complex_Archetype */

/**************************************************************************
 *                            SUBCOMPLEXES                                *
 **************************************************************************/

/* Forward Declarations */

template < class Cell_Complex > class Subcomplex_Chain;
template < class Cell_Complex > class Subcomplex;
template < class Cell_Complex > class Subcomplex_const_iterator;

/* * * * * * * * * * * * * *
 * class Subcomplex_Chain  *
 * * * * * * * * * * * * * */

template < class Cell_Complex >
class Subcomplex_Chain : public Chain_Archetype < std::map < Subcomplex_const_iterator<Cell_Complex>, Default_Ring > > {
public:
  typedef Subcomplex<Cell_Complex> complex_type;
  Subcomplex_Chain ( const complex_type & container );
};

/* * * * * * * * * * *
 * class Subcomplex  *
 * * * * * * * * * * */

template < class Cell_Complex >
class Subcomplex {
public:
	/* typedefs */	 
	typedef Subcomplex_Chain<Cell_Complex> Chain;
  typedef Default_Cell Cell;
	typedef typename Cell_Complex::Ring Ring;
	typedef unsigned long size_type;
	typedef Cell key_type;
	typedef Cell value_type;
  typedef Subcomplex_const_iterator<Cell_Complex> const_iterator;
  typedef const_iterator iterator;
  /* Basic Container */
  std::pair<iterator, bool> insert ( const value_type & insert_me );
  void erase ( const iterator & erase_me );
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
  
  // BEGIN TODO
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
  // END TODO
  
  /* Subcomplex */
  Subcomplex ( const Cell_Complex & super_complex ); 
  Chain project ( const typename Cell_Complex::Chain & project_me ) const;
  Chain project ( const Chain & project_me ) const;
  typename Cell_Complex::const_iterator include ( const const_iterator & include_me ) const;
  typename Cell_Complex::Chain include ( const Chain & include_me ) const;


protected:
  friend class Subcomplex_const_iterator<Cell_Complex>;
  size_type bitmap_size_;
  std::vector < bool > bitmap_;
  std::vector<const_iterator> begin_;
  const_iterator end_;
  std::vector<size_type> size_;
  unsigned long total_size_;
  unsigned int dimension_;
  const Cell_Complex & super_complex_;
};

/* * * * * * * * * * * * * * * * * *
 * class Subcomplex_const_iterator *
 * * * * * * * * * * * * * * * * * */

template < class Cell_Complex >
class Subcomplex_const_iterator {
public:
  typedef Subcomplex<Cell_Complex> complex_type;
  Subcomplex_const_iterator ( void );
  Subcomplex_const_iterator ( const complex_type * const container, 
                             unsigned long data );
  Subcomplex_const_iterator ( const complex_type * const container, 
                              unsigned long data, 
                              const unsigned int dimension ); 
  Subcomplex_const_iterator & operator ++ ( void );
  bool operator != ( const Subcomplex_const_iterator & right_hand_side ) const;
  bool operator == ( const Subcomplex_const_iterator & right_hand_side ) const;
  bool operator < ( const Subcomplex_const_iterator & right_hand_side ) const;
  typename Subcomplex<Cell_Complex>::value_type operator * ( void ) const; 
  unsigned int dimension () const;
  const complex_type & container () const;
  /* For morse::decompose */
  unsigned long data () const;
private:
  friend class Subcomplex<Cell_Complex>;
  const complex_type * container_;
  unsigned long data_;
  unsigned int dimension_;
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Subcomplex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Subcomplex.cpp"
#endif

#endif
