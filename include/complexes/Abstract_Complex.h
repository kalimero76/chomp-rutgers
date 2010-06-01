/*
 *  Abstract_Complex.h
 *  
 *
 *  Created by Shaun Harker on 10/6/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#ifndef CHOMP_ABSTRACT_COMPLEX_
#define CHOMP_ABSTRACT_COMPLEX_

#include <set> /* for map */
#include <utility> /* for pair */
#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for class Cell_Complex_Archetype */

/********************************************************************************
 *                            ABSTRACT COMPLEXES                                *
 ********************************************************************************/

/* Forward Declarations */
template < class > class Abstract_Chain;
template < class > class Abstract_Complex;
template < class > class Abstract_const_iterator;

/* * * * * * * * * * * * *
 * class Abstract_Chain  *
 * * * * * * * * * * * * */

template < class Cell_Type = Default_Cell > 
class Abstract_Chain : public Chain_Archetype<std::map<Abstract_const_iterator<Cell_Type>, Default_Ring> > {
public:
  typedef Abstract_Complex<Cell_Type> complex_type;
  Abstract_Chain ( void );
  Abstract_Chain ( const complex_type & container );
};

/* * * * * * * * * * * * * *
 * class Abstract_Complex  *
 * * * * * * * * * * * * * */

template < class Cell_Type = Default_Cell >
class Abstract_Complex {
public:
  typedef unsigned int size_type;
	/* typedefs */	 
  typedef Cell_Type Cell;
	typedef Abstract_Chain<Cell> Chain;
	typedef typename Chain::Ring Ring;
  typedef Cell value_type;
  typedef Cell key_type;
  typedef Abstract_const_iterator<Cell_Type> const_iterator;
  typedef const_iterator iterator;
  /* Basic Container */
  std::pair<iterator, bool> insert ( const value_type & insert_me );
  void erase ( iterator erase_me );
  iterator find ( const key_type & find_me );
  const_iterator find ( const key_type & find_me ) const;
  iterator begin ( void ) const;
  iterator end ( void ) const;
  size_type size ( void ) const;
  /* Boundary Container */
  iterator begin ( unsigned int dimension ) const;
  iterator end ( unsigned int dimension ) const;
  size_type size ( unsigned int dimension ) const;
  Chain & boundary ( const iterator & input );
  const Chain & boundary ( const const_iterator & input ) const;
  Chain & coboundary ( const iterator & input );
  const Chain & coboundary ( const const_iterator & input ) const;
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
  /* Abstract Container */
  void generate_coboundary_information ( void );
  Abstract_Complex ( void );
  Abstract_Complex ( const Abstract_Complex & copy_me );
  Abstract_Complex ( unsigned int dimension );
  void constructor ( unsigned int dimension );
  size_type mass ( void );
  void swap ( Abstract_Complex & b );

private:
  friend class Abstract_const_iterator<Cell_Type>;
  std::set < Cell > cells_;
  std::vector < const_iterator > begin_;
  const_iterator end_;
  std::vector < size_type > size_;
  size_type total_size_;
  std::map < Cell, Chain > boundary_;
  std::map < Cell, Chain > coboundary_;
  unsigned int dimension_;
  std::map < const_iterator, size_type > index_;
  std::vector < const_iterator > lookup_;
  std::vector< size_type > index_begin_;
  std::vector<Ring> connection_;
  std::vector< size_type> king_count_;
};

/* * * * * * * * * * * * * * * * * 
 * class Abstract_const_iterator *
 * * * * * * * * * * * * * * * * */

template < class Cell_Type >
class Abstract_const_iterator {
public:
  typedef Abstract_Complex<Cell_Type> complex_type;
  Abstract_const_iterator ( void );
  Abstract_const_iterator ( const complex_type * const); 
  Abstract_const_iterator ( const complex_type * const container, 
                            typename std::set<Cell_Type>::const_iterator data, 
                            const unsigned int dimension ); 
  Abstract_const_iterator & operator ++ ( void );
  bool operator != ( const Abstract_const_iterator & right_hand_side ) const;
  bool operator == ( const Abstract_const_iterator & right_hand_side ) const;
  bool operator < ( const Abstract_const_iterator & right_hand_side ) const;
  Cell_Type operator * ( void ) const;
  const Cell_Type * operator -> ( void ) const;
  unsigned int dimension () const;
  const complex_type & container () const;
private:
  friend class Abstract_Complex<Cell_Type>;
  const complex_type * container_;
  typename std::set<Cell_Type>::const_iterator data_;
  unsigned int dimension_;
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Abstract_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Abstract_Complex.cpp"
#endif

#endif
