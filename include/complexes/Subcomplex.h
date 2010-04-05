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

template < class Cell_Complex > class Subcomplex_Container;
template < class Cell_Complex > class Subcomplex;

/* * * * * * * * * * * * * * * *
 * class Subcomplex_Container  *
 * * * * * * * * * * * * * * * */

template < class Cell_Complex >
class Subcomplex_Container : public std::set < typename Cell_Complex::Cell >  {
public:
	/* typedefs */	 
	typedef typename Cell_Complex::Chain Chain;
  typedef typename Cell_Complex::Cell Cell;
	typedef typename Cell_Complex::Ring Ring;
};

/* * * * * * * * * * *
 * class Subcomplex  *
 * * * * * * * * * * */

template < class Cell_Complex >
class Subcomplex : public Cell_Complex_Archetype < Subcomplex_Container < Cell_Complex > > {
public:
  /* typedefs */
  typedef Subcomplex_Container < Cell_Complex > Container;
  typedef typename Cell_Complex::Chain Chain;
  typedef typename Cell_Complex::Cell Cell;
	typedef typename Cell_Complex::Ring Ring;
  typedef typename Container::const_iterator const_iterator;
  using Cell_Complex_Archetype < Container > :: cells;
  using Cell_Complex_Archetype < Container > :: dimension;
  
  /* Constructor */
  Subcomplex ( const Cell_Complex & super_complex );
  
	/* * * * * * * * * * * * * * * * * * * * * * * * *
   * Pure Virtual Functions That Must Be Overriden *
   * * * * * * * * * * * * * * * * * * * * * * * * */
	using Cell_Complex_Archetype < Container > :: Boundary_Map;
  using Cell_Complex_Archetype < Container > :: Coboundary_Map;
  
	/** Returns a copy of the Boundary information. This is only a copy, so subsequently altering this chain does not alter the complex directly. */
	virtual Chain & Boundary_Map ( Chain &, const const_iterator & ) const;
	/** Returns a copy of the Boundary information. This is only a copy, so subsequently altering this chain does not alter the complex directly. */
	virtual Chain & Coboundary_Map ( Chain &, const const_iterator & ) const;
	/** Remove an elementary chain from the complex. All terms from all chains involving this elementary chain will be deleted. NOT IMPLEMENTED. */
	virtual void Remove_Cell ( const Cell & );

private:
  /* Member variables */
  const Cell_Complex & super_complex;
  Chain project_chain ( const Chain & project_me ) const;
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Subcomplex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Subcomplex.cpp"
#endif

#endif