/*
 *  Toplex_Archetype.cpp
 *  
 *
 *  Created by Shaun Harker on 4/16/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "archetypes/Toplex_Archetype.h"


#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */
#include "toplexes/Cubical_Toplex.h" 
template class Toplex_Archetype < Cubical_Toplex_Container >;
#endif
