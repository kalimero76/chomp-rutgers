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

#include "archetypes/Chain_Archetype.h" /* for Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for Cell_Complex */

/* forward declarations */
class Adaptive_Complex;
class Adaptive_const_iterator;
class Adaptive_Tree;

/* * * * * * * * * * * * * * *
 * Adaptive_Tree definition  *
 * * * * * * * * * * * * * * */

class Adaptive_Tree {
public:
  class Node {
  public:
    Node * parent;
    /* If splitting dimension == 0 then the node is a leaf
     * otherwise  it's a node and it splitts in splitting_dimension
     */
    unsigned int splitting_dimension;
    unsigned int leaf_number;
    
    Node * left_child;
    Node * right_child;
    /* If the node is a leaf - faces stores the faces of the elementary cube belonging to this leaf */
    /* First tree_dimension bits of the key indicate which dimensions are reduced
     * 0 - unreduced
     * 1 - reduced
     * Second tree_dimension bits of the key indicate to which side are the reduced dimensions reduced
     * 0 - left
     * 1 - right
     */
    std::vector< std::map < int, bool > > elementary_cells;
  };
  
  class Node_Visitor {
  public:
    Node * current_node;
    
    void Set_To( Node * node );
    /* Functions returns a pointer to the required node.
     * If the node does not exist they return NULL
     */
    Node * Parent();
    Node * Left_Child();
    Node * Right_Child();
    Node * Current_Node();
    bool Is_Leaf();
  };
  

  /* To compare positions of two cubes. One is obtained be going backward along acsending_path
   * while the other one is obtained by procedure of finding neighbors
   */
  class Relative_Positions {
  public:
    int back_min_forward_min; //-1 if back_min < forward_min, 0 if back_min = forward_min and 1 if back_min > forward_min
    int back_min_forward_max;
    int back_max_forward_min;
    int back_max_forward_max;
    
    /*Inplicitly set the positions in the way that back_min = forward_min
     * and back_max = forward_max
     */
    Relative_Positions();
    void Set_To_Same();
    void Set_To_Opposite( bool back_above_forward );
  };
  
  class Descend_Info {
  public:
    Node * current_node;
    std::vector<Relative_Positions> relative_position_table;
    bool back_before_forward;
    bool forward_is_smaller;
    std::vector<int> ascending_path;
  };
  
  unsigned int tree_dimension;
  std::vector<unsigned int> dimension_sizes;
  
  Node tree_root;
  Node_Visitor node_visitor;
  
  Adaptive_Tree( Adaptive_Complex * complex );
  ~Adaptive_Tree( void );
  
  
  /*Splits the leaf int to 2^tree_dimension cubes
   * and add these cubes to the tree. The original leaf
   * won't be leaf any more.
   */
  void Split_Leaf( Node  * leaf_to_split);
  /* Each entry of the first vector must be a vector with size tree_dimension
   * and consists of false and true
   * false - take the left node
   * true - take the right node
   * this identifies a single cube
   */
  bool Add_Full_Cube( std::vector < std::vector <bool> > splitting);
  /* Funcion has to be called after all the cubes are added
   * Without colling the function the taces of the cubes won't be
   * inserted to the structure
   */
  void Finalize();
  void Finalize_Cube(Node * leaf, std::vector< std::map < int, bool > > * full_cube_cells);
  void Fill_With_Cells_Of_Full_Cube(std::vector< std::map < int, bool > > * full_cube_cells);
  bool Cell_Is_Subset_Of_Union (int cell,std::vector< int > * cell_union );
  void Add_Cell_To_Union (int cell, std::vector< int > * intersection );
  void Find_Neighbours( Node * leaf, std::vector<Descend_Info> * bigger_neighbours, std::vector<Descend_Info> * smaller_neighbours);
  void Descend_To_Neighbours(Descend_Info * descend_info, std::vector<Descend_Info> * bigger_neighbours, std::vector<Descend_Info> * smaller_neighbours);
  
  std::vector< Node * > leaf_lookup;
  void Leaf_Lookup_Initialize(Node_Visitor * node_visitor);
  
  Adaptive_Complex * complex_;
  bool finalized;
};

/* * * * * * * * * * * * * *
 * typedef Adaptive_Cell   *
 * * * * * * * * * * * * * */

typedef Default_Cell Adaptive_Cell;

/* * * * * * * * * * * * * * * * *
 * class Adaptive_const_iterator *
 * * * * * * * * * * * * * * * * */

class Adaptive_const_iterator {
public:
  typedef Adaptive_Complex complex_type;
  typedef Adaptive_Cell Cell;
  typedef Adaptive_const_iterator const_iterator;
  Adaptive_const_iterator ( void );
  Adaptive_const_iterator ( const Adaptive_Complex * const);
  Adaptive_const_iterator & operator ++ ( void );
  bool operator != ( const Adaptive_const_iterator & ) const;
  bool operator == ( const Adaptive_const_iterator & ) const;
  bool operator < ( const Adaptive_const_iterator & right_hand_side ) const;
  const Adaptive_Cell & operator * ( void ) const;
  const Adaptive_Cell * operator -> ( void ) const;
  unsigned int dimension () const;
  const Adaptive_Complex & container () const;
private:
  friend class Adaptive_Complex;
  friend class Adaptive_Tree;
  const Adaptive_Complex * referral;
  mutable Adaptive_Cell dereference_value;
  unsigned int dimension_;
  unsigned long full_cube_number;
  std::map<int, bool>::const_iterator piece_iterator;
};

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
  std::vector< double > & Coordinates_Of_Cell ( const Cell & input,  std::vector< double > & coordinates);

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
  friend class Adaptive_Tree;
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
	Adaptive_Tree * adaptive_tree;
};

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Adaptive_Complex.cpp"
#endif

#endif
