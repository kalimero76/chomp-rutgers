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

#include "complexes/Sparse_Subcomplex.h"
#include "complexes/Product_Complex.h"

/*****************************************************************************
 *                            GRAPH COMPLEXES                                *
 *****************************************************************************/

/* * * * * * * * * * * *
 * class Graph_Complex *
 * * * * * * * * * * * */

template < class Toplex_Template >
class Graph_Complex : public Sparse_Subcomplex < Product_Complex < 
                      typename Toplex_Template::Complex, typename Toplex_Template::Complex > > 
{
public:
  typedef Toplex_Template Toplex;
  typedef Sparse_Subcomplex < Product_Complex < 
          typename Toplex::Complex, typename Toplex::Complex > > Parent;
  typedef typename Parent::Chain Chain;
  //typedef typename Toplex::Complex Complex;
  /* Graph Complex */
  template < class Map >
  Graph_Complex ( const Toplex & X, const Toplex & Y, const Map & f );
  ~Graph_Complex ( void );
  typename Toplex::Complex & codomain ( void ); // non-const since giving non-const reference
  typename Toplex::Complex::Chain projectToCodomain ( const Chain & project_me );
  typename Toplex::Complex & domain ( void ); // non-const since giving non-const reference
  typename Toplex::Complex::Chain projectToDomain ( const Chain & project_me );
private:
  /* The order of these matters in constructor: */
  typename Toplex::Complex * domain_;
  typename Toplex::Complex * codomain_;
  std::map < typename Toplex::Top_Cell, typename Toplex::Complex::const_iterator > domain_boxes_;
  std::map < typename Toplex::Top_Cell, typename Toplex::Complex::const_iterator > codomain_boxes_;
  Product_Complex < typename Toplex::Complex, typename Toplex::Complex > * product_;
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Graph_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Graph_Complex.cpp"
#endif

#endif
