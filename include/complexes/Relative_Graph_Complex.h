/*
 *  Relative_Graph_Complex.h
 *  
 *
 *  Created by Shaun Harker on 9/29/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef CHOMP_GRAPH_COMPLEX_
#define CHOMP_GRAPH_COMPLEX_

#include "complexes/Subcomplex.h"
#include "complexes/Product_Complex.h"

/*****************************************************************************
 *                      RELATIVE GRAPH COMPLEXES                             *
 *****************************************************************************/

/* * * * * * * * * * * * * * * * *
 * class Relative_Graph_Complex  *
 * * * * * * * * * * * * * * * * */

template < class Toplex >
class Relative_Graph_Complex 
{
public:
  typedef typename Toplex::Complex::const_iterator FactorCell;
  typedef std::map < FactorCell, long > FactorChain;
  typedef std::pair<FactorCell, FactorCell> Cell;
  typedef std::map<FactorCell, FactorChain > Chain;
  
  typedef Subcomplex < typename Toplex::Complex > Relative_Complex;
  typedef Relative_Complex::Chain Relative_Chain;

  template < Combinatorial_Map >
  Relative_Graph_Complex (const Toplex & T, 
                          const typename Toplex::Subset X,
                          const typename Toplex::Subset A,
                          const typename Toplex::Subset Y,
                          const typename Toplex::Subset B,
                          const Combinatorial_Map & F );
  ~Relative_Graph_Complex ( void );
  
  Relative_Complex & domain ( void ); 
  Relative_Complex & codomain ( void ); 

  Chain cycleLift ( const Relative_Chain & lift_me );
  Relative_Chain projectToDomain ( const Chain & project_me );
  Relative_Chain projectToCodomain ( const Chain & project_me );

private:
  const Toplex & toplex_;
  Relative_Complex domain_;
  Relative_Complex codomain_;
  Relative_Complex makeFiber ( const FactorCell & domain_cell );

};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Relative_Graph_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Relative_Graph_Complex.cpp"
#endif

#endif
