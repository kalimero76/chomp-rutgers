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

#include <vector>
#include <map>
#include <iterator>
#include "boost/foreach.hpp"
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_member.hpp>

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

#include "complexes/Adaptive_Complex.h"

template < class Set >
Set intersect ( const Set & A, const Set & B );

namespace Adaptive_Cubical {
  /* Forward Declarations */
  struct Geometric_Description;
  struct Node;
  class Toplex;
  class Toplex_const_iterator;
  
  /* Typedefs */
  typedef unsigned int size_type; // for > 4E9 cubes, use unsigned long
  typedef size_type Top_Cell;
  typedef double Real; //Canged from float to double by miro

  /* * * * * * * * * * * * * * * * * * * * * * * * * *
   * class Adaptive_Cubical::Geometric_Description  *
   * * * * * * * * * * * * * * * * * * * * * * * * * */
  class Geometric_Description {
  public:
    std::vector < Real > lower_bounds;
    std::vector < Real > upper_bounds;
    Geometric_Description ( void );
    Geometric_Description ( unsigned int size );
    Geometric_Description ( unsigned int size, const Real & value );
    Geometric_Description ( unsigned int size, const Real & lower_value, const Real & upper_value );
    Geometric_Description ( unsigned int size, const std::vector<Real> & lower_values, const std::vector<Real> & upper_value );
    bool intersects ( const Geometric_Description & other, const Real tol ) const;
    bool intersects ( const Geometric_Description & other ) const;
  private: 
    friend class boost::serialization::access; 
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
      unsigned int size = lower_bounds . size ();
      ar & size;
      BOOST_FOREACH ( Real x, lower_bounds ) ar & x;
      BOOST_FOREACH ( Real x, upper_bounds ) ar & x;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
      unsigned int size;
      ar & size;
      lower_bounds . resize ( size );
      upper_bounds . resize ( size );
      for ( unsigned int index = 0; index < size; ++ index ) {
        ar & lower_bounds [ index ];
      } /* for */
      for ( unsigned int index = 0; index < size; ++ index ) {
        ar & upper_bounds [ index ];
      } /* for */
      
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
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
  class Toplex_Subset : public std::unordered_set < Adaptive_Cubical::Top_Cell > {
  public:
    using std::unordered_set < Adaptive_Cubical::Top_Cell >::insert;
    void insert ( const Toplex_Subset & insert_me );
  };
  
  /* * * * * * * * * * * * * * * * * * * * * * * * *
   * class Adaptive_Cubical::Toplex_const_iterator  *
   * * * * * * * * * * * * * * * * * * * * * * * * */
  class Toplex_const_iterator {
  public:
    /* Iterator typedefs */
    typedef std::forward_iterator_tag iterator_category;
    typedef Adaptive_Cubical::Top_Cell value_type;
    typedef Adaptive_Cubical::Top_Cell * pointer;
    typedef std::ptrdiff_t difference_type;
    typedef Adaptive_Cubical::Top_Cell & reference;
    
    typedef Adaptive_Cubical::Node Node;
    Toplex_const_iterator ( void );
    Toplex_const_iterator ( Node * node );
    Toplex_const_iterator & operator ++ ( void );
    bool operator != ( const Toplex_const_iterator & right_hand_side ) const;
    bool operator == ( const Toplex_const_iterator & right_hand_side ) const;
    bool operator < ( const Toplex_const_iterator & right_hand_side ) const;
    const Adaptive_Cubical::Top_Cell & operator * ( void ) const; 
    Node * node ( void );
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
    unsigned int dimension ( void ) const;
    
    /// Subset cover ( const Geometric_Description & geometric_region, 
    /// Return the top cells in toplex which intersect "geometric_region"
    Subset cover ( const Geometric_Description & geometric_region ) const;
    
    /// Subset cover ( const Geometric_Description & geometric_region, 
    ///                const Subset & subset )
    /// Return the top cells in subset which intersect "geometric_region"
    Subset cover ( const Geometric_Description & geometric_region, 
                   const Subset & subset ) const;
    
    Geometric_Description geometry ( const const_iterator & cell_iterator ) const;
    Geometric_Description geometry ( const Top_Cell & cell ) const;

    Complex complex ( void ) const;
    Complex complex ( std::map < Top_Cell, Complex::const_iterator > & boxes ) const;
    Complex complex ( const const_iterator & cell_iterator ) const;
    Complex complex ( const Subset & subset_of_toplex ) const;
    void complex ( Complex * & return_value, 
                   const Subset & subset_of_toplex, 
                   std::map < Top_Cell, Complex::const_iterator > & boxes ) const;

    
    /* Adaptive Toplex */
    Subset subdivide ( iterator cell_to_divide );
    Subset subdivide ( Top_Cell cell_to_divide );
    Subset subdivide ( const Subset & subset_to_divide );
    /// subdivide ( void )
    ///  subdivides everything
    Subset subdivide ( void );
    
    void coarsen ( void );
    /* Adaptive Cubical Toplex */
    Geometric_Description bounds ( void ) const;
    void initialize ( const Geometric_Description & outer_bounds_of_toplex );
    Toplex ( void );
    Toplex ( const Geometric_Description & outer_bounds_of_toplex );
    ~Toplex ( void );
    unsigned int Cell_Depth( const Top_Cell ) const;
    unsigned int Cell_Child_Number( const Top_Cell ) const;
    
  private:
    const_iterator begin_;
    const_iterator end_;
    size_type size_;
    size_type tree_size_;
    std::vector < iterator > find_;
    Node * root_;
    Geometric_Description bounds_;
    unsigned int dimension_;
    mutable Real tolerance;
  };
  
} /* namespace Adaptive_Cubical */

bool Check_if_Intersect (const Adaptive_Cubical::Geometric_Description & A, 
                         const Adaptive_Cubical::Geometric_Description & B );

bool Check_if_Intersect (const Adaptive_Cubical::Toplex & A, 
                         const Adaptive_Cubical::Toplex::Subset & AS, 
                         const Adaptive_Cubical::Toplex & B, 
                         const Adaptive_Cubical::Toplex::Subset & BS );

#ifndef CHOMP_LIBRARY_ONLY_
#include "toplexes/Adaptive_Cubical_Toplex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "toplexes/Adaptive_Cubical_Toplex.cpp"
#endif

#endif
