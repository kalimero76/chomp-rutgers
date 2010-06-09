/*
 *  Product_Complex.h
 *  
 *  Shaun Harker
 *  3/31/10.
 *
 */

#ifndef CHOMP_PRODUCT_COMPLEX_
#define CHOMP_PRODUCT_COMPLEX_

#include <vector>
#include <map>
#include "boost/functional/hash.hpp"

#include "archetypes/Chain_Archetype.h" /* for Default_Cell, Default_Chain */

/**************************************************************************
 *                           PRODUCT COMPLEX                              *
 **************************************************************************/

/* Forward Declarations */
template < class, class > class Product_Cell;
template < class, class > class Product_Chain;
template < class, class > class Product_Complex;
template < class, class > class Product_const_iterator;


/* * * * * * * * * * * *
 * class Product_Cell  *
 * * * * * * * * * * * */

template < class First_Cell_Complex, class Second_Cell_Complex >
std::ostream & operator << ( std::ostream & output_stream, const Product_Cell <First_Cell_Complex, Second_Cell_Complex> & print_me );

template < class First_Cell_Complex_Template, class Second_Cell_Complex_Template >
class Product_Cell {
public:
  typedef First_Cell_Complex_Template First_Cell_Complex;
  typedef Second_Cell_Complex_Template Second_Cell_Complex;
  typedef Product_Complex<First_Cell_Complex, Second_Cell_Complex> complex_type;
  Product_Cell ( void );
  Product_Cell ( const typename First_Cell_Complex::Cell & first_cell, 
                const typename Second_Cell_Complex::Cell & second_cell );
	bool operator < ( const Product_Cell & right_hand_side ) const;
	bool operator == ( const Product_Cell & right_hand_side ) const;
	bool operator != ( const Product_Cell & right_hand_side ) const;
  unsigned int dimension ( void ) const;
private:
  friend std::ostream & operator << <>( std::ostream & output_stream, const Product_Cell & print_me );
  friend class Product_Complex < First_Cell_Complex, Second_Cell_Complex >;
  typename First_Cell_Complex::Cell first_;
  typename Second_Cell_Complex::Cell second_;
  unsigned int dimension_;
};

/* * * * * * * * * * * *
 * class Product_Chain *
 * * * * * * * * * * * */

template < class First_Cell_Complex, class Second_Cell_Complex >
class Product_Chain : public Chain_Archetype < std::map < Product_const_iterator < First_Cell_Complex, Second_Cell_Complex >, typename First_Cell_Complex::Ring > > {};

/* * * * * * * * * * * * *
 * class Product_Complex *
 * * * * * * * * * * * * */

template < class First_Cell_Complex_Template, class Second_Cell_Complex_Template >
class Product_Complex {
public:
	/* typedefs */	 
  typedef First_Cell_Complex_Template First_Cell_Complex;
  typedef Second_Cell_Complex_Template Second_Cell_Complex;
  typedef unsigned int size_type; // for > 4E9 cubes, use unsigned long
	typedef Product_Chain<First_Cell_Complex, Second_Cell_Complex> Chain;
	typedef Product_Cell<First_Cell_Complex, Second_Cell_Complex> Cell;
	typedef Default_Ring Ring;
	typedef Cell key_type;
	typedef Cell value_type;
  typedef Product_const_iterator<First_Cell_Complex, Second_Cell_Complex> const_iterator;
  typedef const_iterator iterator;
  /* Basic Container */
  void clear ( void ) const {} // does not contain any information to clear
  iterator find ( const key_type & find_me ) const;
  iterator begin ( void ) const;
  iterator end ( void ) const;
  size_type size ( void ) const;
  /* Cell Complex */
  iterator begin ( unsigned int dimension ) const;
  iterator end ( unsigned int dimension ) const;
  size_type size ( unsigned int dimension ) const;
  Chain boundary ( const const_iterator & input ) const;
  Chain coboundary ( const const_iterator & input ) const;
  unsigned int dimension ( void ) const;
  /* Index Complex */
  void index ( void );
  size_type index_begin ( unsigned int dimension ) const;
  size_type index_end ( unsigned int dimension ) const;
  size_type index ( const const_iterator & lookup ) const;
  size_type & index ( const const_iterator & lookup );
  std::vector < const_iterator > & lookup ( void );
  const const_iterator & lookup ( size_type index ) const;
  const_iterator & lookup ( size_type index );
  std::vector < int > count_all_boundaries ( void ) const;
  void boundary ( std::vector < size_type > & output, const size_type index ) const;
  void coboundary ( std::vector < size_type > & output, const size_type index ) const;
  void boundary ( std::vector < std::pair <size_type, Ring > > & output, const size_type input ) const;
  void coboundary ( std::vector < std::pair <size_type, Ring > > & output, const size_type input ) const;
  /* Decomposable Complex */
  void decompose ( void );
  char type ( size_type index, unsigned int dimension ) const;
  size_type mate ( size_type queen_index, unsigned int dimension ) const;
  const Ring & connection ( size_type queen_index ) const;
  Ring & connection ( size_type queen_index );  
  size_type ace_begin ( unsigned int dimension ) const;
  size_type ace_end ( unsigned int dimension ) const;
  /* Product Complex */
  Product_Complex ( const First_Cell_Complex & first_factor, const Second_Cell_Complex & second_factor );
  typename First_Cell_Complex::Chain projectFirst ( const Chain & project_me );
  typename Second_Cell_Complex::Chain projectSecond ( const Chain & project_me );

private:
  friend class Product_const_iterator<First_Cell_Complex, Second_Cell_Complex>;
  /* Cell Complex */
  size_type total_size_;
  std::vector<const_iterator> begin_;
  const_iterator end_;
  std::vector<size_type> size_;
  unsigned int dimension_; 
  /* Index Complex */
  std::map < const_iterator, size_type > index_;
  std::vector < const_iterator > lookup_;
  std::vector < size_type > index_begin_;
  /* Decomposable Complex */
  std::vector<Ring> connection_;
  std::vector< int > boundary_count_;
  bool boundary_count_available_;
  std::vector<size_type> king_count_;
  /* Product Complex */
  const First_Cell_Complex & first_factor;
  const Second_Cell_Complex & second_factor;
  typename Chain::Chain_Term tensor_product ( const typename First_Cell_Complex::const_iterator & first_iter, 
                                              const typename Second_Cell_Complex::const_iterator & second_iter ) const;
  Chain tensor_product ( const typename First_Cell_Complex::Chain & first_chain, 
                         const typename Second_Cell_Complex::const_iterator & second_iter ) const;
  Chain tensor_product ( const typename First_Cell_Complex::const_iterator & first_iter, 
                         const typename Second_Cell_Complex::Chain & second_chain ) const;
};

/* * * * * * * * * * * * * *
 * Product_const_iterator  *
 * * * * * * * * * * * * * */

template < class First_Cell_Complex, class Second_Cell_Complex >
std::ostream & operator << ( std::ostream & output_stream, const Product_const_iterator < First_Cell_Complex, Second_Cell_Complex > & print_me );

template < class First_Cell_Complex, class Second_Cell_Complex >
size_t hash_value ( const Product_const_iterator 
                   < First_Cell_Complex, Second_Cell_Complex > & hash_me );

template < class First_Cell_Complex_Template, class Second_Cell_Complex_Template >
class Product_const_iterator {
public:
  typedef First_Cell_Complex_Template First_Cell_Complex;
  typedef Second_Cell_Complex_Template Second_Cell_Complex;
  typedef Product_Complex<First_Cell_Complex, Second_Cell_Complex> complex_type;
  typedef Product_Complex < First_Cell_Complex, Second_Cell_Complex > Complex;
  typedef Product_Cell<First_Cell_Complex, Second_Cell_Complex> Cell;
	typedef Cell key_type;
	typedef Cell value_type;
  Product_const_iterator ( void );
  Product_const_iterator ( const Complex * const container );
  Product_const_iterator ( const Complex * const container, 
                  const typename First_Cell_Complex::const_iterator & first,
                  const typename Second_Cell_Complex::const_iterator & second );
  bool operator != ( const Product_const_iterator & right_hand_side ) const; 
  bool operator == ( const Product_const_iterator & right_hand_side ) const;      
  bool operator < ( const Product_const_iterator & right_hand_side ) const;
  typename Complex::value_type operator * ( void ) const; 
  Product_const_iterator & operator ++ ( void );
  unsigned int dimension ( void ) const;
  const Complex & container ( void ) const;
private:
  friend class Product_Complex < First_Cell_Complex, Second_Cell_Complex >;
  friend std::ostream & operator << <> ( std::ostream & output_stream, const Product_const_iterator < First_Cell_Complex, Second_Cell_Complex > & print_me );

  friend size_t hash_value <> ( const Product_const_iterator 
                            < First_Cell_Complex, Second_Cell_Complex > & hash_me );
  const Complex * container_;
  typename First_Cell_Complex::const_iterator first_;
  typename Second_Cell_Complex::const_iterator second_;
  unsigned int dimension_;
};

template < class First_Cell_Complex, class Second_Cell_Complex >
size_t hash_value ( const Product_const_iterator 
                   < First_Cell_Complex, Second_Cell_Complex > & hash_me ) {
  std::size_t seed = 0;
  boost::hash_combine(seed, hash_me . first_);
  boost::hash_combine(seed, hash_me . second_);
  return seed;
} /* hash_value */

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Product_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Product_Complex.cpp"
#endif

#endif