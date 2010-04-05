/*
 *  Product_Complex.h
 *  
 *  Shaun Harker
 *  3/31/10.
 *
 */

#ifndef CHOMP_PRODUCT_COMPLEX_
#define CHOMP_PRODUCT_COMPLEX_

#include <set> /* for map */
#include "archetypes/Chain_Archetype.h" /* for Default_Cell, Default_Chain */
#include "archetypes/Cell_Complex_Archetype.h" /* for class Cell_Complex_Archetype */

/**************************************************************************
 *                           PRODUCT COMPLEX                              *
 **************************************************************************/

/* Forward Declarations */
template < class, class > class Product_Cell;
template < class, class > class Product_Chain;
template < class, class > class Product_Container;
template < class, class > class Product_Complex;

template < class First_Cell_Complex, class Second_Cell_Complex >
std::ostream & operator << ( std::ostream & output_stream, const Product_Cell <First_Cell_Complex, Second_Cell_Complex> & print_me );

/* * * * * * * * * * * *
 * class Product_Cell  *
 * * * * * * * * * * * */

template < class First_Cell_Complex_Template, class Second_Cell_Complex_Template >
class Product_Cell {
  typedef First_Cell_Complex_Template First_Cell_Complex;
  typedef Second_Cell_Complex_Template Second_Cell_Complex;
  friend class Product_Container < First_Cell_Complex, Second_Cell_Complex >;
  friend class Product_Complex < First_Cell_Complex, Second_Cell_Complex >;
public:
  typename First_Cell_Complex::Cell first;
  typename Second_Cell_Complex::Cell second;
  unsigned int dimension;
  Product_Cell ( void );
  Product_Cell ( const typename First_Cell_Complex::Cell & first_cell, 
                const typename Second_Cell_Complex::Cell & second_cell );
	bool operator < ( const Product_Cell & right_hand_side ) const;
	bool operator == ( const Product_Cell & right_hand_side ) const;
	bool operator != ( const Product_Cell & right_hand_side ) const;
	friend std::ostream & operator << <>( std::ostream & output_stream, const Product_Cell & print_me );
};

/* * * * * * * * * * * *
 * class Product_Chain *
 * * * * * * * * * * * */

template < class First_Cell_Complex, class Second_Cell_Complex >
class Product_Chain : public Chain_Archetype < std::map < Product_Cell < First_Cell_Complex, Second_Cell_Complex >, typename First_Cell_Complex::Ring > > {};

/* * * * * * * * * * * * * *
 * class Product_Container *
 * * * * * * * * * * * * * */

template < class First_Cell_Complex_Template, class Second_Cell_Complex_Template >
class Product_Container {
public:
	/* typedefs */	 
  typedef First_Cell_Complex_Template First_Cell_Complex;
  typedef Second_Cell_Complex_Template Second_Cell_Complex;
	typedef Product_Chain < First_Cell_Complex, Second_Cell_Complex> Chain;
  typedef typename Chain::Cell Cell;
	typedef typename Chain::Ring Ring;
  
  /* Simple Associative Container types */
	typedef Cell key_type;
	typedef Cell value_type;
  typedef unsigned long size_type;

  /* Forward Declarations */
  class const_iterator;
  //class iterator;
  friend class const_iterator;
  //friend class iterator;
  
  /* Member Functions */
  Product_Container ( const First_Cell_Complex & first_factor, const First_Cell_Complex & second_factor );
  size_type size ( void ) const;
	const_iterator find ( const Cell & find_me ) const;
	const_iterator begin ( void ) const;
  const_iterator end ( void ) const;
	//iterator find ( const Cell & find_me );
	//iterator begin ( void );
  //iterator end ( void );
  
  /* Nested Classes */
  
  /* * * * * * * * * * * * * * * * * * *
   * Product_Container::const_iterator *
   * * * * * * * * * * * * * * * * * * */
  
	class const_iterator {
    const Product_Container * referral;
    mutable Product_Cell<First_Cell_Complex, Second_Cell_Complex> dereference_value;
  public:
    typename First_Cell_Complex::const_iterator first;
    typename Second_Cell_Complex::const_iterator second;
		const_iterator ( void );
    const_iterator ( const Product_Container * const referral );
    const_iterator ( const Product_Container * const referral, 
                    const typename First_Cell_Complex::const_iterator & first,
                    const typename Second_Cell_Complex::const_iterator & second );
    //const_iterator ( const iterator & convert_me );
		bool operator != ( const const_iterator & right_hand_side ) const; 
    bool operator == ( const const_iterator & right_hand_side ) const;        
		const typename Product_Container::value_type & operator * ( void ) const; 
		const typename Product_Container::value_type * operator -> ( void ) const;    
    const_iterator & operator ++ ( void );
  };
  
private:
  friend class Product_Complex < First_Cell_Complex_Template, Second_Cell_Complex_Template > ;
  /* Member Variables */
  unsigned int dimension;
  size_type remembered_size;
  const_iterator remember_begin;
  const_iterator remember_end;
  const First_Cell_Complex * first_factor;
  const Second_Cell_Complex * second_factor;
};

/* * * * * * * * * * * * * 
 * class Product_Complex *
 * * * * * * * * * * * * */

template < class First_Cell_Complex_Template, class Second_Cell_Complex_Template >
class Product_Complex : public Cell_Complex_Archetype < Product_Container < First_Cell_Complex_Template, Second_Cell_Complex_Template > > {
public:
  /* typedefs */
  typedef First_Cell_Complex_Template First_Cell_Complex;
  typedef Second_Cell_Complex_Template Second_Cell_Complex;
  typedef Product_Container < First_Cell_Complex, Second_Cell_Complex > Container;
  typedef Product_Chain < First_Cell_Complex, Second_Cell_Complex > Chain;
  typedef Product_Cell < First_Cell_Complex, Second_Cell_Complex >  Cell;
	typedef typename Container::Ring Ring;
  typedef typename Container::const_iterator const_iterator;
  //typedef typename Container::iterator iterator;
  /* Member variables */
  using Cell_Complex_Archetype < Container > :: cells;
  using Cell_Complex_Archetype < Container > :: dimension;
  /* Constructor */
  Product_Complex ( const First_Cell_Complex & first_factor, const Second_Cell_Complex & second_factor );

  using Cell_Complex_Archetype < Container > :: Boundary_Map;
  using Cell_Complex_Archetype < Container > :: Coboundary_Map;
	/* * * * * * * * * * * * * * * * * * *
   * Pure Virtual Functions Overrides  *
   * * * * * * * * * * * * * * * * * * */
	/** Returns a copy of the Boundary information. This is only a copy, so subsequently altering this chain does not alter the complex directly. */
	virtual Chain & Boundary_Map ( Chain &, const const_iterator & ) const;
	/** Returns a copy of the Boundary information. This is only a copy, so subsequently altering this chain does not alter the complex directly. */
	virtual Chain & Coboundary_Map ( Chain &, const const_iterator & ) const;
  
private:
  const First_Cell_Complex & first_factor;
  const Second_Cell_Complex & second_factor;
  typename Chain::Chain_Term tensor_product ( const typename First_Cell_Complex::Cell & first_cell, const typename Second_Cell_Complex::Cell & second_cell ) const;
  Chain tensor_product ( const typename First_Cell_Complex::Chain & first_chain, const typename Second_Cell_Complex::Cell & second_cell ) const;
  Chain tensor_product ( const typename First_Cell_Complex::Cell & first_cell, const typename Second_Cell_Complex::Chain & second_chain ) const;

};

#ifndef CHOMP_LIBRARY_ONLY_
#include "complexes/Product_Complex.hpp"
#endif

#ifdef CHOMP_HEADER_ONLY_
#include "complexes/Product_Complex.cpp"
#endif

#endif