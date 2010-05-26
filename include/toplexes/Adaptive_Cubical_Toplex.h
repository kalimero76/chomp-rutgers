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

#include "complexes/Adaptive_Cubical_Complex.h"

namespace Adaptive_Cubical {
  /* Forward Declarations */
  struct Geometric_Description;
  struct Node;
  class Toplex_Subset;
  class Toplex_Subset_const_iterator;
  class Toplex;
  class Toplex_const_iterator;
  
  /* Typedefs */
  typedef unsigned int size_type; // for > 4E9 cubes, use unsigned long
  typedef size_type Top_Cell;

  /* * * * * * * * * * * * * * * * * * * * * * * * * *
   * struct Adaptive_Cubical::Geometric_Description  *
   * * * * * * * * * * * * * * * * * * * * * * * * * */
  struct Geometric_Description {
    typedef float real;
    std::vector < real > lower_bounds;
    std::vector < real > upper_bounds;
  };
  
  /* * * * * * * * * * * * * * * * *
   * struct Adaptive_Cubical::Node *
   * * * * * * * * * * * * * * * * */
  struct Node {
    Node * left_;
    Node * right_;
    Node * parent_;
    size_type contents_;
    Node ( void );
    ~Node ( void );
  };
  
  /* * * * * * * * * * * * * * * * * * * * *
   * class Adaptive_Cubical::Toplex_Subset *
   * * * * * * * * * * * * * * * * * * * * */
  class Toplex_Subset {
  public:
    /* typedefs */
    typedef Adaptive_Cubical::size_type size_type;
    typedef size_type Top_Cell;
    typedef Adaptive_Cubical::Toplex_Subset_const_iterator const_iterator;
    typedef const_iterator iterator;
    typedef Top_Cell key_type;
    typedef Top_Cell value_type;
    /* Basic Container */
    std::pair<iterator, bool> insert ( const value_type & insert_me );
    void erase ( const iterator & erase_me );
    void clear ( void );
    iterator find ( const key_type & find_me ) const;
    iterator begin ( void ) const;
    iterator end ( void ) const;
    size_type size ( void ) const;
  private:
    std::vector < Adaptive_Cubical::Node * > data_;
  };
  
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * class Adaptive_Cubical::Toplex_Subset_const_iterator  *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  class Toplex_Subset_const_iterator {
  public:
    typedef Adaptive_Cubical::Node Node;
    Toplex_Subset_const_iterator ( void );
    Toplex_Subset_const_iterator & operator ++ ( void );
    bool operator != ( const Toplex_Subset_const_iterator & right_hand_side ) const;
    bool operator == ( const Toplex_Subset_const_iterator & right_hand_side ) const;
    bool operator < ( const Toplex_Subset_const_iterator & right_hand_side ) const;
    const Adaptive_Cubical::Top_Cell & operator * ( void ) const; 
  private:
    Adaptive_Cubical::Node * data_;
    Adaptive_Cubical::Toplex_Subset * container_;
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
    typedef Adaptive_Cubical::Complex Complex;
    typedef Adaptive_Cubical::Geometric_Description Geometric_Description;
    typedef Adaptive_Cubical::Toplex_const_iterator const_iterator;
    typedef const_iterator iterator;
    typedef Top_Cell key_type;
    typedef Top_Cell value_type;
    /* Basic Container */
    void erase ( const iterator & erase_me );
    void clear ( void );
    iterator find ( const key_type & find_me ) const;
    iterator begin ( void ) const;
    iterator end ( void ) const;
    size_type size ( void ) const;
    /* Toplex */
    Subset cover ( const Geometric_Description & geometric_region ) const;
    Geometric_Description getGeometricDescription ( const Top_Cell & cell ) const;
    Complex complex ( const Subset & subset_of_toplex ) const;
    /* Adaptive Toplex */
    Subset subdivide ( const iterator & cell_to_divide );
    Subset children ( const const_iterator & node ) const;
    /* Adaptive Cubical Toplex */
    void initialize ( const Geometric_Description & outer_bounds_of_toplex );
  private:
    const_iterator begin_;
    const_iterator end_;
    size_type size_;
    std::vector < iterator > find_;
  };
  
  /* * * * * * * * * * * * * * * * * * * * * * * * *
   * class Adaptive_Cubical::Toplex_const_iterator  *
   * * * * * * * * * * * * * * * * * * * * * * * * */
  class Toplex_const_iterator {
  public:
    typedef Adaptive_Cubical::Node Node;
    Toplex_const_iterator ( void );
    Toplex_const_iterator & operator ++ ( void );
    bool operator != ( const Toplex_const_iterator & right_hand_side ) const;
    bool operator == ( const Toplex_const_iterator & right_hand_side ) const;
    bool operator < ( const Toplex_const_iterator & right_hand_side ) const;
    const Adaptive_Cubical::Top_Cell & operator * ( void ) const; 
  private:
    Node * data_;
  };
  
} /* namespace Adaptive_Cubical */

#ifndef CHOMP_LIBRARY_ONLY_
#include "toplexes/Adaptive_Cubical_Toplex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "toplexes/Adaptive_Cubical_Toplex.cpp"
#endif

#endif