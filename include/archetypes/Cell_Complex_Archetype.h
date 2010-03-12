/*
 *  Cell_Complex_Archetype.h
 *  
 *  Created by Shaun Harker on 10/6/09.
 *
 */
 
#ifndef _CHOMP_CELL_COMPLEX_ARCHETYPE_
#define _CHOMP_CELL_COMPLEX_ARCHETYPE_

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
	typedef typename Container_Template::Chain Chain;
	/** We typedef the elementary chain type used by the underlying container. */
	typedef typename Container_Template::Chain::Elementary_Chain Elementary_Chain;
	/** We typedef the const_iterator type used by the underlying container */
	typedef typename Container_Template::const_iterator const_iterator;
	/** We typedef the ring type used by the underlying container. */
	typedef typename Container_Template::Chain::Ring Ring;
	/** We typedef the size_type used by the underlying container. */
    typedef typename Container_Template::size_type size_type;
    
	/* * * * * * **
	** Iteration **
	** * * * * * */
	
	/** Store maximum dimension in the complex. */
	unsigned int dimension;
	/** Store a vector of Containers to hold Chain Groups. */
	std::vector<Container> Chain_Groups;
	
	/* * * * * * * * * * * * * * * * * * * * * * **
	**  Read-only Chain Complex Member Functions **
	** * * * * * * * * * * * * * * * * * * * * * */
	
	/** Calculate Boundary and copy answer into the supplied chain */
	virtual Chain & Boundary_Map ( Chain &, const Elementary_Chain & ) const;
	
	/** Calculate Boundary and copy answer into the supplied chain */
	virtual Chain & Boundary_Map ( Chain &, const typename Container::const_iterator & ) const = 0;

	/** Calculate Boundary and copy answer into the supplied chain */
	virtual Chain & Boundary_Map ( Chain &, const Chain & ) const;
		
	/** Calculate Coboundary and copy answer into the supplied chain */
	virtual Chain & Coboundary_Map ( Chain &, const Elementary_Chain & ) const;
	
	/** Calculate Coboundary and copy answer into the supplied chain */
	virtual Chain & Coboundary_Map ( Chain &, const typename Container::const_iterator & ) const = 0;

	/** Calculate Coboundary and copy answer into the supplied chain */
	virtual Chain & Coboundary_Map ( Chain &, const Chain & ) const;
	
	/** Count the number of Elementary Chains in the boundary of a single Elementary Chain */
	virtual size_type Count_Boundaries ( const typename Container::const_iterator & ) const;

	/** Count the number of Elementary Chains in the boundary of a single Elementary Chain */
	virtual size_type Count_Boundaries ( const Elementary_Chain & ) const;
	
	/** Count the number of Elementary Chains in the coboundary of a single Elementary Chain */
	virtual size_type Count_Coboundaries ( const typename Container::const_iterator & ) const;

	/** Count the number of Elementary Chains in the coboundary of a single Elementary Chain */
	virtual size_type Count_Coboundaries ( const Elementary_Chain & ) const;
	
	/** Report number of Elementary Chains in complex. */
	virtual size_type size ( void ) const;
	
	/** Find an elementary chain in the complex. */
	virtual typename Container::const_iterator find ( const Elementary_Chain & ) const;
	
	/** Return an iterator to an elementary chain in the complex of lowest possible dimension. */
	virtual typename Container::const_iterator begin ( void ) const;
	
	/* * * * * * * * * * * * * * * * * * * * * **
	**  Write-only Chain Complex Manipulations **
	** * * * * * * * * * * * * * * * * * * * * */
	
	/** Remove an Elementary Chain from the complex */
	virtual void Remove_Elementary_Chain ( const Elementary_Chain &) = 0;
	
	/* * * * * * * * * * * * * * * * * * * * * **
	**  Read-Write Chain Complex Manipulations **
	** * * * * * * * * * * * * * * * * * * * * */
	
	/** Apply reduction/coreduction techniques in order to simplify the complex without changing its homology. */
	virtual void Geometric_Preprocessing ( void );
};

#ifndef _CHOMP_LIBRARY_ONLY_
#include "archetypes/Cell_Complex_Archetype.hpp"
#endif

#ifdef _CHOMP_HEADER_ONLY_
#include "archetypes/Cell_Complex_Archetype.cpp"
#endif

#endif
