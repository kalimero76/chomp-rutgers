/*
 *  Vector_Complex.hpp
 *  
 *  Contains implementations of template functions
 *
 *  Created by Shaun Harker on 1/5/10.
 *
 */

template < class Cell_Complex_Template >
Vector_Complex::Vector_Complex ( Cell_Complex_Template & original_complex ) {
    Load_From_Cell_Complex ( original_complex ); 
}


template < class Cell_Complex_Template >
static void _translate_chain ( Vector_Complex::Chain & translated_chain, const typename Cell_Complex_Template::Chain & input_chain, std::vector<std::map<unsigned long, unsigned long> > & Tables ) {
	for ( typename Cell_Complex_Template::Chain::const_iterator term = input_chain . begin (); term != input_chain . end (); ++ term ) {
		Vector_Complex::Cell new_elementary_chain;
		new_elementary_chain . name = Tables [ term -> first . dimension ] [ term -> first . name ];
		new_elementary_chain . dimension = term -> first . dimension;
        translated_chain . insert ( Vector_Complex::Chain::value_type ( new_elementary_chain, term -> second ) );  
    } /* for */
    return; 
} /* endfunction */

template < class Cell_Complex_Template >
static void _Monotonic_Tables ( std::vector<std::map<unsigned long, unsigned long> > & Tables, Cell_Complex_Template & old_complex ) {
	/* Produce translation tables for each of the elementary chains in each of the Chain Bases. */
	for ( unsigned int dimension_index = 0; dimension_index <= old_complex . dimension; ++ dimension_index ) {
		std::set < unsigned long > Set_of_Names;
		for ( typename Cell_Complex_Template::Container::const_iterator element = old_complex . cells [ dimension_index ] . begin ();
        element != old_complex . cells [ dimension_index ] . end (); ++ element ) 
			Set_of_Names . insert ( element -> name );
		unsigned long index_to_assign = 0;
		for ( typename std::set<unsigned long>::const_iterator cell_name_iterator = Set_of_Names . begin ();
        cell_name_iterator != Set_of_Names . end (); ++ cell_name_iterator ) {
			Tables [ dimension_index ] [ * cell_name_iterator ] = index_to_assign ++ ; 
			//std::cout << " Table. " << * cell_name_iterator << " --> (" << index_to_assign - 1 << ", " << dimension_index << ")\n"; 
		} /* for */ 
	} /* for */
} /* endfunction */

template < class Cell_Complex_Template >
static void _Non_Monotonic_Tables ( std::vector<std::map<unsigned long, unsigned long> > & Tables, Cell_Complex_Template & old_complex ) {
	/* Produce translation tables for each of the elementary chains in each of the Chain Bases. */
	for ( unsigned int dimension_index = 0; dimension_index <= old_complex . dimension; ++ dimension_index ) {
		//std::cout << "Dimension = " << dimension_index << "\n";
		unsigned long index_to_assign = 0;
		for ( typename Cell_Complex_Template::Container::const_iterator element = old_complex . cells [ dimension_index ] . begin ();
        element != old_complex . cells [ dimension_index ] . end (); ++ element ) {
			Tables [ dimension_index ] [ element -> name ] = index_to_assign ++ ; 
			// std::cout << " Table. " << *element  << " --> (" << index_to_assign - 1 << ", " << dimension_index << ")\n"; 
		} /* for */
	} /* for */
} /* endfunction */


template < class Cell_Complex_Template >
void Vector_Complex::Load_From_Cell_Complex ( Cell_Complex_Template & old_complex ) {
	/* Determine actual dimension of old_complex. (Why? Because there may not be top cells. We want to save space.) */
	for ( unsigned int dimension_index = 0; dimension_index <= old_complex . dimension; ++ dimension_index ) 
		if ( old_complex . cells [ dimension_index ] . size () > 0 ) dimension = dimension_index; 
    
	/* First, clear everything and correctly size the Chain Groups */
	cells . clear ();
	cells . resize ( dimension + 1 );
	
	/* Produce translation tables for each of the elementary chains in each of the Chain Bases. */
	std::vector<std::map<unsigned long, unsigned long> > Tables ( dimension + 1 );
	_Non_Monotonic_Tables ( Tables, old_complex);
	
	/* Allocate space for vector complex chain groups */
	for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) {
        cells [ dimension_index ] . dimension = dimension_index;
		cells [ dimension_index ] . remembered_size = old_complex . cells [ dimension_index ] . size () ;
		cells [ dimension_index ] . resize ( cells [ dimension_index ] . remembered_size ); 
        cells [ dimension_index ] . begin_index = 0; 
    } /* for */
    
	/* Use the translation tables to copy all of the information into this complex. */
	for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) {
		for ( typename Cell_Complex_Template::Container::const_iterator element = old_complex . cells [ dimension_index ] . begin ();
        element != old_complex . cells [ dimension_index ] . end (); ++ element ) {
			unsigned long index_to_assign = Tables [ dimension_index ] [ element -> name ];
			typename Cell_Complex_Template::Chain original_boundary; 
			typename Cell_Complex_Template::Chain original_coboundary;
			old_complex . Boundary_Map ( original_boundary, element );
			old_complex . Coboundary_Map ( original_coboundary, element );
			_translate_chain<Cell_Complex_Template> ( cells [ dimension_index ] . boundary_data [ index_to_assign ], original_boundary, Tables );
			_translate_chain<Cell_Complex_Template> ( cells [ dimension_index ] . coboundary_data [ index_to_assign ], original_coboundary, Tables );
		} /* for */ 
    } /* for */	
return; 
} /* endfunction */


template < class First_Cell_Complex_Template, class Second_Cell_Complex_Template >
static void _Product_Tables ( std::map<std::pair<unsigned int, unsigned int>, std::map<std::pair<unsigned long, unsigned long>, unsigned long> > & Tables, const First_Cell_Complex_Template & first_complex, const Second_Cell_Complex_Template & second_complex ) {
	/* Produce translation tables for each of the elementary chains in each of the Chain Bases. */
	std::vector<unsigned long> index_to_assign ( first_complex . dimension + second_complex . dimension + 1, 0 );
	for ( unsigned int first_dimension_index = 0; first_dimension_index <= first_complex . dimension; ++ first_dimension_index ) 
        for ( unsigned int second_dimension_index = 0; second_dimension_index <= second_complex . dimension; ++ second_dimension_index ) {
            unsigned int dimension_index = first_dimension_index + second_dimension_index;
            for ( typename First_Cell_Complex_Template::Container::const_iterator first_element = first_complex . cells [ first_dimension_index ] . begin ();
            first_element != first_complex . cells [ first_dimension_index ] . end (); ++ first_element ) 
                for ( typename Second_Cell_Complex_Template::Container::const_iterator second_element = second_complex . cells [ second_dimension_index ] . begin ();
                second_element != second_complex . cells [ second_dimension_index ] . end (); ++ second_element ) {
                    //std::cout << "_pt: (" << *first_element << " " << *second_element << " , Assigning (" << index_to_assign [ dimension_index ] << ", " << first_dimension_index + second_dimension_index << ")\n";
                    //if ( index_to_assign [ dimension_index ] > 1000 ) exit(-1);
                    Tables [ std::pair<unsigned int, unsigned int> ( first_dimension_index, second_dimension_index ) ] [ std::pair<unsigned long, unsigned long> ( first_element -> name, second_element -> name ) ] = index_to_assign [ dimension_index ] ++;
                } /* for */
        } /* for */
    return; 
} /* endfunction */

template < class First_Cell_Complex_Template, class Second_Cell_Complex_Template >
static void _translate_chain_tensor_product ( Vector_Complex::Chain & translated_chain, const typename First_Cell_Complex_Template::Chain & first_chain, const typename Second_Cell_Complex_Template::Chain & second_chain, const std::map<std::pair<unsigned int, unsigned int>, std::map<std::pair<unsigned long, unsigned long>, unsigned long> > & Tables ) {
	if ( first_chain . empty () || second_chain . empty () ) return; 
	unsigned int first_dimension_index = first_chain . begin () -> first . dimension;
	unsigned int second_dimension_index = second_chain . begin () -> first . dimension;
	unsigned int dimension = first_dimension_index + second_dimension_index;
	for ( typename First_Cell_Complex_Template::Chain::const_iterator first_term = first_chain . begin ();
    first_term != first_chain . end (); ++ first_term ) 
        for ( typename Second_Cell_Complex_Template::Chain::const_iterator second_term = second_chain. begin ();
        second_term!= second_chain . end (); ++ second_term ) {
            unsigned long translated_name = Tables . find ( std::pair<unsigned int, unsigned int> ( first_dimension_index, second_dimension_index ) ) -> second . find ( std::pair<unsigned long, unsigned long> ( first_term -> first . name, second_term -> first . name ) ) -> second; 
            Vector_Complex::Cell translated_term ( translated_name, dimension );
            //if(translated_name > 1000 ) { std::cout << "_tctp1\n"; 
            //    std::cout << "first_chain = " << first_chain << "\n and " << "second_chain = " << second_chain << "\n";
            //    std::cout << "first_term = " << first_term -> first << "\n and " << "second_term = " << second_term -> first << "\n";
            //    std::cout << "first_di = " << first_dimension_index << "\n and " << "second_di = " << second_dimension_index << "\n";
            //    std::cout << "translated name = " << translated_name << " and " << " dimension = " << dimension << "\n";
            //exit(-1); }
            translated_chain +=  Vector_Complex::Chain::Chain_Term ( translated_term, first_term -> second * second_term -> second ); 
        } /* for */ 
    return; 
} /* endfunction */

template < class First_Cell_Complex_Template, class Second_Cell_Complex_Template >
static void _translate_chain_tensor_product ( Vector_Complex::Chain & translated_chain, const typename First_Cell_Complex_Template::Chain & first_chain, const typename Second_Cell_Complex_Template::Cell & second_element, const std::map<std::pair<unsigned int, unsigned int>, std::map<std::pair<unsigned long, unsigned long>, unsigned long> > & Tables ) {
	if ( first_chain . empty () ) return; 
	unsigned int first_dimension_index = first_chain . begin () -> first . dimension;
	unsigned int second_dimension_index = second_element . dimension;
	unsigned int dimension = first_dimension_index + second_dimension_index;
	for ( typename First_Cell_Complex_Template::Chain::const_iterator first_term = first_chain . begin ();
    first_term != first_chain . end (); ++ first_term ) {
		unsigned long translated_name = Tables . find ( std::pair<unsigned int, unsigned int> ( first_dimension_index, second_dimension_index ) ) -> second . find ( std::pair<unsigned long, unsigned long> ( first_term -> first . name, second_element . name ) ) -> second; 
		Vector_Complex::Cell translated_term ( translated_name, dimension );
        //if(translated_name > 1000 ) { std::cout << "_tctp2\n"; exit(-1); }
        translated_chain +=  Vector_Complex::Chain::Chain_Term ( translated_term, first_term -> second );
    } /* for */ 
    return; 
} /* endfunction */

template < class First_Cell_Complex_Template, class Second_Cell_Complex_Template >
static void _translate_chain_tensor_product ( Vector_Complex::Chain & translated_chain, const typename First_Cell_Complex_Template::Cell & first_element, const typename Second_Cell_Complex_Template::Chain & second_chain, const std::map<std::pair<unsigned int, unsigned int>, std::map<std::pair<unsigned long, unsigned long>, unsigned long> > & Tables ) {
	if ( second_chain . empty () ) return; 
	unsigned int first_dimension_index = first_element . dimension;
	unsigned int second_dimension_index = second_chain . begin () -> first . dimension;
	unsigned int dimension = first_dimension_index + second_dimension_index;
	for ( typename Second_Cell_Complex_Template::Chain::const_iterator second_term = second_chain. begin ();
    second_term!= second_chain . end (); ++ second_term ) {
		unsigned long translated_name = Tables . find ( std::pair<unsigned int, unsigned int> ( first_dimension_index, second_dimension_index ) ) -> second . find ( std::pair<unsigned long, unsigned long> ( first_element . name, second_term -> first . name ) ) -> second; 
		Vector_Complex::Cell translated_term ( translated_name, dimension );
        //if(translated_name > 1000 ) { std::cout << "_tctp3\n"; exit(-1); }
        translated_chain +=  Vector_Complex::Chain::Chain_Term ( translated_term, second_term -> second ); 
    } /* for */ 
    return; 
} /* endfunction */

template < class First_Cell_Complex_Template, class Second_Cell_Complex_Template >
void Vector_Complex::Product_Complex ( const First_Cell_Complex_Template & first_complex, const Second_Cell_Complex_Template & second_complex ) {
	/* First, clear everything and correctly size the Chain Groups */
	cells . clear ();
	cells . resize ( first_complex . dimension + second_complex . dimension + 1 );
	dimension = first_complex . dimension + second_complex . dimension; 
	
	/* Produce translation tables for each of the elementary chains in each of the Chain Bases. */
	std::map<std::pair<unsigned int, unsigned int>, std::map<std::pair<unsigned long, unsigned long>, unsigned long> >  Tables;
	_Product_Tables ( Tables, first_complex, second_complex);
    
	/* Determine sizes of Chain Groups */
	for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) 
		cells [ dimension_index ] . remembered_size = 0;
    
	for ( unsigned int first_dimension_index = 0; first_dimension_index <= first_complex . dimension; ++ first_dimension_index ) 
        for ( unsigned int second_dimension_index = 0; second_dimension_index <= second_complex . dimension; ++ second_dimension_index ) 
            cells [ first_dimension_index + second_dimension_index ] . remembered_size += 
			first_complex . cells [ first_dimension_index ] . size () * second_complex . cells [ second_dimension_index ] . size ();
	
	/* Allocate space for vector complex chain groups */
	for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) {
        cells [ dimension_index ] . dimension = dimension_index;
		cells [ dimension_index ] . resize ( cells [ dimension_index ] . remembered_size ); 
        cells [ dimension_index ] . begin_index = 0; 
    } /* for */
    
	/* Use the translation tables to copy all of the information into this complex. */
	for ( unsigned int first_dimension_index = 0; first_dimension_index <= first_complex . dimension; ++ first_dimension_index ) 
        for ( unsigned int second_dimension_index = 0; second_dimension_index <= second_complex . dimension; ++ second_dimension_index ) {
            unsigned int dimension_index = first_dimension_index + second_dimension_index;
            for ( typename First_Cell_Complex_Template::Container::const_iterator first_element = first_complex . cells [ first_dimension_index ] . begin ();
            first_element != first_complex . cells [ first_dimension_index ] . end (); ++ first_element ) 
                for ( typename Second_Cell_Complex_Template::Container::const_iterator second_element = second_complex . cells [ second_dimension_index ] . begin ();
                second_element != second_complex . cells [ second_dimension_index ] . end (); ++ second_element ) {
                    /* store first_element x second_element */
                    /* determine correct name for elementary chain first x second */
                    unsigned long index_to_assign = Tables [ std::pair<unsigned int, unsigned int> (first_dimension_index, second_dimension_index) ]
                    [ std::pair < unsigned long, unsigned long > ( first_element -> name, second_element -> name ) ];
                    Vector_Complex::Ring unit(1);
                    if ( first_element -> dimension & 1 ) unit = Vector_Complex::Ring(-1);
                    /* produce boundary via formula d(a x b) = da x b +- a x db */
                    typename First_Cell_Complex_Template::Chain first_original_boundary; 
                    first_complex . Boundary_Map ( first_original_boundary, first_element );
                    typename Second_Cell_Complex_Template::Chain second_original_boundary; 
                    second_complex . Boundary_Map ( second_original_boundary, second_element );
                    typename First_Cell_Complex_Template::Chain negative_first_element;
                    negative_first_element += typename First_Cell_Complex_Template::Chain::Chain_Term ( *first_element , unit );		
                    Chain & boundary_chain = cells [ dimension_index ] . boundary_data [ index_to_assign ];
                    //std::cout << " A \n";
                    _translate_chain_tensor_product <First_Cell_Complex_Template, Second_Cell_Complex_Template> ( boundary_chain, first_original_boundary, *second_element, Tables );
                    _translate_chain_tensor_product <First_Cell_Complex_Template, Second_Cell_Complex_Template> ( boundary_chain, negative_first_element, second_original_boundary, Tables );
                    /* produce coboundary via formula d(a x b) = da x b +- a x db */
                    typename First_Cell_Complex_Template::Chain first_original_coboundary; 
                    first_complex . Coboundary_Map ( first_original_coboundary, first_element );
                    typename Second_Cell_Complex_Template::Chain second_original_coboundary; 
                    second_complex . Coboundary_Map ( second_original_coboundary, second_element );
                    Chain & coboundary_chain = cells [ dimension_index ] . coboundary_data [ index_to_assign ];
                    //std::cout << " B : " << *first_element << " " << *second_element << "\n";

                    _translate_chain_tensor_product <First_Cell_Complex_Template, Second_Cell_Complex_Template> ( coboundary_chain, first_original_coboundary, *second_element, Tables );
                    _translate_chain_tensor_product <First_Cell_Complex_Template, Second_Cell_Complex_Template> ( coboundary_chain, negative_first_element, second_original_coboundary, Tables );
                } /* for */
        } /* for */
    return; 
} /* endfunction */
