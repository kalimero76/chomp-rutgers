/*
 *  Toplex_Archetype.h
 *  
 *
 *  Created by Shaun Harker on 4/16/10.
 *  Copyright 2010. All rights reserved.
 *
 */


#ifndef CHOMP_TOPLEX_ARCHETYPE
#define CHOMP_TOPLEX_ARCHETYPE

/** Toplex_Archetype  */

template < class Toplex_Container >
class Toplex_Archetype : public Toplex_Container {
public:
  /* typedefs */
  typedef typename Toplex_Container::Top_Cell Top_Cell;
  typedef typename Toplex_Container::Subset_Container Subset_Container;
  typedef typename Toplex_Container::Geometric_Description Geometric_Description;
  typedef typename Toplex_Container::Complex_Type Complex_Type;
  typedef typename Toplex_Container::const_iterator const_iterator;
  typedef typename Toplex_Container::iterator iterator;
  /* data */
  Toplex_Container cells;
  unsigned int dimension;
  
  virtual ~Toplex_Archetype ( void );
  /* pure virtual member functions */
  virtual Subset_Container Subdivide_Cell ( const typename Toplex_Container::const_iterator & cell_to_divide ) = 0;
  virtual Complex_Type Generate_Complex ( const Subset_Container & subset_of_toplex ) const = 0;
  virtual Subset_Container Outer_Cover ( const Geometric_Description & geometric_region ) const = 0;
  virtual Geometric_Description Get_Geometric_Description ( const Top_Cell & cell ) const = 0;
  
  /* member functions with default implementations */
  virtual Subset_Container Subdivide_Cell ( const Top_Cell & cell_to_divide);

};

template < class Toplex_Template >
bool Check_if_Toplex_Subsets_Intersect ( const typename Toplex_Template::Subset_Container first_subset, 
                                         const typename Toplex_Template::Subset_Container second_subset );

#ifndef CHOMP_LIBRARY_ONLY_
#include "archetypes/Toplex_Archetype.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "archetypes/Toplex_Archetype.cpp"
#endif

#endif