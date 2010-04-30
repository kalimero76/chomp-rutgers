/*
 *  basic.hpp
 *  
 *
 *  Created by Shaun Harker on 4/29/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "basic.h"

template < class Chain >
Chain boundary ( const Chain & input ) {
  Chain return_value ( input . container () );
  for ( typename Chain::const_iterator term_iterator = input . begin (); term_iterator != input . end (); ++ term_iterator ) {
    Chain summand = input . container () . boundary ( term_iterator -> first );
    summand *= term_iterator -> second;
    return_value += summand;
  }
  return return_value;
} /* boundary */

template < class Chain >
Chain coboundary ( const Chain & input ) {
  Chain return_value ( input . container () );
  for ( typename Chain::const_iterator term_iterator = input . begin (); term_iterator != input . end (); ++ term_iterator ) {
    Chain summand = input . container () . coboundary ( term_iterator -> first );
    summand *= term_iterator -> second;
    return_value += summand;
  }
  return return_value;
} /* coboundary */
