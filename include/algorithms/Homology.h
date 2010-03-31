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

/* Print out the homology generators... TODO: return the homology generators */
template < class Cell_Complex >
std::vector < std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > > 
Homology_Generators ( const Cell_Complex & the_complex );

#ifndef CHOMP_LIBRARY_ONLY_
#include "algorithms/Homology.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "algorithms/Homology.cpp"
#endif

#endif
