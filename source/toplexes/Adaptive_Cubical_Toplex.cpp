/*
 *  Adaptive_Cubical_Toplex.cpp
 *  
 *
 *  Created by Shaun Harker on 5/26/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include <deque>
#include <algorithm>

#include "toplexes/Adaptive_Cubical_Toplex.h"

namespace Adaptive_Cubical {
  
  /* * * * * * * * * * * * * * * * * * * * * * * * * *
   * struct Adaptive_Cubical::Geometric_Description  *
   * * * * * * * * * * * * * * * * * * * * * * * * * */
  Geometric_Description::Geometric_Description ( void ) {
  } /* Adaptive_Cubical::Geometric_Description::Geometric_Description */
  
  Geometric_Description::Geometric_Description ( unsigned int size ) 
  : lower_bounds ( size ), upper_bounds ( size ) {
  } /* Adaptive_Cubical::Geometric_Description::Geometric_Description */

  Geometric_Description::Geometric_Description ( unsigned int size, const Real & value ) 
  : lower_bounds ( size, value ), upper_bounds ( size, value ) {
  } /* Adaptive_Cubical::Geometric_Description::Geometric_Description */

  Geometric_Description::Geometric_Description ( unsigned int size, const Real & lower_value, const Real & upper_value ) 
  : lower_bounds ( size, lower_value ), upper_bounds ( size, upper_value ) {
  } /* Adaptive_Cubical::Geometric_Description::Geometric_Description */
  
  bool Geometric_Description::intersects ( const Geometric_Description & other ) const {
    for ( unsigned int dimension_index = 0; dimension_index < lower_bounds . size (); ++ dimension_index ) {
      if ( upper_bounds [ dimension_index ] < other . lower_bounds [ dimension_index ] ||
          other . upper_bounds [ dimension_index ] < lower_bounds [ dimension_index ] ) {
        return false;
      } /* if */
    } /* for */
    return true;
  } /* Adaptive_Cubical::Geometric_Description::intersects */
  
  std::ostream & operator << ( std::ostream & output_stream, const Geometric_Description & print_me ) {
    for ( unsigned int dimension_index = 0; dimension_index < print_me . lower_bounds . size (); ++ dimension_index ) {
      output_stream << "[" << print_me . lower_bounds [ dimension_index ] << ", " << print_me . upper_bounds [ dimension_index ] << "]";
      if ( dimension_index < print_me . lower_bounds . size () - 1 ) output_stream << "x";
    }
    return output_stream;
  } /* Adaptive_Cubical::operator << */

  /* * * * * * * * * * * * * * * * *
   * struct Adaptive_Cubical::Node *
   * * * * * * * * * * * * * * * * */
  Node::Node ( void ) : left_ ( NULL ), right_ ( NULL ) {
  } /* Adaptive_Cubical::Node::Node */
  
  Node::~Node ( void ) {
    if ( left_ != NULL ) delete left_;
    if ( right_ != NULL ) delete right_;
  } /* Adaptive_Cubical::Node::~Node */

  /* * * * * * * * * * * * * * * * * * * * * * * * *
   * class Adaptive_Cubical::Toplex_const_iterator  *
   * * * * * * * * * * * * * * * * * * * * * * * * */
  Toplex_const_iterator::Toplex_const_iterator ( void ) {
  } /* Adaptive_Cubical::Toplex_const_iterator::Toplex_const_iterator */
  
  Toplex_const_iterator::Toplex_const_iterator ( Node * node ) : node_ ( node ) {
  } /* Adaptive_Cubical::Toplex_const_iterator::Toplex_const_iterator */
  
  Toplex_const_iterator & Toplex_const_iterator::operator ++ ( void ) {
    /* Go up until either root is reached, or node is a left child of a parent possessing a right child as well */
    while (  node_ -> parent_ != NULL && ( node_ -> parent_ -> left_ != node_ || node_ -> parent_ -> right_ == NULL ) ) node_ = node_ -> parent_;
    /* If root has been reached, return NULL */
    if ( node_ -> parent_ == NULL ) {
      node_ = NULL;
      return *this;
    } /* if */
    /* Move to right child of parent */
    node_ = node_ -> parent_ -> right_;
    /* Descend to a leaf, taking left route whenever possible */
    while ( 1 ) {
      if ( node_ -> left_ != NULL ) {
        node_ = node_ -> left_;
      } else if ( node_ -> right_ != NULL ) {
        node_ = node_ -> right_;
      } else break;
    } /* while */
    return *this;
  } /* Adaptive_Cubical::Toplex_const_iterator::operator ++ */
  
  bool Toplex_const_iterator::operator != ( const Toplex_const_iterator & right_hand_side ) const {
    return node_ != right_hand_side . node_;
  } /* Adaptive_Cubical::Toplex_const_iterator::operator != */
  
  bool Toplex_const_iterator::operator == ( const Toplex_const_iterator & right_hand_side ) const {
    return node_ == right_hand_side . node_;
  } /* Adaptive_Cubical::Toplex_const_iterator::operator == */
  
  bool Toplex_const_iterator::operator < ( const Toplex_const_iterator & right_hand_side ) const {
    return node_ < right_hand_side . node_; 
  } /* Adaptive_Cubical::Toplex_const_iterator::operator < */
  
  const Adaptive_Cubical::Top_Cell & Toplex_const_iterator::operator * ( void ) const {
    return node_ -> contents_;
  } /* Adaptive_Cubical::Toplex_const_iterator::operator * */
  
  Node * Toplex_const_iterator::node ( void ) {
    return node_;
  } /* Adaptive_Cubical::Toplex_const_iterator::node */

  /* * * * * * * * * * * * * * * * * *
   * class Adaptive_Cubical::Toplex  *
   * * * * * * * * * * * * * * * * * */
  void Toplex::erase ( iterator erase_me ) {
    /* Update begin_ if necessary */
    /* TODO: if this is being used to kill an entire subtree, not just a leaf, then
       this will not update begin_ properly. */
    /* TODO: if both children of a node are erased, erase also that node */
    if ( erase_me == begin_ ) ++ begin_;
    /* Disconnect from parent */
    Node * node = erase_me . node_;
    while ( 1 ) {
      Node * parent = node -> parent_;
      if ( parent == NULL ) break;  
      /* This is not the root: a parent exists. */
      if ( parent -> left_ == node ) {
        /* This is a left-child; disconnect accordingly. */
        parent -> left_ = NULL;
      } else {
        /* This is a right-child; disconnect accordingly. */
        parent -> right_ = NULL;
      } /* if-else */
      if ( parent -> left_ != parent -> right_ ) break;
      /* We will erase this node and move on to erase its parent */
      find_ [ node -> contents_ ] = end_;
      -- size_;
      delete node;
      node = parent;
    } /* while */
    /* Update find_ */
    find_ [ node -> contents_ ] = end_;
    /* Update size_ */
    -- size_;
    /* Deallocate the node */
    delete node;
  } /* Adaptive_Cubical::Toplex::erase */
  
  void Toplex::clear ( void ) {
    size_ = 0;
    tree_size_ = 0;
    find_ . clear ();
    begin_ = end_;
    if ( root_ != NULL ) delete root_;
  } /* Adaptive_Cubical::Toplex::clear */
  
  Toplex::iterator Toplex::find ( const key_type & find_me ) const {
    return find_ [ find_me ];
  } /* Adaptive_Cubical::Toplex::find */
  
  Toplex::iterator Toplex::begin ( void ) const {
    return begin_;
  } /* Adaptive_Cubical::Toplex::begin */
  
  Toplex::iterator Toplex::end ( void ) const {
    return end_;
  } /* Adaptive_Cubical::Toplex::end */
  
  Toplex::size_type Toplex::size ( void ) const {
    return size_;
  } /* Adaptive_Cubical::Toplex::size */
  
  unsigned int Toplex::dimension ( void ) const {
    return dimension_;
  } /* Adaptive_Cubical::Toplex::dimension */

  Toplex::Subset Toplex::cover ( const Geometric_Description & geometric_region ) const {
    /* Todo: optimize geometric checks, it is inefficient to call geometry at every level */
    Subset cover_set;
    std::deque < const_iterator > work_deque;
    work_deque . push_back ( const_iterator ( root_ ) );
    while ( not work_deque . empty () ) {
      const_iterator work_iterator = work_deque . front ();
      work_deque . pop_front ();
      /* Does the geometry of *work_iterator intersect the geometric_region? */
      if ( geometric_region . intersects ( geometry ( work_iterator ) ) ) {
        /* Is it a leaf? */
        bool leaf = true;
        if ( work_iterator . node_ -> left_ != NULL ) {
          work_deque . push_back ( const_iterator ( work_iterator . node_ -> left_ ) );
          leaf = false;
        } /* if */
        if ( work_iterator . node_ -> right_ != NULL ) {
          work_deque . push_back ( const_iterator ( work_iterator . node_ -> right_ ) );
          leaf = false;
        } /* if */
        if ( leaf ) {
          cover_set . insert ( * work_iterator );
        } /* if */
      };/* if */
    } /* while */
    return cover_set;
  } /* Adaptive_Cubical::Toplex::cover */
  
  Toplex::Geometric_Description Toplex::geometry ( const const_iterator & cell_iterator ) const {
    Geometric_Description return_value ( dimension_, Real ( 0 ) );
    //std::cout << "geometry of " << cell_iterator . node_ << " (" << cell_iterator . node_ -> contents_ << ")\n";
    //std::cout << "root = " << root_ << "\n";
    /* Climb the tree */
    Node * node_ptr = cell_iterator . node_;
    while ( node_ptr != root_ ) {
      //std::cout << "visiting " << node_ptr << " with parent " << node_ptr -> parent_ << "\n";
      Node * parent = node_ptr -> parent_;
      unsigned int division_dimension = parent -> dimension_;
      if ( parent -> left_ == node_ptr ) {
        /* This is a left-child */
        return_value . upper_bounds [ division_dimension ] += Real ( 1 );
      } else {
        /* This is a right-child */
        return_value . lower_bounds [ division_dimension ] += Real ( 1 );
      } /* if-else */
      return_value . lower_bounds [ division_dimension ] /= Real ( 2 );
      return_value . upper_bounds [ division_dimension ] /= Real ( 2 );
      node_ptr = parent;
    } /* while */
    for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) {
      /* Produce convex combinations */
      return_value . lower_bounds [ dimension_index ] = return_value . lower_bounds [ dimension_index ] * bounds_ . upper_bounds [ dimension_index ] +
      ( Real ( 1 ) - return_value . lower_bounds [ dimension_index ] ) * bounds_ . lower_bounds [ dimension_index ];
      return_value . upper_bounds [ dimension_index ] = return_value . upper_bounds [ dimension_index ] * bounds_ . lower_bounds [ dimension_index ] +
      ( Real ( 1 ) - return_value . upper_bounds [ dimension_index ] ) * bounds_ . upper_bounds [ dimension_index ];
    } /* for */
    //std::cout << "returning.\n";
    return return_value;
  } /* Adaptive_Cubical::Toplex::geometry */
  
  namespace detail {
    
    struct AddTopCellToComplexFunctor {
      Toplex::Complex & complex;
      const Toplex & toplex;
      AddTopCellToComplexFunctor ( Toplex::Complex & complex, const Toplex & toplex ) 
      : complex(complex), toplex(toplex) {}
      void operator () ( const Toplex::Top_Cell & top_cell ) {
        /* Determine depth of cell */
        unsigned int depth = 0;
        Node * leaf = toplex . find ( top_cell ) . node ();
        Node * node = leaf;
        while ( node -> parent_ != NULL ) {
          node = node -> parent_;
          if ( node -> dimension_ == 0 ) ++ depth;
        } /* while */
        /* Determine 'splitting' used for Add_Full_Cube in Miro Kramar's Adaptive Complex */
        std::vector < std::vector <bool> > splitting ( depth );
        std::vector<bool> subdivision_choice ( toplex . dimension () );
        node = leaf;
        int outer_index = depth;
        int inner_index = toplex . dimension ();
        while ( 1 ) {
          -- inner_index;
          Node * parent = node -> parent_;
          ( parent -> left_ == node ) ? subdivision_choice [ inner_index ] = false : 
                                        subdivision_choice [ inner_index ] = true;
          if ( inner_index == 0 ) {
            -- outer_index;
            splitting [ outer_index ] = subdivision_choice;
            if ( outer_index == 0 ) break;
            inner_index = toplex . dimension ();
          } /* if */
          node = parent;
        } /* while */
        complex . Add_Full_Cube ( splitting );
      } /* operator () */
    };
  } /* namespace detail */
  
  Toplex::Complex Toplex::complex ( void ) const {
    Complex return_value ( dimension_);
    std::for_each ( begin (), end (), detail::AddTopCellToComplexFunctor ( return_value, *this ) );
    return_value . Finalize ();
    return return_value;
  } /* Adaptive_Cubical::Toplex::complex */
  
  Toplex::Complex Toplex::complex ( const const_iterator & cell_iterator ) const {
    Complex return_value ( dimension_);
    detail::AddTopCellToComplexFunctor my_functor ( return_value, *this );
    my_functor ( * cell_iterator );
    return_value . Finalize ();
    return return_value;
  } /* Adaptive_Cubical::Toplex::complex */
  
  Toplex::Complex Toplex::complex ( const Subset & subset_of_toplex ) const {
    Complex return_value ( dimension_);
    std::for_each ( subset_of_toplex . begin (), 
               subset_of_toplex . end (), 
               detail::AddTopCellToComplexFunctor ( return_value, *this ) );
    return_value . Finalize ();
    return return_value;
  } /* Adaptive_Cubical::Toplex::complex */
  
  Toplex::Subset Toplex::subdivide ( iterator cell_to_divide ) {
    Subset children;
    std::deque < std::pair < const_iterator, unsigned int > > work_deque;
    work_deque . push_back ( std::pair < const_iterator, unsigned int > ( cell_to_divide, 0 ) );
    while ( not work_deque . empty () ) {
      std::pair < const_iterator, unsigned int >  work_pair = work_deque . front ();
      work_deque . pop_front ();
      work_pair . first . node_ -> dimension_ = work_pair . second;
      if ( work_pair . second < dimension_ ) {
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
                                 ( const_iterator ( work_pair . first . node_ -> left_ ), work_pair . second + 1 ) );
        work_deque . push_back ( std::pair < const_iterator, unsigned int > 
                                 ( const_iterator ( work_pair . first . node_ -> right_ ), work_pair . second + 1 ) );
      } else {
        //std::cout << "subdivide: inserting " << work_pair . first . node_ -> contents_ << "\n";
        children . insert ( work_pair . first . node_ -> contents_ );
      } /* if-else */
    } /* while */
    return children;
  } /* Adaptive_Cubical::Toplex::subdivide */

  void Toplex::initialize ( const Geometric_Description & outer_bounds_of_toplex ) {
    if ( root_ != NULL ) clear ();
    dimension_ = outer_bounds_of_toplex . lower_bounds . size ();
    bounds_ = outer_bounds_of_toplex;
    root_ = new Node;
    tree_size_ = 1;
    size_ = 1;
    begin_ = const_iterator ( root_ );
    find_ . push_back ( begin_ );
  } /* Adaptive_Cubical::Toplex::initialize */
  
  Toplex::Toplex ( void ) {
    end_ = const_iterator ( NULL );
    begin_ = end_;
    size_ = 0;
    tree_size_ = 0;
    root_ = NULL;
    dimension_ = 0;
  } /* Adaptive_Cubical::Toplex::Toplex */

  Toplex::Toplex ( const Geometric_Description & outer_bounds_of_toplex ) {
    end_ = const_iterator ( NULL );
    begin_ = end_;
    size_ = 0;
    tree_size_ = 0;
    root_ = NULL;
    dimension_ = 0;   
    initialize ( outer_bounds_of_toplex );
  } /* Adaptive_Cubical::Toplex::Toplex */


} /* namespace Adaptive_Cubical */
