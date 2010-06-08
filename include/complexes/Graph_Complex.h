/*
 *  Graph_Complex.h
 *  
 *
 *  Created by Shaun Harker on 6/7/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef CHOMP_GRAPH_COMPLEX_
#define CHOMP_GRAPH_COMPLEX_

#include <set> /* for map */
#include <utility> /* for pair */
#include "archetypes/Chain_Archetype.h" /* for Default_Chain */

/*****************************************************************************
 *                            GRAPH COMPLEXES                                *
 *****************************************************************************/

/* * * * * * * * * * * *
 * class Graph_Complex *
 * * * * * * * * * * * */

template < class Toplex >
class Graph_Complex : public Sparse_Subcomplex < 
                             Product_Complex < typename Toplex::Complex, 
                                               typename Toplex::Complex > > {
public:
  /* Graph Complex */
  template < class Map >
  Graph_Complex ( const Toplex & X, const Toplex & Y, const Map & f );
  
}

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Graph_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Graph_Complex.cpp"
#endif

#endif
