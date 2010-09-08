/*
 * Homology.h
 * CHomP
 * Shaun Harker
 *
 * This header contains definitions for the homology interface.
 *
 */ 

#ifndef CHOMP_HOMOLOGY_
#define CHOMP_HOMOLOGY_

#include <vector>

/* Compute Homology Groups via Smith Normal Form */
template < class Geometric_Complex_Template >
void Homology ( const Geometric_Complex_Template & the_complex );

/* Compute Homology Groups via Smith Normal Form */
template < class Geometric_Complex_Template >
void Homology (  std::vector<int> & Betti_output, std::vector<int> & minimal_number_of_generators_output, const Geometric_Complex_Template & the_complex );

/* Compute the homology of maps */
template < class Toplex, class Map >
void Map_Homology ( const Toplex & X, const Toplex & Y, const Map & f );

/* Print out the homology generators... TODO: return the homology generators */
template < class Cell_Complex >
std::vector < std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > > 
Homology_Generators_SNF ( const Cell_Complex & the_complex, bool trivial_generators = false );

template < class Cell_Complex >
std::vector < std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > > 
Homology_Generators_DMT ( const Cell_Complex & the_complex );

template < class Cell_Complex >
std::vector < std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > > 
Homology_Generators ( const Cell_Complex & the_complex );

/* Conley Index */
// Later separate this into a distinct file

class Conley_Index_t {
public:
  Conley_Index_t ( void );
};

/* Compute the Conley Index of a "subset" of a "toplex", with the
   combinatorial map "map" */
template < class Toplex, class Combinatorial_Map > void
Conley_Index ( Conley_Index_t * output,
               const Toplex & toplex, 
               const typename Toplex::Subset & subset,
               /* const */ Combinatorial_Map & map );


#ifndef CHOMP_LIBRARY_ONLY_
#include "algorithms/Homology.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "algorithms/Homology.cpp"
#endif

#endif
