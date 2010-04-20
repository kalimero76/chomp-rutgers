/*
 *  Adaptive_Complex.cpp
 *
 *
 *  Created by Shaun Harker 4/19/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "complexes/Adaptive_Cubical_Complex.h"

/* * * * * * * * * * * * * * * * * * * * * *
 * Adaptive_Cubical_Container definitions  *
 * * * * * * * * * * * * * * * * * * * * * */

iterator Adaptive_Cubical_Container::begin ( unsigned int dimension ) {
} /* Adaptive_Cubical_Container::begin */

iterator Adaptive_Cubical_Container::end ( unsigned int dimension ) {
} /* Adaptive_Cubical_Container::end */

size_type Adaptive_Cubical_Container::size ( unsigned int dimension ) const {
} /* Adaptive_Cubical_Container::size */

Chain Adaptive_Cubical_Container::boundary ( const const_iterator & input ) {
  /* Algorithm Description: 
   * Define cell := *input
   * For each uncollapsed dimension, collapse cell in both possible ways,
   * obtaining an up_cell and a down_cell. 
   *   Find both up_cell and down_cell in their respective trees, 
   *     obtaining up_node and down_node.
   *   Add all descendants of up_node and down_node to the chain. 
   * end
   */
  Cell cell = *input;
  for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) {
    if ( cell . type [ dimension_index ] == false ) continue; // collapsed dimension, ignore.
    
  }
} /* Adaptive_Cubical_Container::boundary */

Chain Adaptive_Cubical_Container::coboundary ( const const_iterator & input ) {
  /* Algorithm Description: 
   * Define cell := *input.
   * Consider the type of the cell. (Which dimensions are collapsed?)
   * For each collapsed dimension, produce the corresponding type
   * with one more dimension. 
   *   In the storage tree for this type, find all leaves which
   *   represent cells whose closure contains "cell"
   *   Add these leaves to the chain with appropriate coefficients. 
   * end
   */
} /* Adaptive_Cubical_Container::coboundary */

unsigned int Adaptive_Cubical_Container::dimension ( void ) const {
} /* Adaptive_Cubical_Container::dimension */

/* * * * * * * * * * * * * * * * * * * * * * * *
 * Adaptive_Cubical_const_iterator definitions *
 * * * * * * * * * * * * * * * * * * * * * * * */

Adaptive_Cubical_const_iterator::Adaptive_Cubical_const_iterator ( void ) {
} /* Adaptive_Cubical_const_iterator::Adaptive_Cubical_const_iterator */

Adaptive_Cubical_const_iterator & Adaptive_Cubical_const_iterator::operator ++ ( void ) {
} /* Adaptive_Cubical_const_iterator::operator ++ */

bool Adaptive_Cubical_const_iterator::operator != ( const Adaptive_Cubical_const_iterator & right_hand_side ) const {
  return leaf_ != right_hand_side . leaf_;
} /* Adaptive_Cubical_const_iterator::operator != */

bool Adaptive_Cubical_const_iterator::operator == ( const Adaptive_Cubical_const_iterator & right_hand_side ) const {
  return leaf_ == right_hand_side . leaf_;
} /* Adaptive_Cubical_const_iterator::operator == */

bool Adaptive_Cubical_const_iterator::operator < ( const Adaptive_Cubical_const_iterator & right_hand_side ) const {
  return leaf_ < right_hand_side . leaf_;
} /* Adaptive_Cubical_const_iterator::operator < */

const Adaptive_Cubical_Container::value_type & Adaptive_Cubical_const_iterator::operator * ( void ) const {
} /* Adaptive_Cubical_const_iterator::operator * */

const Adaptive_Cubical_Container::value_type * Adaptive_Cubical_const_iterator::operator -> ( void ) const {
} /* Adaptive_Cubical_const_iterator::operator -> */

#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */
#endif
