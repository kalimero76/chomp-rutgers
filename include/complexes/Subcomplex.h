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
template < class Cell_Complex > class Subcomplex;

/* * * * * * * * * * * * * *
 * class Subcomplex_Chain  *
 * * * * * * * * * * * * * */
template < class Cell_Complex >
class Subcomplex_Chain : public Chain_Archetype < std::map < typename std::set < typename Cell_Complex::const_iterator >::const_iterator, Default_Ring > > {
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
  typedef typename std::set < typename Cell_Complex::const_iterator >::const_iterator const_iterator;
  typedef const_iterator iterator;
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
  public std::set < typename Cell_Complex::const_iterator > data_;
  std::vector<const_iterator> begin_;
  std::vector<const_iterator> end_;
  std::vector<size_type> size_;
  const Cell_Complex & super_complex_;
};

/* * * * * * * * * * *
 * class Subcomplex  *
 * * * * * * * * * * */

template < class Cell_Complex >
class Subcomplex : public Cell_Complex_Archetype < Subcomplex_Container < Cell_Complex > > {
public:
  /* Constructor */
  Subcomplex ( const Cell_Complex & super_complex );
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Subcomplex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Subcomplex.cpp"
#endif

#endif