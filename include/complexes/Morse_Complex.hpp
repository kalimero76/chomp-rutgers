/*
 *  Morse_Complex.hpp
 *
 *  Created by Shaun Harker on 10/21/09.
 *
 */


#include <deque> /* for deque in AKQ algorithm */
#include <set> /* for set in AKQ algorithm */
#include <functional> /* for greater<> in morse value orderings */
#include <iostream>

#include <time.h>

/* Morse_Complex Class */


/* morse_detail namespace. Contains dispatch functions. */

namespace morse_detail {

template < class Cell_Complex_Template >
typename Cell_Complex_Template::Container::const_iterator &
Husband ( const Morse_Complex<Cell_Complex_Template> & morse_complex, const typename Cell_Complex_Template::Container::const_iterator & input,
typename Morse_Traits<Cell_Complex_Template>::doesnt_store_husband_pointers) {
	return morse_complex . husband_pointers [ input -> first ]; } /* endfunction */

template < class Cell_Complex_Template >
typename Cell_Complex_Template::Container::const_iterator & 
Husband ( const Morse_Complex<Cell_Complex_Template> & morse_complex, const typename Cell_Complex_Template::Container::const_iterator & input,
typename Morse_Traits<Cell_Complex_Template>::does_store_husband_pointers) {
	return morse_complex . original_complex . Husband ( input ); } /* endfunction */

template < class Cell_Complex_Template >
typename Morse_Traits<Cell_Complex_Template>::morse_value_type & 
Morse_Value ( const Morse_Complex<Cell_Complex_Template> & morse_complex, const typename Cell_Complex_Template::Container::const_iterator & input,
typename Morse_Traits<Cell_Complex_Template>::doesnt_store_morse_values) {
	return morse_complex . morse_values [ input -> first ]; } /* endfunction */

template < class Cell_Complex_Template >
typename Morse_Traits<Cell_Complex_Template>::morse_value_type & 
Morse_Value ( const Morse_Complex<Cell_Complex_Template> & morse_complex, const typename Cell_Complex_Template::Container::const_iterator & input,
typename Morse_Traits<Cell_Complex_Template>::does_store_morse_values) {
	return morse_complex . original_complex . Morse_Value ( input ); }

template < class Cell_Complex_Template >
unsigned char & Flags ( const Morse_Complex<Cell_Complex_Template> & morse_complex, const typename Cell_Complex_Template::Container::const_iterator & input,
typename Morse_Traits<Cell_Complex_Template>::doesnt_store_flags) {
	return morse_complex . flags [ input -> first ]; } /* endfunction */

template < class Cell_Complex_Template >
unsigned char & Flags ( const Morse_Complex<Cell_Complex_Template> & morse_complex, const typename Cell_Complex_Template::Container::const_iterator & input,
typename Morse_Traits<Cell_Complex_Template>::does_store_flags) {
	return morse_complex . original_complex . Flags ( input ); }

} /* namespace morse_detail */

/* Private Member Functions */

template < class Cell_Complex_Template >
typename Cell_Complex_Template::Container::const_iterator & Morse_Complex<Cell_Complex_Template>::
Husband ( const typename Cell_Complex_Template::Container::const_iterator & input) const {
	typename Morse_Traits<Cell_Complex_Template>::does_it_store_husband_pointers category;
	return morse_detail::Husband ( *this, input, category ); } /* endfunction */

template < class Cell_Complex_Template >
typename Morse_Traits<Cell_Complex_Template>::morse_value_type & Morse_Complex<Cell_Complex_Template>::
Morse_Value ( const typename Cell_Complex_Template::Container::const_iterator & input) const {
	typename Morse_Traits<Cell_Complex_Template>::does_it_store_morse_values category;
	return morse_detail::Morse_Value ( *this, input, category ); } /* endfunction */
	
template < class Cell_Complex_Template >
unsigned char & Morse_Complex<Cell_Complex_Template>::Flags ( const typename Cell_Complex_Template::Container::const_iterator & input ) const {
	typename Morse_Traits<Cell_Complex_Template>::does_it_store_flags category;
	return morse_detail::Flags ( *this, input, category ); } /* endfunction */
	
template < class Cell_Complex_Template >
bool Morse_Complex<Cell_Complex_Template>::Is_an_Ace ( unsigned char input) const {
	return input & ACE ? true : false; }

template < class Cell_Complex_Template >
bool Morse_Complex<Cell_Complex_Template>::Is_a_King ( unsigned char input ) const {
	return input & KING ? true : false; }

template < class Cell_Complex_Template >
bool Morse_Complex<Cell_Complex_Template>::Is_a_Queen ( unsigned char input ) const {
	return input & QUEEN ? true : false; }

template < class Cell_Complex_Template >
bool Morse_Complex<Cell_Complex_Template>::Is_Alive ( unsigned char input ) const {
	return input & ALIVE ? true : false; }

template < class Cell_Complex_Template >
bool Morse_Complex<Cell_Complex_Template>::Is_Excised ( unsigned char input ) const {
	return input & EXCISED ? true : false; }


/* Public Member Functions */

/* Constructor */
template < class Cell_Complex_Template >
Morse_Complex<Cell_Complex_Template>::Morse_Complex ( const Cell_Complex_Template & original_complex) : original_complex(original_complex) {
	clock_t start_time, stop_time;
	std::cout << "Constructing Morse Complex.\n";
	start_time = clock ();
	Ace_King_Queen_Algorithm ();
	stop_time = clock ();
	std::cout << "Ace King Queen algorithm complete. Time: " << (float) (stop_time - start_time) / (float) CLOCKS_PER_SEC << "\n";
	start_time = clock ();
	Morse_Boundary_Algorithm ();
	stop_time = clock ();
	std::cout << "Morse Boundary algorithm complete. Time: " << (float) (stop_time - start_time) / (float) CLOCKS_PER_SEC  << "\n";
} /* endfunction */

/* Ace-King-Queen Algorithm */
template < class Cell_Complex_Template >
void Morse_Complex<Cell_Complex_Template>::Ace_King_Queen_Algorithm ( void ) {
	/* We begin with any complex. */

	/* This is a specific implementation of the Ace-King-Queen algorithm
	 * based on greedy usage of King-Queen pairs which are dealt with on a
	 * first come, first serve basis (which results in a breadth first pattern.)
	 * When King-Queen excisions are exhausted, we excise Ace. We then return to
	 * finding King-Queen pairs again. This process is repeated until the entire
	 * complex has been excised.  */

	 /* Implementation notes.
		We make a copy of the complex and reduce it. We store the information
		using const_iterators from the original complex. */

	/* Copy the complex. */
	//std::cout << "AKQ: Copying complex.\n";

	Cell_Complex_Template copy_complex = original_complex ; /* = original complex */


	/* Simplify iterator semantics. */
	typedef typename Cell_Complex_Template::Container::const_iterator const_iterator;

	/* Convenient typedefs */
	typedef typename Cell_Complex_Template::Elementary_Chain Elementary_Chain;
	typedef typename Cell_Complex_Template::Chain Chain;
	typedef typename Cell_Complex_Template::Ring Ring;

	/* Initialize working_queue for Batko-Mrozek method of queuing coreduction pairs. */

	std::deque< Elementary_Chain > working_queue;

	/* Perform initial sweep for K-Q excisions */
	//std::cout << "AKQ: Initial excision sweep.\n";

	for ( unsigned int dimension_index = 0; dimension_index <= copy_complex . dimension; ++ dimension_index )
		for ( const_iterator elementary_chain_iterator = copy_complex . Chain_Groups [ dimension_index ] . begin ();
		elementary_chain_iterator != copy_complex . Chain_Groups [ dimension_index ] . end (); ++ elementary_chain_iterator )
			if ( copy_complex . Count_Boundaries ( elementary_chain_iterator ) == 1 ) {
				working_queue . push_back ( elementary_chain_iterator -> first );
			}
			
	/* Copy working queue into set. */
	std::set< Elementary_Chain > working_set ( working_queue . begin (), working_queue . end () );

	//std::cout << "Entering main AKQ loop. Initial size = " << copy_complex . size () << "\n";

	/* Loop while complex is non-empty. */
	int old_percent = 0;
	while ( copy_complex . size () ) {
		/* Perform K-Q excisions until none remain. */
		//std::cout << "Entered KQ phase\n";
		while ( not working_queue . empty () ) {
			//std::cout << "=== TOP OF KQ Phase Loop ===\n";
			//std::cout << "\n\n";
			int percent = 100 - ( copy_complex . size () * 100 ) / original_complex . size ();
			if ( percent > old_percent ) {
				std::cout << "\r" << copy_complex . size () << " " << original_complex . size () << " (" << percent << ") ";
				std::flush(std::cout);
				old_percent = percent; } /* if */



			/* Pull a cell from the queue that is still in "copy_complex" */
			while ( not working_queue . empty () && working_set . find ( working_queue . front () ) == working_set . end () ) working_queue . pop_front ();
			if ( working_queue . empty () ) break;
			Elementary_Chain cell = working_queue . front ();

			/* The cell is in "copy_complex". We obtain a const_iterator for it. */
			const_iterator cell_iterator = copy_complex . find ( cell );
			
			/* How many cells are in the boundary of "cell"? */
			const int number_of_boundaries = copy_complex . Count_Boundaries ( cell_iterator );

			/* Depending on whether there are 0, 1, or more boundaries of cell in "copy_complex", there are three different behaviors: */
			switch ( number_of_boundaries ) {
			case 1:
				{ /* scope */
				/* Get the one term chain that is the boundary. */
				Chain boundary_chain; copy_complex . Boundary_Map ( boundary_chain, cell_iterator );

				/* Read the one term off from the boundary. */
				std::pair < Elementary_Chain , Ring > queen_term =  * boundary_chain . begin ();

				/* If the ring element is invertible, onwards. Otherwise we can do nothing, so we break. Note Alg 6.1 skips this step. */
				/* OPTIMIZATION TACTIC: Use TRAITS to optimize this line out if possible. */
				/* ERROR: following line shouldn't be commented, so until it is fixed this only works when non-zero
				 * incidence numbers are all units */
				//if ( not chain_term . second . is_invertible () ) break;
				/* We have found an elementary coreduction pair.
				 * Locate the pair in the original complex. */
				const_iterator King = original_complex . find ( cell );
				const_iterator Queen = original_complex . find ( queen_term . first );

				//std::cout << "----- AKQ: Removing the KQ pair " << King -> first << " " << Queen -> first << " -----\n";

				/* Loop through King's boundaries that are not Queen to gather information. */
				Chain original_boundary_chain;
				
				original_complex . Boundary_Map ( original_boundary_chain, King );

				unsigned long int current_morse_value = 0;
				bool alive = false;
				for ( typename Chain::const_iterator chain_term_iterator = original_boundary_chain . begin ();
				chain_term_iterator != original_boundary_chain . end (); ++ chain_term_iterator ) {
					if ( chain_term_iterator -> first == Queen -> first ) continue;
					const_iterator Mistress = original_complex . find ( chain_term_iterator -> first );
					unsigned long int observed_morse_value = Morse_Value ( Mistress );
					if ( observed_morse_value > current_morse_value ) current_morse_value = observed_morse_value;
					if ( Is_Alive ( Flags ( Mistress ) ) ) alive = true; 
				} /* for */

				/* Assign Morse Value */
				Morse_Value ( King ) = Morse_Value ( Queen ) = current_morse_value + 1;
				/* Assign Husband */
				Husband ( Queen ) = King;
				/* Assign Flags */
				Flags ( King ) = KING | EXCISED;
				Flags ( Queen ) = alive ? QUEEN | ALIVE : QUEEN;

				/* Excise the King. */
				working_queue . pop_front ();
				working_set . erase ( cell );
				copy_complex . Remove_Elementary_Chain ( cell );

				/* Find the coboundary of the lower dimensional part. */
				Chain coboundary_chain; copy_complex . Coboundary_Map ( coboundary_chain, queen_term . first );
				//std::cout << "    (queen term) coboundary of " << queen_term . first << " = " << coboundary_chain << "\n";

				/* Loop through coboundary terms and add them to working list. */
				for ( typename Chain::const_iterator coboundary_term = coboundary_chain . begin ();
				coboundary_term != coboundary_chain . end (); coboundary_term ++ ) {
					/* Put the elementary chain associated with the coboundary term in the working queue to be processed. */
					const Elementary_Chain & coboundary_cell = coboundary_term -> first;
					if ( working_set . insert ( coboundary_cell) . second == true ) {
						/* We really inserted the element, it wasn't already there */
						working_queue . push_back ( coboundary_cell );
					} /* if */
				} /* for */

				/* Now excise the Queen. */
				working_set . erase ( queen_term . first );								
				copy_complex . Remove_Elementary_Chain ( queen_term . first );

				break;
				} /* scope */
				
			case 0:
				{ /* scope */
				//std::cout << "----- AKQ: No boundaries for " << cell << " -----\n";

				/* Find the coboundary. */
				Chain coboundary_chain;
				copy_complex . Coboundary_Map( coboundary_chain, cell_iterator );
				//std::cout << "    coboundary of " << generator -> first << " = " << coboundary_chain << "\n";
				/* Loop through coboundary terms and add them to list. */
				for ( typename Chain::const_iterator coboundary_term = coboundary_chain . begin ();
				coboundary_term != coboundary_chain . end (); coboundary_term ++ ) {
					/* Put the elementary chain associated with the coboundary term in the working queue to be processed. */
					const Elementary_Chain & coboundary_cell = coboundary_term -> first;
					if ( working_set . insert ( coboundary_cell ) . second == true ) {
						/* We really inserted the element, it wasn't already there */
						working_queue . push_back ( coboundary_cell );
					} /* if */
				} /* for */
				
				/* We are done with queue element. We can remove it. */
				working_queue . pop_front ();
				working_set . erase ( cell );

				break;
				} /* scope */
			default:
				//std::cout << "----- AKQ: More than two boundaries for " << cell << " -----\n";

				/* We are done with queue element. We can remove it. */
				working_queue . pop_front ();
				working_set . erase ( cell );
			} /* switch */
		} /* while */

		//std::cout << "KQ Phase exited. Copy complex size = " << copy_complex . size () << " \n";

		/* Now the K-Q pairs are excised. If complex is now empty, break loop. */
		if ( copy_complex . size () == 0 ) break;


		/* Excise an Ace. */

		/* Get a low dimension cell for excision. */
		const_iterator cell_iterator = copy_complex . begin ();
		Elementary_Chain cell = cell_iterator -> first;
		
		/* Identify the Ace cell in the original complex. */
		const_iterator Ace = original_complex . find ( cell );

		//std::cout << "----- AKQ: Excising the Ace " << Ace -> first << " -----\n";

		/* Figure out what the Morse value should be by boundary checking. */
		Chain original_boundary_chain;
		original_complex . Boundary_Map ( original_boundary_chain, Ace );


		unsigned long int current_morse_value = 0;
		for ( typename Chain::const_iterator chain_term_iterator = original_boundary_chain . begin ();
		chain_term_iterator != original_boundary_chain . end (); ++ chain_term_iterator ) {
			const_iterator Mistress = original_complex . find ( chain_term_iterator -> first );
			unsigned long int observed_morse_value = Morse_Value ( Mistress );
			if ( observed_morse_value > current_morse_value ) current_morse_value = observed_morse_value; } /* endfor */


		Morse_Value ( Ace ) = current_morse_value + 1;
		Flags ( Ace ) = ACE | ALIVE;


		/* Put all the coboundary elements of the Ace in the working list */
		Chain coboundary_chain; copy_complex . Coboundary_Map ( coboundary_chain, cell_iterator );

		for ( typename Chain::const_iterator coboundary_term = coboundary_chain . begin ();
		coboundary_term != coboundary_chain . end (); coboundary_term ++ ) {
			/* Put the elementary chain associated with the coboundary term in the working queue to be processed. */
			const Elementary_Chain & coboundary_cell = coboundary_term -> first;
			if ( working_set . insert ( coboundary_cell ) . second == true ) {
				/* We really inserted the element, it wasn't already there */
				working_queue . push_back ( coboundary_cell );
			} /* if */
		} /* for */


		/* Now excise the Ace. */
		copy_complex . Remove_Elementary_Chain ( cell );

	} /* while */
	/* We are done. */
	//std::cout << "AKQ Decomposition Complete\n";

} /* endfunction */




template < class Morse_Complex_Template >
Morse_Value_Elementary_Chain < Morse_Complex_Template > ::
Morse_Value_Elementary_Chain ( Morse_Value_Type morse_value,  typename Cell_Complex_Template::Container::const_iterator location )
: morse_value(morse_value), location(location) {}

template < class Morse_Complex_Template >
bool Morse_Value_Elementary_Chain < Morse_Complex_Template > ::
operator < ( const Morse_Value_Elementary_Chain & right_hand_side ) const {
	if ( right_hand_side . morse_value < morse_value ) return true;
	if ( morse_value < right_hand_side . morse_value ) return false;
	return location -> first . Cell_Complex_Template::Elementary_Chain::operator <
		( right_hand_side . location -> first ); }

template < class Morse_Complex_Template >
std::ostream & operator << (std::ostream & output_stream, const Morse_Value_Elementary_Chain < Morse_Complex_Template > & print_me) {
	output_stream << "(" << print_me . location -> first . name << ", " << print_me . location -> first. dimension << "; " << print_me . morse_value << ")";
	return output_stream; }

template < class Morse_Complex_Template >
Morse_Value_Chain < Morse_Complex_Template > ::
Morse_Value_Chain ( void ) {}

template < class Morse_Complex_Template >
Morse_Value_Chain < Morse_Complex_Template > ::
Morse_Value_Chain ( const Original_Chain & copy_me, const Morse_Complex_Template & morse_complex ) {
	const Cell_Complex_Type & original_complex = morse_complex . original_complex;
	for ( typename Original_Chain::const_iterator chain_term = copy_me . begin ();
	chain_term != copy_me . end (); ++ chain_term )  {
		Original_const_iterator position = original_complex . find ( chain_term -> first );
		insert ( typename Morse_Value_Chain::value_type (
			Morse_Value_Elementary_Chain < Morse_Complex_Template > ( morse_complex . Morse_Value ( position ), position ),
			chain_term -> second ) ); } /* for */ } /* endfunction */

/* Morse Boundary Algorithm */
template < class Cell_Complex_Template >
void Morse_Complex<Cell_Complex_Template>::Morse_Boundary_Algorithm ( void ) {
	/* Convenient typedefs. */
	typedef typename Cell_Complex_Template::Container::const_iterator Original_const_iterator;
	typedef typename Cell_Complex_Template::Chain Original_Chain;
	typedef Abstract_Complex::Chain Morse_Chain;
	typedef typename Morse_Traits<Cell_Complex_Template>::morse_value_type Morse_Value_Type;

	typedef typename Morse_Value_Chain < Morse_Complex > :: iterator Work_Chain_iterator;

	/* We make a reference to this, so the semantics more clearly reflect when we are talking about the Morse Complex.
	 * This should not result in slowdowns, because the compiler knows the reference never changes and can bind member accesses at
	 * compile time. */
	Morse_Complex & morse_complex = *this;
	/* First we allocate the empty chain groups. */
	dimension = original_complex . dimension;
	Chain_Groups . resize ( dimension + 1 );

	/* Now we loop through the original complex and insert all of the Aces into the Morse Complex. */
	for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index )
		for ( typename Cell_Complex_Template::Container::const_iterator element = original_complex . Chain_Groups [ dimension_index ] . begin ();
		element != original_complex . Chain_Groups [ dimension_index ] . end (); ++ element )
			if ( Is_an_Ace ( Flags ( element ) ) )
				morse_complex . Chain_Groups [ dimension_index ] . insert ( std::pair < Default_Chain::Elementary_Chain, std::pair < Default_Chain, Default_Chain > >
					( element -> first, std::pair < Default_Chain, Default_Chain > () ) );

	int old_percent = 0;
	int number_computed = 0;
	/* Now we loop through the Aces and find their Morse Boundaries. */
	for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) {
		for ( typename Abstract_Complex::Container::iterator element = morse_complex . Chain_Groups [ dimension_index ] . begin ();
		element != morse_complex . Chain_Groups [ dimension_index ] . end (); ++ element ) {

			int percent = ( number_computed * 100 ) / morse_complex . size ();
			if ( percent > old_percent ) {
				std::cout << "\r" << number_computed << " " << morse_complex . size () << " (" << percent << ") ";
				std::flush(std::cout);
				old_percent = percent; } /* if */
			number_computed ++;

			/* We determine the Morse Boundary of "element". */
			//std::cout << "Finding the Morse Boundary of " << element -> first << "\n";
			/* Initialize Answer Chain. */
			Morse_Chain  answer_chain;
			/* Find original boundary */
			Original_Chain original_boundary;
			original_complex . Boundary_Map ( original_boundary, element -> first );
			/* Initialize Morse Value Chain.
			 * This is where we place the original boundaries into the work chain to
			 * seed the process. We make sure to only put in Living Queens; Kings and Aces
			 * we will always deal with immediately. */
			Morse_Value_Chain < Morse_Complex > work_chain;
			/* Notice the annoying part in what follows where we have to search the complex because
			 * the chain doesn't store locations in the complex directly. OPTIMIZE THIS. */
			for ( typename Original_Chain::iterator chain_term_iterator = original_boundary . begin();
			chain_term_iterator != original_boundary . end (); ++ chain_term_iterator ) {
				typename Cell_Complex_Template::Container::const_iterator location =
					original_complex . find ( chain_term_iterator -> first );
				unsigned char flags = Flags ( location );
				if ( Is_an_Ace ( flags ) ) {
					answer_chain . insert ( *chain_term_iterator ); // or += would work
					//std::cout << "Removing an Ace! " << chain_term_iterator -> first << "\n";
					//std::cout << "   Answer Chain is now " << answer_chain << "\n";
					continue; }
				if ( Is_a_King ( flags ) || not Is_Alive ( flags ) ) continue;
				work_chain . insert ( typename Morse_Value_Chain < Morse_Complex >::value_type
					( Morse_Value_Elementary_Chain < Morse_Complex > ( Morse_Value ( location ), location ),
					chain_term_iterator -> second ) ); 
			} /* for */


			/* We proceed as follows. We use a morse value chain to store the current 'work terms'
			 * The point of this data structure is that it stores the highest morse valued items at the beginning.
			 * We use 'begin' to grab a term and then process it.
			 * Processing a term consists of removing it from the
			 * work structure, and then one of the four options:
			 *   If it is an Ace, put it in the answer chain.
			 *   If it is a King, do nothing and continue on.
			 *   If it is a Queen, check to see if she is alive.
			 *      If she is not alive, do nothing and continue on.
			 *      If she is alive, insert into the work list all of her spouses boundaries other than her (her King's mistresses), with coefficients
			 *        obtained via multiplying the coefficient of the Q term by <Q, dK> <dK, M>. */
			 //std::cout << "Initial boundary chain processed. We proceed with " << work_chain << "\n";
			 while ( not work_chain . empty () ) {
				/* Pull off a term from "work_chain". */
				Work_Chain_iterator current_work_term = work_chain . begin ();

				/* By construction, this term is a Living Queen. */
				Original_const_iterator queen = current_work_term -> first . location;
				Original_Chain king_boundaries;
				original_complex . Boundary_Map ( king_boundaries, Husband ( queen ) );

				/* Calculate the factor that we need to multiply the chain by. */
				typename Original_Chain::iterator queen_term = king_boundaries . find ( queen -> first );
				Ring factor = - current_work_term -> second / queen_term -> second; /* As this must be invertible. */

				/* Multiply the chain terms by the correct factor. */
				king_boundaries *= factor;

				/* Loop through new terms and deal with Aces, Kings, and Dead Queens */
				Original_Chain kept_boundaries;
				for ( typename Original_Chain::const_iterator chain_term_iterator = king_boundaries . begin ();
				chain_term_iterator != king_boundaries . end (); ++ chain_term_iterator ) {
					Original_const_iterator location = original_complex . find ( chain_term_iterator -> first );
					unsigned char flags = Flags ( location );
					if ( Is_an_Ace ( flags ) ) {
						answer_chain += *chain_term_iterator;
						continue; } /* if */
					if ( Is_a_Queen ( flags ) && Is_Alive ( flags ) )
						kept_boundaries += *chain_term_iterator; 
				} /* for */
				/* The kept_boundaries are the living Queens of the king_boundaries. */

				/* Add the remaining king boundary terms into the work_chain. The original queen will be cancelled. */
				work_chain += Morse_Value_Chain < Morse_Complex > ( kept_boundaries, morse_complex );

				//std::cout << "Current Work Chain = " << work_chain << "\n";
			} /* while */
			/* Now we have the answer chain; let's look at it! */
			//std::cout << "Answer = " << answer_chain << "\n";
			morse_complex . Boundary_Map ( element ) = answer_chain; 
		} /* for */ 
	} /* for */
} /* Morse_Complex<>::Morse_Boundary_Algorithm */

template < class Cell_Complex >
typename Cell_Complex::Chain & Morse_Complex<Cell_Complex>::Chain_Correspondence_Algorithm ( typename Cell_Complex::Chain & canonical_chain, const Morse_Complex::Chain & morse_chain ) {
	typedef typename Morse_Value_Chain < Morse_Complex > :: iterator Work_Chain_iterator;
	/* First we include the chain into the original complex */
	canonical_chain = morse_chain; /* this'll break someday */
	typename Cell_Complex::Chain boundary_chain; 
	original_complex . Boundary_Map ( boundary_chain, canonical_chain );
	/* We use the special 'Morse_Value_Chain' for the upcoming algorithm */
	Morse_Value_Chain < Morse_Complex > work_chain ( boundary_chain, *this );
	/* Now we loop until the boundary of "canonical_chain" is canonical. 
	 * This is acheived by throwing out non-queen terms of "work_chain", and
	 * eliminating queen terms by adding kings to "canonical_chain". */
	while ( not work_chain . empty () ) {
		Work_Chain_iterator term = work_chain . begin ();
		typename Cell_Complex::const_iterator cell_iterator = term -> first . location;
		if ( not Is_a_Queen ( Flags ( cell_iterator ) ) ) {
			work_chain . erase ( term );
			continue;
		} /* if */
		typename Cell_Complex::const_iterator king_iterator = Husband ( cell_iterator );
		typename Cell_Complex::Chain king_boundaries;
		original_complex . Boundary_Map ( king_boundaries, king_iterator);

		/* Calculate the factor that we need to multiply the king by. */
		typename Cell_Complex::Chain::iterator queen_term = king_boundaries . find ( cell_iterator -> first );
		Ring factor = - term -> second / queen_term -> second;
		/* Hence canonical_chain + factor * king does not possess this queen term. */
		canonical_chain [ king_iterator -> first ] = factor;
		/* Multiply the chain terms by the correct factor. */
		king_boundaries *= factor;
		/* Add the remaining king boundary terms into the work_chain. The original queen will be cancelled. */
		work_chain += Morse_Value_Chain < Morse_Complex > ( king_boundaries, *this );
	} /* while */
	return canonical_chain;
} /* Morse_Complex<>::Chain_Correspondence_Algorithm */
