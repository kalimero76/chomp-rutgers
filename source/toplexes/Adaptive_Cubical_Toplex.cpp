/*
 *  Adaptive_Cubical_Toplex.cpp
 *  
 *
 *  Created by Shaun Harker on 5/26/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "Adaptive_Cubical_Toplex.h"

namespace Adaptive_Cubical {
  
  /* * * * * * * * * * * * * * * * *
   * struct Adaptive_Cubical::Node *
   * * * * * * * * * * * * * * * * */
  Node::Node ( void ) {
    left_ = right_ = NULL;
  } /* Adaptive_Cubical::Node::Node */
  
  Node::~Node ( void ) {
    if ( left_ != NULL ) delete left_;
    if ( right_ != NULL ) delete right_;
  } /* Adaptive_Cubical::Node::~Node */
  
  /* * * * * * * * * * * * * * * * * * * * *
   * class Adaptive_Cubical::Toplex_Subset *
   * * * * * * * * * * * * * * * * * * * * */
  std::pair<Toplex_Subset::iterator, bool> Toplex_Subset::insert ( const value_type & insert_me ) {
  } /* Adaptive_Cubical::Toplex_Subset::insert */
  
  void Toplex_Subset::erase ( const iterator & erase_me ) {
  } /* Adaptive_Cubical::Toplex_Subset::erase */
  
  void Toplex_Subset::clear ( void ) {
  } /* Adaptive_Cubical::Toplex_Subset::clear */
  
  Toplex_Subset::iterator Toplex_Subset::find ( const key_type & find_me ) const {
  } /* Adaptive_Cubical::Toplex_Subset::find */
  
  Toplex_Subset::iterator Toplex_Subset::begin ( void ) const {
  } /* Adaptive_Cubical::Toplex_Subset::begin */
  
  Toplex_Subset::iterator Toplex_Subset::end ( void ) const {
  } /* Adaptive_Cubical::Toplex_Subset::end */
  
  Toplex_Subset::size_type Toplex_Subset::size ( void ) const {
  } /* Adaptive_Cubical::Toplex_Subset::size */
  
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * class Adaptive_Cubical::Toplex_Subset_const_iterator  *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  Toplex_Subset_const_iterator::Toplex_Subset_const_iterator ( void ) {
  } /* Adaptive_Cubical::Toplex_Subset_const_iterator::Toplex_Subset_const_iterator */
  
  Toplex_Subset_const_iterator & Toplex_Subset_const_iterator::operator ++ ( void ) {
  } /* Adaptive_Cubical::Toplex_Subset_const_iterator::operator ++ */
  
  bool Toplex_Subset_const_iterator::operator != ( const Toplex_Subset_const_iterator & right_hand_side ) const {
  } /* Adaptive_Cubical::Toplex_Subset_const_iterator::operator != */
  
  bool Toplex_Subset_const_iterator::operator == ( const Toplex_Subset_const_iterator & right_hand_side ) const {
  } /* Adaptive_Cubical::Toplex_Subset_const_iterator::operator == */
  
  bool Toplex_Subset_const_iterator::operator < ( const Toplex_Subset_const_iterator & right_hand_side ) const {
  } /* Adaptive_Cubical::Toplex_Subset_const_iterator::operator < */
  
  const Adaptive_Cubical::Top_Cell & Toplex_Subset_const_iterator::operator * ( void ) const {
  } /* Adaptive_Cubical::Toplex_Subset_const_iterator::operator * */
 
  /* * * * * * * * * * * * * * * * * *
   * class Adaptive_Cubical::Toplex  *
   * * * * * * * * * * * * * * * * * */
  void Toplex::erase ( const iterator & erase_me ) {
  } /* Adaptive_Cubical::Toplex::erase */
  
  void Toplex::clear ( void ) {
  } /* Adaptive_Cubical::Toplex::clear */
  
  Toplex::iterator Toplex::find ( const key_type & find_me ) const {
  } /* Adaptive_Cubical::Toplex::find */
  
  Toplex::iterator Toplex::begin ( void ) const {
  } /* Adaptive_Cubical::Toplex::begin */
  
  Toplex::iterator Toplex::end ( void ) const {
  } /* Adaptive_Cubical::Toplex::end */
  
  Toplex::size_type Toplex::size ( void ) const {
  } /* Adaptive_Cubical::Toplex::size */
  
  Toplex::Subset Toplex::cover ( const Geometric_Description & geometric_region ) const {
  } /* Adaptive_Cubical::Toplex::cover */
  
  Toplex::Geometric_Description Toplex::getGeometricDescription ( const Top_Cell & cell ) const {
  } /* Adaptive_Cubical::Toplex::getGeometricDescription */
  
  Toplex::Complex Toplex::complex ( const Subset & subset_of_toplex ) const {
  } /* Adaptive_Cubical::Toplex::complex */
  
  Toplex::Subset Toplex::subdivide ( const iterator & cell_to_divide ) {
  } /* Adaptive_Cubical::Toplex::subdivide */
  
  Toplex::Subset Toplex::children ( const const_iterator & node ) const {
  } /* Adaptive_Cubical::Toplex::children */
  
  void Toplex::initialize ( const Geometric_Description & outer_bounds_of_toplex ) {
  } /* Adaptive_Cubical::Toplex::initialize */
  

  /* * * * * * * * * * * * * * * * * * * * * * * * *
   * class Adaptive_Cubical::Toplex_const_iterator  *
   * * * * * * * * * * * * * * * * * * * * * * * * */
  Toplex_const_iterator::Toplex_const_iterator ( void ) {
  } /* Adaptive_Cubical::Toplex_const_iterator::Toplex_const_iterator */
  
  Toplex_const_iterator::Toplex_const_iterator & operator ++ ( void ) {
  } /* Adaptive_Cubical::Toplex_const_iterator::operator ++ */
  
  bool Toplex_const_iterator::operator != ( const Toplex_const_iterator & right_hand_side ) const {
  } /* Adaptive_Cubical::Toplex_const_iterator::operator != */
  
  bool Toplex_const_iterator::operator == ( const Toplex_const_iterator & right_hand_side ) const {
  } /* Adaptive_Cubical::Toplex_const_iterator::operator == */
  
  bool Toplex_const_iterator::operator < ( const Toplex_const_iterator & right_hand_side ) const {
  } /* Adaptive_Cubical::Toplex_const_iterator::operator < */
  
  const Adaptive_Cubical::Top_Cell & Toplex_const_iterator::operator * ( void ) const {
  } /* Adaptive_Cubical::Toplex_const_iterator::operator * */

} /* namespace Adaptive_Cubical */
