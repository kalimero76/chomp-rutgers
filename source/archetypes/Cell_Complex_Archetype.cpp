/*
 *  Cell_Complex_Archetype.cpp
 *  
 *  Template instances.
 *
 *  Created by Shaun Harker on 10/6/09.
 *
 */

#include "archetypes/Cell_Complex_Archetype.h"

#ifndef _CHOMP_HEADER_ONLY_
/* Template Instances */

#include "complexes/Cubical_Complex.h" 
template class Cell_Complex_Archetype < Cubical_Container >;
#include "complexes/Abstract_Complex.h" 
template class Cell_Complex_Archetype < Abstract_Container >;
#include "complexes/Vector_Complex.h" 
template class Cell_Complex_Archetype < Vector_Container >;
#endif
