/*
 * Sparse_Matrix.hpp
 * chomp-rutgers
 * 4/27/11
 * Shaun Harker
 *
 * This hpp file contains implementations for Sparse Matrices.
 *
 */ 

#include "boost/foreach.hpp"
#include <utility>
#include <algorithm>

template < class Ring >
Ring div ( const Ring y, const Ring x ) {
  /* For some reason, chip-makers decided to make (-2^63) / -1 generate an exception
   rather than returning 2^63 (which overflows into -2^63 ).
   This isn't comprehensible, since all other operations support overflow in the expected
   manner of treating the register as Z_{2^64} . But it means we have to check for it. 
   It is especially disconcerting, since multiplication does not have the same problem.
   However there is a simple workaround. */
  if ( x != Ring ( -1 ) ) {
    return y / x;
  } else {
    return y * x;
  }
}

/// Bezout
/// Given a and b, calculate a, b, and gcd such that
/// s * a + t * b = gcd
/// and gcd is the greatest common divisor of a and b
template < class Ring >
void Bezout (Ring * s_out,
             Ring * t_out,
             Ring * gcd_out,
             const Ring & a, 
             const Ring & b) {
  // Relies upon Euclidean Division being available, and also comparison.
  // The user doesn't need to sort the input.
  bool reversed = false;
  if ( a < b ) reversed = true;
  // For the proof to follow, we assume without loss a >= b
  Ring x = std::min ( a, b ); // Think x = b for proof below
  Ring y = std::max ( a, b ); // Think y = a for proof below
  // For extended euclidean algorithm
  Ring s0 = 1; Ring s1 = 0;
  Ring t0 = 0; Ring t1 = 1;
  while ( x != Ring ( 0 ) ) {
    //std::cout << "top of bezout\n";
    //std::cout << " y = " << y << " and x = " << x << "\n";
    //std::cout << "y - 1 = " << y - 1 << "\n";
    Ring q = div ( y, x );
    Ring r = y - x * q;
    Ring s = s0 - q * s1;
    Ring t = t0 - q * t1;
    //std::cout << "Claim: " << r << " = " << s << " * " << a << " + " << t << " * " << b << "\n";
    /* r = s * a + t * b
     Proof:
     By induction. First, k = 0 case.
     Since s_0 = 1 and t_0 = -q_0, we get
     r_0 = a - q_0 * b, which is indeed the case.
     Now the k = 1 case.
     s_1 = 0 - q_1 * (1)
     t_1 = 1 - q_1 * (-q_0)
     s_1 * a + t_1 * b = 
     -q_1 * a + b + q_0 * q_1 * b =
     b - q_1 ( a - q_0 * b ) =
     b - q_1 * r_0 = r_1.
     Now the inductive step for k > 1. 
     s_k * a + t_k * b =
     (s_{k-2} - q_k * s_{k-1}) * a + (t_{k-2} - q_k * t_{k-1}) * b =
     (s_{k-2} * a + t_{k-2} * b ) - q_k * (s_{k-1} * a + t_{k-1} * b ) =
     r_{k-2} - q_k * r_{k-1} = r_k.
     */
    s0 = s1; s1 = s;
    t0 = t1; t1 = t;
    y = x;
    x = r;
  } /* while */
  // Set output
  // The Bezout coefficients s and t are the second to last ones
  // to be calculated (the last ones give 0 = s*a + t*b)
  if ( not reversed ) {
    * s_out = s0;
    * t_out = t0;
  } else {
    * t_out = s0;
    * s_out = t0;
  }
  * gcd_out = y;
}



// Sparse Matrix Algorithm Definitions

// Copy the submatrix with rows [i0, i1) and columns [j0, j1) into a new Sparse Matrix.
template < class Ring >
Sparse_Matrix<Ring> submatrix ( int i0, int i1, int j0, int j1, const Sparse_Matrix<Ring> & A ) {
  Sparse_Matrix<Ring> result;
  std::cout << "submatrix NOT IMPLEMENTED\n";
  return result;
}

// Multiply
template < class Ring >
Sparse_Matrix<Ring> operator * (const Sparse_Matrix<Ring> & lhs,
                               const Sparse_Matrix<Ring> & rhs) {
  typedef Sparse_Matrix<Ring> Matrix;
  typedef typename Matrix::Index Index;
  typedef typename Matrix::size_type size_type;
  
  int I = lhs . number_of_rows ();
  int K = lhs . number_of_columns ();
  int J = lhs . number_of_columns ();
  Matrix result ( I, J );
  for ( int k = 0; k < K; ++ k ) {
    Index left_column_index = lhs . column_begin ( k );
    Index right_row_index = rhs . row_begin ( k );
    
    if ( lhs . column_size ( k ) < rhs . row_size ( k ) ) {
      left_column_index = lhs . column_begin ( k );
      while ( left_column_index != lhs . end () ) {
        size_type roww = lhs . row ( left_column_index );
        Ring left_value = lhs . read ( left_column_index );
        right_row_index = rhs . row_begin ( k );
        while ( right_row_index != rhs . end () ) {
          
          result . add ( roww, rhs . column ( right_row_index ), 
                        left_value * rhs . read ( right_row_index ) );
          rhs . row_advance ( right_row_index );
        } /* while */
        lhs . column_advance ( left_column_index );
      } /* while */
    } else {
      right_row_index = rhs . row_begin ( k );
      while ( right_row_index != rhs . end () ) {
        size_type col = rhs . column ( right_row_index );
        Ring right_value = rhs . read ( right_row_index );
        left_column_index = lhs . column_begin ( k );
        while ( left_column_index != lhs . end () ) {
          result . add ( lhs . row ( left_column_index ), col,
                        lhs . read ( left_column_index ) * right_value);
          lhs . column_advance ( left_column_index );
        } /* while */
        rhs . row_advance ( right_row_index );
      } /* while */
    } /* if-else */
  } /* for */
  return result;
}

// Add
template < class Ring >
Sparse_Matrix<Ring> operator + (const Sparse_Matrix<Ring> & lhs,
                               const Sparse_Matrix<Ring> & rhs) {
  Sparse_Matrix<Ring> result;
  std::cout << "operator + NOT IMPLEMENTED\n";
  return result;
}

// Subtract
template < class Ring >
Sparse_Matrix<Ring> operator - (const Sparse_Matrix<Ring> & lhs,
                               const Sparse_Matrix<Ring> & rhs) {
  Sparse_Matrix<Ring> result;
  std::cout << "operator - NOT IMPLEMENTED\n";
  return result;
}

/* Sparse_Matrix<> Member Function Definitions */

// For now I'm being inelegant about this

template < class Ring >
Sparse_Matrix<Ring>::Sparse_Matrix ( void ) {
  resize ( 0, 0 );
}

template < class Ring >
Sparse_Matrix<Ring>::Sparse_Matrix ( int i, int j ) {
  resize ( i, j );
}

template < class Ring >
void Sparse_Matrix<Ring>::resize ( int i, int j ) {
  row_begin_ . resize ( i, end () );
  column_begin_ . resize ( j, end () );
  row_sizes_ . resize ( i, 0 );
  column_sizes_ . resize ( j, 0 );
  // row_names and column_names aren't used yet TODO
  cache_A . resize ( std::max (i, j) );
  cache_A_TS . resize ( std::max (i, j), 0 );
  cache_B . resize ( std::max (i, j) );
  cache_B_TS . resize ( std::max (i, j), 0 );
  timestamp = 0;
  // WARNING, no support for shrinking at the moment.
}

template < class Ring >
typename Sparse_Matrix<Ring>::Index Sparse_Matrix<Ring>::new_index ( void ) {
  Index result;
  // First we reclaim garbage space
  if ( not garbage_ . empty () ) {
    result = garbage_ . top ();
    garbage_ . pop ();
    return result;
  }
  // No garbage space is left to reclaim.
  // We make use of the automatically resizing "data_"
  result = data_ . size ();
  data_ . push_back ( Element < Ring > () );
  // This should be fine, as std::vector is smart enough to do length doubling
  // or other tricks
  return result;
}

template < class Ring >
void Sparse_Matrix<Ring>::delete_index ( const Index index ) {
  garbage_ . push ( index );
}

template < class Ring >
void Sparse_Matrix<Ring>::hash_check ( const Index index ) {
  // TODO: Probably this could save some time with extra checks
  size_type i = row ( index );
  size_type j = column ( index );
  if ( std::min ( row_sizes_ [ i ], column_sizes_ [ j ] ) <= HASH_SWITCH ) {
    access_ . erase ( Position ( i, j ) );
  } else {
    access_ . insert ( std::make_pair ( Position (i, j ), index ) );
  } /* if-else */
} /* Sparse_Matrix<Ring>::hash_check */

// TODO: put some hysteresis in the hash switching, lest you
// can be attacked.
template < class Ring >
void Sparse_Matrix<Ring>::increment_row_size ( const size_type i ) {
  ++ row_sizes_ [ i ];
  if ( row_sizes_ [ i ] == 1 + HASH_SWITCH ) {
    Index index = row_begin ( i );
    while ( index != end () ) {
      hash_check ( index );
      row_advance ( index );
    } /* while */
  } /* if */
} /* Sparse_Matrix<Ring>::increment_row_size */

template < class Ring >
void Sparse_Matrix<Ring>::increment_column_size ( const size_type j ) {
  ++ column_sizes_ [ j ];
  if ( column_sizes_ [ j ] == 1 + HASH_SWITCH ) {
    Index index = column_begin ( j );
    while ( index != end () ) {
      hash_check ( index );
      column_advance ( index );
    } /* while */
  } /* if */
} /* Sparse_Matrix<Ring>::increment_row_size */

template < class Ring >
void Sparse_Matrix<Ring>::decrement_row_size ( const size_type i ) {
  -- row_sizes_ [ i ];
  if ( row_sizes_ [ i ] == HASH_SWITCH ) {
    Index index = row_begin ( i );
    while ( index != end () ) {
      hash_check ( index );
      row_advance ( index );
    } /* while */
  } /* if */
} /* Sparse_Matrix<Ring>::decrement_row_size */

template < class Ring >
void Sparse_Matrix<Ring>::decrement_column_size ( const size_type j ) {
  -- column_sizes_ [ j ];
  if ( column_sizes_ [ j ] == HASH_SWITCH ) {
    Index index = column_begin ( j );
    while ( index != end () ) {
      hash_check ( index );
      column_advance ( index );
    } /* while */
  } /* if */
} /* Sparse_Matrix<Ring>::decrement_row_size */

// erase, find
template < class Ring >
void Sparse_Matrix<Ring>::erase ( const Index index ) {
  // Repair the links
  //std::cout << "ERASING " << index << "\n";
  Element < Ring > element = data_ [ index ];
  if ( element . left != end () ) data_ [ element . left ] . right = element . right;
  if ( element . right != end () ) data_ [ element . right ] . left = element . left;
  if ( element . up != end () ) data_ [ element . up ] . down = element . down;
  if ( element . down != end () ) data_ [ element . down ] . up = element . up;
  
  // Update the row_begin_ and column_begin_ vectors if needed
  if ( element . left == end () ) row_begin_ [ element . position . first ] = element . right;
  if ( element . up == end () ) column_begin_ [ element . position . second ] = element . down;
  
  // Empty value from Random Access Hash Table if needed
  // Check if need to include in Random Access Hash Table
  size_type i = row ( index );
  size_type j = column ( index );
  size_type r_size = row_size ( i );
  size_type c_size = column_size ( j );
  if ( std::min ( r_size, c_size ) > HASH_SWITCH ) {
    access_ . erase ( Position (i, j) ) ;
  } /* if */
  
  // Free up the index
  delete_index ( index );
  
  // Update the row and column sizes
  decrement_row_size ( i );
  decrement_column_size ( j );
}

template < class Ring >
typename Sparse_Matrix<Ring>::Index 
Sparse_Matrix<Ring>::find ( const int i, const int j ) const {
  //std::cout << "Looking...\n";
  //static int find_count = 0;
  // Check the size of row i and the size of column j
  size_type r_size = row_size ( i );
  size_type c_size = column_size ( j );
  if ( std::min ( r_size, c_size ) > HASH_SWITCH ) {
    access_iterator it = access_ . find ( Position ( i, j ) );
    if ( it == access_ . end () ) return end ();
    //std::cout << "hash found: " << find_count ++ << "\n";
    return it -> second;
  }
  if ( r_size < c_size ) {
    // Search through row for (i, j)
    //std::cout << "row search\n";
    Index index = row_begin ( i );
    while ( index != end () ) {
      if ( row ( index ) == i && column ( index ) == j ) {
        //std::cout << "found: " << find_count ++ << "\n";
        return index;
      } /* if */
      row_advance ( index );
    } /* while */
    //std::cout << "unfound: " << find_count ++ << "\n";
    return end ();
  } else {
    // Search through column for (i, j)
    //std::cout << "column search\n";
    Index index = column_begin ( j );
    while ( index != end () ) {
      //std::cout << "Magically, index " << index << " is connected downward to " << data_ [ index ] . down << "\n";
      if ( row ( index ) == i && column ( index ) == j ) {
        //std::cout << "found: " << find_count ++ << "\n";
        return index;
      } /* if */
      column_advance ( index );
    } /* while */
    //std::cout << "unfound: " << find_count ++ << "\n";
    return end ();
  } /* if-else */
} /* Sparse_Matrix<Ring>::find */

// read and write

template < class Ring >
Ring Sparse_Matrix<Ring>::read ( const int i, const int j ) const {
  Index index = find ( i, j );
  if ( index == end () ) return Ring ( 0 );
  return read ( index );
}

template < class Ring >
Ring Sparse_Matrix<Ring>::read ( const Index index ) const {
  return data_ [ index ] . value;
}

template < class Ring >
typename Sparse_Matrix<Ring>::Index 
Sparse_Matrix<Ring>::write ( const int i, const int j, const Ring value, bool insert ) {
  /*
   std::cout << "Write Called.\n";
   print_matrix ( *this );
   
   if ( insert ) {
   std::cout << "Forced insert of (" << i << ", " << j << ")\n";
   if ( find (i, j) != end () ) {
   std::cout << "Caller mistaken, the element already exists.\n";
   }
   }
   */
  Index index;
  if ( not insert ) {
    index = find ( i, j );
    if ( index != -1 ) {
      //std::cout << " (" << i << ", " << j << ") was found!\n";
      return write ( index, value ); // deletes it if value is zero, notice
    } /* if */
    //std::cout << " (" << i << ", " << j << ") was not found!\n";
  } /* if */
  
  if ( value == Ring ( 0 ) ) return end (); // if value is zero, quit
  // Allocate new index
  index = new_index ();
  //std::cout << "Allocated a new index " << index << "!\n";
  // Send it to beginning of ith row and jth column
  //std::cout << "INSERTING index = " << index << " at (" << i << ", " << j << ") with value " << value << "\n";
  Element < Ring > element ( Position ( i, j ), value, end (), row_begin ( i ), end (), column_begin ( j ) );
  data_ [ index ] = element;
  if ( row_begin ( i ) != end () ) data_ [ row_begin ( i ) ] . left = index;
  if ( column_begin ( j ) != end () ) data_ [ column_begin ( j ) ] . up = index;
  
  /*
   std::cout << "  row linking  " << data_ [ index ] . left << " " << index << " " << 
   data_ [ index ] . right << " = " << row_begin ( i ) << "\n";
   std::cout << "  col linking  " << data_ [ index ] . up << " " << index << " " << 
   data_ [ index ] . down << " = " << column_begin ( j ) << "\n";
   */
  row_begin_ [ i ] = index;
  column_begin_ [ j ] = index;
  
  // Check if need to include in Random Access Hash Table
  size_type r_size = row_size ( i ) + 1;
  size_type c_size = column_size ( j ) + 1;
  if ( std::min ( r_size, c_size ) > HASH_SWITCH ) {
    access_ [ Position (i, j) ] = index;
  } /* if */
  
  // Update the row and column sizes
  increment_row_size ( i );
  increment_column_size ( j );
  
  return index;
}

template < class Ring >
typename Sparse_Matrix<Ring>::Index Sparse_Matrix<Ring>::write ( Index index, const Ring value ) {
  if ( value == Ring ( 0 ) ) {
    erase ( index );
    return end ();
  } else {
    data_ [ index ] . value = value;
    return index;
  } /* if-else */
}  

// begin and end
template < class Ring >
typename Sparse_Matrix<Ring>::Index 
Sparse_Matrix<Ring>::row_begin ( const int i ) const {
  return row_begin_ [ i ];
}

template < class Ring >
typename Sparse_Matrix<Ring>::Index 
Sparse_Matrix<Ring>::column_begin ( const int j ) const {
  return column_begin_ [ j ];
}

template < class Ring >
typename Sparse_Matrix<Ring>::Index 
Sparse_Matrix<Ring>::end ( void ) const {
  return -1;
}

// traversal
template < class Ring >
void Sparse_Matrix<Ring>::row_advance ( Index & index ) const {
  //std::cout << "row_advance ( " << index << " ) = ";
  index = data_ [ index ] . right;
  //std::cout << index << "\n";
}

template < class Ring >
void Sparse_Matrix<Ring>::column_advance ( Index & index ) const {
  //std::cout << "column_advance ( " << index << " ) = ";
  index = data_ [ index ] . down;
  // std::cout << index << "\n";
}

// row, column
template < class Ring >
typename Sparse_Matrix<Ring>::size_type Sparse_Matrix<Ring>::row ( const Index index ) const {
  return data_ [ index ] . position . first;
}

template < class Ring >
typename Sparse_Matrix<Ring>::size_type Sparse_Matrix<Ring>::column ( const Index index ) const {
  return data_ [ index ] . position . second;
}

// add to some position
template < class Ring >
void Sparse_Matrix<Ring>::add ( int i, int j, const Ring value ) {
  Index index = find ( i, j );
  if ( index == end () ) {
    write ( i, j, value, true );
    return;
  } /* if */
  data_ [ index ] . value += value;
  // If we zeroed out the entry, delete it
  if ( data_ [ index ] . value == Ring ( 0 ) ) {
    erase ( index );
  }
  return;
}


template < class Ring >
typename Sparse_Matrix<Ring>::size_type Sparse_Matrix<Ring>::number_of_rows ( void ) const {
  return row_sizes_ . size ();
}

template < class Ring >
typename Sparse_Matrix<Ring>::size_type Sparse_Matrix<Ring>::number_of_columns ( void ) const {
  return column_sizes_ . size ();
}

template < class Ring >
typename Sparse_Matrix<Ring>::size_type Sparse_Matrix<Ring>::size ( void ) const {
  int result = 0;
  BOOST_FOREACH ( size_type summand, row_sizes_ ) {
    result += summand;
  } /* boost_foreach */
  return result;
}

template < class Ring > 
void Sparse_Matrix<Ring>::row_operation ( const Ring a, const Ring b,
                                        const Ring c, const Ring d,
                                        size_type i, size_type j ) {
  ++ number_of_pivots;
  // Increment timestamp
  ++ timestamp;
  // Note: we expect the stacks are clear. 
  
  // TODO: if timestamp has rolled over (as if) then clear cache
  /* Stage I. Copy a times the contents of row i into cache 1,
   Copy d times the contents of row j into cache 2.
   Mark the timestampts. Do not write to stack. */
  Index i_index = row_begin ( i );
  Index j_index = row_begin ( j );
  while ( i_index != end () ) {
    size_type col = column ( i_index );
    cache_A [ col ] = a * read ( i_index );
    cache_A_TS [ col ] = timestamp;
    row_advance ( i_index );
  }
  
  while ( j_index != end () ) {
    size_type col = column ( j_index );
    cache_B [ col ] = d * read ( j_index );
    cache_B_TS [ col ] = timestamp;
    row_advance ( j_index );
  }
  
  /* Stage II. Copy c * the contents of row i into cache 2,
   Copy b * the contents of row j into cache 1. 
   This time, do not report to the stack if the timestamp matches,
   and do not overwrite if the timestamp matches. If the timestamp doesn't match
   then overwrite and push to the stack */
  
  i_index = row_begin ( i );
  while ( i_index != end () ) {
    size_type col = column ( i_index );
    if ( cache_B_TS [ col ] == timestamp ) {
      cache_B [ col ] += c * read ( i_index );
    } else {
      cache_B [ col ] = c * read ( i_index );
      cache_B_S . push ( col );
    }
    row_advance ( i_index );
  }
  
  j_index = row_begin ( j );
  while ( j_index != end () ) {
    size_type col = column ( j_index );
    if ( cache_A_TS [ col ] == timestamp ) {
      cache_A [ col ] += b * read ( j_index );
    } else {
      cache_A [ col ] = b * read ( j_index );
      cache_A_S . push ( col );
    }
    row_advance ( j_index );
  }
  
  /* Stage III. Replace the entries in row i and row j that are already there with the new values */
  
  i_index = row_begin ( i );
  while ( i_index != end () ) {
    size_type col = column ( i_index );
    write ( i_index, cache_A [ col ] );
    row_advance ( i_index );
  }
  
  j_index = row_begin ( j );
  while ( j_index != end () ) {
    size_type col = column ( j_index );
    write ( j_index, cache_B [ col ] );
    row_advance ( j_index );
  }
  
  /* Stage IV. Unload the stacks and insert the new elements */
  while ( not cache_A_S . empty () ) {
    size_type col = cache_A_S . top ();
    cache_A_S . pop ();
    write ( i, col, cache_A [ col ], true );
  }
  
  while ( not cache_B_S . empty () ) {
    size_type col = cache_B_S . top ();
    cache_B_S . pop ();
    write ( j, col, cache_B [ col ], true );
  }
  
}

template < class Ring > 
void Sparse_Matrix<Ring>::column_operation (const Ring a, const Ring b,
                                           const Ring c, const Ring d,
                                           size_type i, size_type j ) {
  ++ number_of_pivots;
  // Increment timestamp
  ++ timestamp;
  // Note: we expect the stacks are clear. 
  
  //std::cout << "column operation.\n";
  //std::cout << a << " " << b << " " << c << " " << d << " " << i << " " << j << "\n";
  
  // TODO: if timestamp has rolled over (as if) then clear cache
  /* Stage I. Copy a times the contents of column i into cache 1,
   Copy d times the contents of column j into cache 2.
   Mark the timestampts. Do not write to stack. */
  Index i_index = column_begin ( i );
  while ( i_index != end () ) {
    size_type roww = row ( i_index );
    cache_A [ roww ] = a * read ( i_index );
    cache_A_TS [ roww ] = timestamp;
    //std::cout << i_index << "\n";
    column_advance ( i_index );
  }
  //std::cout << "*****\n";
  
  Index j_index = column_begin ( j );
  while ( j_index != end () ) {
    size_type roww = row ( j_index );
    cache_B [ roww ] = d * read ( j_index );
    cache_B_TS [ roww ] = timestamp;
    column_advance ( j_index );
  }
  
  /* Stage II. Copy c * the contents of column i into cache 2,
   Copy b * the contents of column j into cache 1. 
   This time, do not report to the stack if the timestamp matches,
   and do not overwrite if the timestamp matches. If the timestamp doesn't match
   then overwrite and push to the stack */
  
  i_index = column_begin ( i );
  while ( i_index != end () ) {
    size_type roww = row ( i_index );
    if ( cache_B_TS [ roww ] == timestamp ) {
      cache_B [ roww ] += c * read ( i_index );
    } else {
      cache_B [ roww ] = c * read ( i_index );
      cache_B_S . push ( roww );
    }
    column_advance ( i_index );
  }
  
  j_index = column_begin ( j );
  while ( j_index != end () ) {
    size_type roww = row ( j_index );
    if ( cache_A_TS [ roww ] == timestamp ) {
      cache_A [ roww ] += b * read ( j_index );
    } else {
      cache_A [ roww ] = b * read ( j_index );
      cache_A_S . push ( roww );
    }
    column_advance ( j_index );
  }
  
  /* Stage III. Replace the entries in column i and column j that are already there with the new values */
  
  i_index = column_begin ( i );
  while ( i_index != end () ) {
    size_type roww = row ( i_index );
    //std::cout << "III. (" << row ( i_index ) << ", " << column ( i_index ) << ") = " << cache_A [ roww ] << "\n";
    write ( i_index, cache_A [ roww ] );
    column_advance ( i_index );
  }
  
  j_index = column_begin ( j );
  while ( j_index != end () ) {
    size_type roww = row ( j_index );
    //std::cout << "III. (" << row ( j_index ) << ", " << column ( j_index ) << ") = " << cache_B [ roww ] << "\n";
    write ( j_index, cache_B [ roww ] );
    column_advance ( j_index );
  }
  
  /* Stage IV. Unload the stacks and insert the new elements */
  while ( not cache_A_S . empty () ) {
    size_type roww = cache_A_S . top ();
    cache_A_S . pop ();
    //std::cout << "IV. (" << roww << ", " << i << ") = " << cache_A [ roww ] << "\n";
    write ( roww, i, cache_A [ roww ], true );
  }
  
  while ( not cache_B_S . empty () ) {
    size_type roww = cache_B_S . top ();
    cache_B_S . pop ();
    //std::cout << "IV. (" << roww << ", " << j << ") = " << cache_B [ roww ] << "\n";
    write ( roww, j, cache_B [ roww ], true );
  }
}


template < class Ring >
void Sparse_Matrix<Ring>::swap_rows ( const int i, const int j ) {
  // really dumb way
  row_operation (Ring ( 0 ), Ring ( 1 ),
                 Ring ( 1 ), Ring ( 0 ),
                 i, j );
}

template < class Ring >
void Sparse_Matrix<Ring>::swap_columns ( const int i, const int j ) {
  // really dumb way
  column_operation (Ring ( 0 ), Ring ( 1 ),
                    Ring ( 1 ), Ring ( 0 ),
                    i, j );
}

template < class Ring >
typename Sparse_Matrix<Ring>::size_type 
Sparse_Matrix<Ring>::row_size ( const size_type i ) const {
  return row_sizes_ [ i ];
}

template < class Ring >
typename Sparse_Matrix<Ring>::size_type 
Sparse_Matrix<Ring>::column_size ( const size_type j ) const {
  return column_sizes_ [ j ];
}

/* PIVOTING */

/// RowPivot
/// Using the pivot element (i, j), clear the jth column using row operations
template < class Ring >
void RowPivot (Sparse_Matrix<Ring> * U,
               Sparse_Matrix<Ring> * Uinv,
               Sparse_Matrix<Ring> * D,
               const int i,
               const int j) {
  typedef Sparse_Matrix<Ring> Matrix;
  typedef typename Matrix::Index Index;
  // Main Loop
  Index pivot_index = D -> find ( i, j );
  Index index = D -> column_begin ( j );
  while ( index != D -> end () ) { 
    // Perform the pivot operation. Use (i, j) to eliminate element.
    Ring s, t, g, x, y;
    Ring a = D -> read ( pivot_index );
    Ring b = D -> read ( index ); // Remains valid.
    int k = D -> row ( index );
    D -> column_advance ( index );
    // Prevent self-elimination.
    if ( i == k ) continue;
    // Determine necessary row operations with Euclidean Algorithm
    Bezout ( &s, &t, &g, a, b );
    x = div ( a, g );
    y = div ( b, g );
    
    
     // DEBUG
    /*
     std::cout << " Value at pivot = " << a << "\n";
     std::cout << " Elimination value = " << b << "\n";
     std::cout << " Elimination index = " << k << "\n";
     std::cout << " Bezout Formula: " << s << " * " << a << " + " << t << " * " << b << " = " << g << "\n";
     */
    
    /* Explanation of the row operations:
     Apply the 2x2 matrix from the left:
     M=  [  s  t  ]     Minv = [ x  -t ]
     [ -y  x  ]            [ y   s ]
     This means the following: Let I and K represent the ith and kth rows, respectively.
     We do the following:  I' <- s I + t K
     K' <- -y I + x K.
     See that this makes a <- s * a + t * b = g
     and b <- (- b * a + a * b) / g = 0, as desired.
     Also, we have to update U and U_inv. 
     U is to be updated by multiplying on the right by M_inv.
     This involves column operations. Let I and K be the ith and kth columns of U, respectively.
     We set I' <- x * I + y * K
     K' <- -t * I + s * K
     Uinv is to be updated by multiplying on the left by M. This is the same as what we did to D.
     */
    
    D -> row_operation (s, t,
                        -y, x,
                        i, k);
    
    
    Uinv -> row_operation (s, t,
                           -y, x,
                           i, k);
    
    
    U -> column_operation (x, y,
                           -t, s,
                           i, k);
    //std::cout << "Result of elimination step:\n";
    //print_matrix ( *D );
  } /* boost_foreach */
}

/// ColumnPivot
/// Using the pivot element (i, j), clear the ith row using column operations
template < class Ring >
void ColumnPivot (Sparse_Matrix<Ring> * V,
                  Sparse_Matrix<Ring> * Vinv,
                  Sparse_Matrix<Ring> * D,
                  const int i,
                  const int j) {
  
  typedef Sparse_Matrix<Ring> Matrix;
  typedef typename Matrix::Index Index;
  // Main Loop
  Index pivot_index = D -> find ( i, j );
  Index index = D -> row_begin ( i );
  //std::cout << "DEBUG: ColumnPivot. pivot_index = " << pivot_index << " and index = " << index << "\n";
  //std::cout << "DEBUG: D -> end () = " << D -> end () << "\n";

  while ( index != D -> end () ) { 
    //std::cout << "Inside while loop...\n";
    // Perform the pivot operation. Use (i, j) to eliminate element.
    Ring s, t, g, x, y;
    Ring a = D -> read ( pivot_index );
    Ring b = D -> read ( index ); // Remains valid.
    int k = D -> column ( index );
    D -> row_advance ( index );
    //std::cout << " a = " << a << " b = " << b << " k = " << k << " and j = " << j << "\n";
    //std::cout << " (row advanced) index = " << index << "\n";
    // Prevent self-elimination.
    if ( j == k ) continue;
    // Determine necessary row operations with Euclidean Algorithm
    Bezout ( &s, &t, &g, a, b );
    x = div ( a, g );
    y = div ( b, g );
    
    // DEBUG
    /*
     std::cout << " Value at pivot = " << a << "\n";
     std::cout << " Elimination value = " << b << "\n";
     std::cout << " Elimination index = " << k << "\n";
     std::cout << " Bezout Formula: " << s << " * " << a << " + " << t << " * " << b << " = " << g << "\n";
     */
    /* Explanation of the row operations:
     Apply the 2x2 matrix from the right:
     M=  [  s  -y  ]     Minv = [  x  y ]
     [  t   x  ]            [ -t  s ]
     This means the following: Let J and K represent the jth and kth rows, respectively.
     We do the following:  J' <-  s J + t K
     K' <-  -y J + x K.
     See that this makes a <- s * a + t * b = g
     and b <- (- b * a + a * b) / g = 0, as desired.
     Also, we have to update U and U_inv. 
     U is to be updated by multiplying on the left by M_inv.
     This involves column operations. Let J and K be the jth and kth columns of U, respectively.
     We set J' <-  x * J + y * K
     K' <-  -t * J + s * K
     Uinv is to be updated by multiplying on the left by M. This is the same as what we did to D.
     */
    
    D -> column_operation (s, t,
                           -y, x,
                           i, k);
    
    Vinv -> column_operation (s, t,
                              -y, x,
                              i, k);
    
    V -> row_operation (x, y,
                        -t, s,
                        i, k);
    
    //std::cout << "Result of elimination step:\n";
    //print_matrix ( *D );
  } /* while */
  
}

/* SMITH FORM CALCULATION */

/// DoublePivot
/// Using the pivot element (i, j), clear the ith row and jth column using row and column operations
template < class Ring >
void SmithPivot (Sparse_Matrix<Ring> * U,
                 Sparse_Matrix<Ring> * Uinv,
                 Sparse_Matrix<Ring> * V,
                 Sparse_Matrix<Ring> * Vinv,
                 Sparse_Matrix<Ring> * D,
                 const int i,
                 const int j) {
  typedef Sparse_Matrix<Ring> Matrix;
  typedef typename Matrix::Index Index;
  // Obtain the (i, j)th element of D
  Index pivot_index = D -> find ( i, j );
  Ring pivot_value = D -> read ( pivot_index ); 
  
  //DEBUG
  if ( D -> read (i, j) == Ring ( 0 ) ) {
    std::cout << "Pivot cannot be zero!\n";
    exit ( 1 );
  }
   
      
  Ring old_pivot_value ( 0 );
  //std::cout << " **** SMITH PIVOT (" << i << ", " << j << ") value = " << pivot_value << " **** \n";
  //print_matrix ( * D );
  // We assume pivot_value != 0, so this while loop will run at least once:
  while ( pivot_value != old_pivot_value ) {
    old_pivot_value = pivot_value;
    
    //sane_matrix ( *D );
    //std::cout << "***** Calling ColumnPivot on (" << i << ", " << j << ")\n";
    ColumnPivot ( V, Vinv, D, i, j);
    //print_matrix ( *D );
    
    //sane_matrix ( *D );
    //std::cout << "***** Calling RowPivot on (" << i << ", " << j << ")\n";
    RowPivot ( U, Uinv, D, i, j );
    //print_matrix ( *D );
    
    pivot_value = D -> read ( pivot_index ); 
    // if pivot_value has not changed, then no new work has been produced.
  }
  //std::cout << " **** SMITH PIVOT COMPLETE **** \n";
}

/// SmithNormalForm
/// Input: A
/// Produce matrices U, Uinv, V, Vinv, and D
/// such that U D V = A
/// U and V are Z-invertible with inverses Uinv and Vinv
/// D is diagonal, such that d_i | d_{i+1}
template < class Ring >
void SmithNormalForm (Sparse_Matrix<Ring> * U,
                 Sparse_Matrix<Ring> * Uinv,
                 Sparse_Matrix<Ring> * V,
                 Sparse_Matrix<Ring> * Vinv,
                 Sparse_Matrix<Ring> * D,
                 const Sparse_Matrix<Ring> & A) {
  typedef Sparse_Matrix<Ring> Matrix;
  typedef typename Matrix::Index Index;
  typedef typename Matrix::size_type size_type;
  // We copy A into D.
  *D = A;
  
  //std::cout << "SmithNormalForm Calculation.\n";
  //print_matrix ( *D );
  //sane_matrix ( *D );
  // Resize outputs, set to identity
  U -> resize ( A . number_of_rows (), A . number_of_rows () );
  for ( int i = 0; i < A . number_of_rows (); ++ i ) U -> write ( i, i, Ring ( 1 ), true ); 
  *Uinv = *U;
  V -> resize ( A . number_of_columns (), A . number_of_columns () );
  for ( int i = 0; i < A . number_of_columns (); ++ i ) V -> write ( i, i, Ring ( 1 ), true ); 
  *Vinv = *V;
  
  // The algorithm proceeds by selecting a sequence of pivots (t, j_t)
  int t = 0;
  //std::cout << "**** ELIMINATION STAGE ****\n";
  for ( int j = 0; j < D -> number_of_columns (); ++ j ) {
    if ( D -> column_size ( j ) == 0 ) continue;
    
    // We want to find a good pivot row from the jth column
    // The simplest criterion is to pick the one with the smallest size
    size_type pivot_row; // we find this
    size_type best_size = D -> number_of_columns (); // maximum size row could be
    Index index = D -> column_begin ( j );
    while ( index != D -> end () ) {
      size_type which_row = D -> row ( index );
      size_type row_size = D -> row_size ( which_row );
      //std::cout << "Looking at index " << index << ", we see that row " << which_row << " has size " << row_size << "\n";
      if ( row_size <= best_size ) {
        best_size = row_size;
        pivot_row = which_row;
      } /* if */
      D -> column_advance ( index );
    } /* while */
    // At this point we have found a pivot. We'll need to swap rows.
    
    //std::cout << " Just before swapping:\n";
    //print_matrix ( *D );
    //sane_matrix ( *D );
    
    //std::cout << "Preliminary swapping of row/column (" << t << ", " << pivot_row << ")...\n";
    D -> swap_rows ( t, pivot_row );
    U -> swap_columns ( t, pivot_row );
    Uinv -> swap_rows ( t, pivot_row );
    
    
    //print_matrix ( *D );
    //sane_matrix ( *D );
    // Now the pivot_row is really t
    
    // Now we use pivot off from the pivot choice,
    // zeroing out all elements in its row and column except itself.
    // At the end of this procedure, all remaining entries have
    // row number greater than t and column number greater than j
    //std::cout << "Performing the pivot step at (" << t << ", " << j << "):\n";
    SmithPivot ( U, Uinv, V, Vinv, D, t, j );
    //print_matrix ( *D );
    // Now we swap columns, making this the t-th column rather than the jth column
    // (note that j >= t )
    D -> swap_columns ( t, j );
    V -> swap_rows ( t, j );
    Vinv -> swap_columns ( t, j );
    
    // Finally, increment t.
    ++ t;
  } /* for */
  // At this stage, we are almost done. What remains is to ensure the
  // divisability d_i | d_{i+1}.
  // The following trick accomplishes this: one by one, we 
  // copy the value of the diagonals (except the first) to the position
  // immediately to the left
  //std::cout << "*** DIAGONAL DIVISABILITY STAGE ****\n";
  //print_matrix ( *D );

  int r = std::min ( D -> number_of_rows (), D -> number_of_columns () );
  for ( int i = 0; i < r - 1; ++ i ) {
    //std::cout << " i = " << i << " and r = " << r << "\n";
    D -> write ( i + 1, i,  D -> read ( i + 1, i + 1 ) );
    // This is a column operation; we need to update V and Vinv
    /* The matrix in question is
     M =  [ 1 0 ]    Minv = [  1 0 ]
     [ 1 1 ]           [ -1 1 ]
     We should multiply V on the left by Minv and Vinv on the right by M.
     */
    
    Vinv -> column_operation (Ring ( 1 ), Ring ( 1 ),
                              Ring ( 0 ), Ring ( 1 ),
                              i, i + 1 );
    
    
    V -> row_operation (Ring ( 1 ), Ring ( 0 ),
                        Ring ( -1 ), Ring ( 1 ),
                        i, i + 1 );
    
    SmithPivot ( U, Uinv, V, Vinv, D, i, i );
    D -> write ( i, i, abs ( D -> read ( i, i ) ) );
    D -> write ( i + 1, i + 1, abs ( D -> read ( i + 1, i + 1 ) ) );

    //print_matrix ( *D );

  }
  //std::cout << "Total number of pivot moves: " << number_of_pivots << "\n";
}

/// ColumnEchelon
/// Input: A
/// Output: B, where B is A in column echelon form.


/* This function produces a "Sparse_Matrix" to represent the boundary map of the appropriate dimension in the complex. */
/* TODO: this only will work for Default_Cell */
template < class Cell_Complex >
void Sparse_Matrix_Boundary_Map ( Sparse_Matrix < typename Cell_Complex::Ring > & output_matrix, 
                                  const Cell_Complex & complex, const unsigned int dimension ) {
    /* Check dimension to see whether or not it is within bounds. */
	if ( dimension > complex . dimension () || dimension < 0 ) return;
  if ( dimension == 0 ) {
    output_matrix . resize ( 0, complex . size ( 0 ) );
    return;
  } 
  
  output_matrix . resize ( complex . size ( dimension - 1 ), complex . size ( dimension ) );
  unsigned long target_start = complex . index ( complex . begin ( dimension - 1 ) );
  unsigned long source_start = complex . index ( complex . begin ( dimension ) );
  
	/* Now we loop through all elementary chains of dimension "map_dimension". */
	for ( typename Cell_Complex::const_iterator group_iterator = complex . begin ( dimension ); 
	group_iterator != complex . end ( dimension ); ++ group_iterator ) {
    unsigned long group_index = complex . index ( group_iterator );
		/* We find the boundary of the current elementary chain. */
		typename Cell_Complex::Chain boundary_chain = complex . boundary ( group_iterator );
		/* We loop through the terms in the boundary we have found. */
		for ( typename Cell_Complex::Chain::iterator chain_iterator = boundary_chain . begin (); 
         chain_iterator != boundary_chain . end (); ++ chain_iterator ) {
			output_matrix . write ( complex . index ( chain_iterator -> first ) - target_start, 
                              group_index - source_start, 
                              chain_iterator -> second ); 
    } /* for */
  } /* for */
} /* Sparse_Matrix_Boundary_Map */
