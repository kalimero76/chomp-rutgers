/*
 *  Morse_Theory.h
 *  
 *
 *  Created by Shaun Harker on 4/28/10.
 *  Copyright 2010. All rights reserved.
 *
 */

template < class Cell_Complex>
Morse_Complex<Cell_Complex> Ace_King_Queen_Algorithm ( const Cell_Complex & cell_complex );

void Morse_Boundary_Algorithm ( void );
typename Cell_Complex::Chain & Chain_Correspondence_Algorithm ( typename Cell_Complex::Chain & canonical_chain, const Chain & morse_chain );


/* Morse Boundary Algorithm */
template < class Cell_Complex_Template >
void Morse_Complex<Cell_Complex_Template>::Morse_Boundary_Algorithm ( void ) {
	/* Convenient typedefs. */
	typedef typename Cell_Complex_Template::Container::const_iterator Original_const_iterator;
	typedef typename Cell_Complex_Template::Chain Original_Chain;
	typedef Chain Morse_Chain;
	typedef typename Morse_Traits<Cell_Complex_Template>::morse_value_type Morse_Value_Type;
  
	typedef typename Morse_Value_Chain < Morse_Complex > :: iterator Work_Chain_iterator;
  
	/* We make a reference to this, so the semantics more clearly reflect when we are talking about the Morse Complex.
	 * This should not result in slowdowns, because the compiler knows the reference never changes and can bind member accesses at
	 * compile time. */
	Morse_Complex & morse_complex = *this;
	/* First we allocate the empty chain groups. */
	dimension = original_complex . dimension;
	cells . resize ( dimension + 1 );
  
	/* Now we loop through the original complex and insert all of the Aces into the Morse Complex. */
	for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index )
		for ( typename Cell_Complex_Template::Container::const_iterator element = original_complex . cells [ dimension_index ] . begin ();
         element != original_complex . cells [ dimension_index ] . end (); ++ element )
			if ( Is_an_Ace ( Flags ( element ) ) ) morse_complex . cells [ dimension_index ] . insert ( *element ) ;
  
	int old_percent = 0;
	int number_computed = 0;
	/* Now we loop through the Aces and find their Morse Boundaries. */
	for ( unsigned int dimension_index = 0; dimension_index <= dimension; ++ dimension_index ) {
		for ( iterator element = morse_complex . cells [ dimension_index ] . begin ();
         element != morse_complex . cells [ dimension_index ] . end (); ++ element ) {
      
			int percent = ( number_computed * 100 ) / morse_complex . size ();
			if ( percent > old_percent ) {
				std::cout << "\r" << number_computed << " " << morse_complex . size () << " (" << percent << ") ";
				std::flush(std::cout);
      old_percent = percent; } /* if */
			number_computed ++;
      
			/* We determine the Morse Boundary of "element". */
			//std::cout << "Finding the Morse Boundary of " << *element << "\n";
			/* Initialize Answer Chain. */
			Morse_Chain  answer_chain;
			/* Find original boundary */
			Original_Chain original_boundary;
			original_complex . Boundary_Map ( original_boundary, *element );
			/* Initialize Morse Value Chain.
			 * This is where we place the original boundaries into the work chain to
			 * seed the process. We make sure to only put in Living Queens; Kings and Aces
			 * we will always deal with immediately. */
			Morse_Value_Chain < Morse_Complex > work_chain;
			/* Notice the annoying part in what follows where we have to search the complex because
			 * the chain doesn't store locations in the complex directly. OPTIMIZE THIS. */
			for ( typename Original_Chain::iterator chain_term_iterator = original_boundary . begin();
           chain_term_iterator != original_boundary . end (); ++ chain_term_iterator ) {
				typename Cell_Complex_Template::Container::const_iterator location =
        original_complex . find ( chain_term_iterator -> first );
				unsigned char flags = Flags ( location );
				if ( Is_an_Ace ( flags ) ) {
					answer_chain . insert ( *chain_term_iterator ); // or += would work
					//std::cout << "Removing an Ace! " << chain_term_iterator -> first << "\n";
					//std::cout << "   Answer Chain is now " << answer_chain << "\n";
        continue; }
				if ( Is_a_King ( flags ) || not Is_Alive ( flags ) ) continue;
				work_chain . insert ( typename Morse_Value_Chain < Morse_Complex >::value_type
                             ( Morse_Value_Cell < Morse_Complex > ( Morse_Value ( location ), location ),
                              chain_term_iterator -> second ) ); 
			} /* for */
      
      
			/* We proceed as follows. We use a morse value chain to store the current 'work terms'
			 * The point of this data structure is that it stores the highest morse valued items at the beginning.
			 * We use 'begin' to grab a term and then process it.
			 * Processing a term consists of removing it from the
			 * work structure, and then one of the four options:
			 *   If it is an Ace, put it in the answer chain.
			 *   If it is a King, do nothing and continue on.
			 *   If it is a Queen, check to see if she is alive.
			 *      If she is not alive, do nothing and continue on.
			 *      If she is alive, insert into the work list all of her spouses boundaries other than her (her King's mistresses), with coefficients
			 *        obtained via multiplying the coefficient of the Q term by <Q, dK> <dK, M>. */
      //std::cout << "Initial boundary chain processed. We proceed with " << work_chain << "\n";
      while ( not work_chain . empty () ) {
				/* Pull off a term from "work_chain". */
				Work_Chain_iterator current_work_term = work_chain . begin ();
        
				/* By construction, this term is a Living Queen. */
				Original_const_iterator queen = current_work_term -> first . location;
				Original_Chain king_boundaries;
				original_complex . Boundary_Map ( king_boundaries, Husband ( queen ) );
        
				/* Calculate the factor that we need to multiply the chain by. */
				typename Original_Chain::iterator queen_term = king_boundaries . find ( *queen );
				Ring factor = - current_work_term -> second / queen_term -> second; /* As this must be invertible. */
        
				/* Multiply the chain terms by the correct factor. */
				king_boundaries *= factor;
        
				/* Loop through new terms and deal with Aces, Kings, and Dead Queens */
				Original_Chain kept_boundaries;
				for ( typename Original_Chain::const_iterator chain_term_iterator = king_boundaries . begin ();
             chain_term_iterator != king_boundaries . end (); ++ chain_term_iterator ) {
					Original_const_iterator location = original_complex . find ( chain_term_iterator -> first );
					unsigned char flags = Flags ( location );
					if ( Is_an_Ace ( flags ) ) {
						answer_chain += *chain_term_iterator;
          continue; } /* if */
					if ( Is_a_Queen ( flags ) && Is_Alive ( flags ) )
						kept_boundaries += *chain_term_iterator; 
				} /* for */
				/* The kept_boundaries are the living Queens of the king_boundaries. */
        
				/* Add the remaining king boundary terms into the work_chain. The original queen will be cancelled. */
				work_chain += Morse_Value_Chain < Morse_Complex > ( kept_boundaries, morse_complex );
        
				//std::cout << "Current Work Chain = " << work_chain << "\n";
			} /* while */
			/* Now we have the answer chain; let's look at it! */
			//std::cout << "Answer = " << answer_chain << "\n";
			morse_complex . Boundary_Map ( element ) = answer_chain; 
		} /* for */ 
	} /* for */
} /* Morse_Complex<>::Morse_Boundary_Algorithm */

template < class Cell_Complex >
typename Cell_Complex::Chain & Morse_Complex<Cell_Complex>::Chain_Correspondence_Algorithm ( typename Cell_Complex::Chain & canonical_chain, const Morse_Complex::Chain & morse_chain ) {
	typedef typename Morse_Value_Chain < Morse_Complex > :: iterator Work_Chain_iterator;
	/* First we include the chain into the original complex */
	canonical_chain = morse_chain; /* this'll break someday */
	typename Cell_Complex::Chain boundary_chain; 
	original_complex . Boundary_Map ( boundary_chain, canonical_chain );
	/* We use the special 'Morse_Value_Chain' for the upcoming algorithm */
	Morse_Value_Chain < Morse_Complex > work_chain ( boundary_chain, *this );
	/* Now we loop until the boundary of "canonical_chain" is canonical. 
	 * This is acheived by throwing out non-queen terms of "work_chain", and
	 * eliminating queen terms by adding kings to "canonical_chain". */
	while ( not work_chain . empty () ) {
		Work_Chain_iterator term = work_chain . begin ();
		typename Cell_Complex::const_iterator cell_iterator = term -> first . location;
		if ( not Is_a_Queen ( Flags ( cell_iterator ) ) ) {
			work_chain . erase ( term );
			continue;
		} /* if */
		typename Cell_Complex::const_iterator king_iterator = Husband ( cell_iterator );
		typename Cell_Complex::Chain king_boundaries;
		original_complex . Boundary_Map ( king_boundaries, king_iterator);
    
		/* Calculate the factor that we need to multiply the king by. */
		typename Cell_Complex::Chain::iterator queen_term = king_boundaries . find ( *cell_iterator );
		Ring factor = - term -> second / queen_term -> second;
		/* Hence canonical_chain + factor * king does not possess this queen term. */
		canonical_chain [ *king_iterator ] = factor;
		/* Multiply the chain terms by the correct factor. */
		king_boundaries *= factor;
		/* Add the remaining king boundary terms into the work_chain. The original queen will be cancelled. */
		work_chain += Morse_Value_Chain < Morse_Complex > ( king_boundaries, *this );
	} /* while */
	return canonical_chain;
} /* Morse_Complex<>::Chain_Correspondence_Algorithm */
