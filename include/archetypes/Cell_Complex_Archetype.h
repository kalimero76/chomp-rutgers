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
class Cell_Complex_Archetype {
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
	
	/** We typedef the underlying container. */
	typedef Container_Template Container; 
	/** We typedef the chain type used by the underlying container. */
	typedef typename Container::Chain Chain;
	/** We typedef the elementary chain type used by the underlying container. */
	typedef typename Container::Chain::Cell Cell;
	/** We typedef the const_iterator type used by the underlying container */
	typedef typename Container::const_iterator const_iterator;
	/** We typedef the ring type used by the underlying container. */
	typedef typename Container::Chain::Ring Ring;
	/** We typedef the size_type used by the underlying container. */
    typedef typename Container::size_type size_type;
    
	/* * * * * * **
	** Iteration **
	** * * * * * */
	
	/** Store maximum dimension in the complex. */
	unsigned int dimension;
	/** Store a vector of Containers to hold Chain Groups. */
	std::vector<Container> cells;
	
	/* * * * * * * * * * * * * * * * * * * * * * **
	**  Read-only Chain Complex Member Functions **
	** * * * * * * * * * * * * * * * * * * * * * */
	
	/** Calculate Boundary and copy answer into the supplied chain */
	virtual Chain & Boundary_Map ( Chain &, const Cell & ) const;
	
	/** Calculate Boundary and copy answer into the supplied chain */
	virtual Chain & Boundary_Map ( Chain &, const const_iterator & ) const = 0;

	/** Calculate Boundary and copy answer into the supplied chain */
	virtual Chain & Boundary_Map ( Chain &, const Chain & ) const;
		
	/** Calculate Coboundary and copy answer into the supplied chain */
	virtual Chain & Coboundary_Map ( Chain &, const Cell & ) const;
	
	/** Calculate Coboundary and copy answer into the supplied chain */
	virtual Chain & Coboundary_Map ( Chain &, const const_iterator & ) const = 0;

	/** Calculate Coboundary and copy answer into the supplied chain */
	virtual Chain & Coboundary_Map ( Chain &, const Chain & ) const;
	
	/** Count the number of Elementary Chains in the boundary of a single Elementary Chain */
	virtual size_type Count_Boundaries ( const const_iterator & ) const;

	/** Count the number of Elementary Chains in the boundary of a single Elementary Chain */
	virtual size_type Count_Boundaries ( const Cell & ) const;
	
	/** Count the number of Elementary Chains in the coboundary of a single Elementary Chain */
	virtual size_type Count_Coboundaries ( const const_iterator & ) const;

	/** Count the number of Elementary Chains in the coboundary of a single Elementary Chain */
	virtual size_type Count_Coboundaries ( const Cell & ) const;
	
	/** Report number of Elementary Chains in complex. */
	virtual size_type size ( void ) const;
	
	/** Find an elementary chain in the complex. */
	virtual typename Container::const_iterator find ( const Cell & ) const;
	
	/** Return cells [ k ] . begin () for the smallest k for which 
      cells [ k ] . begin () != cells [ k ] . end () */
	virtual typename Container::const_iterator begin ( void ) const;
	
	/** Return cells [ dimension ] . end () */
	virtual typename Container::const_iterator end ( void ) const;
  
	/* * * * * * * * * * * * * * * * * * * * * **
	**  Write-only Chain Complex Manipulations **
	** * * * * * * * * * * * * * * * * * * * * */
	
	/** Remove an Elementary Chain from the complex */
	virtual void Remove_Cell ( const Cell &) = 0;
	
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "archetypes/Cell_Complex_Archetype.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "archetypes/Cell_Complex_Archetype.cpp"
#endif

#endif
