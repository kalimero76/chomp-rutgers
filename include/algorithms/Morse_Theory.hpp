/*
 *  Morse_Theory.hpp
 *  
 *
 *  Created by Shaun Harker on 4/28/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include <algorithm>  /* for_each */
#include <functional> /* greater<> */

#include "algorithms/basic.h"
#include "algorithms/Morse_Theory.h"
#include "complexes/Subcomplex.h"

Morse_Chain & operator += ( Morse_Chain & output, const Morse_Chain::value_type & value ) {
  std::pair < Morse_Chain::iterator, bool > insert_return = output . insert ( value );
  if ( insert_return . second == false ) 
    if ( ( insert_return . first -> second += value . second ) == 0 ) 
      output . erase ( insert_return . first );
  return output;
} /* operator += */

std::ostream & operator << ( std::ostream & output, const Morse_Chain & print_me ) {
  for (  Morse_Chain::const_iterator term_iterator = print_me . begin (); term_iterator != print_me . end (); ++ term_iterator ) {
    output << term_iterator -> second << " *[" << term_iterator -> first << "] + ";
  } /* for */
  return output;
} /* operator << */

namespace morse_detail {
  class Decompose_Functor { /* used by morse::decompose */
  public:
    Decompose_Functor ( unsigned long * king_queue, 
                        unsigned long & king_end,
                        std::vector < int > & number_of_boundaries ) 
                      : king_queue ( king_queue ), 
                        king_end ( king_end ),
                        number_of_boundaries ( number_of_boundaries ) {}
    void operator () ( const unsigned long & coboundary_index ) {
      if ( -- number_of_boundaries [ coboundary_index ] == 1 )
        king_queue [ king_end ++ ] =  coboundary_index;
    } /* operator () */
    
  private:
    unsigned long * king_queue;
    unsigned long & king_end;
    std::vector < int > & number_of_boundaries;
  };
} /* namespace morse_detail */

namespace morse {
  
  template < class Cell_Complex > std::vector < unsigned long >
  decompose ( Cell_Complex & cell_complex ) {
    using namespace morse_detail;
    unsigned long complex_size = cell_complex . size ();
    /* data structures */
    //std::deque< unsigned long > king_queue;
    unsigned long * king_queue  = new unsigned long [ complex_size ];
    std::vector < unsigned long > data ( complex_size, 0 );
    std::vector < unsigned long > husband ( complex_size );
    std::vector < typename Cell_Complex::Ring > connection ( complex_size, 0 );

    unsigned long king_begin = 0;
    unsigned long king_end = 0;
    typedef std::vector < std::pair < unsigned long, typename Cell_Complex::Ring > > Index_Chain;
    typedef std::vector < unsigned long > Index_List;
    Index_Chain boundary;
    Index_List coboundary;
    /* Initialize a vector for storing the number of boundaries of cells */
    std::vector < int > number_of_boundaries = cell_complex . count_all_boundaries ();
    /* Initialize a functor for processing coboundaries of excised cells */
    Decompose_Functor process_coboundary ( king_queue, king_end, number_of_boundaries );
    /* Perform initial sweep, recording number of boundaries and queueing free-coface collapses */    
    //for ( unsigned long cell_index = 0; cell_index < complex_size; ++ cell_index ) {
    //  if ( number_of_boundaries [ cell_index ] == 1 ) king_queue . push_back ( cell_index );
    //} /* for */
    /* Loop while complex is non-empty. */
    unsigned long ace_index = 0;
    unsigned long cells_remaining = complex_size;
    std::vector < unsigned long > king_count ( cell_complex . dimension () + 2, 0 );

    /* * * * * * * 
     * MAIN LOOP *
     * * * * * * */
    
    while ( cells_remaining ) {
      
      /* * * * * * * * * * * * 
       * KING-QUEEN ROUTINE  *
       * * * * * * * * * * * */
      
      while ( cells_remaining && king_begin < king_end ) {     
        /* Pick a King. */
        while ( king_begin < king_end ) {
          if ( number_of_boundaries [ king_queue [ king_begin ] ] == 1 ) break; 
          ++ king_begin;
        } /* while */
        if ( king_begin == king_end ) break;
        const unsigned long & king_index = king_queue [ king_begin ];
        unsigned long queen_index;
        typename Cell_Complex::Ring coefficient;
        /* Get the queen_index */ 
        cell_complex . boundary ( boundary, king_index );
        for ( typename Index_Chain::const_iterator index_iterator = boundary . begin ();
              index_iterator != boundary . end (); ++ index_iterator ) {
          if ( number_of_boundaries [ index_iterator -> first ] == 0 ) {
            queen_index = index_iterator -> first;
            coefficient = index_iterator -> second;
            /* TODO: worry about non-Z Rings */
            break;
          }
        } /* for */
        if ( coefficient != 1 && coefficient != -1 ) continue;
        /* Assign Decomposition Information */
        connection [ queen_index ] = coefficient;
        unsigned int dimension = cell_complex . lookup ( king_index ) . dimension (); /*TODO optimize */
        data [ queen_index ] = 1 + ++ king_count [ dimension ];   
        husband [ queen_index ] = king_index;
        /* Remove the King and Queen from copy_complex. */
        number_of_boundaries [ king_index ] = -1;
        number_of_boundaries [ queen_index ] = -1;
        cells_remaining -= 2;
        /* Coboundary processing of King and Queen. */
        cell_complex . coboundary ( coboundary, king_index );
        for_each ( coboundary . begin (), coboundary . end (), process_coboundary );
        cell_complex . coboundary ( coboundary, queen_index );
        for_each ( coboundary . begin (), coboundary . end (), process_coboundary );
      } /* while */
      
      /* * * * * * * * 
       * ACE ROUTINE *
       * * * * * * * */
      
      while ( cells_remaining && king_begin == king_end ) {
        /* Pick an Ace. */
        while ( number_of_boundaries [ ace_index ] != 0 ) ++ ace_index;
        /* Assign Decomposition Information */
        data [ ace_index ] = 1;     
        /* Remove the Ace from copy_complex. */
        number_of_boundaries [ ace_index ] = -1;
        -- cells_remaining;
        /* Coboundary processing of Ace. */
        cell_complex . coboundary ( coboundary, ace_index );
        for_each ( coboundary . begin (), coboundary . end (), process_coboundary );
      } /* while */
    } /* while */
    delete [] king_queue;
    
    /* * * * * * * * * * * * 
     * MORSE INDEX ROUTINE *
     * * * * * * * * * * * */
    std::vector < typename Cell_Complex::const_iterator > new_lookup ( complex_size + 1 );
    for ( unsigned int dimension_index = 0; dimension_index <= cell_complex . dimension (); ++ dimension_index ) {
      const unsigned long start_index = cell_complex . index_begin ( dimension_index );
      const unsigned long stop_index = cell_complex . index_end ( dimension_index );
      unsigned long ace_index = stop_index - king_count [ dimension_index ] - 1;
      const unsigned long king_offset = cell_complex . index_end ( dimension_index + 1 ) + 1;
      const unsigned long queen_offset = start_index - 2;
      for ( unsigned long cell_index = start_index; cell_index < stop_index; ++ cell_index ) {
        switch ( data [ cell_index ] ) {
          case 0: /* King */ break;
          case 1: /* Ace */
            cell_complex . index ( new_lookup [ ace_index ] = cell_complex . lookup ( cell_index ) ) = ace_index;
            -- ace_index;
            break;
          default: /* Queen */
            unsigned long queen_index = queen_offset + data [ cell_index ];
            unsigned long king_index = king_offset - data [ cell_index ];
            cell_complex . index ( new_lookup [ queen_index ] = cell_complex . lookup ( cell_index ) ) = queen_index;
            cell_complex . index ( new_lookup [ king_index ] = cell_complex . lookup ( husband [ cell_index ] ) ) = king_index;
            cell_complex . connection ( queen_index ) = connection [ cell_index ];
            break;
        } /* switch */
      } /* for */
    } /* for */
    cell_complex . lookup () . swap ( new_lookup );
    return king_count;
  } /* morse::decompose */
  
#if 0
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
      unsigned long cell_index = container . index ( cell_iterator );
      if ( not Is_a_Queen ( container . flags ( cell_index ) ) ) {
        work_chain . erase ( term );
        continue;
      } /* if */
      /* The term is a queen. We add its husband to the output with the correct coefficient */
      const typename Cell_Complex::const_iterator king_iterator = 
        container . lookup ( container . husband ( cell_index ) );
      typename Cell_Complex::Chain king_boundaries = container . boundary ( king_iterator );
      /* Calculate the factor that we need to multiply the king by. */
      const typename Cell_Complex::Chain::iterator queen_term = king_boundaries . find ( cell_iterator );
      const typename Cell_Complex::Ring factor = - term -> second / queen_term -> second;
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
    const Cell_Complex & container = input . container ();
    typename Cell_Complex::Chain return_value ( container );
    /* Loop through the terms of "input" and insert the ace terms into "return_value" */
    for ( typename Cell_Complex::Chain::const_iterator term_iterator = input . begin (); 
         term_iterator != input . end (); ++ term_iterator ) {
      if ( Is_an_Ace ( container . flags ( container . index ( term_iterator -> first ) ) ) )
        return_value . insert ( * term_iterator );
    } /* for */
    return return_value;
  } /* morse::project */
 
#endif
  
  template < class Cell_Complex > Morse_Chain
  morse_boundary ( unsigned long ace_index, const Cell_Complex & complex ) {
    Morse_Chain answer_chain;
    Morse_Chain work_chain;
    /* Get the dimension of the resulting chain */
    unsigned int dimension = complex . lookup ( ace_index ) . dimension ();
    if ( dimension == 0 ) return answer_chain;
    -- dimension;
    /* Initial processing of Ace boundaries */ 
    typedef std::vector < std::pair < unsigned long, typename Cell_Complex::Ring > > Index_Chain;
    Index_Chain boundaries;
    complex . boundary ( boundaries, ace_index );
    for ( typename Index_Chain::const_iterator term_iterator = boundaries . begin ();
         term_iterator != boundaries . end (); ++ term_iterator ) {
      typename Index_Chain::value_type value = * term_iterator;
      if ( value . first >= complex . ace_end ( dimension ) ) {
        /* It's a King */ continue;
      } /* if */
      if ( value . first < complex . ace_begin ( dimension ) ) {
        /* It's a Queen. */
        work_chain . insert ( value );
      } else {
        /* It's an Ace */
        answer_chain . insert ( value );
      } /* if */
    } /* for */
    /* Main loop */
    while ( not work_chain . empty () ) {
      const Morse_Chain::iterator queen_term = work_chain . begin ();
      const unsigned long & queen_index = queen_term -> first;
      const unsigned long king_index = complex . mate ( queen_index, dimension );
      complex . boundary ( boundaries, king_index );
      /* Determine the factor */
      const typename Cell_Complex::Ring factor = 
        - queen_term -> second / complex . connection ( queen_index );
      /* Process the boundary */
      for ( typename Index_Chain::const_iterator term_iterator = boundaries . begin ();
           term_iterator != boundaries . end (); ++ term_iterator ) {
        typename Index_Chain::value_type value = * term_iterator;
        if ( value . first >= complex . ace_end ( dimension ) ) {
          /* It's a King */ continue;
        } /* if */
        value . second *= factor;
        if ( value . first < complex . ace_begin ( dimension ) ) {
          /* It's a Queen. */
          work_chain += value;
        } else {
          /* It's an Ace */
          answer_chain += value;
        } /* if */
      } /* for */
    } /* while */
    return answer_chain;
  } /* morse::morse_boundary */
  
  template < class Cell_Complex > Morse_Complex 
  reduction ( const Cell_Complex & complex ) {
    /* Construct reduced complex */
    Morse_Complex reduced_complex ( complex . dimension () );
    /* Loop through the original complex and insert all of the Aces into "reduced". */
    for ( unsigned int dimension_index = 0; dimension_index <= complex . dimension (); ++ dimension_index ) {
      /* Loop through and insert all the dimension_index Aces */
      unsigned long start = complex . ace_begin ( dimension_index );
      unsigned long stop = complex . ace_end ( dimension_index );
      for ( unsigned long ace_index = start; ace_index < stop; ++ ace_index )
        reduced_complex . insert ( Default_Cell ( ace_index, dimension_index ) );
    } /* for */
    /* Introduce the boundary and coboundary information into the reduced complex. */
    for ( typename Morse_Complex::const_iterator cell_iterator = reduced_complex . begin ();
         cell_iterator != reduced_complex . end (); ++ cell_iterator ) {
      /* morse_boundary = project o alpha o boundary o include */ { 
        Morse_Chain upstairs_boundary = morse_boundary < Cell_Complex > ( cell_iterator -> data (), complex );
        Morse_Complex::Chain & downstairs_boundary = reduced_complex . boundary ( cell_iterator );
        for ( Morse_Chain::const_iterator term_iterator = upstairs_boundary . begin (); 
             term_iterator != upstairs_boundary . end (); ++ term_iterator ) {
          downstairs_boundary [ reduced_complex . 
                               find ( Default_Cell ( term_iterator -> first, 
                                                    cell_iterator -> dimension () - 1 ) ) ] 
          = term_iterator -> second;
        } /* for */
      } /* scope */
      /* morse_coboundary = project o alpha o coboundary o include */ { 
      /*  Morse_Chain upstairs_coboundary = morse_coboundary < Cell_Complex > ( cell_iterator -> data (), complex );
        Morse_Complex::Chain & downstairs_coboundary = reduced_complex . coboundary ( cell_iterator );
        for ( Morse_Chain::const_iterator term_iterator = upstairs_coboundary . begin (); 
             term_iterator != upstairs_coboundary . end (); ++ term_iterator ) {
          downstairs_coboundary [ reduced_complex . 
                                 find ( Default_Cell ( term_iterator -> first, 
                                                      cell_iterator -> dimension () + 1 ) ) ] 
          = term_iterator -> second;
        } /* for */       
      } /* scope */
    } /* for */
    reduced_complex . index ();
    return reduced_complex;
  } /* morse::reduction */
  
} /* namespace morse */