/*
 * Sparse_Matrix.h
 * chomp-rutgers
 * 4/27/11
 * Shaun Harker
 *
 * This header contains declarations for Sparse Matrices.
 *
 */ 

#ifndef CHOMP_SPARSE_MATRIX_
#define CHOMP_SPARSE_MATRIX_

#include <vector>
#include <stack>
#include "boost/functional/hash.hpp"

#define GCC_VERSION (__GNUC__ * 10000 \
+ __GNUC_MINOR__ * 100 \
+ __GNUC_PATCHLEVEL__)
/* Test for GCC > 4.2.0 */
#if GCC_VERSION > 40200
#include <tr1/unordered_map> //PORTABILITY ISSUE
namespace std { using namespace tr1; }
#else
#warning Old Version of GCC -- using hash_set instead of unordered_set
#include <ext/hash_map>
namespace std { using namespace __gnu_cxx; }
#define unordered_set hash_set
#endif


#define HASH_SWITCH 10

//int number_of_pivots = 0;

// Sparse Matrix Data Structure

typedef std::pair < int, int > Position;

// Forward Declarations
template < class Ring > struct Element;
template < class Ring > class Sparse_Matrix;

// struct Element
template < class Ring >
struct Element {
  typedef int size_type;
  Position position;
  Ring value;
  size_type left;
  size_type right;
  size_type up;
  size_type down;
  
  Element ( void ) {}
  
  Element ( Position position, Ring value, size_type left, size_type right, size_type up, size_type down) :
  position(position), value(value), left(left), right(right), up(up), down(down) {}
  
  template < class T > Element ( const T & copy_me ) : 
  position( copy_me . position), value( copy_me . value), left( copy_me . left), 
  right( copy_me . right), up( copy_me . up), down( copy_me . down) {}

};

// friends of Sparse_Matrix
// Multiply
template < class Ring >
Sparse_Matrix<Ring> operator * (const Sparse_Matrix<Ring> & rhs,
                               const Sparse_Matrix<Ring> & lhs);

// Add
template < class Ring >
Sparse_Matrix<Ring> operator + (const Sparse_Matrix<Ring> & rhs,
                               const Sparse_Matrix<Ring> & lhs);

// Subtract
template < class Ring >
Sparse_Matrix<Ring> operator - (const Sparse_Matrix<Ring> & rhs,
                               const Sparse_Matrix<Ring> & lhs);

// class Sparse Matrix
template < class Ring >
class Sparse_Matrix {
public:
  // typedefs
  typedef Ring value_type;
  typedef int Index;
  typedef int size_type;
private:
  
public: // not friends with different templated versions, weirdly
  // data to store the Sparse Matrix
  std::vector < Element<Ring> > data_;
  // Garbage Collection structure
  std::stack < Index > garbage_;
  // data to assist in O(1) random access times
  std::unordered_map < Position, Index, boost::hash< Position > > access_;
  typedef std::unordered_map < Position, Index, boost::hash< Position > >::const_iterator access_iterator;
  // data to store the beginning of the rows and columns
  std::vector < Index > row_begin_;
  std::vector < Index > column_begin_;
  // data to store amount of non-zero elements per row and column
  std::vector < size_type > row_sizes_;
  std::vector < size_type > column_sizes_;
  // data to handle quick permutations of rows and columns
  std::vector < size_type > row_names_;
  std::vector < size_type > column_names_;
  // data to handle quick linear algebra
  std::vector < Ring > cache_A;
  std::vector < size_type > cache_A_TS;
  std::stack < size_type > cache_A_S;
  std::vector < Ring > cache_B;
  std::vector < size_type > cache_B_TS;
  std::stack < size_type > cache_B_S;
  size_type timestamp;
  // technicals
  Index new_index ( void );
  void delete_index ( const Index index );
  void hash_check ( const Index index );
  void increment_row_size ( const size_type i );
  void increment_column_size ( const size_type j );
  void decrement_row_size ( const size_type i );
  void decrement_column_size ( const size_type j );
  // friends
  
  friend Sparse_Matrix<Ring> operator * <> ( const Sparse_Matrix<Ring> &, const Sparse_Matrix<Ring> & );
  friend Sparse_Matrix<Ring> operator + <> ( const Sparse_Matrix<Ring> &, const Sparse_Matrix<Ring> & );
  friend Sparse_Matrix<Ring> operator - <> ( const Sparse_Matrix<Ring> &, const Sparse_Matrix<Ring> & );
  
public:
  
  // erase, find
  void erase ( const Index index );
  Index find ( const size_type i, const size_type j ) const;
  
  // read, write
  Ring read ( const size_type i, const size_type j ) const;
  Ring read ( Index index ) const;
  Index write ( const size_type i, const size_type j,                 // notice the optimization argument
               const Ring value, bool insert = false );  // it prevents the search when set to true
  Index write ( Index index, const Ring value );
  
  // begin and end
  Index row_begin ( const size_type i ) const;
  Index column_begin ( const size_type j ) const;
  Index end ( void ) const;
  
  // traversal. row_advance advances within a row, column_advance within a column
  void row_advance ( Index & index ) const;
  void column_advance ( Index & index ) const;
  
  // row and column (to learn position)
  size_type row ( const Index index ) const;
  size_type column ( const Index index ) const;
  
  // add to some position
  void add ( size_type i, size_type j, const Ring value );
  
  // constructors 
  Sparse_Matrix ( void );
  Sparse_Matrix ( size_type i, size_type j );
  template < class T > Sparse_Matrix ( const T & copy_me );
  
  // size and resize
  void resize ( size_type i, size_type j );
  size_type size ( void ) const; // sparsity size
  
  // linear algebra
  void row_operation (const Ring a, const Ring b,
                      const Ring c, const Ring d,
                      size_type i, size_type j );
  
  void column_operation (const Ring a, const Ring b,
                         const Ring c, const Ring d,
                         size_type i, size_type j );
  
  // keep
  void swap_rows ( const size_type i, const size_type j );
  void swap_columns ( const size_type i, const size_type j );
  size_type number_of_rows ( void ) const;
  size_type number_of_columns ( void ) const;
  size_type row_size ( const size_type i ) const;
  size_type column_size ( const size_type j ) const;
  
};

// Sparse Matrix Algorithms

template < class Ring >
void print_matrix ( const Sparse_Matrix<Ring> & print_me ) {
  typedef int size_type;
  size_type I = print_me . number_of_rows ();
  size_type J = print_me . number_of_columns ();
  std::cout << " Matrix is " << I << " x " << J << "\n";
  for ( size_type i = 0; i < I; ++ i ) {
    std::cout << "[";
    for ( size_type j = 0; j < J; ++ j ) {
      std::cout << print_me . read ( i, j ) << " ";
    } /* for */
    std::cout << "]\n";
  } /* for */
  //char c;
  //std::cin >> c;
  
  std::cout << "\n";
}
template < class Ring >
void sane_matrix ( const Sparse_Matrix<Ring> & print_me ) {
  typedef int size_type;
  std::cout <<  "MATRIX SANITY CHECK\n";
  size_type I = print_me . number_of_rows ();
  size_type J = print_me . number_of_columns ();
  std::cout << "Checking rows.\n";
  for ( size_type i = 0; i < I; ++ i ) {
    int ind = print_me . row_begin ( i );
    int count = 0;
    while ( ind != -1 ) {
      std::cout << "(" << print_me . row ( ind ) << ", " << print_me . column ( ind ) << "; " << print_me . read ( ind ) << " | " << ind << ") \n";
      print_me . row_advance ( ind );
      ++ count;
    }
    if ( count != print_me . row_size ( i ) ) {
      std::cout << "Discrepancy on row i = " << i << "\n";
    }
  } /* for */
  std::cout << "Checking columns.\n";

  for ( size_type j = 0; j < J; ++ j ) {
    int ind = print_me . column_begin ( j );
    int count = 0;
    while ( ind != -1 ) {
      std::cout << "(" << print_me . row ( ind ) << ", " << print_me . column ( ind ) << "; " << print_me . read ( ind ) << " | " << ind << ") \n";
      print_me . column_advance ( ind );
      ++ count;
    }
    if ( count != print_me . column_size ( j ) ) {
      std::cout << "Discrepancy on column j = " << j << "\n";
    }  
  } /* for */
  //char c;
  //std::cin >> c;
  
  //std::cout << "\n";
}
// Copy the submatrix with rows [i0, i1) and columns [j0, j1) size_typeo a new Sparse Matrix.
//template < class Ring >
//Sparse_Matrix<Ring> submatrix ( size_type i0, size_type i1, size_type j0, size_type j1, const Sparse_Matrix<Ring> & A );


/// SmithNormalForm
/// Input: A
/// Produce matrices U, Uinv, V, Vinv, and D
/// such that U D V = A
/// U and V are Z-invertible with inverses Uinv and Vinv
/// D is diagonal, such that d_i | d_{i+1}
template < class Ring >
void
SmithNormalForm (Sparse_Matrix<Ring> * U,
                 Sparse_Matrix<Ring> * Uinv,
                 Sparse_Matrix<Ring> * V,
                 Sparse_Matrix<Ring> * Vinv,
                 Sparse_Matrix<Ring> * D,
                 const Sparse_Matrix<Ring> & A);

/// Submatrix
/// Input: A, top, bottom, left, right
/// Produce matrix B = A(top:bottom, left:right)

template < class Ring >
void
Submatrix (Sparse_Matrix<Ring> * B, 
           const typename Sparse_Matrix<Ring>::size_type top,
           const typename Sparse_Matrix<Ring>::size_type bottom,
           const typename Sparse_Matrix<Ring>::size_type left,
           const typename Sparse_Matrix<Ring>::size_type right,
           const Sparse_Matrix<Ring> & A);

/// ColumnEchelon
/// Input: A
/// Output: B, where B is A in column echelon form.

template < class Ring >
void ColumnEchelon (Sparse_Matrix<Ring> * B, const Sparse_Matrix<Ring> & A);

// TODO

/** Sparse_Matrix_Boundary_Map creates a sparse representation of the boundary map \f$\partial_k : C_k \rightarrow C_{k-1}\f$. The output is returned in "output_matrix",
* which is supplied by reference. The dimension \f$k\f$ is supplied as the parameter "dimension" */
template < class Cell_Complex >
void Sparse_Matrix_Boundary_Map ( Sparse_Matrix < typename Cell_Complex::Ring > & output_matrix, 
                                  const Cell_Complex & complex, const unsigned int dimension ); 

template < class Cell_Complex >
Sparse_Matrix<typename Cell_Complex::Ring> SparseMatrixRepresentation 
( const std::vector < std::pair < typename Cell_Complex::Chain, unsigned int > > & input, 
  const unsigned int dimension, const Cell_Complex & complex );

#ifndef CHOMP_LIBRARY_ONLY_
#include "algorithms/matrix/Sparse_Matrix.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "algorithms/matrix/Sparse_Matrix.cpp"
#endif

#endif
