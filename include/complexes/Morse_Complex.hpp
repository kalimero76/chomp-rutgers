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
  
  enum { ACE = 0x01, KING = 0x02, QUEEN = 0x04, ALIVE = 0x08, EXCISED = 0x10 };
  
  bool Is_an_Ace ( unsigned char input) const {
    return input & ACE ? true : false; 
  } /* Is_an_Ace */
  
  bool Is_a_King ( unsigned char input ) const {
    return input & KING ? true : false; 
  }/* Is_a_King */
  
  bool Is_a_Queen ( unsigned char input ) const {
    return input & QUEEN ? true : false; 
  }/* Is_a_Queen */
  
  bool Is_Alive ( unsigned char input ) const {
    return input & ALIVE ? true : false; 
  }/* Is_Alive */
  
  bool Is_Excised ( unsigned char input ) const {
    return input & EXCISED ? true : false; 
  } /* Is_Excised */
  
  
  /* * * * * * * * * * * *
   * class Morse_Compare *
   * * * * * * * * * * * */
  template < class Cell_Complex >
  class Morse_Compare {
  public:
    bool operator () ( const typename Cell_Complex::const_iterator & left, const typename Cell_Complex::const_iterator & right ) {
      
    }
  };
  
  /* * * * * * * * * * *
   * class Morse_Chain *
   * * * * * * * * * * */
  template < class Cell_Complex >
  class Morse_Chain : public Chain_Archetype < std::map < typename Cell_Complex::const_iterator, Default_Ring, Morse_Compare < Cell_Complex > > > {
  };
  
  
} /* namespace morse_detail */


/* Public Member Functions */

template < class Cell_Complex > typename Morse_Complex<Cell_Complex>::const_iterator & 
Morse_Complex<Cell_Complex>:Morse_Complex ( const Cell_Complex & cell_complex ) : cell_complex_(cell_complex) {
} /* Morse_Complex<>::Morse_Complex */

template < class Cell_Complex > typename Morse_Complex<Cell_Complex>::const_iterator & 
Morse_Complex<Cell_Complex>::husband ( const const_iterator & ) {
} /* Morse_Complex<>::husband */

template < class Cell_Complex > unsigned int & 
Morse_Complex<Cell_Complex>::value ( const const_iterator & ) {
} /* Morse_Complex<>::value */

template < class Cell_Complex > unsigned char & 
Morse_Complex<Cell_Complex>::flags ( const const_iterator & ) {
} /* Morse_Complex<>::flags */

template < class Cell_Complex > typename Morse_Complex<Cell_Complex>::Chain 
Morse_Complex<Cell_Complex>::canonicalize ( const Chain & input ) const {
} /* Morse_Complex<>::canonicalize */

template < class Cell_Complex > typename Morse_Complex<Cell_Complex>::Chain 
Morse_Complex<Cell_Complex>::complete ( const Chain & input ) const {
} /* Morse_Complex<>::complete */

template < class Cell_Complex > typename Morse_Complex<Cell_Complex>::Chain 
Morse_Complex<Cell_Complex>::project ( const Chain & input ) const {
} /* Morse_Complex<>::project */

