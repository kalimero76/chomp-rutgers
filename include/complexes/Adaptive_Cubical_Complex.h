/*
 *  Adaptive_Cubical_Complex.h
 *
 *
 *  Created by Shaun Harker on 4/19/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef CHOMP_ADAPTIVE_COMPLEX_
#define CHOMP_ADAPTIVE_COMPLEX_

#include <deque>
#include <utility>

#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for Cell_Complex */

/* Forward Declarations */
struct Adaptive_Cubical_Node;
class Adaptive_Cubical_Cell;
class Adaptive_Cubical_Chain;
class Adaptive_Cubical_const_iterator;
class Adaptive_Cubical_Container;
class Adaptive_Cubical_Complex;

/** struct Adaptive_Cubical_Node **/
struct Adaptive_Cubical_Node {
  Adaptive_Cubical_Node * parent;
  Adaptive_Cubical_Node * left;
  Adaptive_Cubical_Node * right;
  unsigned int splitting_dimension;
};

/** class Adaptive_Cubical_Cell **/
class Adaptive_Cubical_Cell {
public:
  unsigned int dimension;
  unsigned int pathLength ( void ) const;
  const unsigned int & splittingDimension ( unsigned int index ) const;
  unsigned int & splittingDimension ( unsigned int index );
  const bool & splittingDirection ( unsigned int index ) const;
  bool & splittingDirection ( unsigned int index );

private:
  friend class Adaptive_Cubical_Container;
  friend class Adaptive_Cubical_const_iterator;
  unsigned int type_;
  std::deque < std::pair < unsigned int, bool > > data_;
};

/** class Adaptive_Cubical_Chain **/
class Adaptive_Cubical_Chain {
};

/** class Adaptive_Cubical_Container. **/
class Adaptive_Cubical_Container {
public:
  /* typedefs */
  typedef Default_Ring Ring;
  typedef Adaptive_Cubical_Cell Cell;
  typedef Default_Chain Chain;
  typedef size_t size_type;
  typedef Cell value_type;
  typedef Cell key_type;
  typedef Adaptive_Cubical_const_iterator const_iterator;
  typedef const_iterator iterator;
  
  /* Constructors */
  Adaptive_Cubical_Container ( void );
  Adaptive_Cubical_Container ( unsigned int dimension );
  ~Adaptive_Cubical_Container ( void );
  
  /* "Simple Associative Container" concept (just the basics)*/
  iterator find ( const key_type & find_me ) const;
  void erase ( const iterator & erase_me );
  
  /* "Cell Container" concept */
  iterator begin ( unsigned int dimension ) const;
  iterator end ( unsigned int dimension ) const;
  size_type size ( unsigned int dimension ) const;
  Chain boundary ( const const_iterator & input ) const;
  Chain coboundary ( const const_iterator & input ) const;
  unsigned int dimension ( void ) const;
  
private:
  /* Friends */
  friend class Adaptive_Cubical_Complex;
  friend class Adaptive_Cubical_const_iterator;
  /* Data */
  unsigned int dimension_;
  typedef Adaptive_Cubical_Node Node;
  std::vector < Node * > tree_data_;
  std::vector < const_iterator > begin_;
  std::vector < const_iterator > end_;
  std::vector < size_type > size_;
};

/** class Adaptive_Cubical_const_iterator **/
class Adaptive_Cubical_const_iterator {
public:
  Adaptive_Cubical_const_iterator ( void );  
  Adaptive_Cubical_const_iterator ( const Adaptive_Cubical_Container * const referral );  
  Adaptive_Cubical_const_iterator & operator ++ ( void );
  bool operator != ( const Adaptive_Cubical_const_iterator & right_hand_side ) const;
  bool operator == ( const Adaptive_Cubical_const_iterator & right_hand_side ) const;
  bool operator < ( const Adaptive_Cubical_const_iterator & right_hand_side ) const;
  const Adaptive_Cubical_Container::value_type & operator * ( void ) const;
  const Adaptive_Cubical_Container::value_type * operator -> ( void ) const;
private:
  friend class Adaptive_Cubical_Container;
  typedef Adaptive_Cubical_Node Node;
  unsigned int dimension_;
  Node * node_;
  mutable Adaptive_Cubical_Cell dereference_value;
  const Adaptive_Cubical_Container * referral_;
  unsigned long type_;
  void next_type ( void );
};

/** class Adaptive_Cubical_Complex **/
class Adaptive_Cubical_Complex : public Cell_Complex_Archetype < Adaptive_Cubical_Container > {
public:
  /** addCube. Add the cell and also those cells which comprise its closure. */
  void addCube ( const Cell & cell );
  /** addCell. Add the cell. Subdivide other cells as necessary. */
  void addCell ( const Cell & cell );

};

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Adaptive_Cubical_Complex.cpp"
#endif

#endif
