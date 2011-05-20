/*
 *  Relative_Graph_Complex.h
 *  
 *
 *  Created by Shaun Harker on 9/29/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef CHOMP_RELATIVE_GRAPH_COMPLEX_
#define CHOMP_RELATIVE_GRAPH_COMPLEX_

#define GCC_VERSION (__GNUC__ * 10000 \
+ __GNUC_MINOR__ * 100 \
+ __GNUC_PATCHLEVEL__)
/* Test for GCC > 4.2.0 */
#if GCC_VERSION > 40200
#include <tr1/unordered_set> //PORTABILITY ISSUE
namespace std { using namespace tr1; }
#else
#warning Old Version of GCC -- using hash_set instead of unordered_set
#include <ext/hash_set>
namespace std { using namespace __gnu_cxx; }
#define unordered_set hash_set
#endif

#include "complexes/Subcomplex.h"
#include "complexes/Product_Complex.h"

/*****************************************************************************
 *                      RELATIVE GRAPH COMPLEXES                             *
 *****************************************************************************/

/* * * * * * * * * * * * * * * * *
 * class Relative_Graph_Complex  *
 * * * * * * * * * * * * * * * * */

template < class Toplex, class Combinatorial_Map >
class Relative_Graph_Complex 
{
public:
  typedef typename Toplex::Complex FactorComplex;
  typedef typename Toplex::size_type size_type;
  typedef typename FactorComplex::Ring Ring;
  typedef Subcomplex < FactorComplex > Relative_Complex;
  typedef typename Relative_Complex::Cell Relative_Cell;
  typedef typename Relative_Complex::Chain Relative_Chain;

  typedef typename FactorComplex::Cell FactorCell;
  typedef typename FactorComplex::Chain FactorChain;

  typedef std::pair < Relative_Cell, Relative_Cell > Cell;
  typedef std::map < Relative_Cell, Relative_Chain > Chain;

  template < class Subset >
  Relative_Graph_Complex (const Toplex & T, 
                          const Subset X,
                          const Subset A,
                          const Subset Y,
                          const Subset B,
                          const Combinatorial_Map & F );
  ~Relative_Graph_Complex ( void );
  
  Relative_Complex & domain ( void ); 
  Relative_Complex & codomain ( void ); 

  Chain cycleLift ( const Relative_Chain & lift_me );
  Relative_Chain projectToDomain ( const Chain & project_me );
  Relative_Chain projectToCodomain ( const Chain & project_me );

  void printChain ( const Chain & print_me );
  
private:
  const Toplex & toplex_;
  typename std::unordered_set<typename Toplex::Top_Cell> A_;
  Relative_Complex domain_;
  Relative_Complex codomain_;
public:
  // for debugging
  FactorComplex * full_domain_;
  FactorComplex * full_codomain_;
private:
  const Combinatorial_Map & F_;
  void makeFiber ( Relative_Complex & fiber, const Relative_Cell & domain_cell );

  std::map < typename Toplex::Top_Cell, typename FactorComplex::const_iterator > X_boxes_, Y_boxes_;
  std::map < typename FactorComplex::const_iterator, typename Toplex::Top_Cell > X_boxes_inv_;
};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Relative_Graph_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
//#include "complexes/Relative_Graph_Complex.cpp"
#endif

#endif
