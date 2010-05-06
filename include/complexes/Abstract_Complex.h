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
	/* typedefs */	 
  typedef Cell_Type Cell;
	typedef Abstract_Chain<Cell> Chain;
	typedef typename Chain::Ring Ring;
  typedef size_t size_type;
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
  unsigned long index_begin ( unsigned int dimension ) const;
  unsigned long index_end ( unsigned int dimension ) const;
  unsigned long index ( const const_iterator & lookup ) const;
  unsigned long & index ( const const_iterator & lookup );
  const const_iterator & lookup ( unsigned long index ) const;
  const_iterator & lookup ( unsigned long index );
  /* TODO:
  std::vector < int > count_all_boundaries ( void ) const;
  void boundary_index_list ( std::vector < unsigned long > & list, unsigned long index ) const;
  void coboundary_index_list ( std::vector < unsigned long > & list, unsigned long index ) const;
  Morse_Chain boundary ( const unsigned long input ) const;
  Morse_Chain coboundary ( const unsigned long input ) const;
  */
  /* Decomposable Complex */
  /* TODO:
  void decompose ( void );
  char type ( unsigned long index, unsigned int dimension ) const;
  unsigned long mate ( unsigned long queen_index, unsigned int dimension ) const;
  unsigned long ace_begin ( unsigned int dimension ) const;
  unsigned long ace_end ( unsigned int dimension ) const;
   */ 
  /* Abstract Container */
  Abstract_Complex ( void );
  Abstract_Complex ( unsigned int dimension );
private:
  friend class Abstract_const_iterator<Cell_Type>;
  std::set < Cell > cells_;
  std::vector < const_iterator > begin_;
  const_iterator end_;
  std::vector < size_type > size_;
  unsigned long total_size_;
  std::map < Cell, Chain > boundary_;
  std::map < Cell, Chain > coboundary_;
  unsigned int dimension_;
  std::map < const_iterator, unsigned int > index_;
  std::vector < const_iterator > lookup_;
  std::vector<unsigned long> index_begin_;
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
