/*
 *  Adaptive_Cubical_Toplex.h
 *  
 *
 *  Created by Shaun Harker on 5/26/10.
 *  Copyright 2010. All rights reserved.
 *
 */


#ifndef CHOMP_ADAPTIVE_CUBICAL_TOPLEX_
#define CHOMP_ADAPTIVE_CUBICAL_TOPLEX_

#include <ext/hash_set>

//namespace std {
//  using namespace __gnu_cxx;
//} /* namespace std */

#include "complexes/Adaptive_Complex.h"

namespace Adaptive_Cubical {
  /* Forward Declarations */
  struct Geometric_Description;
  struct Node;
  class Toplex;
  class Toplex_const_iterator;
  
  /* Typedefs */
  typedef unsigned int size_type; // for > 4E9 cubes, use unsigned long
  typedef size_type Top_Cell;
  typedef float Real;

  /* * * * * * * * * * * * * * * * * * * * * * * * * *
   * struct Adaptive_Cubical::Geometric_Description  *
   * * * * * * * * * * * * * * * * * * * * * * * * * */
  struct Geometric_Description {
    std::vector < Real > lower_bounds;
    std::vector < Real > upper_bounds;
    Geometric_Description ( void );
    Geometric_Description ( unsigned int size );
    Geometric_Description ( unsigned int size, const Real & value );
    Geometric_Description ( unsigned int size, const Real & lower_value, const Real & upper_value );
    bool intersects ( const Geometric_Description & other ) const;
  };
  
  std::ostream & operator << ( std::ostream & output_stream, const Geometric_Description & print_me );
  
  /* * * * * * * * * * * * * * * * *
   * struct Adaptive_Cubical::Node *
   * * * * * * * * * * * * * * * * */
  struct Node {
    Node * left_;
    Node * right_;
    Node * parent_;
    size_type contents_;
    unsigned int dimension_;
    Node ( void );
    ~Node ( void );
  };
  
  /* * * * * * * * * * * * * * * * * * * * *
   * class Adaptive_Cubical::Toplex_Subset *
   * * * * * * * * * * * * * * * * * * * * */
  typedef std::hash_set < Adaptive_Cubical::Top_Cell > Toplex_Subset;
  
  /* * * * * * * * * * * * * * * * * * * * * * * * *
   * class Adaptive_Cubical::Toplex_const_iterator  *
   * * * * * * * * * * * * * * * * * * * * * * * * */
  class Toplex_const_iterator {
  public:
    typedef Adaptive_Cubical::Node Node;
    Toplex_const_iterator ( void );
    Toplex_const_iterator ( Node * node );
    Toplex_const_iterator & operator ++ ( void );
    bool operator != ( const Toplex_const_iterator & right_hand_side ) const;
    bool operator == ( const Toplex_const_iterator & right_hand_side ) const;
    bool operator < ( const Toplex_const_iterator & right_hand_side ) const;
    const Adaptive_Cubical::Top_Cell & operator * ( void ) const; 
  private:
    friend class Toplex;
    Node * node_;
  };
  
  /* * * * * * * * * * * * * * * * * *
   * class Adaptive_Cubical::Toplex  *
   * * * * * * * * * * * * * * * * * */
  class Toplex {
  public:
    /* typedefs */
    typedef Adaptive_Cubical::size_type size_type;
    typedef Adaptive_Cubical::Top_Cell Top_Cell;
    typedef Adaptive_Cubical::Toplex_Subset Subset;
    typedef Adaptive_Complex Complex;
    typedef Adaptive_Cubical::Geometric_Description Geometric_Description;
    typedef Adaptive_Cubical::Toplex_const_iterator const_iterator;
    typedef const_iterator iterator;
    typedef Top_Cell key_type;
    typedef Top_Cell value_type;
    /* Basic Container */
    void erase ( iterator erase_me );
    void clear ( void );
    iterator find ( const key_type & find_me ) const;
    iterator begin ( void ) const;
    iterator end ( void ) const;
    size_type size ( void ) const;
    /* Toplex */
    Subset cover ( const Geometric_Description & geometric_region ) const;
    Geometric_Description geometry ( const const_iterator & cell_iterator ) const;
    Complex complex ( const Subset & subset_of_toplex ) const;
    /* Adaptive Toplex */
    Subset subdivide ( iterator cell_to_divide );
    /* Adaptive Cubical Toplex */
    void initialize ( const Geometric_Description & outer_bounds_of_toplex );
    Toplex ( void );
    Toplex ( const Geometric_Description & outer_bounds_of_toplex );

  private:
    const_iterator begin_;
    const_iterator end_;
    size_type size_;
    size_type tree_size_;
    std::vector < iterator > find_;
    Node * root_;
    Geometric_Description bounds_;
    unsigned int dimension_;
  };
  
} /* namespace Adaptive_Cubical */

#ifndef CHOMP_LIBRARY_ONLY_
#include "toplexes/Adaptive_Cubical_Toplex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "toplexes/Adaptive_Cubical_Toplex.cpp"
#endif

#endif