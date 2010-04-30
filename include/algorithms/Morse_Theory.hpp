/*
 *  Morse_Theory.hpp
 *  
 *
 *  Created by Shaun Harker on 4/28/10.
 *  Copyright 2010. All rights reserved.
 *
 */
#include <deque>
#include <set>

#include "algorithms/basic.h"
#include "algorithms/Morse_Theory.h"
#include "complexes/Subcomplex.h"

#define MORSE_DEBUG

/* morse_detail namespace. */

namespace morse_detail {
  
  enum { ACE = 0x01, KING = 0x02, QUEEN = 0x04, ALIVE = 0x08, EXCISED = 0x10 };
  
  bool Is_an_Ace ( unsigned char input) {
    return input & ACE ? true : false; 
  } /* Is_an_Ace */
  
  bool Is_a_King ( unsigned char input ) {
    return input & KING ? true : false; 
  } /* Is_a_King */
  
  bool Is_a_Queen ( unsigned char input ) {
    return input & QUEEN ? true : false; 
  } /* Is_a_Queen */
  
  bool Is_Alive ( unsigned char input ) {
    return input & ALIVE ? true : false; 
  } /* Is_Alive */
  
  bool Is_Excised ( unsigned char input ) {
    return input & EXCISED ? true : false; 
  } /* Is_Excised */
  
  
  /* * * * * * * * * * * *
   * class Morse_Compare *
   * * * * * * * * * * * */
  
  template < class Cell_Complex >
  class Morse_Compare {
  public:
    Morse_Compare ( const Cell_Complex & complex ) : complex_(complex) {}
    bool operator () ( const typename Cell_Complex::const_iterator & left, const typename Cell_Complex::const_iterator & right ) {
      unsigned int left_value = complex_ . value ( left );
      unsigned int right_value = complex_ . value ( right );
      if ( left_value == right_value ) 
        return left < right;
      else return left_value > right_value;
    }
  private:
    const Cell_Complex & complex_;
  };
  
  /* * * * * * * * * * *
   * class Morse_Chain *
   * * * * * * * * * * */
  
  template < class Cell_Complex >
  class Morse_Chain : public Chain_Archetype < std::map < typename Cell_Complex::const_iterator, Default_Ring, Morse_Compare < Cell_Complex > > > {
  public:
    Morse_Chain ( const typename Cell_Complex::Chain & copy_me ) 
    : Chain_Archetype < std::map < typename Cell_Complex::const_iterator, Default_Ring, Morse_Compare < Cell_Complex > > > 
    ( copy_me . begin (), copy_me . end (), Morse_Compare < Cell_Complex > ( copy_me . container () ), copy_me . container () ) {
    }
  };
  
} /* namespace morse_detail */

namespace morse {
  
  template < class Cell_Complex > void 
  decompose ( Cell_Complex & cell_complex ) {
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
    cell_complex . initialize_for_decomposition ();
    Subcomplex< Cell_Complex > copy_complex ( cell_complex );
    typedef typename Cell_Complex::Ring Ring;
    typedef typename Subcomplex < Cell_Complex > ::const_iterator sub_const_iterator;
    typedef typename Subcomplex < Cell_Complex > ::Chain sub_Chain;
    typedef typename Cell_Complex::const_iterator original_const_iterator;
    typedef typename Cell_Complex::Chain original_Chain;
    
    /* Initialize working_queue for Batko-Mrozek method of queuing coreduction pairs. */
    std::deque< sub_const_iterator > working_queue;
    
    /* Perform initial sweep for K-Q excisions */
#ifdef MORSE_DEBUG
    std::cout << "AKQ: Initial excision sweep.\n";
#endif
    for ( sub_const_iterator cell_iterator = copy_complex . begin (); cell_iterator != copy_complex . end (); ++ cell_iterator )
      if ( copy_complex . boundary ( cell_iterator ) . size () == 1 ) working_queue . push_back ( cell_iterator );
    
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
        int percent = 100 - ( copy_complex . size () * 100 ) / cell_complex . size ();
        if ( percent > old_percent ) {
          std::cout << "\r" << copy_complex . size () << " " << cell_complex . size () << " (" << percent << ") ";
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
            original_Chain original_boundary_chain = cell_complex . boundary ( King );
            unsigned long int current_value = 0;
            bool alive = false;
            for ( typename original_Chain::const_iterator chain_term_iterator = original_boundary_chain . begin ();
                 chain_term_iterator != original_boundary_chain . end (); ++ chain_term_iterator ) {
              if ( chain_term_iterator -> first == Queen ) continue;
              unsigned long int observed_value = cell_complex . value ( chain_term_iterator -> first );
              if ( observed_value > current_value ) current_value = observed_value;
              if ( Is_Alive ( cell_complex . flags ( chain_term_iterator -> first ) ) ) alive = true; 
            } /* for */
            
            /* Assign Morse Value */
            cell_complex . value ( King ) = cell_complex . value ( Queen ) = current_value + 1;
            /* Assign Husband */
            cell_complex . husband ( Queen ) = King;
            /* Assign Flags */
            cell_complex . flags ( King ) = KING;
            cell_complex . flags ( Queen ) = alive ? QUEEN | ALIVE : QUEEN;
            
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
      original_Chain original_boundary_chain = cell_complex . boundary ( Ace );
      unsigned long int current_value = 0;
      for ( typename original_Chain::const_iterator chain_term_iterator = original_boundary_chain . begin ();
           chain_term_iterator != original_boundary_chain . end (); ++ chain_term_iterator ) {
        unsigned long observed_value = cell_complex . value ( chain_term_iterator -> first );
        if ( observed_value > current_value ) current_value = observed_value; 
      } /* for */
      cell_complex . value ( Ace ) = current_value + 1;
      cell_complex . flags ( Ace ) = ACE | ALIVE;
      
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
    return;
  } /* morse::decompose */
  
  template < class Cell_Complex > typename Cell_Complex::Chain 
  alpha ( const typename Cell_Complex::Chain & input ) {
    /* alpha = id + boundary o gamma */
    typename Cell_Complex::Chain return_value = boundary ( gamma < Cell_Complex > ( input ) );
    return_value += input;
    return return_value;
  } /* morse::alpha */
  
  template < class Cell_Complex > typename Cell_Complex::Chain 
  beta ( const typename Cell_Complex::Chain & input ) {
    /* beta = id + gamma o boundary */
    typename Cell_Complex::Chain return_value = gamma < Cell_Complex > ( boundary ( input ) );
    return_value += input;
    return return_value;
  } /* morse::beta */
  
  template < class Cell_Complex > typename Cell_Complex::Chain 
  gamma ( const typename Cell_Complex::Chain & input ) {
    using namespace morse_detail;
    const Cell_Complex & container = input . container ();
    /* Find the king-chain k such that input + bd(k) is canonical */
    typename Cell_Complex::Chain king_chain ( container ); 
    Morse_Chain < Cell_Complex > work_chain ( input );
    while ( not work_chain . empty () ) {
      typename Morse_Chain < Cell_Complex >::iterator term = work_chain . begin ();
      /* Unless the term is a queen, there is nothing to be done. */
      typename Cell_Complex::const_iterator cell_iterator = term -> first;
      if ( not Is_a_Queen ( container . flags ( cell_iterator ) ) ) {
        work_chain . erase ( term );
        continue;
      } /* if */
      /* The term is a queen. We add its husband to the output with the correct coefficient */
      typename Cell_Complex::const_iterator king_iterator = container . husband ( cell_iterator );
      typename Cell_Complex::Chain king_boundaries = container . boundary ( king_iterator );
      /* Calculate the factor that we need to multiply the king by. */
      typename Cell_Complex::Chain::iterator queen_term = king_boundaries . find ( cell_iterator );
      typename Cell_Complex::Ring factor = - term -> second / queen_term -> second;
      /* Hence canonical_chain + factor * king does not possess this queen term. */
      king_chain [ king_iterator ] = factor;
      /* Multiply the chain terms by the correct factor. */
      king_boundaries *= factor;
      /* Add the remaining king boundary terms into the work_chain. The original queen will be cancelled. */
      work_chain += Morse_Chain < Cell_Complex > ( king_boundaries );
    } /* while */
    return king_chain;
  } /* morse::gamma */
  
  template < class Cell_Complex > typename Cell_Complex::Chain 
  project ( const typename Cell_Complex::Chain & input ) {
    using namespace morse_detail;
    typename Cell_Complex::Chain return_value ( input . container () );
    /* Loop through the terms of "input" and insert the ace terms into "return_value" */
    for ( typename Cell_Complex::Chain::const_iterator term_iterator = input . begin (); 
         term_iterator != input . end (); ++ term_iterator ) {
      if ( Is_an_Ace ( input . container () . flags ( term_iterator -> first ) ) )
        return_value . insert ( * term_iterator );
    }
    return return_value;
  } /* morse::project */
 
  template < class Cell_Complex >
  Abstract_Complex< typename Cell_Complex::const_iterator> 
  reduction ( const Cell_Complex & complex ) {
    using namespace morse_detail;
    /* TODO: project o alpha has a more efficient implementation using ALIVE flag */
    /* Construct reduced complex */
    typedef Abstract_Complex< typename Cell_Complex::const_iterator> Reduced_Complex;
    Reduced_Complex reduced_complex ( complex . dimension () );
    /* Loop through the original complex and insert all of the Aces into "reduced". */
    for ( typename Cell_Complex::const_iterator cell_iterator = complex . begin ();
          cell_iterator != complex . end (); ++ cell_iterator )
      if ( Is_an_Ace ( complex . flags ( cell_iterator ) ) ) reduced_complex . insert ( cell_iterator ) ;
    /* Introduce the boundary and coboundary information into the reduced complex. */
    for ( typename Reduced_Complex::const_iterator cell_iterator = reduced_complex . begin ();
          cell_iterator != reduced_complex . end (); ++ cell_iterator ) {
      /* morse_boundary = project o alpha o boundary o include */ { 
        typename Cell_Complex::Chain upstairs_boundary = project < Cell_Complex > ( alpha < Cell_Complex > ( complex . boundary ( * cell_iterator ) ) );
        typename Reduced_Complex::Chain & downstairs_boundary = reduced_complex . boundary ( cell_iterator );
        for ( typename Cell_Complex::Chain::const_iterator term_iterator = upstairs_boundary . begin (); 
             term_iterator != upstairs_boundary . end (); ++ term_iterator ) {
          downstairs_boundary [ reduced_complex . find ( term_iterator -> first ) ] = term_iterator -> second;
        } /* for */
      } /* scope */
      /* morse_coboundary = project o alpha o coboundary o include */ { 
        typename Cell_Complex::Chain upstairs_coboundary = project < Cell_Complex > ( alpha < Cell_Complex > ( complex . coboundary ( * cell_iterator ) ) );
        typename Reduced_Complex::Chain & downstairs_coboundary = reduced_complex . coboundary ( cell_iterator );
        for ( typename Cell_Complex::Chain::const_iterator term_iterator = upstairs_coboundary . begin (); 
             term_iterator != upstairs_coboundary . end (); ++ term_iterator ) {
          downstairs_coboundary [ reduced_complex . find ( term_iterator -> first ) ] = term_iterator -> second;
        } /* for */
      } /* scope */
    } /* for */
    return reduced_complex;
  } /* morse::reduction */
  
} /* namespace morse */