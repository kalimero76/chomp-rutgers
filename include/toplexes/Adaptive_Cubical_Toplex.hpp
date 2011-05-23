/*
 *  Adaptive_Cubical_Toplex.hpp
 *  
 *
 *  Created by Shaun Harker on 5/26/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include <deque>
#include <vector>
#include <utility>
#include "boost/foreach.hpp"


template < class Set >
Set intersect ( const Set & A, const Set & B ) {
  /* Generic implementation, assumes unordered sets */
  Set return_value;
  for ( typename Set::const_iterator it = A . begin (); 
       it != A . end (); ++ it ) {
    typename Set::value_type element = * it;
    if ( B . find ( element ) != B . end () ) return_value . insert ( element );
  } /* for */
  return return_value;
} /* intersect */

namespace Adaptive_Cubical {
  
  template < class InsertIterator >
  void Toplex::cover ( InsertIterator & ii, const Geometric_Description & geometric_region ) const {
    
    /* Use a stack, not a queue, and do depth first search.
     The advantage of this is that we can maintain the geometry during our Euler Tour.
     We can maintain our geometry without any roundoff error if we use the standard box
     [0,1]^d. To avoid having to translate to real coordinates at each leaf, we instead
     convert the input to these standard coordinates, which we put into integers. */
    
    // Step 1. Convert input to standard coordinates. 
    Geometric_Description region ( dimension_ );
    static std::vector<unsigned long> LB ( dimension_);
    static std::vector<unsigned long> UB ( dimension_);
#define INTPHASEWIDTH (((unsigned long)1) << 60)
    static Real bignum ( INTPHASEWIDTH );
    for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) {
      region . lower_bounds [ dimension_index ] = 
      (geometric_region . lower_bounds [ dimension_index ] - bounds_ . lower_bounds [ dimension_index ]) /
      (bounds_ . upper_bounds [ dimension_index ] - bounds_ . lower_bounds [ dimension_index ]);
      region . upper_bounds [ dimension_index ] = 
      (geometric_region . upper_bounds [ dimension_index ] - bounds_ . lower_bounds [ dimension_index ]) /
      (bounds_ . upper_bounds [ dimension_index ] - bounds_ . lower_bounds [ dimension_index ]);
      if ( region . lower_bounds [ dimension_index ] < Real ( 0 ) ) 
        region . lower_bounds [ dimension_index ] = Real ( 0 );
      if ( region . lower_bounds [ dimension_index ] > Real ( 1 ) ) 
        region . lower_bounds [ dimension_index ] = Real ( 1 );
      LB [ dimension_index ] = (unsigned long) ( bignum * region . lower_bounds [ dimension_index ] );
      if ( region . upper_bounds [ dimension_index ] < Real ( 0 ) ) 
        region . upper_bounds [ dimension_index ] = Real ( 0 );
      if ( region . upper_bounds [ dimension_index ] > Real ( 1 ) ) 
        region . upper_bounds [ dimension_index ] = Real ( 1 );
      UB [ dimension_index ] = (unsigned long) ( bignum * region . upper_bounds [ dimension_index ] );
    }
    
    // Step 2. Perform DFS on the Toplex tree, recursing whenever we have intersection,
    //         (or adding leaf to output when we have leaf intersection)
    static std::vector<unsigned long> NLB ( dimension_);
    static std::vector<unsigned long> NUB ( dimension_);
    for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) {
      //if ( LB [ dimension_index ] > 0 ) -- LB [ dimension_index ];
      //if ( UB [ dimension_index ] < INTPHASEWIDTH ) ++ UB [ dimension_index ];
      NLB [ dimension_index ] = 0;
      NUB [ dimension_index ] = INTPHASEWIDTH;
    }
    //std::cout << "C\n";
    
    /* Strategy. 
     We will take the Euler Tour using a 4-state machine.
     There are Four states.
     0 = Just Descended. Check for an intersection.
     1 = Descend to the left
     2 = Descend to right 
     3 = Rise.
     */
    
    Node * N = root_;
    char state = 0; 
    
    //std::cout << "D\n";
    
    while ( 1 ) {
      //std::cout << "Entering Loop, state = " << (int) state << "\n";
      //std::cout << " N = " << N << "\n";
      if ( state == 0 ) {
        // If we have descended here, then we should check for intersection.
        bool intersect_flag = true;
        for ( unsigned int d = 0; d < dimension_; ++ d ) {
          if ( LB[d] > NUB[d] || UB[d] < NLB [d] ) {  // INTERSECTION CHECK
            intersect_flag = false;
            break;
          }
        }
        
        if ( intersect_flag ) {
          //std::cout << "Detected intersection.\n";
          // Check if its a leaf.
          if ( N -> left_ == NULL ) {
            if ( N -> right_ == NULL ) {
              // Here's what we are looking for.
              * ii ++ = N -> contents_; // OUTPUT
              // Issue the order to rise.
              //std::cout << "Issue rise.\n";
              state = 3;
            } else {
              // Issue the order to descend to the right.
              //std::cout << "Issue descend right.\n";
              state = 2;
            } 
          } else {
            // Issue the order to descend to the left.   
            //std::cout << "Issue descend left.\n";
            state = 1;
          }
        } else {
          // No intersection, issue order to rise.
          //std::cout << "No intersection. \n";
          //std::cout << "Issue Rise.\n";
          state = 3;
        } // intersection check complete
      } // state 0
      
      if ( state == 1 ) {
        // We have been ordered to descend to the left.
        //std::cout << "Descend left.\n";
        unsigned int div_dim = N -> dimension_;
        NUB[div_dim] -= ( (NUB[div_dim]-NLB[div_dim]) >> 1 );
        N = N -> left_;
        state = 0;
        continue;
      } // state 1
      
      if ( state == 2 ) {
        // We have been ordered to descend to the right.
        //std::cout << "Descend right.\n";
        unsigned int & div_dim = N -> dimension_;
        NLB[div_dim] += ( (NUB[div_dim]-NLB[div_dim]) >> 1 );
        N = N -> right_;
        state = 0;
        continue;
      } // state 2
      
      if ( state == 3 ) {
        // We have been ordered to rise.
        //std::cout << "Rise.\n";
        Node * P = N -> parent_;
        // Can't rise if root.
        if ( P == NULL ) break; // algorithm complete
        unsigned int & div_dim = P -> dimension_;
        if ( P -> left_ == N ) {
          // This is a left child.
          //std::cout << "We are rising from left.\n";
          NUB[div_dim] += NUB[div_dim]-NLB[div_dim];
          // If we rise from the left child, we order parent to go right.
          state = 2;
        } else {
          // This is the right child.
          //std::cout << "We are rising from right.\n";
          NLB[div_dim] -= NUB[div_dim]-NLB[div_dim];
          // If we rise from the right child, we order parent to rise.
          state = 3;
        }
        N = P;
      } // state 3
      
    } // while loop
  } // cover
  
  template < class InsertIterator >
  void Toplex::subdivide ( InsertIterator & ii, iterator cell_to_divide ) {
    std::deque < std::pair < const_iterator, unsigned int > > work_deque;
    work_deque . push_back ( std::pair < const_iterator, unsigned int >
                            (cell_to_divide,
                             cell_to_divide . node () -> dimension_ ) );
    while ( not work_deque . empty () ) {
      std::pair < const_iterator, unsigned int >  work_pair = work_deque . front ();
      work_deque . pop_front ();
      if ( work_pair . second < dimension_ ) {
        work_pair . first . node_ -> dimension_ = work_pair . second;
        /* We must subdivide further */
        work_pair . first . node_ -> left_ = new Node;
        work_pair . first . node_ -> right_ = new Node;
        /* Update begin_, size_, tree_size_, find_ and initialize new nodes.*/
        if ( begin_ == work_pair . first . node_ )
          begin_ . node_ = work_pair . first . node_ -> left_;
        ++ size_;
        work_pair . first . node_ -> left_ -> contents_ = tree_size_ ++;
        work_pair . first . node_ -> left_ -> parent_ = work_pair . first . node_;
        find_ . push_back ( const_iterator ( work_pair . first . node_ -> left_ ) );
        work_pair . first . node_ -> right_ -> contents_ = tree_size_ ++;
        work_pair . first . node_ -> right_ -> parent_ = work_pair . first . node_;
        find_ . push_back ( const_iterator ( work_pair . first . node_ -> right_ ) );
        /* Push the children onto the work_deque */
        work_deque . push_back ( std::pair < const_iterator, unsigned int >
                                (const_iterator ( work_pair . first . node_ -> left_ ),
                                 work_pair . second + 1 ) );
        work_deque . push_back ( std::pair < const_iterator, unsigned int >
                                (const_iterator ( work_pair . first . node_ -> right_ ),
                                 work_pair . second + 1 ) );
      } else {
        work_pair . first . node_ -> dimension_ = 0;
        //std::cout << "subdivide: inserting " << work_pair . first . node_ -> contents_ << "\n";
        * ii ++ = work_pair . first . node_ -> contents_;
      } /* if-else */
    } /* while */
  } /* Adaptive_Cubical::Toplex::subdivide */
  
  template < class InsertIterator, class Container >
  void Toplex::subdivide ( InsertIterator & ii, const Container & subset_to_divide ) {
    BOOST_FOREACH ( Top_Cell cell, subset_to_divide ) {
      subdivide ( ii, find ( cell ) );
    }
  }
  
#if 0   //not until previous versions depracated.
  template < class InsertIterator >
  void Toplex::subdivide ( InsertIterator & ii ) {
    // TODO: this is fine if you are only using it for the initial subdivision.
    //  But in theory you shouldn't be relying on Toplex::Subset, which I am trying to
    //  deprecate.
      subdivide ( ii, cover ( bounds () ) );
  }
#endif
  
  template < class InputIterator >
  void Toplex::complex ( Complex * & return_value, 
                InputIterator start, InputIterator stop, // begin and end of subset container
                std::map < Top_Cell, Complex::const_iterator > & boxes ) const {
    return_value = new Complex ( dimension_);
    /* Warning: will fail if there are leafs which are not perfect cubes */
    /* Warning: assumes iteration pattern through top cells of both complex and toplex are same */
    /* Now put the complex together */
    std::for_each ( start, stop, 
                   detail::AddTopCell ( boxes, *return_value, *this ) );
    return_value -> Finalize ();
  }
  
} /* namespace Adapative_Cubical */