/*
 *  Adaptive_Complex.h
 *
 *
 *  Created by Miroslav Kramar on 10/30/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#ifndef CHOMP_ADAPTIVE_COMPLEX_
#define CHOMP_ADAPTIVE_COMPLEX_

#include <vector>
#include <map>
#include "boost/functional/hash.hpp"

#include "archetypes/Chain_Archetype.h" /* for Default_Chain */

/* forward declarations */
class Adaptive_Complex;
class Adaptive_const_iterator;
size_t hash_value ( const Adaptive_const_iterator & hash_me );
class Adaptive_Tree;






namespace Adaptive_Complex_detail {
  
  typedef unsigned int size_type;
  
  class Cube_Cells {
  private:
    unsigned int dimension_;
    std::vector<bool> cell_data_;
  public:
    Cube_Cells ( unsigned int dimension );
    const bool read ( size_type name ) const;
    void write ( size_type name, bool value );
  };
  
  struct Node {
    char type; // 0 for nodes, 1 for leafs
    size_type child_number;
    Node * parent;
    void * data;
    Node ( void );
    /* Node ( bool type, unsigned int dimension )
     * If type = 0, it is node. 
     *    We initialize data to point to a vector<Node *> of  size 2 ^ dimension
     * If type = 1, it is a leaf.
     *    We initialize data to point to a Cube_Cells struct 
     */
    Node ( bool type, unsigned int dimension );
    /* Recursive Deconstructor */
    ~Node ( void );
    /* Either Cube_Cells * or std::vector < Node * > * */
  };
  
  Node * operator ++ ( Node * & node );
  
  struct Cell {
    Node * node;
    size_type code;
  };
  
  struct GeoCell {
    Node * node;
    size_type code;
  };
  
  typedef std::list < GeoCell > Division_History;
  
  std::list < std::list < GeoCell > > aliases ( const GeoCell & cell );

} /* namespace detail */

/* * * * * * * * * * * * * *
 * typedef Adaptive_Cell   *
 * * * * * * * * * * * * * */

typedef Adaptive_Complex_detail::Cell Adaptive_Cell;


/* * * * * * * * * * * * * * * * *
 * class Adaptive_const_iterator *
 * * * * * * * * * * * * * * * * */

class Adaptive_const_iterator {
public:
  /* Typedefs */
  typedef Adaptive_Complex complex_type;
  typedef Adaptive_Cell Cell;
  typedef Adaptive_const_iterator const_iterator;
  /* Constructors */
  Adaptive_const_iterator ( void );
  Adaptive_const_iterator ( const Adaptive_Complex * const container );
  Adaptive_const_iterator ( const Adaptive_Complex * const container, const Cell & cell );
  /* Iterator */
  Adaptive_const_iterator & operator ++ ( void );
  const Adaptive_Cell & operator * ( void ) const;
  /* Relations */
  bool operator != ( const Adaptive_const_iterator & ) const;
  bool operator == ( const Adaptive_const_iterator & ) const;
  bool operator < ( const Adaptive_const_iterator & right_hand_side ) const;
  /* Cell Iterator */
  unsigned int dimension () const;
  const Adaptive_Complex & container () const;
private:
  friend class Adaptive_Complex;
  friend size_t hash_value ( const Adaptive_const_iterator & hash_me );
  const Adaptive_Complex * container_;
  unsigned int dimension_;
  Cell cell_;
};

inline size_t hash_value ( const Adaptive_const_iterator & hash_me ) {
  boost::hash < unsigned long > hasher;
  return hasher ( * reinterpret_cast < const unsigned long * > ( & hash_me . piece_iterator ) );
} /* hash_value */

/* * * * * * * * * * * * * *
 * typedef Adaptive_Chain  *
 * * * * * * * * * * * * * */

typedef Chain_Archetype < std::map < Adaptive_const_iterator, Default_Ring > > Adaptive_Chain;

/* * * * * * * * * * * * * *
 * class Adaptive_Complex  *
 * * * * * * * * * * * * * */

class Adaptive_Complex {
public:
  /* typedefs */
  typedef unsigned int size_type; // for > 4E9 cubes, use unsigned long
	typedef Adaptive_Chain Chain;
	typedef Adaptive_Cell Cell;
	typedef Default_Ring Ring;
	typedef Cell key_type;
	typedef Cell value_type;
  typedef Adaptive_const_iterator const_iterator;
  typedef const_iterator iterator;
  /* Basic Container */
  std::pair<iterator, bool> insert ( const value_type & insert_me );
  void erase ( const iterator & erase_me );
  void clear ( void );
  iterator find ( const key_type & find_me ) const;
  iterator begin ( void ) const;
  iterator end ( void ) const;
  size_type size ( void ) const;
  /* Cell Complex */
  iterator begin ( unsigned int dimension ) const;
  iterator end ( unsigned int dimension ) const;
  size_type size ( unsigned int dimension ) const;
  Chain boundary ( const const_iterator & input ) const;
  Chain coboundary ( const const_iterator & input ) const;
  unsigned int dimension ( void ) const;
  /* Index Complex */
  void index ( void );
  size_type index_begin ( unsigned int dimension ) const;
  size_type index_end ( unsigned int dimension ) const;
  size_type index ( const const_iterator & lookup ) const;
  size_type & index ( const const_iterator & lookup );
  std::vector < const_iterator > & lookup ( void );
  const const_iterator & lookup ( size_type index ) const;
  const_iterator & lookup ( size_type index );
  std::vector < int > count_all_boundaries ( void ) const;
  void boundary ( std::vector < size_type > & output, const size_type index ) const;
  void coboundary ( std::vector < size_type > & output, const size_type index ) const;
  void boundary ( std::vector < std::pair <size_type, Ring > > & output, const size_type input ) const;
  void coboundary ( std::vector < std::pair <size_type, Ring > > & output, const size_type input ) const;
  /* Decomposable Complex */
  void decompose ( void );
  char type ( size_type index, unsigned int dimension ) const;
  size_type mate ( size_type queen_index, unsigned int dimension ) const;
  const Ring & connection ( size_type queen_index ) const;
  Ring & connection ( size_type queen_index );
  size_type ace_begin ( unsigned int dimension ) const;
  size_type ace_end ( unsigned int dimension ) const;
  void preprocess ( void ) {}
	/* Adaptive_Complex */
  Adaptive_Complex ( void );
	Adaptive_Complex ( unsigned int complex_dimension );
	/** Copy constructor (deep copy required) */
	Adaptive_Complex ( const Adaptive_Complex &);
	/** Deconstructor (deep deconstruction required) */
	~Adaptive_Complex ( void );

	/* 
	 */
	bool Add_Full_Cube( std::vector < unsigned int > splitting);
	
  /* 
	 */
	void Finalize();

private:
  friend class Adaptive_const_iterator;
  /* Cell Complex */
  size_type total_size_;
  std::vector<const_iterator> begin_;
  const_iterator end_;
  std::vector<size_type> size_;
  unsigned int dimension_;
  /* Index Complex */
  std::map < const_iterator, size_type > index_;
  std::vector < const_iterator > lookup_;
  std::vector<size_type> index_begin_;
  /* Decomposable Complex */
  std::vector<Ring> connection_;
  std::vector< int > boundary_count_;
  std::vector<size_type> king_count_;
  /* Adaptive Complex */
	Node * root_;
  std::vector < size_type > packing_code_;
  std::vector < size_type > packing_code_begin_;
  std::vector < size_type > geometric_code_;

  size_type geometric_code ( const size_type pcode ) const;
  size_type packing_code ( const size_type gcode ) const;
  size_type packing_code_begin ( const unsigned int dimension ) const;
  size_type packing_code_end ( const unsigned int dimension ) const;


};

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Adaptive_Complex.cpp"
#endif

#endif
