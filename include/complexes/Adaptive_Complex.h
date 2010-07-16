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


/* * * * * * * * * * * * * *
 * typedef Adaptive_Cell   *
 * * * * * * * * * * * * * */

typedef Default_Cell Adaptive_Cell;


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
    size_type next ( size_type name ) const;
    size_type begin ( unsigned int dimension ) const;
    size_type end ( unsigned int dimension ) const;
    size_type begin ( void ) const;
    size_type end ( void ) const;
  };
  
  struct Node {
    char type; // 0 for nodes, 1 for leafs
    size_type child_number;
    Node * parent;
    void * data;
    /* Either Cube_Cells * or std::vector < Node * > * */
  };
  
  struct Cell_Name {
    Node * node;
    size_type cell_code;
  };
  
  struct Relation {
    size_type child_number;  
    size_type cell_code;
  };
  
  typedef std::list < Cell_Name > Division_History;
  

  std::list < std::list < Cell_Name > > aliases ( const Cell_Name & cell_name );

} /* namespace detail */



/* * * * * * * * * * * * * * * * *
 * class Adaptive_const_iterator *
 * * * * * * * * * * * * * * * * */

class Adaptive_const_iterator {
public:
  typedef Adaptive_Complex complex_type;
  typedef Adaptive_Cell Cell;
  typedef Adaptive_const_iterator const_iterator;
  Adaptive_const_iterator ( void );
  Adaptive_const_iterator ( const Cell_Name & my_cell );
  Adaptive_const_iterator ( const Adaptive_Complex * const);
  Adaptive_const_iterator & operator ++ ( void );
  bool operator != ( const Adaptive_const_iterator & ) const;
  bool operator == ( const Adaptive_const_iterator & ) const;
  bool operator < ( const Adaptive_const_iterator & right_hand_side ) const;
  const Adaptive_Cell & operator * ( void ) const;
  unsigned int dimension () const;
  const Adaptive_Complex & container () const;
private:
  friend class Adaptive_Complex;
  friend size_t hash_value ( const Adaptive_const_iterator & hash_me );
  const Adaptive_Complex * referral;
  unsigned int dimension_;
  Cell_Name cell_;
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
  double Coordinates_Of_Cell ( const Cell & input,  std::vector< double > & coordinates);

	/* ways to make it */
	/* Each entry of the first vector must be a vector with size tree_dimension
	 * and consists of 0 and 1
	 * 0 - take the left node
	 * 1 - take the right node
	 * this identifies a single cube
	 */
	bool Add_Full_Cube( std::vector < std::vector <bool> > splitting);
	/* Function has to be called after all the cubes are added
	 * Without colling the function the taces of the cubes won't be
	 * inserted to the structure
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
  std::vector<Cell > & Find_Elementary_Cell( std::vector<Cell> & output, const Cell & input) const;
	std::vector< Adaptive_Tree::Descend_Info > & Find_Possible_Owners( std::vector< Adaptive_Tree::Descend_Info > & possible_owners, const Cell & input, bool to_all_neighbours ) const;
	std::vector< Adaptive_Tree::Descend_Info > & Descend_To_Possible_Owners( std::vector< Adaptive_Tree::Descend_Info > & possible_owners, Adaptive_Tree::Descend_Info descend_info, int splitting_nodes, bool to_all_neighbours) const;
	Node * root_;
};

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Adaptive_Complex.cpp"
#endif

#endif
