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

Adaptive_Cubical_Container::Adaptive_Cubical_Container ( unsigned int dimension ) : dimension_(dimension) {
  tree_data_ . resize ( 1 << dimension_, NULL );
  /* TODO */
}

Adaptive_Cubical_Container::~Adaptive_Cubical_Container ( void ) {
  /* TODO */
}
                           
Adaptive_Cubical_Container::iterator Adaptive_Cubical_Container::find ( const key_type & find_me ) {
  iterator return_value;
  if ( return_value . node_ = tree_data_ [ find_me . type_ ] == NULL ) return end_ [ dimension_ ];
  for ( unsigned index = 0; index < find_me . pathLength (); ++ index ) {
    return_value . node_ = find_me . splittingDirection ( index ) ? return_value . node_ -> right :
                           return_value . node_ -> left;
  } /* for */
  return_value . type_ = find_me . type_;
  return return_value;
} /* Adaptive_Cubical_Container::find */

void Adaptive_Cubical_Container::erase ( const iterator & erase_me ) {
  /* Assuming it's a leaf... */
  Node * delete_me = erase_me . node_;
  Node * parent = delete_me -> parent;
  /* Climb and erase the tree until we reach a node with another unpruned branch. */
  while ( parent != NULL ) {
    delete_me == parent -> left ? parent -> left = NULL : parent -> right = NULL;
    if ( parent -> left != parent -> right ) break; 
    delete delete_me;
    delete_me = parent;
    parent = delete_me -> parent;
  }
  delete delete_me;
  /* Update end ( dimension - 1) and begin ( dimension ) if necessary */
  if ( erase_me == begin_ [ erase_me . dimension_ ] ) {
    ++ begin_ [ erase_me . dimension_ ];
    if ( erase_me . dimension_ > 0 ) 
      end_ [ erase_me . dimension - 1 ] = begin_ [ erase_me . dimension_ ];
  } /* if */
  /* Update size ( dimension ) */
  -- size_ [ erase_me . dimension ];
  
} /* Adaptive_Cubical_Container::erase */

Adaptive_Cubical_Container::iterator Adaptive_Cubical_Container::begin ( unsigned int dimension ) {
  return begin_ [ dimension ];
} /* Adaptive_Cubical_Container::begin */

Adaptive_Cubical_Container::iterator Adaptive_Cubical_Container::end ( unsigned int dimension ) {
  return end_ [ dimension ];
} /* Adaptive_Cubical_Container::end */

Adaptive_Cubical_Container::size_type Adaptive_Cubical_Container::size ( unsigned int dimension ) const {
  return size_ [ dimension ];
} /* Adaptive_Cubical_Container::size */

Adaptive_Cubical_Container::Chain Adaptive_Cubical_Container::boundary ( const const_iterator & input ) {
  /* Algorithm Description: 
   * Define cell := *input
   * For each uncollapsed dimension, collapse type
   *   Find both up_cell and down_cell in their respective trees, 
   *     obtaining up_node and down_node.
   *   Add all descendants of up_node and down_node to the chain. 
   * end
   */
  Chain output;
  for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) {
    if ( not input . type & 1 << dimension_index ) continue; // collapsed dimension, ignore.
    Cell cell = *input;
    /* Collapse the dimension */
    cell . type ^= 1 << dimension_index;
    /* Get book-keeping structures ready. */
    std::stack < std::pair < const_node_iterator, unsigned int > > work_stack;
    std::pair < const_node_iterator, unsigned int > work_item;
    const_node_iterator & node = work_item . first;
    unsigned int & index = work_item . second;
    
    /* Find the left-matching cells. */
    node = tree_data_ [ cell . type ] . root ();
    index = 0;

    /* Set node to the root of the appropriate tree */
    node = tree_data_ [ cell . type ] . root ();
    index = 0;
    while ( 1 ) {
      /* Does this node split, or is it a leaf? */
      if ( node -> splitting_dimension == dimension_ ) { /* It is a leaf. */
        /* Store into the boundary chain. */
        output += Chain::Chain_Term ( node, coefficient );
        /* Is there any work left? */
        if ( work_stack . empty () ) break;
        /* Get next work item and continue. */
        work_item = work_stack . top ();
        work_stack . pop ();
        /* We need to go down the right branch. */
        node . goRight ();
        continue; 
      }
      if ( node -> splitting_dimension == cell . splitting_dimension ( index ) ) { 
        /* Follow the path. */
        cell . splitting_direction ( index ) ? node . goRight () : node . goLeft ();
        ++ index;
      } else { /* It must be a split. */
        /* Store a work item. */
        work_stack . push ( std::pair < const_node_iterator, unsigned int > ( node, index ) );
        /* Continue processing down left branch. */
        node . goLeft ();
      }
    }
    
    /* Find the right-matching cells. */
    /* First we alter cell so that the final split on dimension_index to the left
     * is switched to the right */
    for ( int path_index = cell . pathLength (); path_index >= 0; -- path_index ) {
      if ( cell . splittingDimension ( path_index ) == dimension_index &&
          cell . splittingDirection ( path_index ) == false ) {
        cell . splittingDirection ( path_index ) = true;
        break;
      } /* if */
    } /* for */
    
    while ( 1 ) {
      /* Does this node split, or is it a leaf? */
      if ( node -> splitting_dimension == dimension_ ) { /* It is a leaf. */
        /* Store into the boundary chain. */
        output += Chain::Chain_Term ( node, coefficient );
        /* Is there any work left? */
        if ( work_stack . empty () ) break;
        /* Get next work item and continue. */
        work_item = work_stack . top ();
        work_stack . pop ();
        /* We need to go down the right branch. */
        node . goRight ();
        continue; 
      }
      if ( node -> splitting_dimension == cell . splitting_dimension ( index ) ) { 
        /* Follow the path. */
        cell . splitting_direction ( index ) ? node . goRight () : node . goLeft ();
        ++ index;
      } else { 
        if ( node -> splitting_dimension == dimension_index ) {
          /* Then we should go left. */
          node . goLeft ();
        } else {
          /* It must be a split. */
          /* Store a work item. */
          work_stack . push ( std::pair < const_node_iterator, unsigned int > ( node, index ) );
          /* Continue processing down left branch. */
          node . goLeft ();
        } /* if-else */
      } /* if-else */
    } /* while */
  } /* for */
  return output;
} /* Adaptive_Cubical_Container::boundary */

Adaptive_Cubical_Container::Chain Adaptive_Cubical_Container::coboundary ( const const_iterator & input ) {
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
  Chain output;
  return output;
} /* Adaptive_Cubical_Container::coboundary */

unsigned int Adaptive_Cubical_Container::dimension ( void ) const {
  return dimension_;
} /* Adaptive_Cubical_Container::dimension */

/* * * * * * * * * * * * * * * * * * * * * * * *
 * Adaptive_Cubical_const_iterator definitions *
 * * * * * * * * * * * * * * * * * * * * * * * */

Adaptive_Cubical_const_iterator::Adaptive_Cubical_const_iterator ( void ) {
} /* Adaptive_Cubical_const_iterator::Adaptive_Cubical_const_iterator */

Adaptive_Cubical_const_iterator ( const Adapative_Cubical_Container * const referral ) : referral_(referral) {
} /* Adaptive_Cubical_const_iterator::Adaptive_Cubical_const_iterator */

Adaptive_Cubical_const_iterator & Adaptive_Cubical_const_iterator::operator ++ ( void ) {
  Node * parent; 
  Node * child = node_;
  /* Ascend until we have ascended from the left */
  while ( parent = child -> up != NULL && child == parent -> right );
  /* Deal with the three possibilities: */
  if ( parent == NULL ) {        // Possibility 1. This type is exhausted.
    next_type ();
    if ( node_ == NULL ) return; // Possibility 2. We have reached end().
  } else {
    node_ = parent -> right;     // Possibility 3. Normal operation. Go right once.
  } /* if-else */
  /* Descend left until we reach a leaf. */
  while ( node_ -> left != NULL ) node_ = node_ -> left; 
} /* Adaptive_Cubical_const_iterator::operator ++ */

bool Adaptive_Cubical_const_iterator::operator != ( const Adaptive_Cubical_const_iterator & right_hand_side ) const {
  return node_ != right_hand_side . node_;
} /* Adaptive_Cubical_const_iterator::operator != */

bool Adaptive_Cubical_const_iterator::operator == ( const Adaptive_Cubical_const_iterator & right_hand_side ) const {
  return node_ == right_hand_side . node_;
} /* Adaptive_Cubical_const_iterator::operator == */

bool Adaptive_Cubical_const_iterator::operator < ( const Adaptive_Cubical_const_iterator & right_hand_side ) const {
  return node_ < right_hand_side . node_;
} /* Adaptive_Cubical_const_iterator::operator < */

const Adaptive_Cubical_Container::value_type & Adaptive_Cubical_const_iterator::operator * ( void ) const {
  dereference_value . type = type_;
  Node * parent; 
  Node * child = node_;
  while ( parent = child -> up != NULL ) {
    dereference_value . splitting_direction . push_front ( child == parent -> right );
    dereference_value . splitting_dimension . push_front ( parent -> splitting_dimension );
  }
  return & dereference_value;
} /* Adaptive_Cubical_const_iterator::operator * */

const Adaptive_Cubical_Container::value_type * Adaptive_Cubical_const_iterator::operator -> ( void ) const {
  return & * *this;
} /* Adaptive_Cubical_const_iterator::operator -> */

void Adaptive_Cubical_const_iterator::next_type ( void ) {
  const unsigned long maximum = 1 << referral -> dimension_ ; 
  const unsigned long dimension = 0;
  unsigned long temp = type_;
  for ( unsigned int bit = 0; bit < referral -> dimension_; ++ bit ) {
    dimension += temp % 2;
    temp = temp >> 1; 
  } /* for */
  
  while ( 1 ) {
		type ++ ;
    /* See if dimension is exhausted */
		if ( type_ >= maximum ) {
      type_ = 0;
			++ dimension;
      /* See if all dimensions are exhausted */
      if ( dimension > referral -> dimension_ ) {
        node_ = NULL; // This is end(). 
        return;
      }
    }
		unsigned long temp = type_;
		unsigned int sum = 0;
		for ( unsigned int bit = 0; bit < referral -> dimension_; ++ bit ) {
			sum += temp % 2;
			temp = temp >> 1; 
    } /* for */
		if ( sum == dimension ) {
      /* Check to see if tree is non-empty */
      if ( not referral -> tree_data_ [ type_ ] . empty () ) {
        node_ = referral -> tree_data_ [ type_ ] . root ();
        return;
      }
    }
  } 
} /* Adaptive_Cubical_const_iterator::next_type */

#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */
#endif
