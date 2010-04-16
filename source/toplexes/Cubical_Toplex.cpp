/*
 *  Cubical_Toplex.cpp
 *  
 *
 *  Created by Shaun Harker on 4/16/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "toplexes/Cubical_Toplex.h"
#include "complexes/Cubical_Complex.h"
#include <algorithm>

void Cubical_Toplex::Initialize ( const Geometric_Description & new_outer_bounds_of_toplex, const std::vector < unsigned int > & new_dimension_sizes ) {
  clear ();
  outer_bounds_of_toplex = new_outer_bounds_of_toplex;
  dimension_sizes = new_dimension_sizes;
  dimension = dimension_sizes . size ();
  jump_values . resize ( dimension + 2 );
  jump_values [ 0 ] = 1;
  for ( unsigned int dimension_index = 1; dimension_index <= dimension + 1; ++ dimension_index )
    jump_values [ dimension_index ] = jump_values [ dimension_index - 1 ] * dimension_sizes [ dimension_index - 1];
}

Cubical_Toplex::Subset_Container Cubical_Toplex::Subdivide_Cell ( const const_iterator & cell_to_divide ) {
  std::cout << "Cubical Toplex does not implement Subdivide_Cell. \n"; 
  exit (-1);
  return Cubical_Toplex::Subset_Container();
}

std::vector < unsigned int > Cubical_Toplex::getCoordinatesFromName ( unsigned long name ) const {
  std::vector < unsigned int > cube_coordinates ( dimension );
  for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) {
    cube_coordinates [ dimension_index ] = name % jump_values [ dimension_index + 1 ];
    name = name / jump_values [ dimension_index + 1 ];
  } /* for */
  return cube_coordinates;
} /* Cubical_Toplex::getCoordinatesFromName */


/* Function object used in Generate_Complex */
struct add_the_full_cubes {
  Cubical_Complex & complex_reference;
  const Cubical_Toplex & toplex_reference;
  add_the_full_cubes ( Cubical_Complex & complex_reference, const Cubical_Toplex & toplex_reference ) 
  : complex_reference(complex_reference), toplex_reference(toplex_reference) {}
  void operator () ( const Cubical_Toplex::Top_Cell & cell ) {
    complex_reference . Add_Full_Cube ( toplex_reference . getCoordinatesFromName ( cell . name ) );
  } /* operator () */
}; /* struct add_the_full_cubes */

Cubical_Toplex::Complex_Type Cubical_Toplex::Generate_Complex ( const Subset_Container & subset_of_toplex ) const {
  Cubical_Complex return_value;
  /* Set size of Cubical_Complex. */
  // INEFFICIENT: Making complex of equal size, despite the fact the bounding box could be much smaller. //
  return_value . Allocate_Bitmap ( dimension_sizes );
  /* Add the full cubes. */
  // INEFFICIENT: Add_Full_Cube for different top cells will add the same cells without need. //
  // Technique: Declare a function object and use the STL for_each algorithm.  
  for_each ( subset_of_toplex . begin (), subset_of_toplex . end (), add_the_full_cubes ( return_value, *this ) );
  /* Return the Cubical_Complex. */
  return return_value;
}



static inline unsigned int getIndex ( float input, float bottom, float top, unsigned int bins ) {
  return (unsigned int ) ( ( input - bottom ) / ( top - bottom ) * (float) bins );
}

Bounding_Box Cubical_Toplex::getBoundingBox ( const Geometric_Description & geometric_region ) const {
  Bounding_Box return_value;
  return_value . lower_bounds . resize ( dimension );
  return_value . upper_bounds . resize ( dimension );
  for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) {
    return_value . lower_bounds [ dimension_index ] = getIndex ( geometric_region . lower_bounds [ dimension_index ],
                                                                 outer_bounds_of_toplex . lower_bounds [ dimension_index ], 
                                                                 outer_bounds_of_toplex . upper_bounds [ dimension_index ], 
                                                                 dimension_sizes [ dimension_index ] );
    return_value . upper_bounds [ dimension_index ] = getIndex ( geometric_region . upper_bounds [ dimension_index ],
                                                                 outer_bounds_of_toplex . lower_bounds [ dimension_index ], 
                                                                 outer_bounds_of_toplex . upper_bounds [ dimension_index ], 
                                                                 dimension_sizes [ dimension_index ] );    
                                                                
  } /* for */
  return return_value;
} /* Cubical_Toplex::getBoundingBox */


/* function object used in Outer_Cover */
struct insert_if_touching {
  Cubical_Toplex::Subset_Container & subset_reference;
  const Cubical_Toplex & toplex_reference;
  const Cubical_Toplex::Geometric_Description & geometric_region;
  Bounding_Box bounding_box;
  insert_if_touching ( Cubical_Toplex::Subset_Container & subset_reference, const Cubical_Toplex & toplex_reference, const Cubical_Toplex::Geometric_Description & geometric_region ) 
  : subset_reference(subset_reference), toplex_reference(toplex_reference), geometric_region(geometric_region) {
    bounding_box = toplex_reference . getBoundingBox ( geometric_region );
  }
  void operator () ( const Cubical_Toplex::Top_Cell & cell ) {
    std::vector < unsigned int > cube_coordinates = toplex_reference . getCoordinatesFromName ( cell . name );
    bool is_inside = true;
    for ( unsigned int dimension_index = 0; dimension_index <= toplex_reference . dimension; ++ dimension_index ) {
      if ( cube_coordinates [ dimension_index ] < bounding_box . lower_bounds [ dimension_index ] ||
          cube_coordinates [ dimension_index ] > bounding_box . upper_bounds [ dimension_index ] ) {
        is_inside = false;
        break;
      } /* if */
    } /* for */
    if ( is_inside ) subset_reference . insert ( cell );
  } /* operator () */
}; /* struct insert_if_touching */

Cubical_Toplex::Subset_Container Cubical_Toplex::Outer_Cover ( const Geometric_Description & geometric_region ) const {
  Subset_Container return_value;
  // INEFFICIENT: Should find a way to use bounding box to reduce number of iterates checked. */
  for_each ( cells . begin (), cells . end (), insert_if_touching ( return_value, *this, geometric_region ) );
  return return_value;
} /* Cubical_Toplex::Outer_Cover */

static inline unsigned int getLowFloatFromIndex ( unsigned int input, float bottom, float top, unsigned int bins ) {
  return bottom + ( top - bottom ) * (float) input  / (float) bins ;
} /* getLowFloatFromIndex */

static inline unsigned int getHighFloatFromIndex ( unsigned int input, float bottom, float top, unsigned int bins ) {
  return bottom + ( top - bottom ) * (float) ( input + 1 ) / (float) bins ;
} /* getHighFloatFromIndex */

Cubical_Toplex::Geometric_Description Cubical_Toplex::Get_Geometric_Description ( const Top_Cell & cell ) const {
  Geometric_Description return_value;
  return_value . lower_bounds . resize ( dimension );
  return_value . upper_bounds . resize ( dimension );
  std::vector < unsigned int > cube_coordinates = getCoordinatesFromName ( cell . name );
  for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) {
    return_value . lower_bounds [ dimension_index ] = getLowFloatFromIndex ( cube_coordinates [ dimension_index ],
                                                                             outer_bounds_of_toplex . lower_bounds [ dimension_index ], 
                                                                             outer_bounds_of_toplex . upper_bounds [ dimension_index ], 
                                                                             dimension_sizes [ dimension_index ] ); 
    return_value . lower_bounds [ dimension_index ] = getHighFloatFromIndex ( cube_coordinates [ dimension_index ],
                                                                              outer_bounds_of_toplex . lower_bounds [ dimension_index ], 
                                                                              outer_bounds_of_toplex . upper_bounds [ dimension_index ], 
                                                                              dimension_sizes [ dimension_index ] );  
  } /* for */
  return return_value;
} /* Cubical_Toplex::Get_Geometric_Description */

#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */
#endif
