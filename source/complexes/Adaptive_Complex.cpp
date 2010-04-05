/*
 *  Adaptive_Complex.cpp
 *
 *
 *  Created by Miro Kramar 2010/01/10.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "complexes/Adaptive_Complex.h"

/*
 * Adaptive tree implementaion
 */


/*/////////////////////////////////////////////////////////////////////////////
 * Adaptive tree :: Relative_Positions Implementation ////////////////////////////////
 */////////////////////////////////////////////////////////////////////////////

Adaptive_Container::Adaptive_Tree::Relative_Positions::Relative_Positions(){
	Set_To_Same();
}
void Adaptive_Container::Adaptive_Tree::Relative_Positions::Set_To_Same(){
	back_min_forward_min = 0;
	back_min_forward_max = -1;
	back_max_forward_min = 1;
	back_max_forward_max = 0;
}
void Adaptive_Container::Adaptive_Tree::Relative_Positions::Set_To_Opposite( bool back_above_forward ){
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

void Adaptive_Container::Adaptive_Tree::Node_Visitor::Set_To(Node * node)
{
	current_node = node;
}

Adaptive_Container::Adaptive_Tree::Node * Adaptive_Container::Adaptive_Tree::Node_Visitor::Current_Node()
{
	return current_node;
}
Adaptive_Container::Adaptive_Tree::Node * Adaptive_Container::Adaptive_Tree::Node_Visitor::Parent()
{
	if( current_node -> parent == NULL)
		return NULL;
	current_node = current_node -> parent;
	return current_node;
}

Adaptive_Container::Adaptive_Tree::Node *  Adaptive_Container::Adaptive_Tree::Node_Visitor::Left_Child()
{
	if( current_node -> left_child == NULL)
		return NULL;
	current_node = current_node -> left_child;
	return current_node;
}

Adaptive_Container::Adaptive_Tree::Node *  Adaptive_Container::Adaptive_Tree::Node_Visitor::Right_Child()
{
	if( current_node -> right_child == NULL)
		return NULL;
	current_node = current_node -> right_child;
	return current_node;
}

bool  Adaptive_Container::Adaptive_Tree::Node_Visitor::Is_Leaf(){
	if ( current_node->splitting_dimension == 0 )
		return true;
	return false;
}

/*//////////////////////////////////////////////////////////////////
 * Leaf_LookUp Implplementation ///////////////////////////////////
 *//////////////////////////////////////////////////////////////////

void Adaptive_Container::Adaptive_Tree::Leaf_LookUp::Initialize(Adaptive_Container::Adaptive_Tree::Node_Visitor * node_visitor){
	Node * test_node;
	bool end = false;
	//clear the look up table
	leaves_lookup_table.clear();
	//in order travers throught all nodes an add them to leaves_lookup_table
	while( node_visitor->Left_Child() != NULL);
	do{
		if( node_visitor->Is_Leaf( ) ){
			node_visitor->Current_Node()->leaf_number = leaves_lookup_table.size();
			leaves_lookup_table.push_back( node_visitor->Current_Node() );
		}
		if( node_visitor->Right_Child() != NULL)
			while( node_visitor->Left_Child() != NULL);
		else{
			test_node = node_visitor->Current_Node();
			//The conditions must be evaluated from left to right. Is that true?
			while( test_node->parent != NULL && test_node == test_node->parent->right_child )
				test_node = node_visitor->Parent();
			if( node_visitor->Parent() == NULL )
				end = true;
		}
	}while( !end );
}

unsigned int Adaptive_Container::Adaptive_Tree::Leaf_LookUp::Size(){
	return leaves_lookup_table.size();
}

Adaptive_Container::Adaptive_Tree::Node * Adaptive_Container::Adaptive_Tree::Leaf_LookUp::First_Leaf(){
	if( leaves_lookup_table.size() == 0)
		return NULL;
	current_leaf = 0;
	return leaves_lookup_table[current_leaf];
}

Adaptive_Container::Adaptive_Tree::Node * Adaptive_Container::Adaptive_Tree::Leaf_LookUp::Next_Leaf(){
	++current_leaf;
	if( current_leaf >= leaves_lookup_table.size() )
			return NULL;
	return leaves_lookup_table[current_leaf];
}
Adaptive_Container::Adaptive_Tree::Node * Adaptive_Container::Adaptive_Tree::Leaf_LookUp::Leaf( unsigned int index){
	if( index < leaves_lookup_table.size() )
		return leaves_lookup_table[ index ];
	return NULL;
}
/*//////////////////////////////////////////////////////////////////
 * Tree implementaion //////////////////////////////////////////////
 *//////////////////////////////////////////////////////////////////

/* Constructor */
Adaptive_Container::Adaptive_Tree::Adaptive_Tree(){
	tree_root . parent = NULL;
	tree_root . left_child = NULL;
	tree_root . right_child = NULL;
	tree_root . splitting_dimension = 0;
}

Adaptive_Container::Adaptive_Tree::Adaptive_Tree(unsigned int dimension){
	tree_root . parent = NULL;
	tree_root . left_child = NULL;
	tree_root . right_child = NULL;
	tree_root . splitting_dimension = 0;
	tree_dimension = dimension;
	dimension_sizes.resize( dimension + 1, 0);
}

Adaptive_Container::Adaptive_Tree::~Adaptive_Tree(){

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

void Adaptive_Container::Adaptive_Tree::Split_Leaf( Adaptive_Container::Adaptive_Tree::Node * leaf_to_split){
	std::vector< Adaptive_Container::Adaptive_Tree::Node * > nodes_to_add;
	Adaptive_Container::Adaptive_Tree::Node * node;

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

bool Adaptive_Container::Adaptive_Tree::Add_Full_Cube( std::vector < std::vector <bool> > splitting){
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

void Adaptive_Container::Adaptive_Tree::Descend_To_Neighbours(Descend_Info * descend_info, std::vector<Descend_Info> * bigger_neighbours, std::vector<Descend_Info> * smaller_neighbours){
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
void Adaptive_Container::Adaptive_Tree::Find_Neighbours( Node * leaf, std::vector<Descend_Info> * bigger_neighbours, std::vector<Descend_Info> * smaller_neighbours){
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

void Adaptive_Container::Adaptive_Tree::Fill_With_Cells_Of_Full_Cube(std::vector< std::map < int, bool > > * full_cube_cells){
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
}

bool Adaptive_Container::Adaptive_Tree::Cell_Is_Subset_Of_Union (int cell, std::vector< int > * cell_union ){

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

void Adaptive_Container::Adaptive_Tree::Add_Cell_To_Union (int cell, std::vector< int > * cell_union ){

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
void Adaptive_Container::Adaptive_Tree::Finalize_Cube(Node * leaf, std::vector< std::map < int, bool > > * full_cube_cells){
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
	for( dimension_index = 0; dimension_index < tree_dimension; ++dimension_index )
		for( std::map<int, bool>::iterator cell_iterator = (*full_cube_cells)[dimension_index].begin(); cell_iterator != (*full_cube_cells)[dimension_index].end(); ++cell_iterator){
			//If cell is not in smaller neighbours
			if( !Cell_Is_Subset_Of_Union( cell_iterator -> first, &smaller_neighbours_intersection ) ){
				//if cube has a full dimensional cell or cell is in bigger neigbhour
				if( leaf->elementary_cells[ tree_dimension ].size() > 0 || Cell_Is_Subset_Of_Union( cell_iterator -> first, &bigger_neighbours_intersection ) ){
					leaf->elementary_cells[ dimension_index ].insert ( std::pair< int , bool >( cell_iterator -> first, true) );
					++dimension_sizes[ dimension_index ];
				}
			}
		}

	if( leaf->elementary_cells[ tree_dimension ].size() > 0 )
		++dimension_sizes[ tree_dimension ];

	//if nothing was added clear the vector of elementary_cells
	bool vector_is_empty = true;
	for( dimension_index = 0; dimension_index <= tree_dimension && vector_is_empty; ++dimension_index)
		if( leaf->elementary_cells[ dimension_index ].size() > 0 )
			vector_is_empty = false;
	if( vector_is_empty )
		leaf->elementary_cells.clear();
}

void Adaptive_Container::Adaptive_Tree::Finalize(){
	std::vector< std::map < int, bool > > full_cube_cells;

	std::cout << "Initializing tree \n";
	node_visitor.Set_To( &tree_root );
	leaf_lookup.Initialize( &node_visitor);

	std::cout << "Tree initialized \n";
	std::cout << "Number of cubes = " << leaf_lookup.Size() <<  "\n";

	Fill_With_Cells_Of_Full_Cube( &full_cube_cells );
	//Go thru all the leaves and add lower dimensional cells
	for( Node * leaf = leaf_lookup.First_Leaf(); leaf != NULL; leaf = leaf_lookup.Next_Leaf() ){
		//std::cout << "Initaizing cube cubes = " << leaf->leaf_number << " out of " << leaf_lookup.Size() << " \n";
		Finalize_Cube( leaf, &full_cube_cells);
	}
	std::cout << "Cubes Intialized \n";
	return;
}

/*/////////////////////////////////////////////////////////////////
 * Adaptive_Coatinar::const_iterator implementation ///////////////
 */////////////////////////////////////////////////////////////////


Adaptive_Container::const_iterator::const_iterator( void ) : referral(NULL) { }

Adaptive_Container::const_iterator::const_iterator( const   Adaptive_Container * const referral ) : referral(referral) { }

const Adaptive_Container::value_type & Adaptive_Container::const_iterator::operator * ( void ) const {
	dereference_value . name = piece_iterator -> first + (full_cube_number << (referral -> space_dimension << 1 ) );
	dereference_value . dimension = referral -> chain_dimension;
	return dereference_value; } /* endfunction */

const Adaptive_Container::value_type * Adaptive_Container::const_iterator::operator -> ( void ) const {
	return &**this;
} /* address of overloaded * of builtin * of this */

Adaptive_Container::const_iterator & Adaptive_Container::const_iterator::operator ++ ( void ) {

	if( piece_iterator !=referral->adaptive_tree->leaf_lookup.Leaf( full_cube_number )->elementary_cells[ referral->chain_dimension ].end() )
	    if ( ++piece_iterator !=referral->adaptive_tree->leaf_lookup.Leaf( full_cube_number )->elementary_cells[ referral->chain_dimension ].end() ) {
			return *this;
	}
	while( 1 ){
		++full_cube_number;
		if( full_cube_number == referral->adaptive_tree->leaf_lookup.leaves_lookup_table.size() ){
			return *this;
		}
		else if ( ( referral->adaptive_tree->leaf_lookup.Leaf( full_cube_number )->elementary_cells.size() > referral->chain_dimension )  &&
				( referral->adaptive_tree->leaf_lookup.Leaf( full_cube_number )->elementary_cells[ referral->chain_dimension ].size() > 0) ){
			piece_iterator = referral->adaptive_tree->leaf_lookup.Leaf( full_cube_number )->elementary_cells[ referral->chain_dimension ].begin();
			return *this;
		}
	}
} /* endfunction */

bool Adaptive_Container::const_iterator::operator != ( const const_iterator & right_hand_side ) const {
	if( full_cube_number != right_hand_side.full_cube_number ) return true;
	//in the case we are at the end
	if( full_cube_number == referral->adaptive_tree->leaf_lookup.leaves_lookup_table.size() ) return false;
	if( piece_iterator != right_hand_side.piece_iterator ) return true;
	return false;
}

bool Adaptive_Container::const_iterator::operator == ( const const_iterator & right_hand_side ) const {
	if( full_cube_number != right_hand_side.full_cube_number ) return false;
	//in the case we are at the end
	if( full_cube_number == referral->adaptive_tree->leaf_lookup.leaves_lookup_table.size() ) return true;
	if( piece_iterator != right_hand_side.piece_iterator ) return false;
	return true;}

Adaptive_Container::size_type Adaptive_Container::size ( void ) const{
	if ( adaptive_tree->dimension_sizes.size() <= chain_dimension ) return 0;
	return adaptive_tree->dimension_sizes[ chain_dimension ];}

Adaptive_Container::const_iterator Adaptive_Container::begin ( void ) const {
	const_iterator return_value ( this );
	unsigned long temp_full_cube_number = 0;

	while( 1 ){
		if( temp_full_cube_number == adaptive_tree->leaf_lookup.leaves_lookup_table.size() )
			return end();
		else if ( ( adaptive_tree->leaf_lookup.Leaf( temp_full_cube_number )->elementary_cells.size() > chain_dimension )  &&
				( adaptive_tree->leaf_lookup.Leaf( temp_full_cube_number )->elementary_cells[ chain_dimension ].size() > 0) ){
			return_value.full_cube_number = temp_full_cube_number;
			return_value.piece_iterator = adaptive_tree->leaf_lookup.Leaf( temp_full_cube_number )->elementary_cells[ chain_dimension ].begin();
			//To prevent the warning
			//return_value.dereference_value.name = adaptive_tree->leaf_lookup.Leaf( temp_full_cube_number )->elementary_cells[ chain_dimension ].begin() -> first + (return_value.full_cube_number << (chain_dimension << 1 ) );
			//return_value.dereference_value.dimension = chain_dimension;
			return return_value;
		}
		++temp_full_cube_number;
	}
}

Adaptive_Container::const_iterator Adaptive_Container::end ( void ) const {
	const_iterator return_value ( this );
	return_value.full_cube_number = adaptive_tree->leaf_lookup.leaves_lookup_table.size();
	return return_value;
}

Adaptive_Container::const_iterator Adaptive_Container::find ( const Adaptive_Container::key_type & key ) const {
	const_iterator return_value ( this );
	int cell_ID;
	return_value.full_cube_number = key.name >> ( 2 * space_dimension );
	cell_ID = key.name ^ ( return_value.full_cube_number  << ( 2 * space_dimension ) );
	return_value.piece_iterator = adaptive_tree->leaf_lookup.Leaf( return_value.full_cube_number )->elementary_cells[ chain_dimension ].find(cell_ID);
	return return_value;
	//TODO if cell is not present it should return end;
}
/*///////////////////////////////////////////////////////////////////////////////////////
 * Adaptive_Conatiner Implementaion /////////////////////////////////////////////////////
 *///////////////////////////////////////////////////////////////////////////////////////
bool Adaptive_Container::Add_Full_Cube( std::vector < std::vector <bool> > splitting){
	return adaptive_tree->Add_Full_Cube( splitting );
}
void Adaptive_Container::Finalize(){
	adaptive_tree->Finalize();
}
/*/////////////////////////////////////////////////////////////////
* Adaptive_complex implementation /////////////////////////////////
*//////////////////////////////////////////////////////////////////

/* Constructor */
Adaptive_Complex::Adaptive_Complex ( void ) {

} /* endfunction */

Adaptive_Complex::Adaptive_Complex ( unsigned int complex_dimension )  {
	dimension = complex_dimension;
	Adaptive_Container::Adaptive_Tree * tree = new Adaptive_Container::Adaptive_Tree(complex_dimension);

	cells .resize(dimension + 1);
	for(unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) {
		cells [ dimension_index ] .adaptive_tree = tree;
		cells [ dimension_index ] .space_dimension = complex_dimension;
		cells [ dimension_index ] .chain_dimension = dimension_index;
	}
} /* endfunction */

/* Copy Constructor */
Adaptive_Complex::Adaptive_Complex ( const Adaptive_Complex & copy_me) {
	dimension = copy_me.dimension;
	Adaptive_Container::Adaptive_Tree * tree = new Adaptive_Container::Adaptive_Tree(dimension);

	cells .resize(dimension + 1);
	for(unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) {
		cells [ dimension_index ] .adaptive_tree = tree;
		cells [ dimension_index ] .space_dimension = dimension;
		cells [ dimension_index ] .chain_dimension = dimension_index;
	}

	tree->dimension_sizes = copy_me.cells[0].adaptive_tree->dimension_sizes;
	tree->leaf_lookup.leaves_lookup_table.resize( copy_me.cells[0].adaptive_tree->leaf_lookup.leaves_lookup_table.size() );

	/*reconstruct the tree*/
	typedef Adaptive_Container::Adaptive_Tree::Node Node;
	Node * current_node_in_old_tree;
	Node * current_node_in_new_tree;

	std::vector< Node * > old_tree_nodes_stack;
	std::vector< Node * > new_tree_nodes_stack;

	new_tree_nodes_stack.push_back( &tree->tree_root );
	old_tree_nodes_stack.push_back( &copy_me.cells[0].adaptive_tree->tree_root );
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
			tree->leaf_lookup.leaves_lookup_table[ current_node_in_new_tree->leaf_number ] = current_node_in_new_tree;
			if( current_node_in_old_tree->elementary_cells.size() != 0 ){
				current_node_in_new_tree->elementary_cells = current_node_in_old_tree->elementary_cells;
			}
		}
		else{
			Node * new_node_1 = new Adaptive_Container::Adaptive_Tree::Node;
			Node * new_node_2 = new Adaptive_Container::Adaptive_Tree::Node;
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
}
/* Destructor */
Adaptive_Complex::~Adaptive_Complex ( void ) {
	if ( cells.size() > 0 && cells[0].adaptive_tree != NULL )
		delete  cells[0].adaptive_tree;
} /* endfunction */

Adaptive_Complex::Chain & Adaptive_Complex::Boundary_Map ( Adaptive_Complex::Chain & output, const Adaptive_Complex::Container::const_iterator & input) const
{
	int coincidence_index;
	Cell cell;
	Cell lower_dimensional_cell;
	std::vector<Cell> lower_dimensional_cell_pieces;
	std::pair< Cell, long > lower_dimensional_pair;

	cell = *input;

	lower_dimensional_cell.dimension = cell.dimension -1;
	lower_dimensional_pair.first.dimension = cell.dimension -1;

	output.clear();

	coincidence_index = 1;
	/*Go thru all cells which are one dimesion lower than input*/
	for( unsigned int dimension_index = 0; dimension_index < dimension; ++dimension_index ){
		 if( ( cell.name >> ( dimension + dimension_index ) ) % 2 == 0  ){/*if the dimension is not reduced*/
			 /*find the pieces for the left cell*/
			 lower_dimensional_cell.name = cell.name + (1 << ( dimension + dimension_index) );
			 /*Process the chain*/
			 Find_Elementary_Cell( lower_dimensional_cell_pieces, lower_dimensional_cell);
			 for(std::vector<Cell>::iterator cell_iterator = lower_dimensional_cell_pieces.begin();  cell_iterator != lower_dimensional_cell_pieces.end();++ cell_iterator ){
				lower_dimensional_pair.first.name = cell_iterator->name;
				lower_dimensional_pair.second = -coincidence_index;
				output.insert( lower_dimensional_pair );
			 }
			 /*find the pieces for the right cell*/
			 lower_dimensional_cell.name +=  (1 <<  dimension_index );
			 /*Process the chain*/
			Find_Elementary_Cell( lower_dimensional_cell_pieces, lower_dimensional_cell);
			for(std::vector<Cell>::iterator cell_iterator = lower_dimensional_cell_pieces.begin();  cell_iterator != lower_dimensional_cell_pieces.end();++ cell_iterator ){
				lower_dimensional_pair.first.name = cell_iterator->name;
				lower_dimensional_pair.second = coincidence_index;
				output.insert( lower_dimensional_pair );
			 }
			coincidence_index = -coincidence_index;
		 }
	}

	return output;
}

Adaptive_Complex::Chain & Adaptive_Complex::Coboundary_Map ( Adaptive_Complex::Chain & output, const Adaptive_Complex::Container::const_iterator & input) const
{
	unsigned int space_dimension;
	unsigned int full_cube_number;
	unsigned int cell_ID;
	unsigned int neighbour_full_cube_number;
	unsigned int neighbour_cell_ID;
	Cell cell;
	Cell higher_dimensional_cell;
	std::vector<Cell> higher_dimensional_cell_pieces;
	std::pair< Cell, long > higher_dimensional_pair;
	std::vector< Adaptive_Container::Adaptive_Tree::Descend_Info >  possible_owners;

	Adaptive_Container::Adaptive_Tree * adaptive_tree = cells[ input -> dimension ].adaptive_tree;

	space_dimension = cells[ 0 ].space_dimension;
	cell.name = input -> name;
	cell.dimension = input -> dimension;
	full_cube_number = input -> name >> ( 2 * space_dimension );

	output.clear();
	if( cell.dimension >= space_dimension )
		return output;

	higher_dimensional_cell.dimension = cell.dimension + 1;
	higher_dimensional_pair.first.dimension = cell.dimension + 1;

	/* go thru all 1 dimension higher dimensional cells in the cube*/
	for(unsigned int dimension_index = 0; dimension_index < space_dimension; ++dimension_index ){
		cell_ID = cell.name - ( full_cube_number << ( 2 * space_dimension ) );
		if( ( cell_ID >> ( space_dimension + dimension_index ) ) % 2 == 1 ){/*if the dimension is  reduced*/
			cell_ID -=  ( 1 << ( space_dimension + dimension_index) );
			if( ( cell_ID >> ( dimension_index ) ) % 2 == 1 )/*if the dimension is  reduced to the right side*/
				cell_ID -=  (1 <<  dimension_index );
			/* if face is present in the cube add it to coboundary*/
			if( (adaptive_tree->leaf_lookup.Leaf( full_cube_number )->elementary_cells.size() >  higher_dimensional_cell.dimension ) &&
				(adaptive_tree->leaf_lookup.Leaf( full_cube_number )->elementary_cells[ higher_dimensional_cell.dimension ].find( cell_ID) != adaptive_tree->leaf_lookup.Leaf( full_cube_number )->elementary_cells[ higher_dimensional_cell.dimension ].end() )){
				higher_dimensional_cell.name = (full_cube_number << ( 2 * space_dimension ) ) + cell_ID;
				higher_dimensional_pair.first = higher_dimensional_cell;
				higher_dimensional_pair.second = 1;
				output.insert( higher_dimensional_pair );
			}
		}
	}

	/*Find all posiblle neighbours and look if they have a piece of coboundary*/
	Find_Possible_Owners( possible_owners, cell, true );
	/*go thru all neighbours*/
	std::vector< Adaptive_Container::Adaptive_Tree::Descend_Info >::iterator cube_iterator;
	for( cube_iterator = possible_owners.begin(); cube_iterator != possible_owners.end(); ++cube_iterator ){
		//if the original cube is smaller go down the ascent path and update the descend info
		while( cube_iterator->ascending_path.size() > 0 && ( cube_iterator->forward_is_smaller ) ){
			unsigned int splitting_dimension = space_dimension - 1 - ( cube_iterator->ascending_path.size()  - 1 )  %  space_dimension;
			if( cube_iterator->relative_position_table[ splitting_dimension ].back_max_forward_min == 0)
				if( cube_iterator->ascending_path.back() == 0)
					cube_iterator->forward_is_smaller = false;
			if( cube_iterator->relative_position_table[ splitting_dimension ].back_min_forward_max == 0)
				if( cube_iterator->ascending_path.back() == 1)
					cube_iterator->forward_is_smaller = false;
			if( cube_iterator->relative_position_table[ splitting_dimension ].back_min_forward_min == 0)
				if( cube_iterator->ascending_path.back() == 1)
					cube_iterator->relative_position_table[ splitting_dimension ].back_min_forward_min = 1;
			if( cube_iterator->relative_position_table[ splitting_dimension ].back_max_forward_max == 0)
				if( cube_iterator->ascending_path.back() == 0)
					cube_iterator->relative_position_table[ splitting_dimension ].back_max_forward_max = -1;
			cube_iterator->ascending_path.pop_back();
		}
		/* look at all possible coboundry pieces in the cube*/
		cell_ID = cell.name - ( full_cube_number << ( 2 * space_dimension ) );
		for(unsigned int expand_dimension = 0; expand_dimension < space_dimension; ++expand_dimension ){
			bool cell_present_in_neighbour = true;
			neighbour_cell_ID = 0;
			if( ( cell_ID >> ( space_dimension + expand_dimension ) ) % 2 == 1 ){/*if the dimension is  reduced then expand it if it is possible to expand*/
				if( cube_iterator->relative_position_table[ expand_dimension ].back_min_forward_min == 1 &&  cube_iterator->relative_position_table[ expand_dimension ].back_min_forward_max != 0 )
					if( ( cell_ID >>  expand_dimension ) % 2 == 0 )
						cell_present_in_neighbour = false;
				if( cube_iterator->relative_position_table[ expand_dimension ].back_max_forward_max == -1 &&  cube_iterator->relative_position_table[ expand_dimension ].back_max_forward_min != 0 )
					if( ( cell_ID >>  expand_dimension ) % 2 == 1 )
						cell_present_in_neighbour = false;
				if( cell_present_in_neighbour )
					for(unsigned int dimension_index = 0; dimension_index < space_dimension; ++dimension_index ){
						if( ( ( cell_ID >> ( space_dimension + dimension_index ) ) % 2 == 1 ) && (expand_dimension != dimension_index ) ){/*if the dimension is  reduced and was not expanded*/
							//if the face is not at the boundary of the neighbouring cube
							if( cube_iterator->relative_position_table[ dimension_index ].back_min_forward_min == 1 &&  cube_iterator->relative_position_table[ dimension_index ].back_min_forward_max != 0 )
								if( ( cell_ID >>  dimension_index ) % 2 == 0 )
									cell_present_in_neighbour = false;
							if( cube_iterator->relative_position_table[ dimension_index ].back_max_forward_max == -1 &&  cube_iterator->relative_position_table[ dimension_index ].back_max_forward_min != 0 )
								if( ( cell_ID >>  dimension_index ) % 2 == 1 )
									cell_present_in_neighbour = false;
							if( cell_present_in_neighbour ){
								neighbour_cell_ID += 1 << ( dimension_index + space_dimension );
								if( cube_iterator->relative_position_table[ dimension_index ].back_min_forward_max == 0	)/*cubes are on the opposite sides ie the face is on the right hand side*/
									neighbour_cell_ID += 1 << ( dimension_index );
								else if ( cube_iterator->relative_position_table[ dimension_index ].back_max_forward_min != 0 &&
										( cell_ID >>  dimension_index ) % 2 == 1  )/*cubes are on the same side again face is on the right hand side*/
									neighbour_cell_ID += 1 << ( dimension_index );
							}
						}
					}
					/*Add the  the cell if it is present*/
					if( cell_present_in_neighbour ){
						neighbour_full_cube_number = cube_iterator->current_node->leaf_number;
						if( (adaptive_tree->leaf_lookup.Leaf( neighbour_full_cube_number )->elementary_cells.size() >  higher_dimensional_cell.dimension ) &&
							(adaptive_tree->leaf_lookup.Leaf( neighbour_full_cube_number )->elementary_cells[ higher_dimensional_cell.dimension ].find( neighbour_cell_ID) != adaptive_tree->leaf_lookup.Leaf( neighbour_full_cube_number )->elementary_cells[ higher_dimensional_cell.dimension ].end() )){
							higher_dimensional_cell.name = (neighbour_full_cube_number << ( 2 * space_dimension ) ) + neighbour_cell_ID;
							higher_dimensional_pair.first = higher_dimensional_cell;
							higher_dimensional_pair.second = 1;
							output.insert( higher_dimensional_pair );
						}
					}
			}
		}
	}
	return output;
}

void Adaptive_Complex::Remove_Cell ( const Adaptive_Complex::Cell & input) {
	unsigned int full_cube_number;
	unsigned int cell_ID;
	Adaptive_Container::Adaptive_Tree * adaptive_tree = cells[ input.dimension ].adaptive_tree;

	full_cube_number = input.name >> ( 2 * dimension );
	cell_ID = input.name ^ ( full_cube_number << (2 * dimension ) );

	unsigned int size = adaptive_tree->leaf_lookup.Leaf( full_cube_number )->elementary_cells[ input.dimension ].size();
	adaptive_tree->leaf_lookup.Leaf( full_cube_number )->elementary_cells[ input.dimension ].erase(cell_ID);
	if( size > adaptive_tree->leaf_lookup.Leaf( full_cube_number )->elementary_cells[ input.dimension ].size() )
		-- adaptive_tree -> dimension_sizes [ input.dimension];
}

std::vector< double > & Adaptive_Complex::Coordinates_Of_Cell ( const Cell & input,  std::vector< double > & coordinates){
	unsigned int full_cube_number;
	Adaptive_Container::Adaptive_Tree * adaptive_tree = cells[ input.dimension ].adaptive_tree;

	Adaptive_Container::Adaptive_Tree::Node * new_node = NULL;
	Adaptive_Container::Adaptive_Tree::Node * old_node = NULL;
	std::vector<int> acsending_path;

	full_cube_number = input.name >> ( 2 * dimension );
	new_node = adaptive_tree->leaf_lookup.Leaf( full_cube_number );
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
	coordinates.resize( dimension );
	unsigned int split_count = 1;
	unsigned int splitting_dimension = 0;
	while( acsending_path.size() > 0 ){
		if( acsending_path.back() == 1 )
			coordinates[ splitting_dimension ] += (double)1 / (double) ( 1 << split_count );

		++splitting_dimension;
		splitting_dimension = splitting_dimension % dimension;
		if( splitting_dimension == 0 )
			++split_count;
		acsending_path.pop_back();
	}
	return coordinates;
}
bool Adaptive_Complex::Add_Full_Cube( std::vector < std::vector <bool> > splitting) {
	if ( cells.size() == 0 )
		return false;
	return cells[0].Add_Full_Cube( splitting );
}

void Adaptive_Complex::Finalize(){
	if ( cells.size() != 0 )
		cells[0].Finalize( );
}

std::vector<Adaptive_Complex::Cell> & Adaptive_Complex::Find_Elementary_Cell( std::vector<Adaptive_Complex::Cell> & output, const Adaptive_Complex::Cell & input) const {

	unsigned int full_cube_number;
	unsigned int cell_ID;
	unsigned int neighbour_cube_number;
	unsigned int neighbour_cell_ID;
	Adaptive_Container::Adaptive_Tree * adaptive_tree = cells[ input.dimension ].adaptive_tree;
	std::vector< Adaptive_Container::Adaptive_Tree::Descend_Info > possible_owners;

	output.clear();

	full_cube_number = input.name >> ( 2 * dimension );
	cell_ID = input.name - ( full_cube_number << (2 * dimension ) );

	//if cell is present in the cube with ID full_cube_number
	std::map < int, bool > elementary_cells = adaptive_tree->leaf_lookup.Leaf( full_cube_number )->elementary_cells[ input.dimension ];
	if( elementary_cells.find( cell_ID) != elementary_cells.end() ){
		output.push_back( input );
		return output;
	}

	Find_Possible_Owners( possible_owners, input, false);
	/* Go thru all possible owners and add the cells */
	std::vector< Adaptive_Container::Adaptive_Tree::Descend_Info >::iterator it;
	for( it = possible_owners.begin(); it != possible_owners.end(); ++it){
		neighbour_cube_number = it->current_node->leaf_number;
		neighbour_cell_ID = 0;
		for(unsigned int dimension_index = 0; dimension_index < dimension; ++dimension_index){
			if( ( ( cell_ID >> ( dimension_index + dimension ) ) % 2 ) == 1 ){/*if it is a reduced dimension*/
				neighbour_cell_ID +=1 << ( dimension_index + dimension );
				if( ( ( cell_ID >>  dimension_index   ) % 2 ) == 1 ){/*if cell is on the right side*/
					if( it->relative_position_table[ dimension_index ].back_max_forward_min != 0 ) /* cubes are not on the opposite sides (same side)*/
						neighbour_cell_ID += ( 1 << dimension_index );
				}
				else /*if cell is on the left side*/
					if( it->relative_position_table[ dimension_index ].back_min_forward_max == 0 ) /* cubes are on the opposite sides */
						neighbour_cell_ID +=1 << dimension_index;
			}
			if( it->current_node->elementary_cells.size() > input.dimension )
				if( it->current_node->elementary_cells[ input.dimension ].find( neighbour_cell_ID ) != it->current_node->elementary_cells[ input.dimension ].end() ){
					Cell insert_chain;
					insert_chain.dimension = input.dimension;
					insert_chain.name = ( neighbour_cube_number << ( 2 * dimension ) ) + neighbour_cell_ID;
					output.push_back( insert_chain );
				}
		}
	}
	return output;
}

std::vector< Adaptive_Container::Adaptive_Tree::Descend_Info > & Adaptive_Complex::Find_Possible_Owners( std::vector< Adaptive_Container::Adaptive_Tree::Descend_Info > & possible_owners, const Adaptive_Complex::Cell & input, bool to_all_neighbours ) const {
	unsigned int full_cube_number;
	unsigned int cell_ID;
	/* Bit 1 at space_dimension + splitting_dimension - 1 means that a node with the splitting_dimension is supposed to be viseted from left
	 * Bit 1 at splitting_dimension - 1 means that a node with the splitting_dimension is supposed to be viseted from right
	 */
	int nodes_to_be_visited = 0;
	int nodes_visited = 0;
	Adaptive_Container::Adaptive_Tree * adaptive_tree = cells[ input.dimension ].adaptive_tree;

	Adaptive_Container::Adaptive_Tree::Node * new_node = NULL;
	Adaptive_Container::Adaptive_Tree::Node * old_node = NULL;
	std::vector<int> acsending_path;
	Adaptive_Container::Adaptive_Tree::Descend_Info descend_info;

	full_cube_number = input.name >> ( 2 * dimension );
	cell_ID = input.name - ( full_cube_number << (2 * dimension ) );
	new_node = adaptive_tree->leaf_lookup.Leaf( full_cube_number );

	//prepare nodes_to_be_visited
	for(unsigned int dimension_index = 0; dimension_index < dimension; ++dimension_index){
		if( ( cell_ID >> ( dimension_index + dimension ) ) % 2 != 0 )/* reduced dimension */
			if( ( cell_ID >> dimension_index ) % 2 != 0 ) /* reduced to the right : suppose to be visited from left*/
				nodes_to_be_visited += ( 1 << ( dimension_index +  dimension ) );
			else /* reduced to the left : suppose to be visited from the right */
				nodes_to_be_visited += ( 1 << dimension_index  );
	}
	while( ( new_node->parent != NULL ) && ( nodes_to_be_visited != nodes_visited ) ){
			//climbing up
			old_node = new_node;
			new_node = new_node->parent;
			//We arrived from left
			if( new_node->left_child == old_node)
			{
				//if we never arived from left before and its a splitting dimension visited from the left child
				if( (  ( nodes_visited >> ( dimension + new_node->splitting_dimension - 1 ) ) % 2 == 0 ) &&
						( ( nodes_to_be_visited >> ( dimension + new_node->splitting_dimension - 1 ) ) % 2 != 0 ) ){
					// update visited_nodes and start descending
					nodes_visited += ( 1 << ( dimension + new_node->splitting_dimension - 1 ) );
					descend_info.current_node = new_node->right_child;
					descend_info.back_before_forward = true;
					descend_info.forward_is_smaller = false;
					descend_info.ascending_path = acsending_path;
					descend_info.relative_position_table.clear();
					descend_info.relative_position_table.resize(dimension);
					descend_info.relative_position_table[ new_node->splitting_dimension - 1 ].Set_To_Opposite( true );
					Descend_To_Possible_Owners( possible_owners, descend_info, nodes_visited,  to_all_neighbours );
				}
				acsending_path.push_back( 0 );
			}
			//we arived from right
			else{
				//if we never arived from right before and its a pliting dimension visited from the right child
				if( ( ( nodes_visited >> ( new_node->splitting_dimension - 1 ) ) % 2 == 0 ) &&
						( ( nodes_to_be_visited >> ( new_node->splitting_dimension - 1 ) ) % 2 != 0 ) ){
					// update visited_nodes and start descending
					nodes_visited += 1 << ( new_node->splitting_dimension - 1 );
					descend_info.current_node = new_node->left_child;
					descend_info.back_before_forward = false;
					descend_info.forward_is_smaller = false;
					descend_info.ascending_path = acsending_path;
					descend_info.relative_position_table.clear();
					descend_info.relative_position_table.resize(dimension);
					descend_info.relative_position_table[ new_node->splitting_dimension - 1 ].Set_To_Opposite( false );
					Descend_To_Possible_Owners( possible_owners, descend_info, nodes_visited, to_all_neighbours  );
				}
				acsending_path.push_back( 1 );
			}
		}

	return possible_owners;
}


std::vector< Adaptive_Container::Adaptive_Tree::Descend_Info > & Adaptive_Complex::Descend_To_Possible_Owners( std::vector< Adaptive_Container::Adaptive_Tree::Descend_Info > & possible_owners, Adaptive_Container::Adaptive_Tree::Descend_Info descend_info, int splitting_nodes, bool to_all_neighbours ) const{
	std::vector<Adaptive_Container::Adaptive_Tree::Descend_Info> descend_info_stack;
	Adaptive_Container::Adaptive_Tree::Descend_Info current_descend;
	unsigned int space_dimension = dimension;

	descend_info_stack.push_back(descend_info);
	while( descend_info_stack.size() > 0 ){
		current_descend = descend_info_stack.back();
		descend_info_stack.pop_back();
		while(  current_descend.current_node->splitting_dimension != 0  ){/*Node in the descending path is not a leaf*/
			if( current_descend.ascending_path.size() > 0 ){ /*Ascending path is not empty*/
				//if cubes are on the opposite sides in this dimension we have to take the opposite child as we came from in ascending_path
				if( current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_min_forward_max == 0 ||
					current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_max_forward_min == 0 ){
					if( current_descend.ascending_path.back() == 0 )
						current_descend.current_node = current_descend.current_node->right_child;
					else
						current_descend.current_node = current_descend.current_node->left_child;
					current_descend.ascending_path.pop_back();
				}
				//current_descend.ascending_path is not empty so if the cubes are not on the opposite sides then they have a common side in this dimension
				else{
					//if the node is a spliiting node (node with reduced dimension visited from the right child )
					if( ( current_descend.ascending_path.back() == 0 && (splitting_nodes >> (space_dimension + current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 ) ||
					  ( current_descend.ascending_path.back() == 1 && (splitting_nodes >> ( current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 )	)
					{
						bool last_split = true;
						//check if there is another split in the same dimension and the same direction as the one for the current_node
						if ( current_descend.ascending_path.size() > space_dimension )
							for( unsigned int split =  space_dimension - current_descend.current_node->splitting_dimension;    split < ( current_descend.ascending_path.size() - space_dimension + 1); split +=  space_dimension )
								if( current_descend.ascending_path[ split ] == current_descend.ascending_path[ current_descend.ascending_path.size() - 1 ] )
									last_split = false;
						//for the last splitting we have to check also the opposite path
						if( last_split ){
							Adaptive_Container::Adaptive_Tree::Descend_Info copy_current_descend;
							copy_current_descend.current_node = current_descend.current_node;
							copy_current_descend.relative_position_table = current_descend.relative_position_table;
							copy_current_descend.back_before_forward = current_descend.back_before_forward;
							copy_current_descend.ascending_path = current_descend.ascending_path;
							copy_current_descend.forward_is_smaller = current_descend.forward_is_smaller;
							if( copy_current_descend.ascending_path.back() == 0 ){
								copy_current_descend.relative_position_table[ copy_current_descend.current_node->splitting_dimension - 1 ].Set_To_Opposite( true );
								copy_current_descend.current_node = copy_current_descend.current_node->right_child;
							}
							else{
								copy_current_descend.relative_position_table[ copy_current_descend.current_node->splitting_dimension - 1 ].Set_To_Opposite( false );
								copy_current_descend.current_node = copy_current_descend.current_node->left_child;
							}
							copy_current_descend.ascending_path.pop_back();
							//add the copy to the stack
							descend_info_stack.push_back( copy_current_descend );
						}
					}
					//This path is taken in both case cubes on the same side and it is not the last spliting in this dimension and this direction (it is the last splitting)
					if( current_descend.ascending_path.back() == 0 )
						current_descend.current_node = current_descend.current_node->left_child;
					else
						current_descend.current_node = current_descend.current_node->right_child;
					current_descend.ascending_path.pop_back();
				}
			}
			else{/* Ascending path is empty */
				current_descend.forward_is_smaller = true;
				if( ( splitting_nodes >> (space_dimension + current_descend.current_node->splitting_dimension - 1 )) % 2 == 1  ||
				    (splitting_nodes >> ( current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 	){/*if the node is a spliiting node*/

					if( current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_max_forward_min == 0 )/*Forward cube on the right side*/
						current_descend.current_node = current_descend.current_node->left_child;
					else if( current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_min_forward_max == 0 )/*Forward cube on the left side*/
						current_descend.current_node = current_descend.current_node->right_child;
					else{/* cubes are touching in an interval*/
						if( ( splitting_nodes >> (space_dimension + current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 ){/*cell is on the right*/
							current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_min_forward_min = -1;
							current_descend.current_node = current_descend.current_node->right_child;
						}
						else if( ( splitting_nodes >> ( current_descend.current_node->splitting_dimension - 1 )) % 2 == 1 ){/*cell is on the left*/
							current_descend.relative_position_table[ current_descend.current_node->splitting_dimension - 1 ].back_max_forward_max = 1;
							current_descend.current_node = current_descend.current_node->left_child;
						}
					}
				}
				else{/* node is not a splitting node*/
					/*Add the right cube to the stack*/
					Adaptive_Container::Adaptive_Tree::Descend_Info copy_current_descend;
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
				}
			}
		}
		/* Cube is not smaller but is the first one in the lexicographical order */
		if( ( current_descend.ascending_path.size() == 0 ) && ( !descend_info.back_before_forward ) )
			current_descend.forward_is_smaller = true;
		/* if we searhc for all the neighbours */
		if( to_all_neighbours )
			current_descend.forward_is_smaller = true;
		if( current_descend.forward_is_smaller )
			possible_owners.push_back( current_descend );
	}
	return possible_owners;
}

#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */
#endif
