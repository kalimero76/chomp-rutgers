/*
 *  Adaptive_Complex.h
 *
 *
 *  Created by Miroslav Kramar on 10/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CHOMP_ADAPTIVE_COMPLEX_
#define _CHOMP_ADAPTIVE_COMPLEX_



#include "Chain_Archetype.h" /* for Default_Chain */
#include "Cell_Complex_Archetype.h" /* for Cell_Complex */


/** class Adaptive_Container. **/

class Adaptive_Container {
public:
	/* typedefs */
	typedef Default_Chain Chain;
	typedef Default_Chain::Elementary_Chain Elementary_Chain;
	typedef Default_Chain::Ring Ring;
	typedef unsigned long size_type;

	/* Conform to STL type container */
	typedef Elementary_Chain key_type;
	//typedef Elementary_Properties data_type;
	//typedef Elementary_Properties mapped_type;
	//typedef std::pair<Elementary_Chain, Elementary_Properties> value_type;
	typedef bool data_type;
	typedef bool mapped_type;
	typedef std::pair<Elementary_Chain, bool> value_type;

	unsigned int space_dimension;
	unsigned int chain_dimension;


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

		class Leaf_LookUp {
		public:
			std::vector< Node * > leaves_lookup_table;
			unsigned int current_leaf;

			/* Intializes leafs_lookup_table and sets the iterator to the begining
			 * node_visitor must be set to the tree_root
			 */
			void Initialize(Node_Visitor * node_visitor);

			/* returns the number of the leafs */
			unsigned int Size();
			/* returns a pointer to the first leaf; if there are no leafs returns NULL */
			Node * First_Leaf();
			/* returns a pointer to the next leaf; if we were in the last leaf returns NULL */
			Node * Next_Leaf();
			/* returns a pointer to the leaf with number index, if the leaf does not exist retrns NULL */
			Node * Leaf( unsigned int index);

		};

		/* To compare positins of to cubes. One is obtaied be going backward along acsending_path
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
		Leaf_LookUp leaf_lookup;

		Adaptive_Tree( void );
		Adaptive_Tree( unsigned int dimension );
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


	};
	class const_iterator {
	public:
		const Adaptive_Container * referral;
		mutable Adaptive_Container::value_type dereference_value;

		unsigned long full_cube_number; /* shouldn't this be private? */
		std::map<int, bool>::const_iterator piece_iterator; /* shouldn't this be private? */

		const_iterator ( void );
		const_iterator ( const Adaptive_Container * const); /* const correctness? */

		const_iterator & operator ++ ( void );
		bool operator != ( const const_iterator & ) const;
		bool operator == ( const const_iterator & ) const;
		const Adaptive_Container::value_type & operator * ( void ) const;
		const Adaptive_Container::value_type * operator -> ( void ) const;
	};
	size_type size ( void ) const;
	const_iterator begin ( void ) const;
	const_iterator end ( void ) const;
	const_iterator find ( const key_type & ) const;

	bool Add_Full_Cube( std::vector < std::vector <bool> > splitting);
	/* Function has to be called after all the cubes are added
	 * Without colling the function the taces of the cubes won't be
	 * inserted to the structure
	 */
	void Finalize();

	Adaptive_Tree * adaptive_tree;
};

class Adaptive_Complex : public Cell_Complex_Archetype < Adaptive_Container > {
public:
	/* See Cell_Complex_Archetype */

	/** Constructor */
	Adaptive_Complex ( void );
	Adaptive_Complex ( unsigned int complex_dimension );

	/** Copy constructor (deep copy required) */
	Adaptive_Complex ( const Adaptive_Complex &);


	/** Copy constructor (deep copy required) */
	//Adaptive_Complex ( const Adaptive_Complex & );

	/** Deconstructor (deep deconstruction required) */
	~Adaptive_Complex ( void );

	/* * * * * * * * * * * * * * * * * * * * * * * * **
	** Pure Virtual Functions That Must Be Overriden **
	** * * * * * * * * * * * * * * * * * * * * * * * */

	virtual Chain & Boundary_Map ( Chain &, const Container::const_iterator & ) const;
	virtual Chain & Coboundary_Map ( Chain &, const Container::const_iterator & ) const;
	virtual void Remove_Elementary_Chain ( const Elementary_Chain & );

	std::vector< double > & Coordinates_Of_Elementary_Chain ( const Elementary_Chain & input,  std::vector< double > & coordinates);


	using Cell_Complex_Archetype<Adaptive_Container>::Boundary_Map;
	using Cell_Complex_Archetype<Adaptive_Container>::Coboundary_Map;

	/* Ways to interact with this */

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

	/* these funcions will be private*/
	std::vector<Elementary_Chain > & Find_Elementary_Cell( std::vector<Elementary_Chain> & output, const Elementary_Chain & input) const;
	std::vector< Adaptive_Container::Adaptive_Tree::Descend_Info > & Find_Possible_Owners( std::vector< Adaptive_Container::Adaptive_Tree::Descend_Info > & possible_owners, const Elementary_Chain & input, bool to_all_neighbours ) const;
	std::vector< Adaptive_Container::Adaptive_Tree::Descend_Info > & Descend_To_Possible_Owners( std::vector< Adaptive_Container::Adaptive_Tree::Descend_Info > & possible_owners, Adaptive_Container::Adaptive_Tree::Descend_Info descend_info, int splitting_nodes, bool to_all_neighbours) const;


};

#ifdef _CHOMP_HEADER_ONLY_
#include "Adaptive_Complex.cpp"
#endif

#endif
