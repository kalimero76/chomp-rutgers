/*
 *  Adaptive_Cubical_Toplex.hpp
 *  
 *
 *  Created by Shaun Harker on 5/26/10.
 *  Copyright 2010. All rights reserved.
 *
 */

template < class Set >
Set intersect ( const Set & A, const Set & B ) {
  /* Generic implementation, assumes unordered sets */
  Set return_value;
  for ( typename Set::const_iterator it = A . begin (); 
        it != A . end (); ++ it ) {
    typename Set::value_type element = * it;
    if ( B . find ( element ) != B . end () ) return_value . insert ( element );
  } /* for */
  return return_value;
} /* intersect */
