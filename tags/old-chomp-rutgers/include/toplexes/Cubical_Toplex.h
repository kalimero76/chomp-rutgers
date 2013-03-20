/*
 *  Toplex_Archetype.h
 *  
 *
 *  Created by Shaun Harker on 4/16/10.
 *  Copyright 2010. All rights reserved.
 *
 */


#ifndef CHOMP_CUBICAL_TOPLEX_
#define CHOMP_CUBICAL_TOPLEX_

#include <set>
#include "archetypes/Toplex_Archetype.h"
#include "complexes/Cubical_Complex.h"

struct Bounding_Box {
  std::vector < unsigned int > lower_bounds;
  std::vector < unsigned int > upper_bounds;
};

/** Cubical_Toplex_Geometric_Description */
/* Represents a rectangle \Prod_i [ lower_bounds [ i ], upper_bounds [ i ] ] */
class Cubical_Toplex_Geometric_Description {
public:
  std::vector < float > lower_bounds;
  std::vector < float > upper_bounds;
};

/** Cubical_Toplex_Subset_Container */ 
class Cubical_Toplex_Subset_Container : public std::set < Default_Cell > {};

/** Cubical_Toplex_Container */
class Cubical_Toplex_Container : public std::set < Default_Cell > {
public:
  typedef Default_Cell Top_Cell;
  typedef Cubical_Complex Complex_Type;
  typedef Cubical_Toplex_Geometric_Description Geometric_Description;
  typedef Cubical_Toplex_Subset_Container Subset_Container;
};

/** Cubical_Toplex  */
class Cubical_Toplex : public Toplex_Archetype < Cubical_Toplex_Container > {
  /* data */
  /* A rectangle which describes a bounding box for the toplex. */
  Geometric_Description outer_bounds_of_toplex;
  /* A vector of unsigned int's which gives the number of divisions in each direction */
  std::vector < unsigned int > dimension_sizes;
  std::vector < unsigned long > jump_values;


public:  
  /* initialization */
  void Initialize ( const Geometric_Description & outer_bounds_of_toplex, const std::vector < unsigned int > & dimension_sizes );
  /* pure virtual functions from base class */
  Subset_Container Subdivide_Cell ( const const_iterator & cell_to_divide );
  Complex_Type Generate_Complex ( const Subset_Container & subset_of_toplex ) const;
  Subset_Container Outer_Cover ( const Geometric_Description & geometric_region ) const;
  Geometric_Description Get_Geometric_Description ( const Top_Cell & cell ) const;
  
  /* implementation */
  std::vector < unsigned int > getCoordinatesFromName ( unsigned long name ) const;
  Bounding_Box getBoundingBox ( const Geometric_Description & geometric_region ) const;
};

template < class Toplex_Template >
bool Check_if_Toplex_Subsets_Intersect ( const typename Toplex_Template::Subset_Container first_subset, 
                                        const typename Toplex_Template::Subset_Container second_subset );

#ifndef CHOMP_LIBRARY_ONLY_
#include "toplexes/Cubical_Toplex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "toplexes/Cubical_Toplex.cpp"
#endif

#endif