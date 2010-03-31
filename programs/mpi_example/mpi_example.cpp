#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#include <sstream>
#include <string>

//#include <sys/time.h>

#define CHOMP_HEADER_ONLY_
#include "Cubical_Complex.h"	/* for class Cubical_Complex */
#include "Vector_Complex.h"		/* for class Vector_Complex */
#include "Morse_Complex.h"      /* for Morse_Complex */
#include "Homology.h"			/* for function Homology(...)*/

#include "mpi.h"

#define WORKTAG 1
#define RETIRETAG 2
#define READYTAG 3

int master(int argc, char **argv);
int worker(void);

template < class T >
void print_my_vector ( std::vector<T> print_me ) {
	for ( unsigned int index = 0; index < print_me . size (); ++ index ) std::cout << print_me [ index ] << " "; }
	

template < class T >
T vector_prod ( const std::vector<T> multiply_my_elements_together ) {
	T return_value(1);
	for ( unsigned int index = 0; index < multiply_my_elements_together . size (); ++ index ) 
		return_value *= (multiply_my_elements_together [ index ] - 4); /* what the hell? */
	return return_value; } /* endfunction */
	
template < class T >
T vector_sum ( const std::vector<T> add_my_elements_together ) {
	T return_value(0);
	for ( unsigned int index = 0; index < add_my_elements_together . size (); ++ index ) 
		return_value += add_my_elements_together [ index ] ;
	return return_value; } /* endfunction */
	
void generate_random_cubical_complex ( Cubical_Complex & my_cubical_complex, std::vector < unsigned int > & dimension_sizes, float probability ) {
	my_cubical_complex . Allocate_Bitmap ( dimension_sizes );
	int max_count = (int) ( (float) vector_prod ( dimension_sizes) * probability );
	for ( int count = 0; count < max_count; ++ count ) {
		std::vector < unsigned int > cube_coordinates ( dimension_sizes . size () );
		for ( unsigned int dimension_index = 0; dimension_index < my_cubical_complex . dimension; ++ dimension_index ) 
			cube_coordinates [ dimension_index ] = 2 + rand() % (dimension_sizes [ dimension_index ] - 4);
		my_cubical_complex . Add_Full_Cube ( cube_coordinates ); } /* for */ } /* endfunction */

/* assume given a 'full' cubical complex, save it in simple (#, #, ... #) format */
void save_cubical_complex ( Cubical_Complex & my_cubical_complex, const char * file_name ) {
	std::ofstream output_file ( file_name ); 
	const unsigned int dimension = my_cubical_complex . dimension;
	const Cubical_Container & top_container = my_cubical_complex . cells [ dimension ];
	
	output_file << "(";
	for ( unsigned int dimension_index = 0; dimension_index < dimension; dimension_index ++ ) {
		output_file << top_container . dimension_sizes -> operator [] ( dimension_index ); 
		if ( dimension_index + 1 != dimension ) output_file << ", "; }
	output_file << ")\n";
	
	for ( Cubical_Container::const_iterator full_cube = top_container. begin () ; 
	full_cube != top_container . end (); ++ full_cube ) {
		unsigned long name = full_cube -> name >> dimension;
		output_file << "(";
		for ( unsigned int dimension_index = 0; dimension_index < dimension; dimension_index ++ ) {
			unsigned long current_dimension_size = top_container . dimension_sizes -> operator [] ( dimension_index );
			output_file << name % current_dimension_size;
			if ( dimension_index + 1 != dimension ) output_file << ", ";
			name = name / current_dimension_size; } /* for */
		output_file << ")\n"; } /* for */
	
	output_file . close (); } /* endfunction */
	
class compute_results {
public:
	unsigned long original_size;
	unsigned long morse_size;
	unsigned long homology_size;
	float total_time;
	float morse_time;
	float smith_time;
};

template < class Cell_Complex_Template >
compute_results compute_example ( Cell_Complex_Template & my_complex ) {
	clock_t start_clock, stop_clock, total_time_start, total_time_stop;
	float total_time, morse_time, homology_time;
	/* Create Morse Complex */
	total_time_start = clock ();
	std::cout << "  Creating Morse Complex from Original Complex ... \n";
	start_clock = clock ();
	Morse_Complex<Cell_Complex_Template> my_morse_complex ( my_complex ); 
	std::cout << " ... Morse Complex created: ";
	stop_clock = clock (); 
	std::cout << (float) ( stop_clock - start_clock ) / (float) CLOCKS_PER_SEC << " elapsed \n";
	morse_time = (float) ( stop_clock - start_clock ) / (float) CLOCKS_PER_SEC;
	
	/* Tell me interesting things about the Morse Complex */
	std::cout << "  Original Sizes (by increasing dimension): ";
	for ( unsigned int dim = 0; dim <= my_complex . dimension; dim ++ ) 
		std::cout << my_complex . cells [ dim ] . size () << " ";  
	std::cout << "\n";
	
	std::cout << "  Morse Sizes (by increasing dimension): ";
	for ( unsigned int dim = 0; dim <= my_morse_complex . dimension; dim ++ ) 
		std::cout << my_morse_complex . cells [ dim ] . size () << " "; 
	std::cout << "\n";
	

	/* Compute the homology */
	std::vector<int> Betti_Numbers, Minimal_Number_of_Generators;
	std::cout << "  Computing Homology ... \n";
	start_clock = clock ();
	Homology < Morse_Complex < Cell_Complex_Template > > ( Betti_Numbers, Minimal_Number_of_Generators, my_morse_complex );
	stop_clock = clock ();
	std::cout << "  Homology computed. "; 
	std::cout << (float) ( stop_clock - start_clock ) / (float) CLOCKS_PER_SEC << " elapsed \n";
	homology_time = (float) ( stop_clock - start_clock ) / (float) CLOCKS_PER_SEC;
	
	std::cout << "  Betti Numbers: "; print_my_vector ( Betti_Numbers ); 
	std::cout << "\n";

	total_time_stop = clock ();
	total_time = (float) ( total_time_stop - total_time_start) / (float) CLOCKS_PER_SEC ;
	std::cout << "  STATISTICS \n";
	std::cout << "  ---------- \n";

	std::cout << "  Total time: " << total_time << " seconds.\n";
	std::cout << "  Cells in original complex: " << my_complex . size () << "\n";
	std::cout << "  Cells in Morse complex: " << my_morse_complex . size () << "\n";
	std::cout << "  Homology size: " << (float)vector_sum(Minimal_Number_of_Generators) << "\n";
	std::cout << "  Compression factor = " << 100.0 - 100.0 * (float) my_morse_complex . size () / (float) my_complex . size () << "\%\n";
	std::cout << "  Morse / Homology = " << (float)my_morse_complex . size () / (float)vector_sum(Minimal_Number_of_Generators) << "\n";
	std::cout << "  Cells per second:" << (float) my_complex . size () / total_time << "\n"; 
	std::cout << "  Cells per second of Morse time:" << (float) my_complex . size () / morse_time << "\n"; 
	std::cout << "  Cells per second of Smith time:" << (float) my_morse_complex . size () / homology_time << "\n"; 
	std::cout << "  ----------\n";
	
	compute_results return_value;
	return_value . original_size =  my_complex . size ();
	return_value . morse_size =  my_morse_complex . size ();
	return_value . homology_size = vector_sum(Minimal_Number_of_Generators) ;
	return_value . total_time =  total_time;
	return_value . morse_time =  morse_time;
	return_value . smith_time =  homology_time;

    return return_value;

}

void manifold_example (  unsigned int dimension  ) {

	std::cout << "\n---- Manifold Example: T^d ----\n";
	/* Generate a Vector_Complex representation of S^1 */
	std::vector<unsigned int> sizes(2);
	sizes[0] = 3; sizes[1] = 3;
	std::vector<unsigned int> coordinates(2);
	coordinates[0] = 1; coordinates[1] = 1;
	Cubical_Complex my_cubical_complex;
	my_cubical_complex . Allocate_Bitmap ( sizes );
	my_cubical_complex . Add_Full_Cube ( coordinates );
	my_cubical_complex . Remove_Cell ( * my_cubical_complex . cells [ 2 ] . begin () );
	Vector_Complex my_vector_complex ( my_cubical_complex );

	/* Now S^1 is in my_vector_complex. */
	
	/* Compute on Tori */
	Vector_Complex Torus = my_vector_complex;
	for ( unsigned int dimension_index = 2; dimension_index <= dimension; dimension_index ++ ) {
		std::cout << "  Producing a " << dimension_index << "-torus. \n";
		Vector_Complex New_Torus; 
		New_Torus . Product_Complex < Vector_Complex, Vector_Complex > ( my_vector_complex, Torus );
		Torus = New_Torus; 
		New_Torus = Vector_Complex ();
		/* Give Information */
		std::cout << "  --- " << dimension_index << "-Torus ---\n";
		compute_example ( Torus );
	} /* for */

	return;
}



compute_results cubical_example (  int dimension, int width, float probability ) {
	
	/* Generate Random Cubical Complex */
	std::cout << "\n---- Random Complex of dimension " << dimension << ", width " << width << " ----\n";
	std::vector < unsigned int > dimension_sizes (  dimension, width );
	Cubical_Complex my_cubical_complex; 
	std::cout << "  Generating Random Cubical Complex... ";
	generate_random_cubical_complex ( my_cubical_complex, dimension_sizes, probability );
	std::cout << "  Random Cubical Complex Generated \n";
	
	/* Compute on it, return. */
    return compute_example ( my_cubical_complex );

}

void imperfect_product_example ( void ) {
	std::cout << "\n---- Imperfect Product Example ----\n";
	/* Generate Random Cubical Complex */
	unsigned int dimension = 4;
	unsigned int width = 15;
	float probability = .2;
	std::vector < unsigned int > dimension_sizes (  dimension, width );
	Cubical_Complex my_random_complex; 
	generate_random_cubical_complex ( my_random_complex, dimension_sizes, probability );
	
	/* Compute on it. */
    compute_example ( my_random_complex );

	/* Generate a Vector_Complex representation of S^1 */
	std::vector<unsigned int> sizes(2);
	sizes[0] = 3; sizes[1] = 3;
	std::vector<unsigned int> coordinates(2);
	coordinates[0] = 1; coordinates[1] = 1;
	Cubical_Complex my_cubical_complex;
	my_cubical_complex . Allocate_Bitmap ( sizes );
	my_cubical_complex . Add_Full_Cube ( coordinates );
	my_cubical_complex . Remove_Cell ( * my_cubical_complex . cells [ 2 ] . begin () );
	Vector_Complex my_vector_complex ( my_cubical_complex );
	
	Vector_Complex my_product_complex;
	my_product_complex . Product_Complex < Cubical_Complex, Vector_Complex > ( my_random_complex, my_vector_complex ) ; 
	
	compute_example ( my_product_complex );
	return;
}



int main(int argc, char **argv) {
	int myrank, numtasks; 
	
	/* Initialize MPI */
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	
	/* Find out my identity in the default communicator */
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	
	if (myrank == 0) {
		master(argc, argv); /* I am the master. */
	} else {
		worker();  /* I am a worker. */
	}
	
	/* Shut down MPI */
	
	MPI_Finalize();
	return 0;
}

int produce_message ( int * message, int job_number ) {
   if ( job_number < 200 ) {
		message [ 0 ] = 3;
		message [ 1 ] = 8 + 2 * job_number;
		message [ 2 ] = 20;
		message [ 3 ] = job_number; 
		return WORKTAG; 
	} /* if */
	return RETIRETAG;	
}

/***********
* master
*		rank 0 executes this, coordinates the workers
***********/

int master(int argc, char* argv[]) {
	/* DECLARATIONS */
	int number_of_workers;
	
	const int MAX_LENGTH = 1024;
	char result [ 1024 ];
	int message [ 4 ];
	int job_number = 0;
	
	MPI_Status status;
	
	/* Learn number of workers */
	//MPI_Comm_size( MPI_COMM_WORLD, & number_of_workers );
	number_of_workers = 0;
	
	std::ofstream myfile ( "/home/sharker/srh_chomp/programs/konstantin/output.m" );
	
	do {
		/* Receive results from a worker */
		
		MPI_Recv(result,           /* message buffer */
		MAX_LENGTH,                 /*length of result message*/
		MPI_CHAR,        /* of type double real */
		MPI_ANY_SOURCE,    /* receive from any sender */
		MPI_ANY_TAG,       /* any type of message */
		MPI_COMM_WORLD,    /* default communicator */
		&status);          /* info about the received message */
		
		/* Store output from worker */
		if ( status . MPI_TAG == WORKTAG) {
		  std::cout << result;
		  myfile << result;
		}
		if ( status . MPI_TAG == READYTAG) {
		  number_of_workers ++;
		}
			
		/* Produce new work message */
		int tag = produce_message( message, job_number ++ );
		
		/* Send the worker a new work unit */
		if ( tag == WORKTAG ) {
			MPI_Send(&message,             /* message buffer */
				4,                 /* one data item */
				MPI_INT,           /* data item is an integer */
				status.MPI_SOURCE, /* to who we just received from */
				WORKTAG,           /* user chosen message tag */
				MPI_COMM_WORLD);   /* default communicator */
		} /* if */
		
		if ( tag == RETIRETAG) {
			MPI_Send(0, 0, MPI_INT, status.MPI_SOURCE, RETIRETAG, MPI_COMM_WORLD);
			-- number_of_workers; 
		} /* if */
		
	} while ( number_of_workers > 0 );
	
	// close the file
	myfile.close();
	return 0; 
}


int worker(void) {
	MPI_Status status;
	int message[4];  // used for containing messages from master process

	/* send ready message */ 
        MPI_Send ( 0, 0, MPI_CHAR, 0, READYTAG, MPI_COMM_WORLD );

	while(1) {
		
		/* Receive a message from the master */
		MPI_Recv(message, 4, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		
		/* Check the tag of the received message: Retire? */
		if (status.MPI_TAG == RETIRETAG) return 0;
		
		/* Read Message */
		int dimension = message[0];
		int width = message[1];
		float probability = (float) message[2] / 100.0;
		int index = message[3];
		/* Do computation. */
		compute_results answer = cubical_example ( dimension, width, probability); 
		
		/* Format output */
		std::stringstream output;

		output << " data{" << dimension << "}(" << index + 1 << ", 1) = " << answer . original_size << ";\n"; 
		output << " data{" << dimension << "}(" << index + 1 << ", 2) = " << answer . morse_size << ";\n"; 
		output << " data{" << dimension << "}(" << index + 1 << ", 3) = " << answer . homology_size << ";\n";
		output << " data{" << dimension << "}(" << index + 1 << ", 4) = " << answer . total_time << ";\n"; 
		output << " data{" << dimension << "}(" << index + 1 << ", 5) = " << answer . morse_time << ";\n"; 
		output << " data{" << dimension << "}(" << index + 1 << ", 6) = " << answer . smith_time << ";\n"; 

		std::string my_string = output . str ();
	

		/* Send the result back */
		MPI_Send( const_cast<char *> (my_string . c_str ()) , my_string . size (), MPI_CHAR, 0, WORKTAG, MPI_COMM_WORLD);
		
		
	}
	
	/* Unreachable code. */
	
	MPI_Finalize();
	return 0;
}
