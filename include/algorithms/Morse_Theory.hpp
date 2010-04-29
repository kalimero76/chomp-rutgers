/*
 *  Morse_Theory.hpp
 *  
 *
 *  Created by Shaun Harker on 4/28/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "Morse_Theory.h"
#include "complexes/Subcomplex.h"
#define MORSE_DEBUG
/* Ace-King-Queen Algorithm */
template < class Cell_Complex >
Morse_Complex<Cell_Complex> Ace_King_Queen_Algorithm ( const Cell_Complex & cell_complex ) {
	/* We begin with any complex. */
  
	/* This is a specific implementation of the Ace-King-Queen algorithm
	 * based on greedy usage of King-Queen pairs which are dealt with on a
	 * first come, first serve basis (which results in a breadth first pattern.)
	 * When King-Queen excisions are exhausted, we excise Ace. We then return to
	 * finding King-Queen pairs again. This process is repeated until the entire
	 * complex has been excised.  */
  
	/* Copy the complex. */
#ifdef MORSE_DEBUG
	std::cout << "AKQ: Copying complex.\n";
#endif
  using namespace morse_detail;
  Morse_Complex<Cell_Complex> original_complex ( cell_complex );
	Subcomplex< Morse_Complex < Cell_Complex > > copy_complex ( original_complex );
  typedef long Ring;
  typedef typename Subcomplex< Morse_Complex < Cell_Complex > >::const_iterator sub_const_iterator;
  typedef typename Subcomplex< Morse_Complex < Cell_Complex > >::Chain sub_Chain;
  typedef typename Cell_Complex::const_iterator original_const_iterator;
  typedef typename Cell_Complex::Chain original_Chain;

	/* Initialize working_queue for Batko-Mrozek method of queuing coreduction pairs. */
	std::deque< sub_const_iterator > working_queue;
  
	/* Perform initial sweep for K-Q excisions */
#ifdef MORSE_DEBUG
	std::cout << "AKQ: Initial excision sweep.\n";
#endif
	for ( sub_const_iterator cell_iterator = copy_complex . begin (); cell_iterator != copy_complex . end (); ++ cell_iterator )
    if ( copy_complex . Count_Boundaries ( cell_iterator ) == 1 ) working_queue . push_back ( cell_iterator );

	/* Copy working queue into set. */
	std::set< sub_const_iterator > working_set ( working_queue . begin (), working_queue . end () );
#ifdef MORSE_DEBUG
	std::cout << "Entering main AKQ loop. Initial size = " << copy_complex . size () << "\n";
#endif
	/* Loop while complex is non-empty. */
	int old_percent = 0;
	while ( copy_complex . size () ) {
		/* Perform K-Q excisions until none remain. */
#ifdef MORSE_DEBUG
    std::cout << "Entered KQ phase\n";
#endif
		while ( not working_queue . empty () ) {
#ifdef MORSE_DEBUG
			std::cout << "=== TOP OF KQ Phase Loop ===\n";
			std::cout << "\n\n";
#endif
      /* Print Progress */
			int percent = 100 - ( copy_complex . size () * 100 ) / original_complex . size ();
			if ( percent > old_percent ) {
				std::cout << "\r" << copy_complex . size () << " " << original_complex . size () << " (" << percent << ") ";
				std::flush(std::cout);
        old_percent = percent; 
      } /* if */
      
			/* Pull a cell from the queue that is still in "copy_complex" */
			while ( not working_queue . empty () && working_set . find ( working_queue . front () ) == working_set . end () ) working_queue . pop_front ();
			if ( working_queue . empty () ) break;
			sub_const_iterator cell_iterator = working_queue . front ();
#ifdef MORSE_DEBUG
      std::cout << "Looking at cell " << * * cell_iterator << "\n"; /* TODO: implement << for iterators */
#endif
			/* Find the boundary of the cell. */
      sub_Chain boundary_chain = copy_complex . boundary ( cell_iterator );

			/* How many cells are in the boundary of "cell"? */
			const int number_of_boundaries = boundary_chain . size ();

			/* Depending on whether there are 0, 1, or more boundaries of cell in "copy_complex", there are three different behaviors: */
			switch ( number_of_boundaries ) {
        case 1: /* We have found an elementary coreduction pair. */
				{ /* scope */
          
          /* Read the one term off from the boundary. */
          std::pair < sub_const_iterator, Ring > queen_term =  * boundary_chain . begin ();
          
          /* TODO : make the following line compile. Otherwise, can only handle coefficients 0, 1, -1. */
          //if ( not chain_term . second . is_invertible () ) break;
          
          /* Locate the pair in the original complex. */
          original_const_iterator King = * cell_iterator;
          original_const_iterator Queen = * queen_term . first;
#ifdef MORSE_DEBUG
          std::cout << "----- AKQ: Removing the KQ pair " << *King << " " << *Queen << " -----\n";
#endif
          /* Loop through King's boundaries that are not Queen to gather information. */
          original_Chain original_boundary_chain = original_complex . boundary ( King );
          unsigned long int current_value = 0;
          bool alive = false;
          for ( typename original_Chain::const_iterator chain_term_iterator = original_boundary_chain . begin ();
               chain_term_iterator != original_boundary_chain . end (); ++ chain_term_iterator ) {
            if ( chain_term_iterator -> first == Queen ) continue;
            unsigned long int observed_value = original_complex . value ( chain_term_iterator -> first );
            if ( observed_value > current_value ) current_value = observed_value;
            if ( Is_Alive ( original_complex . flags ( chain_term_iterator -> first ) ) ) alive = true; 
          } /* for */
          
          /* Assign Morse Value */
          original_complex . value ( King ) = original_complex . value ( Queen ) = current_value + 1;
          /* Assign Husband */
          original_complex . husband ( Queen ) = King;
          /* Assign Flags */
          original_complex . flags ( King ) = KING;
          original_complex . flags ( Queen ) = alive ? QUEEN | ALIVE : QUEEN;
          
          /* Excise the King. */
          working_queue . pop_front ();
          working_set . erase ( cell_iterator );
          copy_complex . erase ( cell_iterator );
          
          /* Find the coboundary of the lower dimensional part. */
          sub_Chain coboundary_chain = copy_complex . coboundary ( queen_term . first );
#ifdef MORSE_DEBUG
          std::cout << "    (queen term) coboundary of " << * * queen_term . first ; // << " = " << coboundary_chain << "\n";
#endif
          /* Loop through coboundary terms and add them to working list. */
          for ( typename sub_Chain::const_iterator coboundary_term = coboundary_chain . begin ();
               coboundary_term != coboundary_chain . end (); coboundary_term ++ ) {
            /* Put the elementary chain associated with the coboundary term in the working queue to be processed. */
            const sub_const_iterator & coboundary_iterator = coboundary_term -> first;
            if ( working_set . insert ( coboundary_iterator ) . second == true ) {
              /* We really inserted the element, it wasn't already there */
              working_queue . push_back ( coboundary_iterator );
            } /* if */
          } /* for */
          
          /* Now excise the Queen. */
          working_set . erase ( queen_term . first );								
          copy_complex . erase ( queen_term . first );
          
          break;
				} /* scope */
          
        case 0:
				{ /* scope */
#ifdef MORSE_DEBUG
          std::cout << "----- AKQ: No boundaries for " << * * cell_iterator << " -----\n";
#endif
          /* Find the coboundary. */
          sub_Chain coboundary_chain = copy_complex . coboundary ( cell_iterator );
          /* Loop through coboundary terms and add them to list. */
          for ( typename sub_Chain::const_iterator coboundary_term = coboundary_chain . begin ();
               coboundary_term != coboundary_chain . end (); coboundary_term ++ ) {
            /* Put the elementary chain associated with the coboundary term in the working queue to be processed. */
            const sub_const_iterator & coboundary_iterator = coboundary_term -> first;
            if ( working_set . insert ( coboundary_iterator ) . second == true ) {
              /* We really inserted the element, it wasn't already there */
              working_queue . push_back ( coboundary_iterator );
            } /* if */
          } /* for */
          
          /* We are done with queue element. We can remove it. */
          working_queue . pop_front ();
          working_set . erase ( cell_iterator );
          
          break;
				} /* scope */
        default:
#ifdef MORSE_DEBUG
          std::cout << "----- AKQ: More than two boundaries for " << * * cell_iterator << " -----\n";
#endif
          /* We are done with queue element. We can remove it. */
          working_queue . pop_front ();
          working_set . erase ( cell_iterator );
			} /* switch */
		} /* while */
#ifdef MORSE_DEBUG
		std::cout << "KQ Phase exited. Copy complex size = " << copy_complex . size () << " \n";
#endif
		/* Now the K-Q pairs are excised. If complex is now empty, break loop. */
		if ( copy_complex . size () == 0 ) break;
    
		/* Excise an Ace. */
		/* Get a low dimension cell for excision. */
		sub_const_iterator cell_iterator = copy_complex . begin ();
		
		/* Identify the Ace cell in the original complex. */
		original_const_iterator Ace = * cell_iterator;
#ifdef MORSE_DEBUG
		std::cout << "----- AKQ: Excising the Ace " << *Ace << " -----\n";
#endif
		/* Figure out what the Morse value should be by boundary checking. */
		original_Chain original_boundary_chain = original_complex . boundary ( Ace );
		unsigned long int current_value = 0;
		for ( typename original_Chain::const_iterator chain_term_iterator = original_boundary_chain . begin ();
          chain_term_iterator != original_boundary_chain . end (); ++ chain_term_iterator ) {
			unsigned long observed_value = original_complex . value ( chain_term_iterator -> first );
      if ( observed_value > current_value ) current_value = observed_value; 
    } /* for */
		original_complex . value ( Ace ) = current_value + 1;
		original_complex . flags ( Ace ) = ACE | ALIVE;
    
		/* Put all the coboundary elements of the Ace in the working list */
		sub_Chain coboundary_chain = copy_complex . coboundary ( cell_iterator );
		for ( typename sub_Chain::const_iterator coboundary_term = coboundary_chain . begin ();
         coboundary_term != coboundary_chain . end (); coboundary_term ++ ) {
			/* Put the elementary chain associated with the coboundary term in the working queue to be processed. */
			const sub_const_iterator & coboundary_iterator = coboundary_term -> first;
			if ( working_set . insert ( coboundary_iterator ) . second == true ) {
				/* We really inserted the element, it wasn't already there */
				working_queue . push_back ( coboundary_iterator );
			} /* if */
		} /* for */
    
		/* Now excise the Ace. */
		copy_complex . erase ( cell_iterator );
    
	} /* while */
	/* We are done. */
#ifdef MORSE_DEBUG
	std::cout << "AKQ Decomposition Complete\n";
#endif
  return original_complex;
} /* endfunction */
