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

#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for Cell_Complex */

/* Forward Declarations */
class Adaptive_Cubical_const_iterator;
class Adaptive_Cubical_Container;
class Adaptive_Cubical_Complex;

/** class Adaptive_Cubical_Container. **/
class Adaptive_Cubical_Container {
public:
  /* typedefs */
  typedef Default_Ring Ring;
  typedef Default_Cell Cell;
  typedef Default_Chain Chain;
  typedef size_t size_type;
  typedef value_type Cell;
  typedef key_type Cell;
  typedef Adaptive_Cubical_const_iterator const_iterator;
  typedef const_iterator iterator;
  
  /* Constructors */
  Adaptive_Cubical_Container ( void );
  Adaptive_Cubical_Container ( unsigned int dimension );
  ~Adaptive_Cubical_Container ( void );
  
  /* "Simple Associative Container" concept (just the basics)*/
  iterator find ( const key_type & );
  void erase ( iterator erase_me );
  
  /* "Cell Container" concept */
  iterator begin ( unsigned int dimension );
  iterator end ( unsigned int dimension );
  size_type size ( unsigned int dimension ) const;
  Chain boundary ( const const_iterator & input );
  Chain coboundary ( const const_iterator & input );
  unsigned int dimension ( void ) const;
  
private:
  /* Friends */
  friend class Adaptive_Cubical_Complex;
  friend class Adaptive_Cubical_const_iterator;
  /* Data */
  unsigned int dimension_;
  std::vector < Node * > tree_data_;
  
};

/** class Adaptive_Cubical_const_iterator **/
class Adaptive_Cubical_const_iterator {
public:
  Adaptive_Cubical_const_iterator ( void );  
  Adaptive_Cubical_const_iterator ( const Adapative_Cubical_Container * const referral );  
  Adaptive_Cubical_const_iterator & operator ++ ( void );
  bool operator != ( const Adaptive_Cubical_const_iterator & right_hand_side ) const;
  bool operator == ( const Adaptive_Cubical_const_iterator & right_hand_side ) const;
  bool operator < ( const Adaptive_Cubical_const_iterator & right_hand_side ) const;
  const Adaptive_Cubical_Container::value_type & operator * ( void ) const;
  const Adaptive_Cubical_Container::value_type * operator -> ( void ) const;
private:
  Node * leaf_;
  const Adapative_Cubical_Container * referral_;
  unsigned long type_;
};

/** class Adaptive_Cubical_Complex **/
class Adaptive_Cubical_Complex : public Cell_Complex_Archetype < Adaptive_Cubical_Container > {};

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Adaptive_Cubical_Complex.cpp"
#endif

#endif
