/*
 *  basic.h
 *  
 *
 *  Created by Shaun Harker on 4/29/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef CHOMP_BASIC_H
#define CHOMP_BASIC_H

/* still toying with this 
template < class Chain >
Chain boundary ( const Chain & input );

template < class Chain >
Chain coboundary ( const Chain & input );

template < class Chain >
Chain operator + ( const Chain & left_hand_side, const Chain & right_hand_side );

template < class Chain >
Chain operator - ( const Chain & left_hand_side, const Chain & right_hand_side );

template < class Chain >
Chain & operator += ( Chain & left_hand_side, const Chain & right_hand_side );

template < class Chain >
Chain & operator -= ( Chain & left_hand_side, const Chain & right_hand_side );

template < class Chain >
Chain & operator += ( Chain & left_hand_side, const typename Chain::value_type & right_hand_side );

template < class Chain >
Chain & operator -= ( Chain & left_hand_side, const typename Chain::value_type & right_hand_side );

template < class Chain >
Chain & operator *= ( Chain & left_hand_side, const typename Chain::mapped_type & right_hand_side );
*/

#ifndef CHOMP_LIBRARY_ONLY_
#include "algorithms/basic.hpp"
#endif

//#ifdef CHOMP_HEADER_ONLY_
//#include "algorithms/basic.cpp"
//#endif

#endif