/*
 *  Cell_Complex_Archetype.h
 *  
 *  Created by Shaun Harker on 10/6/09.
 *
 */
 
#ifndef CHOMP_CELL_COMPLEX_ARCHETYPE_
#define CHOMP_CELL_COMPLEX_ARCHETYPE_

#include <vector> /* for the vector of chain groups in Cell_Complex_Archetype */

/* * * * * * * * * * * * * * * * *
 * class Cell_Complex_Archetype  *
 * * * * * * * * * * * * * * * * */

template < class Container_Template >
class Cell_Complex_Archetype : public Container_Template {
public:

	/* * * * * * * * * * * * * * * **
	** Constructor and Destructor  **
	** * * * * * * * * * * * * * * */
	
	/** Constructor. Trivial {} implementation. */
	Cell_Complex_Archetype ( void ) ;
	/** Deconstructor. Trivial {} implementation. */
	virtual ~Cell_Complex_Archetype ( void );
	
	/* * * * * * * * * * **
	** Type Definitions  **
	** * * * * * * * * * */
  typedef Container_Template Container; 
	typedef typename Container::Chain Chain;
	typedef typename Container::Cell Cell;
	typedef typename Container::const_iterator const_iterator;
  typedef const_iterator iterator;
	typedef typename Container::Ring Ring;
  typedef typename Container::size_type size_type;
	typedef typename Container::value_type value_type;
  typedef typename Container::key_type key_type;
  
  /* expected from base class */
  using Container_Template::begin;
  using Container_Template::end;
  using Container_Template::size;
  using Container_Template::find;
  using Container_Template::erase;
  using Container_Template::boundary;
  using Container_Template::coboundary;
  using Container_Template::dimension;

  
	/* * * * * * * * * * * * * * * * * * * * * * **
	**  Read-only Chain Complex Member Functions **
	** * * * * * * * * * * * * * * * * * * * * * */
	
  /** Calculate Boundary and copy answer into the supplied chain */
	Chain & Boundary_Map ( Chain &, const const_iterator & ) const;
  
	/** Calculate Boundary and copy answer into the supplied chain */
	Chain & Boundary_Map ( Chain &, const Cell & ) const;
	
	/** Calculate Boundary and copy answer into the supplied chain */
	Chain & Boundary_Map ( Chain &, const Chain & ) const;
		
  /** Calculate Coboundary and copy answer into the supplied chain */
	Chain & Coboundary_Map ( Chain &, const const_iterator & ) const;
  
	/** Calculate Coboundary and copy answer into the supplied chain */
	Chain & Coboundary_Map ( Chain &, const Cell & ) const;
	
	/** Calculate Coboundary and copy answer into the supplied chain */
	Chain & Coboundary_Map ( Chain &, const Chain & ) const;
	
	/** Count the number of Elementary Chains in the boundary of a single Elementary Chain */
	size_type Count_Boundaries ( const const_iterator & ) const;

	/** Count the number of Elementary Chains in the boundary of a single Elementary Chain */
	size_type Count_Boundaries ( const Cell & ) const;
	
	/** Count the number of Elementary Chains in the coboundary of a single Elementary Chain */
	size_type Count_Coboundaries ( const const_iterator & ) const;

	/** Count the number of Elementary Chains in the coboundary of a single Elementary Chain */
	size_type Count_Coboundaries ( const Cell & ) const;
	
  /* Simple Associative Container */
  iterator begin ( void ) const;
  iterator end ( void ) const;
  size_type size ( void ) const;
  
  /* TODO : add functions to increase conformity to S.A.C. */
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "archetypes/Cell_Complex_Archetype.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "archetypes/Cell_Complex_Archetype.cpp"
#endif

#endif
