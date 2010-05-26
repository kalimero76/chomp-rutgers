/*
 *  Adaptive_Complex.cpp
 *
 *
 *  Created by Miro Kramar 2010/01/10.
 *  Copyright 2009. All rights reserved.
 *
 */

#include "complexes/Adaptive_Complex.h"
#include "algorithms/Morse_Theory.h"

/*/////////////////////////////////////////////////////////////////////////////
 * Adaptive tree :: Relative_Positions Implementation ////////////////////////////////
 */////////////////////////////////////////////////////////////////////////////

Adaptive_Tree::Relative_Positions::Relative_Positions(){
	Set_To_Same();
}
void Adaptive_Tree::Relative_Positions::Set_To_Same(){
	back_min_forward_min = 0;
	back_min_forward_max = -1;
	back_max_forward_min = 1;
	back_max_forward_max = 0;
}
void Adaptive_Tree::Relative_Positions::Set_To_Opposite( bool back_above_forward ){
	if( back_above_forward ){
		back_min_forward_min = -1;
		back_min_forward_max = -1;
		back_max_forward_min = 0;
		back_max_forward_max = -1;}
	else{
		back_min_forward_min = 1;
		back_min_forward_max = 0;
		back_max_forward_min = 1;
		back_max_forward_max = 1;}
}

/*/////////////////////////////////////////////////////////////////////////////
 * Adaptive tree :: NodeVisitor Implementation ////////////////////////////////
 */////////////////////////////////////////////////////////////////////////////

void Adaptive_Tree::Node_Visitor::Set_To(Node * node)
{
	current_node = node;
}

Adaptive_Tree::Node * Adaptive_Tree::Node_Visitor::Current_Node()
{
	return current_node;
}
Adaptive_Tree::Node * Adaptive_Tree::Node_Visitor::Parent()
{
	if( current_node -> parent == NULL)
		return NULL;
	current_node = current_node -> parent;
	return current_node;
}

Adaptive_Tree::Node *  Adaptive_Tree::Node_Visitor::Left_Child()
{
	if( current_node -> left_child == NULL)
		return NULL;
	current_node = current_node -> left_child;
	return current_node;
}

Adaptive_Tree::Node *  Adaptive_Tree::Node_Visitor::Right_Child()
{
	if( current_node -> right_child == NULL)
		return NULL;
	current_node = current_node -> right_child;
	return current_node;
}

bool  Adaptive_Tree::Node_Visitor::Is_Leaf(){
	if ( current_node->splitting_dimension == 0 )
		return true;
	return false;
}

/*//////////////////////////////////////////////////////////////////
 * Leaf_LookUp Implplementation ///////////////////////////////////
 *//////////////////////////////////////////////////////////////////

void Adaptive_Tree::Leaf_Lookup_Initialize(Adaptive_Tree::Node_Visitor * node_visitor){
	Node * test_node;
	bool end = false;
	//clear the look up table
	leaf_lookup.clear();
	//in order travers throught all nodes an add them to leaves_lookup_table
	while( node_visitor->Left_Child() != NULL);
	do{
		if( node_visitor->Is_Leaf( ) ){
			node_visitor->Current_Node()->leaf_number = leaf_lookup.size();
			leaf_lookup.push_back( node_visitor->Current_Node() );
		}
		if( node_visitor->Right_Child() != NULL)
			while( node_visitor->Left_Child() != NULL);
		else{
			test_node = node_visitor->Current_Node();
			while( test_node->parent != NULL && test_node == test_node->parent->right_child )
				test_node = node_visitor->Parent();
			if( node_visitor->Parent() == NULL )
				end = true;
		}
	} while( !end );
}


/*//////////////////////////////////////////////////////////////////
 * Tree implementaion //////////////////////////////////////////////
 *//////////////////////////////////////////////////////////////////

/* Constructor */
Adaptive_Tree::Adaptive_Tree( Adaptive_Complex * complex )  : complex_(complex) {
	tree_root . parent = NULL;
	tree_root . left_child = NULL;
	tree_root . right_child = NULL;
  tree_dimension = complex_ -> dimension_;
	tree_root . splitting_dimension = 0;
  finalized = false;
}

Adaptive_Tree::~Adaptive_Tree(){

	Node * test_node;
	bool end = false;
	node_visitor.Set_To( &tree_root );
	while( node_visitor.Left_Child() != NULL);
	do{
		if( node_visitor.Right_Child() != NULL)
			while( node_visitor.Left_Child() != NULL);
		else{
			test_node = node_visitor.Current_Node();
			while( test_node->parent != NULL && test_node == test_node->parent->right_child ){
				test_node = node_visitor.Parent();
				delete test_node->right_child;
				test_node->right_child = NULL;
			}
			if( node_visitor.Parent() == NULL )
				end = true;
			else{
				test_node = node_visitor.Current_Node();
				delete test_node->left_child;
				test_node->left_child = NULL;
			}
		}
	}while( !end );
}

void Adaptive_Tree::Split_Leaf( Adaptive_Tree::Node * leaf_to_split){
	std::vector< Adaptive_Tree::Node * > nodes_to_add;
	Adaptive_Tree::Node * node;

	++leaf_to_split->splitting_dimension;
	nodes_to_add .push_back( leaf_to_split );
	while(!nodes_to_add .empty() ){
		node = nodes_to_add .back();
		nodes_to_add .pop_back();
		if( node->splitting_dimension <= tree_dimension){
			node->left_child = new Node;
			node->right_child = new Node;
			node->left_child->parent = node;
			node->right_child->parent = node;
			node->left_child->splitting_dimension = node->splitting_dimension + 1;
			node->right_child->splitting_dimension = node->splitting_dimension + 1;
			nodes_to_add.push_back(node ->left_child);
			nodes_to_add.push_back(node ->right_child);
		}
		else{
			node->splitting_dimension = 0;
			node->left_child = NULL;
			node->right_child = NULL;
		}
	}
}

bool Adaptive_Tree::Add_Full_Cube( std::vector < std::vector <bool> > splitting){
	//check if the dimesnins of the splitting are correct
	if ( splitting .size() == 0)
		return false;
	for( unsigned int  number_of_splits = 0; number_of_splits < splitting .size(); ++ number_of_splits ){
		if( splitting [ number_of_splits ].size() != tree_dimension )
			return false;
	}

	node_visitor.Set_To( &tree_root );
	//for each division we go to the cube given by splitting_vector
	for( unsigned int  number_of_splits = 0; number_of_splits < splitting .size(); ++ number_of_splits ){
		//If node is a leaf we have to split it
		if( node_visitor.Current_Node()->splitting_dimension == 0)
			Split_Leaf( node_visitor.Current_Node() );
		//set the node_visitor to the cube given by the splitting vector
		for(unsigned int dimension_index = 0; dimension_index < tree_dimension; ++ dimension_index)
			( splitting [ number_of_splits ][dimension_index] ) ?  node_visitor.Right_Child() : node_visitor.Left_Child();;
	}
	//Add the highset dimensional face to the faces of the leaf
	int faceID = 0;
	node_visitor.Current_Node()->elementary_cells.resize( tree_dimension + 1 );
	node_visitor.Current_Node()->elementary_cells[tree_dimension].insert ( std::pair< int , bool >(faceID, true) );
	return true;
}

void Adaptive_Tree::Descend_To_Neighbours(Descend_Info * descend_info, std::vector<Descend_Info> * bigger_neighbours, std::vector<Descend_Info> * smaller_neighbours){
	std::vector<Descend_Info> descend_info_stack;
	Descend_Info current_descend;

	descend_info_stack.push_back( *descend_info );
	while( descend_info_stack.size() > 0 ){
		current_descend = descend_info_stack.back();
		descend_info_stack.pop_back();
		while( ( current_descend.ascending_path.size() > 0 ) && ( current_descend.current_node->splitting_dimension != 0 ) ){
			//if cubes are on the opposite sides in this dimension we have to take the opposite child as we came from in ascending_path
			if( current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_min_forward_max == 0 ||
				current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_max_forward_min == 0 ){
				( current_descend.ascending_path.back() == 0 ) ? current_descend.current_node = current_descend.current_node->right_child : current_descend.current_node = current_descend.current_node->left_child;
				current_descend.ascending_path.pop_back();
			}
			//current_descend.ascending_path is not empty so if the cubes are not on the opposite sides then they have a common side in this dimension
			else{
				bool last_split = true;
				//check if there is another split in the same dimension and the same direcrion as the one for the current_node
				if ( current_descend.ascending_path.size() > tree_dimension )
					for( unsigned int split =  tree_dimension - current_descend.current_node->splitting_dimension;    split < ( current_descend.ascending_path.size() - tree_dimension + 1); split +=  tree_dimension )
						if( current_descend.ascending_path[ split ] == current_descend.ascending_path[ current_descend.ascending_path.size() - 1 ] )
							last_split = false;
				//for the last spliting we have to check also the opposite path
				if( last_split ){
					descend_info_stack.push_back( current_descend );
					descend_info_stack.back().ascending_path.pop_back();
					if( current_descend.ascending_path.back() == 0 ){
						descend_info_stack.back().relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].Set_To_Opposite( true );
						descend_info_stack.back().current_node = current_descend.current_node->right_child;
					}
					else{
						descend_info_stack.back().relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].Set_To_Opposite( false );
						descend_info_stack.back().current_node = current_descend.current_node->left_child;
					}
				}
				//We follow the ascend path
				if( current_descend.ascending_path.back() == 0 )
					current_descend.current_node = current_descend.current_node->left_child;
				else
					current_descend.current_node = current_descend.current_node->right_child;
				current_descend.ascending_path.pop_back();
			}
		}

		//neighbour is bigger
		if( current_descend.ascending_path.size() > 0 ){
			if( current_descend.current_node->elementary_cells.size() > 0 )
				bigger_neighbours->push_back( current_descend );
		}
		else{
			//neighbour is smaller
			if( ( current_descend.current_node->splitting_dimension != 0 ) ||  ( !current_descend.back_before_forward ) )
				smaller_neighbours->push_back( current_descend );
			else if( current_descend.current_node->elementary_cells.size() > 0 )
				bigger_neighbours->push_back( current_descend );
		}
	}
}
/* finds all smaller neighbouring cubes and bigger neighbouring cubes which are filled in (the heighest dimensional face is present)*/
void Adaptive_Tree::Find_Neighbours( Node * leaf, std::vector<Descend_Info> * bigger_neighbours, std::vector<Descend_Info> * smaller_neighbours){
	//find all neighboring cubes

	//first tree_dimension of bits are for the nodes which are suppose to be visieted form right
	//the second tree_dimension of bits are for the nodes which are suppose to be visieted form left
	//1 - is supposed to be viseted
	int nodes_to_be_visited;
	int nodes_visited = 0;
	Node * new_node = leaf;
	Node * old_node = NULL;
	Descend_Info descend_info;

	descend_info.ascending_path.clear();
	descend_info.relative_position_table.resize(tree_dimension);
	//nodes with all splitting dimensions are supposed to be visited from both sides
	nodes_to_be_visited = ( 1 << ( 2 * tree_dimension ) ) - 1;

	while( ( new_node->parent != NULL ) && ( nodes_to_be_visited != nodes_visited ) ){
		//climbing up
		old_node = new_node;
		new_node = new_node->parent;
		//We arrived from left
		if( new_node->left_child == old_node)
		{
			//if we never arived from left before, update visited_nodes and start descending
			if( ( nodes_visited >> ( tree_dimension + new_node->splitting_dimension - 1 ) ) % 2 == 0 ){
				nodes_visited += 1 << ( tree_dimension + new_node->splitting_dimension - 1 );
				descend_info.current_node = new_node->right_child;
				descend_info.back_before_forward = true;
				descend_info.relative_position_table[ new_node->splitting_dimension - 1 ].Set_To_Opposite( true );
				Descend_To_Neighbours( &descend_info, bigger_neighbours, smaller_neighbours );
				descend_info.relative_position_table[ new_node->splitting_dimension - 1 ].Set_To_Same();
			}
			descend_info.ascending_path.push_back( 0 );
		}
		//we arived from right
		else{
			//if we never arived from left before, update visited_nodes and start descending
			if ( ( nodes_visited >> ( new_node->splitting_dimension - 1 ) ) % 2 == 0 ){
				nodes_visited += 1 << ( new_node->splitting_dimension - 1 );
				descend_info.current_node = new_node->left_child;
				descend_info.back_before_forward = false;
				descend_info.relative_position_table[ new_node->splitting_dimension - 1 ].Set_To_Opposite( false );
				Descend_To_Neighbours( &descend_info, bigger_neighbours, smaller_neighbours );
				descend_info.relative_position_table[ new_node->splitting_dimension - 1 ].Set_To_Same();
			}
			descend_info.ascending_path.push_back( 1 );
		}
	}
}

void Adaptive_Tree::Fill_With_Cells_Of_Full_Cube(std::vector< std::map < int, bool > > * full_cube_cells){
	unsigned int cell_codimension;
	unsigned int cell_dimension;
	unsigned int cell_ID;
	unsigned int temp;
	unsigned int position;
	std::vector<int> reduced_dimension_positions;

	full_cube_cells->resize( tree_dimension + 1 );

	for( int cell_index = 0; cell_index < ( 1 << tree_dimension ); ++cell_index ){
		cell_codimension = 0;
		reduced_dimension_positions.clear();
		temp = cell_index;
		for( position = 0; position < tree_dimension; ++position){
			if( temp % 2 != 0)
			{
				++cell_codimension;
				reduced_dimension_positions.push_back( position );
			}
			temp = temp >> 1;
		}
		//we add all possible sides on which the cell can be
		for(int intervals_side = 0; intervals_side < (1 << cell_codimension); ++intervals_side){
			cell_ID = 0;
			temp = intervals_side;
			for(unsigned int i = 0; i < cell_codimension; ++i){
				if( temp % 2 != 0)
					cell_ID += 1 << reduced_dimension_positions[i];
				temp = temp >> 1;
			}
			cell_ID += ( cell_index << tree_dimension );
			cell_dimension = tree_dimension - cell_codimension;
			(*full_cube_cells)[ cell_dimension ].insert ( std::pair< int , bool >(cell_ID, true) );
		}
	}
} /* Adaptive_Tree::Fill_With_Cells_Of_Full_Cube */

bool Adaptive_Tree::Cell_Is_Subset_Of_Union (int cell, std::vector< int > * cell_union ){

	int cell_reduced_dimensions;
	int cell_reduced_to;
	int union_cell;
	int union_cell_reduced_dimensions;
	int union_cell_reduced_to;

	if( cell_union->size() == 0 )
		return false;

	cell_reduced_dimensions =  cell >> tree_dimension;
	cell_reduced_to = cell ^ ( cell_reduced_dimensions << tree_dimension );
	for(unsigned int cell_index = 0; cell_index < cell_union->size(); ++cell_index ){
		union_cell = (*cell_union)[ cell_index ];
		union_cell_reduced_dimensions =  union_cell >> tree_dimension;
		if(  ( union_cell_reduced_dimensions & ( ~cell_reduced_dimensions ) ) ==  0 ){
			union_cell_reduced_to = union_cell ^ ( union_cell_reduced_dimensions << tree_dimension );
			if( ( ( union_cell_reduced_dimensions & cell_reduced_to ) ^ union_cell_reduced_to ) == 0 )
				return true;
		}
	}
	return false;
}

void Adaptive_Tree::Add_Cell_To_Union (int cell, std::vector< int > * cell_union ){

	int cell_reduced_dimensions;
	int cell_reduced_to;
	int union_cell;
	int union_cell_reduced_dimensions;
	int union_cell_reduced_to;

	if( cell_union->size() == 0 ){
		cell_union->push_back( cell );
		return;
	}

	cell_reduced_dimensions =  cell >> tree_dimension;
	cell_reduced_to = cell ^ ( cell_reduced_dimensions << tree_dimension );
	for(unsigned int cell_index = 0; cell_index < cell_union->size(); ++cell_index ){
		union_cell = (*cell_union)[ cell_index ];
		union_cell_reduced_dimensions =  union_cell >> tree_dimension;
		union_cell_reduced_to = union_cell ^ ( union_cell_reduced_dimensions << tree_dimension );
		if(  ( union_cell_reduced_dimensions & ( ~cell_reduced_dimensions ) ) ==  0 ){
			if( ( ( union_cell_reduced_dimensions & cell_reduced_to ) ^ union_cell_reduced_to ) == 0 )
				return;
		}
		if(  ( cell_reduced_dimensions & ( ~union_cell_reduced_dimensions ) ) ==  0 ){
			if( ( ( cell_reduced_dimensions & union_cell_reduced_to ) ^ cell_reduced_to ) == 0 ){
				(*cell_union)[ cell_index ] = cell;
				return;
			}
		}
	}
	cell_union->push_back( cell );
}
void Adaptive_Tree::Finalize_Cube(Node * leaf, std::vector< std::map < int, bool > > * full_cube_cells){
	std::vector<Descend_Info> bigger_neighbours;
	std::vector<Descend_Info> smaller_neighbours;
	std::vector< int > bigger_neighbours_intersection;
	std::vector< int > smaller_neighbours_intersection;
	unsigned int neighbour_index;
	unsigned int dimension_index;
	int intersection;
	//Find the neighbours
	Find_Neighbours( leaf, &bigger_neighbours, &smaller_neighbours);


	//Find intersections with bigger cubes
	bigger_neighbours_intersection.clear();
	for( neighbour_index = 0; neighbour_index < bigger_neighbours.size(); ++neighbour_index ){
		intersection = 0;
		for( dimension_index = 0; dimension_index < tree_dimension; ++dimension_index ){
			if( bigger_neighbours[ neighbour_index ].relative_position_table[ dimension_index ].back_min_forward_max == 0)
				intersection += ( 1 << ( dimension_index  + tree_dimension ) );
			if( bigger_neighbours[ neighbour_index ].relative_position_table[ dimension_index ].back_max_forward_min == 0)
				intersection += ( ( 1 << ( dimension_index  + tree_dimension ) ) + ( 1 <<  dimension_index ) );
		}
		Add_Cell_To_Union( intersection, &bigger_neighbours_intersection );
	}
	if( bigger_neighbours.size() == 0 && leaf->elementary_cells.size() == 0 )
		return;

	//Find intersections with smaller cubes
	smaller_neighbours_intersection.clear();
	for( neighbour_index = 0; neighbour_index < smaller_neighbours.size(); ++neighbour_index ){
		intersection = 0;
		for( dimension_index = 0; dimension_index < tree_dimension; ++dimension_index ){
			if( smaller_neighbours[ neighbour_index ].relative_position_table[ dimension_index ].back_min_forward_max == 0)
				intersection += ( 1 << ( dimension_index  + tree_dimension ) );
			if( smaller_neighbours[ neighbour_index ].relative_position_table[ dimension_index ].back_max_forward_min == 0)
				intersection += ( ( 1 << ( dimension_index  + tree_dimension ) ) + ( 1 <<  dimension_index ) );
		}
		Add_Cell_To_Union( intersection, &smaller_neighbours_intersection );
	}

	//go thru all the possible cells in the cube
	leaf->elementary_cells.resize( tree_dimension + 1 );
	for( dimension_index = 0; dimension_index < tree_dimension; ++dimension_index ) {
		for( std::map<int, bool>::iterator cell_iterator = (*full_cube_cells)[dimension_index].begin(); cell_iterator != (*full_cube_cells)[dimension_index].end(); ++cell_iterator){
			//If cell is not in smaller neighbours
			if( !Cell_Is_Subset_Of_Union( cell_iterator -> first, &smaller_neighbours_intersection ) ){
				//if cube has a full dimensional cell or cell is in bigger neigbhour
				if( leaf->elementary_cells[ tree_dimension ].size() > 0 || Cell_Is_Subset_Of_Union( cell_iterator -> first, &bigger_neighbours_intersection ) ){
					leaf->elementary_cells[ dimension_index ].insert ( std::pair< int , bool >( cell_iterator -> first, true) );
					++ complex_ -> size_[ dimension_index ];
          ++ complex_ -> total_size_;
				}
			}
		}
  }

	if( leaf->elementary_cells[ tree_dimension ].size() > 0 ) {
    ++ complex_ -> size_[ tree_dimension ];
    ++ complex_ -> total_size_;  
  } /* if */

	//if nothing was added clear the vector of elementary_cells
	bool vector_is_empty = true;
	for( dimension_index = 0; dimension_index <= tree_dimension && vector_is_empty; ++dimension_index)
		if( leaf->elementary_cells[ dimension_index ].size() > 0 )
			vector_is_empty = false;
	if( vector_is_empty )
		leaf->elementary_cells.clear();
}

void Adaptive_Tree::Finalize(){
  if ( finalized ) return;
	std::vector< std::map < int, bool > > full_cube_cells;

	std::cout << "Initializing tree \n";
	node_visitor.Set_To( &tree_root );
	Leaf_Lookup_Initialize( &node_visitor);

	std::cout << "Tree initialized \n";
	std::cout << "Number of cubes = " << leaf_lookup.size() <<  "\n";

	Fill_With_Cells_Of_Full_Cube( &full_cube_cells );
	//Go thru all the leaves and add lower dimensional cells
	for( unsigned long leaf_index = 0; leaf_index < leaf_lookup . size (); ++ leaf_index ) {
		//std::cout << "Initializing cube = " << leaf_index << " out of " << leaf_lookup . size () << " \n";
		Finalize_Cube( leaf_lookup [ leaf_index ], &full_cube_cells);
	}
	std::cout << "Cubes Intialized \n";
  finalized = true;
	return;
} /* Adaptive_Tree::Finalize */

/*/////////////////////////////////////////////////////////////////
 * Adaptive_const_iterator implementation ///////////////
 */////////////////////////////////////////////////////////////////


Adaptive_const_iterator::Adaptive_const_iterator( void ) : referral(NULL) { }

Adaptive_const_iterator::Adaptive_const_iterator( const   Adaptive_Complex * const referral ) : referral(referral) { }

const Adaptive_Complex::value_type & Adaptive_const_iterator::operator * ( void ) const {
	dereference_value . data () = piece_iterator -> first + (full_cube_number << (referral -> dimension_ << 1 ) );
	dereference_value . dimension () = dimension_;
	return dereference_value; 
} /* Adaptive_const_iterator::operator * */

const Adaptive_Complex::value_type * Adaptive_const_iterator::operator -> ( void ) const {
	return &**this;
} /* address of overloaded * of builtin * of this */

Adaptive_const_iterator & Adaptive_const_iterator::operator ++ ( void ) {

	if( piece_iterator !=referral->adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ dimension_ ].end() )
	    if ( ++piece_iterator !=referral->adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ dimension_ ].end() ) {
			return *this;
	}
	while ( 1 ) {
		++ full_cube_number;
		if ( full_cube_number == referral->adaptive_tree->leaf_lookup.size() ) {
      if ( dimension_ < referral -> dimension_ ) {
        ++ dimension_;
        full_cube_number = 0;
        //std::cout << "increasing dimension!\n";
      } else {
        /* Reached end_ */
        return *this = referral -> end_;
      } /* if-else */
		}
    //std::cout << "full cube number = " << full_cube_number << "\n";
    //std::cout << "cells.size = " << referral->adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells.size() << "\n";
    //std::cout << "dimension = " << dimension_ << "\n";
    //std::cout << "referral= " << & referral -> adaptive_tree -> leaf_lookup [ full_cube_number ] -> elementary_cells << "\n";

		if ( ( referral->adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells.size() > 0 )  &&
				( referral->adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ dimension_ ].size() > 0) ){
			piece_iterator = referral->adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ dimension_ ].begin();
			return *this;
		}
	}
} /* endfunction */

bool Adaptive_const_iterator::operator != ( const const_iterator & right_hand_side ) const {
	if( full_cube_number != right_hand_side.full_cube_number ) return true;
	//in the case we are at the end
	//if( full_cube_number == referral->adaptive_tree->leaf_lookup.size() ) return false;
	if( piece_iterator != right_hand_side.piece_iterator ) return true;
	return false;
}

bool Adaptive_const_iterator::operator == ( const const_iterator & right_hand_side ) const {
	if( full_cube_number != right_hand_side.full_cube_number ) return false;
	//in the case we are at the end
	//if( full_cube_number == referral->adaptive_tree->leaf_lookup.size() ) return true;
	if( piece_iterator != right_hand_side.piece_iterator ) return false;
	return true;}

bool Adaptive_const_iterator::operator < ( const Adaptive_const_iterator & right_hand_side ) const {
  if ( dimension_ == right_hand_side . dimension_ ) {
    if ( full_cube_number == right_hand_side . full_cube_number )
      return * reinterpret_cast < const unsigned long * > ( & piece_iterator ) < 
      * reinterpret_cast < const unsigned long * > ( & right_hand_side . piece_iterator );    
    return full_cube_number < right_hand_side . full_cube_number;
  }
  return dimension_ < right_hand_side . dimension_;
} /* Adaptive_const_iterator::operator < */

unsigned int Adaptive_const_iterator::dimension () const {
  return dimension_;
} /* Adaptive_const_iterator::dimension */

const Adaptive_Complex & Adaptive_const_iterator::container () const {
  return * referral;
} /* Adaptive_const_iterator::container */


/*/////////////////////////////////////////////////////////////////
* Adaptive_complex implementation /////////////////////////////////
*//////////////////////////////////////////////////////////////////

/* Basic Container */

void Adaptive_Complex::erase ( const Adaptive_const_iterator & input) {
	unsigned int full_cube_number;
	unsigned int cell_ID;
  Adaptive_Cell cell = *input;
	full_cube_number = cell . data () >> ( 2 * dimension_ );
	cell_ID = cell . data () ^ ( full_cube_number << (2 * dimension_ ) );
  
	unsigned int size = adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ cell . dimension() ].size();
	adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ cell . dimension() ].erase(cell_ID);
	if( size > adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ cell . dimension() ].size() ) {
		-- size_ [ cell . dimension () ];
    -- total_size_;
  } /* if */
  
  /* TODO: fix begin_ */
  
} /* Adaptive_Complex::erase */

Adaptive_const_iterator Adaptive_Complex::find ( const Adaptive_Complex::key_type & key ) const {
	const_iterator return_value ( this );
	int cell_ID;
	return_value.full_cube_number = key.data () >> ( 2 * dimension_ );
	cell_ID = key.data () ^ ( return_value.full_cube_number  << ( 2 * dimension_ ) );
	return_value.piece_iterator = adaptive_tree->leaf_lookup [ return_value.full_cube_number ] ->elementary_cells[ key . dimension () ].find(cell_ID);
  return_value.dimension_ = key . dimension ();
	return return_value;
	//TODO if cell is not present it should return end;
}

void Adaptive_Complex::clear ( void ) {
  delete adaptive_tree;
  begin_ . clear ();
  begin_ . reserve ( 0 );
  index_ . clear ();
  lookup_ . clear ();
  lookup_ . reserve ( 0 );
  index_begin_ . clear ();
  index_begin_ . reserve ( 0 );
  connection_ . clear ();
  connection_ . reserve ( 0 );
  boundary_count_ . clear ();
  boundary_count_ . reserve ( 0 );
  king_count_ . clear ();
  king_count_ . reserve ( 0 );
}

Adaptive_Complex::iterator Adaptive_Complex::begin ( void ) const {
	return begin_ [ 0 ];
} /* Adaptive_Complex::begin */

Adaptive_Complex::iterator Adaptive_Complex::end ( void ) const {	
  return end_;
} /* Adaptive_Complex::end */

Adaptive_Complex::size_type Adaptive_Complex::size ( void ) const {
	return total_size_; 
} /* Adaptive_Complex::size */

/* Cell Complex */

Adaptive_Complex::iterator Adaptive_Complex::begin ( unsigned int dimension  ) const {
	return begin_ [ dimension ];
} /* Adaptive_Complex::begin */

Adaptive_Complex::iterator Adaptive_Complex::end ( unsigned int dimension ) const {	
  return begin_ [ dimension + 1 ];
} /* Adaptive_Complex::end */

Adaptive_Complex::size_type Adaptive_Complex::size ( unsigned int dimension ) const {
	return size_ [ dimension ]; 
} /* Adaptive_Complex::size */

Adaptive_Complex::Chain Adaptive_Complex::boundary ( const Adaptive_const_iterator & input ) const {
  Adaptive_Complex::Chain output;
	int coincidence_index;
	Cell cell;
	Cell lower_dimensional_cell;
	std::vector<Cell> lower_dimensional_cell_pieces;
	cell = *input;
	lower_dimensional_cell . dimension () = cell . dimension () - 1;
	output.clear();
	coincidence_index = 1;
	/*Go thru all cells which are one dimesion lower than input*/
	for( unsigned int dimension_index = 0; dimension_index < dimension_; ++dimension_index ){
		 if( ( cell.data () >> ( dimension_ + dimension_index ) ) % 2 == 0  ){/*if the dimension is not reduced*/
			 /*find the pieces for the left cell*/
			 lower_dimensional_cell.data () = cell.data () + ( 1 << ( dimension_ + dimension_index ) );
			 /*Process the chain*/
			 Find_Elementary_Cell( lower_dimensional_cell_pieces, lower_dimensional_cell );
			 for ( std::vector<Cell>::iterator cell_iterator = lower_dimensional_cell_pieces.begin();  cell_iterator != lower_dimensional_cell_pieces.end(); ++ cell_iterator )
         output.insert( std::pair < const_iterator, Ring > ( find ( *cell_iterator ), - coincidence_index ) );
			 /*find the pieces for the right cell*/
			 lower_dimensional_cell.data () += ( 1 <<  dimension_index );
			 /*Process the chain*/
			Find_Elementary_Cell( lower_dimensional_cell_pieces, lower_dimensional_cell);
			for ( std::vector<Cell>::iterator cell_iterator = lower_dimensional_cell_pieces.begin();  cell_iterator != lower_dimensional_cell_pieces.end();++ cell_iterator )
        output.insert( std::pair < const_iterator, Ring > ( find ( *cell_iterator ), coincidence_index ) );
			coincidence_index = -coincidence_index;
		 } /* if */
	} /* for */
	return output;
} /* Adaptive_Complex::boundary */

Adaptive_Complex::Chain Adaptive_Complex::coboundary ( const Adaptive_const_iterator & input) const {
  Adaptive_Complex::Chain output;
	unsigned int full_cube_number;
	unsigned int cell_ID;
	unsigned int neighbour_full_cube_number;
	unsigned int neighbour_cell_ID;
	Cell cell;
	Cell higher_dimensional_cell;
	std::vector<Cell> higher_dimensional_cell_pieces;
	std::vector< Adaptive_Tree::Descend_Info >  possible_owners;

	cell = *input;
	full_cube_number = cell . data () >> ( 2 * dimension_ );

	output.clear();
	if( cell.dimension () >= dimension_ )
		return output;

	higher_dimensional_cell . dimension () = cell . dimension () + 1;

  Ring sign ( 1 );
	/* go thru all 1 dimension higher dimensional cells in the cube*/
	for(unsigned int dimension_index = 0; dimension_index < dimension_; ++dimension_index ){
		cell_ID = cell.data () - ( full_cube_number << ( 2 * dimension_ ) );
		if( ( cell_ID >> ( dimension_ + dimension_index ) ) % 2 == 1 ){ /*if the dimension is  reduced*/
      Ring coincidence;
			cell_ID ^=  ( 1 << ( dimension_ + dimension_index) );
			if( ( cell_ID >> ( dimension_index ) ) % 2 == 1 ) { /* if the dimension is reduced to the right side */
				cell_ID ^=  ( 1 <<  dimension_index );
        coincidence = sign;
      } else { /* Reduced to the left side */
        coincidence = -sign;
      } /* if-else */
			/* if face is present in the cube add it to coboundary*/
			if( (adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells.size() >  higher_dimensional_cell . dimension () ) &&
				(adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ higher_dimensional_cell . dimension () ].find( cell_ID) != adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ higher_dimensional_cell . dimension () ].end() )){
				higher_dimensional_cell.data () = (full_cube_number << ( 2 * dimension_ ) ) + cell_ID;
				output . insert ( std::pair < const_iterator, Ring > ( find ( higher_dimensional_cell ), coincidence ) );
			}
		} else { /* Dimension is not reduced */
      sign = - sign;
    } /* if-else */
	} /* for */

	/*Find all posiblle neighbours and look if they have a piece of coboundary*/
	Find_Possible_Owners( possible_owners, cell, true );
	/*go thru all neighbours*/
	std::vector< Adaptive_Tree::Descend_Info >::iterator cube_iterator;
	for( cube_iterator = possible_owners . begin (); cube_iterator != possible_owners . end (); ++ cube_iterator ) {
		//if the original cube is smaller go down the ascent path and update the descend info
		while( cube_iterator->ascending_path.size() > 0 && ( cube_iterator->forward_is_smaller ) ) {
			unsigned int splitting_dimension = dimension_ - 1 - ( cube_iterator->ascending_path.size()  - 1 )  %  dimension_;
			if ( cube_iterator -> relative_position_table [ splitting_dimension ] . back_max_forward_min == 0 &&
				   cube_iterator -> ascending_path.back() == 0 )
        cube_iterator -> forward_is_smaller = false;
			if ( cube_iterator -> relative_position_table [ splitting_dimension ] . back_min_forward_max == 0 &&
				   cube_iterator -> ascending_path.back () == 1 )
        cube_iterator -> forward_is_smaller = false;
			if ( cube_iterator -> relative_position_table[ splitting_dimension ] . back_min_forward_min == 0 &&
           cube_iterator -> ascending_path.back () == 1 )
        cube_iterator -> relative_position_table [ splitting_dimension ] . back_min_forward_min = 1;
			if ( cube_iterator -> relative_position_table [ splitting_dimension ] . back_max_forward_max == 0 &&
           cube_iterator -> ascending_path . back () == 0 )
        cube_iterator->relative_position_table[ splitting_dimension ].back_max_forward_max = -1;
			cube_iterator->ascending_path.pop_back();
		} /* while */
    
		/* look at all possible coboundary pieces in the cube*/
		cell_ID = cell.data () - ( full_cube_number << ( 2 * dimension_ ) );
		for ( unsigned int expand_dimension = 0; expand_dimension < dimension_; ++ expand_dimension ) {
			bool continue_flag = false; /* will set to true if cell is not present in neighbor */
      Ring sign ( 1 );
      Ring coincidence;
			neighbour_cell_ID = 0;
			if ( not ( ( cell_ID >> ( dimension_ + expand_dimension ) ) & 1 ) ) continue; /* dimension is not reduced */
      /* The dimension is reduced so it is possible to expand*/
      if ( cube_iterator->relative_position_table[ expand_dimension ].back_min_forward_min == 1 &&  
          cube_iterator->relative_position_table[ expand_dimension ].back_min_forward_max != 0 &&
          not ( ( cell_ID >>  expand_dimension ) & 1 ) ) continue; /* cell is not present in neighbor */
      if ( cube_iterator->relative_position_table[ expand_dimension ].back_max_forward_max == -1 && 
          cube_iterator->relative_position_table[ expand_dimension ].back_max_forward_min != 0  &&
          ( ( cell_ID >>  expand_dimension ) & 1 ) ) continue; /* cell is not present in neighbor */
      for ( unsigned int dimension_index = 0; dimension_index < dimension_; ++ dimension_index ) {
        if ( not ( ( cell_ID >> ( dimension_ + dimension_index ) ) & 1 ) ) {
          /* This dimension is not reduced */
          sign = - sign;
          continue;  
        } /* if */
        /* This dimension is reduced. */
        if ( expand_dimension == dimension_index ) {
          if ( cube_iterator->relative_position_table[ dimension_index ].back_min_forward_max == 0	||
             ( cube_iterator->relative_position_table[ dimension_index ].back_max_forward_min != 0 
              && ( ( cell_ID >>  dimension_index ) & 1 ) ) )  {
            /* The cell is the right-boundary of the found coboundary w.r.t expand_dimension */
            coincidence = sign;
          } else {
            /* The cell is the left-boundary of the found coboundary w.r.t expand_dimension */
            coincidence = - sign;
          } /* if-else */
          continue; 
        } /* if */
        /* The dimension is reduced and was not expanded*/
        //if the face is not at the boundary of the neighbouring cube
        if ( cube_iterator->relative_position_table[ dimension_index ].back_min_forward_min == 1 &&  
            cube_iterator->relative_position_table[ dimension_index ].back_min_forward_max != 0 &&
            not ( ( cell_ID >>  dimension_index ) & 1 ) )  {
          /* cell is not present in neighbor */
          continue_flag = true;
          break;
        } /* if */
        if ( cube_iterator->relative_position_table[ dimension_index ].back_max_forward_max == -1 &&  
            cube_iterator->relative_position_table[ dimension_index ].back_max_forward_min != 0 &&
            ( ( cell_ID >>  dimension_index ) & 1 ) ) {
          /* cell is not present in neighbor */
          continue_flag = true;
          break;
        } /* if */
        neighbour_cell_ID |= 1 << ( dimension_index + dimension_ );
        if( cube_iterator->relative_position_table[ dimension_index ].back_min_forward_max == 0	)/*cubes are on the opposite sides i.e. the face is on the right hand side*/
          neighbour_cell_ID |= 1 << ( dimension_index );
        else if ( cube_iterator->relative_position_table[ dimension_index ].back_max_forward_min != 0 && ( ( cell_ID >>  dimension_index ) & 1 )  ) 
          /*cubes are on the same side again face is on the right hand side*/
          neighbour_cell_ID |= 1 << ( dimension_index );
      } /* for */
      if ( continue_flag ) continue;
      /*Add the the cell if it is present*/
      neighbour_full_cube_number = cube_iterator->current_node->leaf_number;
      if( (adaptive_tree->leaf_lookup [ neighbour_full_cube_number ]->elementary_cells.size() >  higher_dimensional_cell . dimension () ) &&
          (adaptive_tree->leaf_lookup [ neighbour_full_cube_number ]->elementary_cells[ higher_dimensional_cell . dimension () ].find( neighbour_cell_ID) 
          != adaptive_tree->leaf_lookup [ neighbour_full_cube_number ]->elementary_cells[ higher_dimensional_cell . dimension () ].end() )){
        
        higher_dimensional_cell.data () = (neighbour_full_cube_number << ( 2 * dimension_ ) ) + neighbour_cell_ID;
        output . insert ( std::pair < const_iterator, Ring > ( find ( higher_dimensional_cell ), coincidence ) );
      } /* if */
    } /* for */
  } /* for */
	return output;
} /* Adaptive_Complex::coboundary */

unsigned int Adaptive_Complex::dimension ( void ) const {
  return dimension_;
} /* Adaptive_Complex::dimension */

/* Index Complex */

void Adaptive_Complex::index ( void ) {
  lookup_ . resize ( total_size_ + 1 );
  connection_ . resize ( total_size_, 0 );
  size_type indx = 0;
  for ( const_iterator lookup = begin (); lookup != end (); ++ lookup, ++ indx ) { 
    index_ [ lookup ] = indx;
    lookup_ [ indx ] = lookup;
  } /* for */
  index_ [ end_ ] = total_size_;
  lookup_ [ total_size_ ] = end_;
  index_begin_ . resize ( dimension_ + 2, 0 );
  size_type sum = 0;
  for ( unsigned int dimension_index = 0; dimension_index <= dimension_; ++ dimension_index ) {
    sum += size_ [ dimension_index ];
    index_begin_ [ dimension_index + 1 ] = sum;
  } /* for */
} /* Adaptive_Complex::index */

Adaptive_Complex::size_type Adaptive_Complex::index_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ]; 
} /* Adaptive_Complex::index_begin */ 

Adaptive_Complex::size_type Adaptive_Complex::index_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ];
} /* Adaptive_Complex::index_end */

Adaptive_Complex::size_type Adaptive_Complex::index ( const const_iterator & lookup ) const {
  return index_ . find ( lookup ) -> second;
} /* Adaptive_Complex::index */

Adaptive_Complex::size_type & Adaptive_Complex::index ( const const_iterator & lookup ) {
  return index_ [ lookup ];
} /* Adaptive_Complex::index */

std::vector < Adaptive_const_iterator > & Adaptive_Complex::lookup ( void ) {
  return lookup_;
} /* Adaptive_Complex::lookup */

const Adaptive_const_iterator & Adaptive_Complex::lookup ( size_type index ) const {
  return lookup_ [ index ];
} /* Adaptive_Complex::lookup */

Adaptive_const_iterator & Adaptive_Complex::lookup ( size_type index ) {
  return lookup_ [ index ];
} /* Adaptive_Complex::lookup */

/* Decomposable Complex */

std::vector < int > Adaptive_Complex::count_all_boundaries ( void ) const {
  std::vector < int > number_of_boundaries ( total_size_ );
  for ( unsigned long cell_index = 0; cell_index < total_size_; ++ cell_index ) {
    number_of_boundaries [ cell_index ] = boundary ( lookup_ [ cell_index ] ) . size ();
  } /* for */
  return number_of_boundaries;
} /* Adaptive_Complex::count_all_boundaries */

class push_functor {
public:
  push_functor ( const Adaptive_Complex & complex, std::vector < Adaptive_Complex::size_type > & output ) : complex(complex), output(output) {}
  void operator () ( std::pair < const Adaptive_const_iterator, Adaptive_Complex::Ring > & term ) {
    output . push_back ( complex . index ( term . first ) );
  }
  const Adaptive_Complex & complex;
  std::vector < Adaptive_Complex::size_type > & output;
};

void Adaptive_Complex::boundary ( std::vector < size_type > & output, const size_type cell_index ) const {
  output . clear ();
	Chain boundary_chain =  boundary ( lookup_ [ cell_index ] );
  for_each ( boundary_chain . begin (), boundary_chain . end (), push_functor ( *this, output ) );
} /* Adaptive_Complex::boundary */

void Adaptive_Complex::coboundary ( std::vector < size_type > & output, const size_type cell_index ) const {
  output . clear ();
  Chain coboundary_chain =  coboundary ( lookup_ [ cell_index ] );
  for_each ( coboundary_chain . begin (), coboundary_chain . end (), push_functor ( *this, output ) );
} /* Adaptive_Complex::coboundary */

class push_functor_with_ring {
public:
  push_functor_with_ring ( const Adaptive_Complex & complex, std::vector < std::pair< Adaptive_Complex::size_type, Default_Ring > > & output ) : complex(complex), output(output) {}
  void operator () ( std::pair < const Adaptive_const_iterator, Adaptive_Complex::Ring > & term ) {
    output . push_back ( std::pair< Adaptive_Complex::size_type, Default_Ring > ( complex . index ( term . first ), term . second ) );
  }
  const Adaptive_Complex & complex;
  std::vector < std::pair< Adaptive_Complex::size_type, Default_Ring > > & output;
};

void Adaptive_Complex::boundary ( std::vector < std::pair< size_type, Default_Ring > > & output, const size_type cell_index ) const {
  output . clear ();
  Chain boundary_chain =  boundary ( lookup_ [ cell_index ] );
  for_each ( boundary_chain . begin (), boundary_chain . end (), push_functor_with_ring ( *this, output ) );
} /* Adaptive_Complex::boundary */

void Adaptive_Complex::coboundary ( std::vector < std::pair< size_type, Ring > > & output, const size_type cell_index ) const {
  output . clear ();
  Chain coboundary_chain =  coboundary ( lookup_ [ cell_index ] );
  for_each ( coboundary_chain . begin (), coboundary_chain . end (), push_functor_with_ring ( *this, output ) );
} /* Adaptive_Complex::coboundary */

void Adaptive_Complex::decompose ( void ) {
  index (); 
  king_count_ = morse::decompose ( *this );
} /*  Adaptive_Complex::decompose */

char Adaptive_Complex::type ( size_type index, unsigned int dimension ) const {
  if ( index < index_begin_ [ dimension ] + king_count_ [ dimension + 1 ] ) return 0; /* QUEEN */
  if ( index < index_begin_ [ dimension + 1 ] - king_count_ [ dimension ] ) return 1; /* ACE */
  return 2; /* KING */
} /* Adaptive_Complex::type */

Adaptive_Complex::size_type Adaptive_Complex::mate ( size_type queen_index, unsigned int dimension ) const {
  return index_begin_ [ dimension ] + index_begin_ [ dimension + 2 ] - queen_index - 1;
} /* Adaptive_Complex::mate */

const Adaptive_Complex::Ring & Adaptive_Complex::connection ( size_type queen_index ) const {
  return connection_ [ queen_index ];
} /* Adaptive_Complex::connection */

Adaptive_Complex::Ring & Adaptive_Complex::connection ( size_type queen_index ) {
  return connection_ [ queen_index ];
} /* Adaptive_Complex::connection */

Adaptive_Complex::size_type Adaptive_Complex::ace_begin ( unsigned int dimension ) const {
  return index_begin_ [ dimension ] + king_count_ [ dimension + 1 ];
} /* Adaptive_Complex::ace_begin */

Adaptive_Complex::size_type Adaptive_Complex::ace_end ( unsigned int dimension ) const {
  return index_begin_ [ dimension + 1 ] - king_count_ [ dimension ];
} /* Adaptive_Complex::ace_end */

/* Adaptive_Complex */

bool Adaptive_Complex::Add_Full_Cube( std::vector < std::vector <bool> > splitting){
	return adaptive_tree->Add_Full_Cube( splitting );
}
void Adaptive_Complex::Finalize(){
	adaptive_tree->Finalize();
  /* Gives correct values to begin_*/
  /* Set end_ */
  end_ . full_cube_number = adaptive_tree->leaf_lookup . size ();
  /* Find begin [ 0 ] */ /* TODO: this fails if there are no vertices */
  begin_ [ 0 ] = end_;
  for ( unsigned long temp_full_cube_number = 0; temp_full_cube_number < adaptive_tree -> leaf_lookup . size (); ++ temp_full_cube_number ) {
    if ( ( adaptive_tree->leaf_lookup [ temp_full_cube_number ]->elementary_cells.size() > 0 )  &&
        ( adaptive_tree->leaf_lookup [ temp_full_cube_number ]->elementary_cells[ 0 ].size() > 0) ){
      begin_ [ 0 ] . full_cube_number = temp_full_cube_number;
      begin_ [ 0 ] . piece_iterator = adaptive_tree->leaf_lookup [ temp_full_cube_number ]->elementary_cells[ 0 ].begin();
      begin_ [ 0 ] . dimension_ = 0;
      begin_ [ 0 ] . referral = this;
      break;
    } /* if */
  } /* for */
  /* Iterate through the complex to fill in begin_ */
  unsigned int dimension = 0;
  for ( const_iterator lookup = begin (); lookup != end (); ++ lookup ) { 
    //std::cout << "lookup = " << * lookup << "\n";
    while ( dimension < lookup . dimension () ) {
      ++ dimension;
      begin_ [ dimension ] = lookup;
    }
  } /* for */
  while ( dimension < dimension_ ) {
    ++ dimension;
    begin_ [ dimension ] = end_;
  } /* while */
  begin_ [ dimension_ + 1 ] = end_;
}

/* Constructor */
Adaptive_Complex::Adaptive_Complex ( void ) {
  
} /* endfunction */

Adaptive_Complex::Adaptive_Complex ( unsigned int complex_dimension ) : dimension_(complex_dimension)  {
	adaptive_tree = new Adaptive_Tree ( this );
  size_ . resize ( dimension_ + 1, 0 );
  total_size_ = 0;
  begin_ . resize ( dimension_ + 2 );
} /* Adaptive_Complex::Adaptive_Complex */

/* Copy Constructor */
Adaptive_Complex::Adaptive_Complex ( const Adaptive_Complex & copy_me) {
	dimension_ = copy_me.dimension_;
	adaptive_tree = new Adaptive_Tree ( this );
  
  total_size_ = copy_me . total_size_;
  size_ = copy_me . size_;
  
	adaptive_tree->leaf_lookup.resize( copy_me.adaptive_tree->leaf_lookup.size() );
  
	/*reconstruct the tree*/
	typedef Adaptive_Tree::Node Node;
	Node * current_node_in_old_tree;
	Node * current_node_in_new_tree;
  
	std::vector< Node * > old_tree_nodes_stack;
	std::vector< Node * > new_tree_nodes_stack;
  
	new_tree_nodes_stack.push_back( &adaptive_tree->tree_root );
	old_tree_nodes_stack.push_back( &copy_me.adaptive_tree->tree_root );
	while( new_tree_nodes_stack.size() > 0 ){
		current_node_in_old_tree = old_tree_nodes_stack.back();
		old_tree_nodes_stack.pop_back();
		current_node_in_new_tree = new_tree_nodes_stack.back();
		new_tree_nodes_stack.pop_back();
    
		current_node_in_new_tree->splitting_dimension = current_node_in_old_tree->splitting_dimension;
		if( current_node_in_new_tree->splitting_dimension == 0){/* it's a leaf */
			current_node_in_new_tree->leaf_number = current_node_in_old_tree->leaf_number;
			current_node_in_new_tree->left_child = NULL;
			current_node_in_new_tree->right_child = NULL;
			adaptive_tree->leaf_lookup[ current_node_in_new_tree->leaf_number ] = current_node_in_new_tree;
			if( current_node_in_old_tree->elementary_cells.size() != 0 ){
				current_node_in_new_tree->elementary_cells = current_node_in_old_tree->elementary_cells;
			}
		}
		else{
			Node * new_node_1 = new Adaptive_Tree::Node;
			Node * new_node_2 = new Adaptive_Tree::Node;
			new_node_1->parent = current_node_in_new_tree;
			current_node_in_new_tree->left_child = new_node_1;
			new_tree_nodes_stack.push_back( new_node_1 );
			old_tree_nodes_stack.push_back( current_node_in_old_tree->left_child );
			new_node_2->parent = current_node_in_new_tree;
			current_node_in_new_tree->right_child = new_node_2;
			new_tree_nodes_stack.push_back( new_node_2 );
			old_tree_nodes_stack.push_back( current_node_in_old_tree->right_child );
		}
	}
  adaptive_tree -> finalized = copy_me . adaptive_tree -> finalized;
  Finalize ();
}
/* Destructor */
Adaptive_Complex::~Adaptive_Complex ( void ) {
  delete adaptive_tree;
} /* endfunction */

std::vector< double > & Adaptive_Complex::Coordinates_Of_Cell ( const Cell & input,  std::vector< double > & coordinates){
	unsigned int full_cube_number;

	Adaptive_Tree::Node * new_node = NULL;
	Adaptive_Tree::Node * old_node = NULL;
	std::vector<int> acsending_path;

	full_cube_number = input.data () >> ( 2 * dimension_ );
	new_node = adaptive_tree->leaf_lookup [ full_cube_number ];
	while(  new_node->parent != NULL ){
		//climbing up
		old_node = new_node;
		new_node = new_node->parent;
		//We arrived from left
		if( new_node->left_child == old_node)
			acsending_path.push_back( 0 );
		//we arived from right
		else
			acsending_path.push_back( 1 );
	}

	coordinates.clear();
	coordinates.resize( dimension_ );
	unsigned int split_count = 1;
	unsigned int splitting_dimension = 0;
	while( acsending_path.size() > 0 ){
		if( acsending_path.back() == 1 )
			coordinates[ splitting_dimension ] += (double)1 / (double) ( 1 << split_count );

		++splitting_dimension;
		splitting_dimension = splitting_dimension % dimension_;
		if( splitting_dimension == 0 )
			++split_count;
		acsending_path.pop_back();
	}
	return coordinates;
}

std::vector<Adaptive_Complex::Cell> & Adaptive_Complex::Find_Elementary_Cell( std::vector<Adaptive_Complex::Cell> & output, const Adaptive_Complex::Cell & input) const {

	unsigned int full_cube_number;
	unsigned int cell_ID;
	unsigned int neighbour_cube_number;
	unsigned int neighbour_cell_ID;
	std::vector< Adaptive_Tree::Descend_Info > possible_owners;

	output.clear();

	full_cube_number = input.data () >> ( 2 * dimension_ );
	cell_ID = input.data () - ( full_cube_number << (2 * dimension_ ) );

	//if cell is present in the cube with ID full_cube_number
	std::map < int, bool > elementary_cells = adaptive_tree->leaf_lookup [ full_cube_number ]->elementary_cells[ input.dimension() ];
	if( elementary_cells.find( cell_ID) != elementary_cells.end() ){
		output.push_back( input );
		return output;
	}

	Find_Possible_Owners( possible_owners, input, false);
	/* Go thru all possible owners and add the cells */
	std::vector< Adaptive_Tree::Descend_Info >::iterator it;
	for( it = possible_owners.begin(); it != possible_owners.end(); ++it){
		neighbour_cube_number = it->current_node->leaf_number;
		neighbour_cell_ID = 0;
		for(unsigned int dimension_index = 0; dimension_index < dimension_; ++dimension_index){
			if( ( ( cell_ID >> ( dimension_index + dimension_ ) ) % 2 ) == 1 ){/*if it is a reduced dimension*/
				neighbour_cell_ID +=1 << ( dimension_index + dimension_ );
				if( ( ( cell_ID >>  dimension_index   ) % 2 ) == 1 ){/*if cell is on the right side*/
					if( it->relative_position_table[ dimension_index ].back_max_forward_min != 0 ) /* cubes are not on the opposite sides (same side)*/
						neighbour_cell_ID += ( 1 << dimension_index );
				}
				else /*if cell is on the left side*/
					if( it->relative_position_table[ dimension_index ].back_min_forward_max == 0 ) /* cubes are on the opposite sides */
						neighbour_cell_ID +=1 << dimension_index;
			}
			if( it->current_node->elementary_cells.size() > input.dimension() )
				if( it->current_node->elementary_cells[ input.dimension() ].find( neighbour_cell_ID ) != it->current_node->elementary_cells[ input.dimension() ].end() )
					output.push_back( Cell ( ( neighbour_cube_number << ( 2 * dimension_ ) ) + neighbour_cell_ID, input.dimension() ) );
		} /* for */
	}
	return output;
} /* Adaptive_Complex::Find_Elementary_Cell */

std::vector< Adaptive_Tree::Descend_Info > & Adaptive_Complex::Find_Possible_Owners( std::vector< Adaptive_Tree::Descend_Info > & possible_owners, const Adaptive_Complex::Cell & input, bool to_all_neighbours ) const {
	unsigned int full_cube_number;
	unsigned int cell_ID;
	/* Bit 1 at dimension_ + splitting_dimension - 1 means that a node with the splitting_dimension is supposed to be viseted from left
	 * Bit 1 at splitting_dimension - 1 means that a node with the splitting_dimension is supposed to be viseted from right
	 */
	int nodes_to_be_visited = 0;
	int nodes_visited = 0;

  //std::cout << "Find_Possible_Owners\n";
	Adaptive_Tree::Node * new_node = NULL;
	Adaptive_Tree::Node * old_node = NULL;
	std::vector<int> acsending_path;
	Adaptive_Tree::Descend_Info descend_info;

	full_cube_number = input.data () >> ( 2 * dimension_ );
	cell_ID = input.data () - ( full_cube_number << (2 * dimension_ ) );
	new_node = adaptive_tree->leaf_lookup [ full_cube_number ];

	//prepare nodes_to_be_visited
	for(unsigned int dimension_index = 0; dimension_index < dimension_; ++dimension_index){
		if( ( cell_ID >> ( dimension_index + dimension_ ) ) % 2 != 0 )/* reduced dimension */
			if( ( cell_ID >> dimension_index ) % 2 != 0 ) /* reduced to the right : suppose to be visited from left*/
				nodes_to_be_visited |= ( 1 << ( dimension_index +  dimension_ ) );
			else /* reduced to the left : suppose to be visited from the right */
				nodes_to_be_visited |= ( 1 << dimension_index  );
	}
	while( ( new_node->parent != NULL ) && ( nodes_to_be_visited != nodes_visited ) ){
			//climbing up
    //std::cout << "climbing up \n";
			old_node = new_node;
			new_node = new_node->parent;
      /* Did we arrive from the left? */
			if( new_node->left_child == old_node) 
			{
        //We arrived from left
        //std::cout << "arrived from left \n";
				//if we never arived from left before and its a splitting dimension visited from the left child
				if( (  ( nodes_visited >> ( dimension_ + new_node->splitting_dimension - 1 ) ) % 2 == 0 ) &&
						( ( nodes_to_be_visited >> ( dimension_ + new_node->splitting_dimension - 1 ) ) % 2 != 0 ) ){
					// update visited_nodes and start descending
					nodes_visited |= ( 1 << ( dimension_ + new_node->splitting_dimension - 1 ) );
					descend_info.current_node = new_node->right_child;
					descend_info.back_before_forward = true;
					descend_info.forward_is_smaller = false;
					descend_info.ascending_path = acsending_path;
					descend_info.relative_position_table.clear();
					descend_info.relative_position_table.resize(dimension_);
					descend_info.relative_position_table[ new_node->splitting_dimension - 1 ].Set_To_Opposite( true );
          //std::cout << descend_info . current_node << " --> Descending A ";
          //if ( descend_info . current_node -> right_child != NULL )          
          //  std::cout << descend_info . current_node -> right_child -> leaf_number << " - ";


					Descend_To_Possible_Owners( possible_owners, descend_info, nodes_to_be_visited/*nodes_visited*/,  to_all_neighbours );
          //for ( unsigned int s = 0; s < possible_owners . size (); ++ s ) std::cout << possible_owners [ s ] . current_node -> leaf_number << ", ";
          //std::cout << "\n";
				}
				acsending_path.push_back( 0 );
			} else { //we arived from right
        //std::cout << "arrived from right \n";
				//if we never arived from right before and its a pliting dimension visited from the right child
				if( ( ( nodes_visited >> ( new_node->splitting_dimension - 1 ) ) % 2 == 0 ) &&
						( ( nodes_to_be_visited >> ( new_node->splitting_dimension - 1 ) ) % 2 != 0 ) ){
					// update visited_nodes and start descending
					nodes_visited |= 1 << ( new_node->splitting_dimension - 1 );
					descend_info.current_node = new_node->left_child;
					descend_info.back_before_forward = false;
					descend_info.forward_is_smaller = false;
					descend_info.ascending_path = acsending_path;
					descend_info.relative_position_table.clear();
					descend_info.relative_position_table.resize(dimension_);
					descend_info.relative_position_table[ new_node->splitting_dimension - 1 ].Set_To_Opposite( false );
          //std::cout << descend_info . current_node << " --> Descending B ";
					Descend_To_Possible_Owners( possible_owners, descend_info, nodes_to_be_visited/*nodes_visited*/, to_all_neighbours  );
          //for ( unsigned int s = 0; s < possible_owners . size (); ++ s ) std::cout << possible_owners [ s ] . current_node -> leaf_number << ", ";
          //std::cout << "\n";
				}
				acsending_path.push_back( 1 );
			}
		}

	return possible_owners;
} /* Adaptive_Complex::Find_Possible_Owners */


std::vector< Adaptive_Tree::Descend_Info > & Adaptive_Complex::Descend_To_Possible_Owners( std::vector< Adaptive_Tree::Descend_Info > & possible_owners, Adaptive_Tree::Descend_Info descend_info, int splitting_nodes, bool to_all_neighbours ) const{
	std::vector<Adaptive_Tree::Descend_Info> descend_info_stack;
	Adaptive_Tree::Descend_Info current_descend;

	descend_info_stack.push_back(descend_info);
	while( descend_info_stack.size() > 0 ){
		current_descend = descend_info_stack.back();
		descend_info_stack.pop_back();
    //std::cout << "* current node = " << current_descend.current_node << "\n";
		while(  current_descend.current_node->splitting_dimension != 0  ){/*Node in the descending path is not a leaf*/
			if( current_descend.ascending_path.size() > 0 ){ /*Ascending path is not empty*/
        //std::cout << " Ascending path is not empty \n ";

				//if cubes are on the opposite sides in this dimension we have to take the opposite child as we came from in ascending_path
				if( current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_min_forward_max == 0 ||
					current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_max_forward_min == 0 ){
					if( current_descend.ascending_path.back() == 0 ) {
            //std::cout << "right1 ";
						current_descend.current_node = current_descend.current_node->right_child;
					} else {
            //std::cout << "left1 ";
						current_descend.current_node = current_descend.current_node->left_child;
          }
					current_descend.ascending_path.pop_back();
				}
				//current_descend.ascending_path is not empty so if the cubes are not on the opposite sides then they have a common side in this dimension
				else{
					//if the node is a spliiting node (node with reduced dimension visited from the right child )
					if( ( current_descend.ascending_path.back() == 0 && (splitting_nodes >> (dimension_ + current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 ) ||
					  ( current_descend.ascending_path.back() == 1 && (splitting_nodes >> ( current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 )	)
					{
            //std::cout << "node is a splitting node. ";
						bool last_split = true;
						//check if there is another split in the same dimension and the same direction as the one for the current_node
						if ( current_descend.ascending_path.size() > dimension_ )
							for( unsigned int split =  dimension_ - current_descend.current_node->splitting_dimension;    split < ( current_descend.ascending_path.size() - dimension_ + 1); split +=  dimension_ )
								if( current_descend.ascending_path[ split ] == current_descend.ascending_path[ current_descend.ascending_path.size() - 1 ] )
									last_split = false;
						//for the last splitting we have to check also the opposite path
						if( last_split ){
              //std::cout << "last_split ";
							Adaptive_Tree::Descend_Info copy_current_descend;
							copy_current_descend.current_node = current_descend.current_node;
							copy_current_descend.relative_position_table = current_descend.relative_position_table;
							copy_current_descend.back_before_forward = current_descend.back_before_forward;
							copy_current_descend.ascending_path = current_descend.ascending_path;
							copy_current_descend.forward_is_smaller = current_descend.forward_is_smaller;
							if( copy_current_descend.ascending_path.back() == 0 ){
                //std::cout << "right2 ";
								copy_current_descend.relative_position_table[ copy_current_descend.current_node->splitting_dimension - 1 ].Set_To_Opposite( true );
								copy_current_descend.current_node = copy_current_descend.current_node->right_child;
							}
							else{
                //std::cout << "left2 ";
								copy_current_descend.relative_position_table[ copy_current_descend.current_node->splitting_dimension - 1 ].Set_To_Opposite( false );
								copy_current_descend.current_node = copy_current_descend.current_node->left_child;
							}
							copy_current_descend.ascending_path.pop_back();
							//add the copy to the stack
							descend_info_stack.push_back( copy_current_descend );
						}
					}
					//This path is taken in both case cubes on the same side and it is not the last spliting in this dimension and this direction (it is the last splitting)
					if( current_descend.ascending_path.back() == 0 ) {
            //std::cout << "left3 ";
						current_descend.current_node = current_descend.current_node->left_child;
					} else {
            //std::cout << "right3 ";
						current_descend.current_node = current_descend.current_node->right_child;
          }
					current_descend.ascending_path.pop_back();
				}
			}
			else{/* Ascending path is empty */
        //std::cout << " Ascending path is empty \n ";

				current_descend.forward_is_smaller = true;
				if( ( splitting_nodes >> (dimension_ + current_descend.current_node->splitting_dimension - 1 )) % 2 == 1  ||
				    (splitting_nodes >> ( current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 	){/*if the node is a spliiting node*/

					if( current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_max_forward_min == 0 )/*Forward cube on the right side*/ {
						current_descend.current_node = current_descend.current_node->left_child;
            //std::cout << "left4. ";
          } else if( current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_min_forward_max == 0 )/*Forward cube on the left side*/ {
            //std::cout << "right4. ";
						current_descend.current_node = current_descend.current_node->right_child;
          } else {/* cubes are touching in an interval*/
            //std::cout << "cubes are touching in an interval. ";
						if( ( splitting_nodes >> (dimension_ + current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 ){/*cell is on the right*/
              //std::cout << "right5. ";
							current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_min_forward_min = -1;
							current_descend.current_node = current_descend.current_node->right_child;
						} else if( ( splitting_nodes >> ( current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 ){/*cell is on the left*/
              //std::cout << "left5. ";
							current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_max_forward_max = 1;
							current_descend.current_node = current_descend.current_node->left_child;
						}
					}
				} else {/* node is not a splitting node*/
					/*Add the right cube to the stack*/
					Adaptive_Tree::Descend_Info copy_current_descend;
					copy_current_descend.current_node = current_descend.current_node->right_child;;
					copy_current_descend.relative_position_table = current_descend.relative_position_table;
					copy_current_descend.back_before_forward = current_descend.back_before_forward;
					copy_current_descend.ascending_path = current_descend.ascending_path;
					copy_current_descend.forward_is_smaller = current_descend.forward_is_smaller;
					copy_current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_min_forward_min = -1;
					descend_info_stack.push_back( copy_current_descend );
					/*continue with the left cube */
					current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_max_forward_max = 1;
					current_descend.current_node = current_descend.current_node->left_child;
          //std::cout << "left6. ";
				}
			}
		} /* while */
		/* Cube is not smaller but is the first one in the lexicographical order */
		if( ( current_descend.ascending_path.size() == 0 ) && ( !descend_info.back_before_forward ) )
			current_descend.forward_is_smaller = true;
		/* if we searhc for all the neighbours */
		if( to_all_neighbours )
			current_descend.forward_is_smaller = true;
		if( current_descend.forward_is_smaller ) {
      //std::cout << "pushing " << current_descend . current_node -> leaf_number << ". ";
			possible_owners.push_back( current_descend );
    } else {
      //std::cout << "not pushing " << current_descend . current_node -> leaf_number << ".";
    }
	}
	return possible_owners;
} /* Adaptive_Complex::Descend_To_Possible_Owners */

#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */
#endif
