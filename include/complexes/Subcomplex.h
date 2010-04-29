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
template < class Cell_Complex > class Subcomplex_Container;
template < class Cell_Complex > class Subcomplex_const_iterator;
template < class Cell_Complex > class Subcomplex;

/* * * * * * * * * * * * * *
 * class Subcomplex_Chain  *
 * * * * * * * * * * * * * */
template < class Cell_Complex >
class Subcomplex_Chain : public Chain_Archetype < std::map < Subcomplex_const_iterator<Cell_Complex>, Default_Ring > > {
};

/* * * * * * * * * * * * * * * *
 * class Subcomplex_Container  *
 * * * * * * * * * * * * * * * */

template < class Cell_Complex >
class Subcomplex_Container {
public:
	/* typedefs */	 
	typedef Subcomplex_Chain<Cell_Complex> Chain;
  typedef typename Cell_Complex::const_iterator Cell;
	typedef typename Cell_Complex::Ring Ring;
	typedef unsigned long size_type;
	typedef Cell key_type;
	typedef Cell value_type;
  typedef Subcomplex_const_iterator<Cell_Complex> const_iterator;
  typedef const_iterator iterator;
  /* constructor */
  void initialize ( const Cell_Complex & super_complex );
  /* Simple Associative Container, Unique Associative Container, Cell Container */
  std::pair<iterator, bool> insert ( const value_type & insert_me );
  void erase ( const iterator & erase_me );
  iterator find ( const key_type & find_me ) const;
  iterator begin ( unsigned int dimension ) const;
  iterator end ( unsigned int dimension ) const;
  size_type size ( unsigned int dimension ) const;
  Chain boundary ( const const_iterator & input ) const;
  Chain coboundary ( const const_iterator & input ) const;
  unsigned int dimension ( void ) const;
  Chain project ( const typename Cell_Complex::Chain & project_me ) const;
  typename Cell_Complex::Chain include ( const Chain & include_me ) const;
private:
  friend class Subcomplex_const_iterator<Cell_Complex>;
  std::set < typename Cell_Complex::const_iterator > data_;
  std::vector<const_iterator> begin_;
  std::vector<const_iterator> end_;
  std::vector<size_type> size_;
  unsigned int dimension_;
  const Cell_Complex * super_complex_;
};

/* * * * * * * * * * * * * * * * * *
 * class Subcomplex_const_iterator *
 * * * * * * * * * * * * * * * * * */

template < class Cell_Complex >
class Subcomplex_const_iterator {
public:
  Subcomplex_const_iterator ( void );
  Subcomplex_const_iterator ( const Subcomplex_Container<Cell_Complex> * const); 
  Subcomplex_const_iterator ( const Subcomplex_Container<Cell_Complex> * const referral, typename std::set < typename Cell_Complex::const_iterator >::const_iterator data, const unsigned int dimension ); 
  Subcomplex_const_iterator & operator ++ ( void );
  bool operator != ( const Subcomplex_const_iterator & right_hand_side ) const;
  bool operator == ( const Subcomplex_const_iterator & right_hand_side ) const;
  bool operator < ( const Subcomplex_const_iterator & right_hand_side ) const;
  typename Subcomplex_Container<Cell_Complex>::value_type operator * ( void ) const; 
  unsigned int dimension () const;
private:
  friend class Subcomplex_Container<Cell_Complex>;
  const Subcomplex_Container<Cell_Complex> * referral_;
  typename std::set < typename Cell_Complex::const_iterator >::const_iterator data_;
  unsigned int dimension_;
};

/* * * * * * * * * * *
 * class Subcomplex  *
 * * * * * * * * * * */

template < class Cell_Complex >
class Subcomplex : public Cell_Complex_Archetype < Subcomplex_Container < Cell_Complex > > {
public:
  /* constructor */
  Subcomplex ( const Cell_Complex & super_complex );
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Subcomplex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Subcomplex.cpp"
#endif

#endif