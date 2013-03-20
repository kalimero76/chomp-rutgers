/* Test program for Map_Homology
 * Shaun Harker 
 */

/* STD includes */
#include <iostream>
#include <ctime>

#define CHOMP_HEADER_ONLY_
#define TEST_PROGRAM

/* CHOMP includes */
#include "toplexes/Adaptive_Cubical_Toplex.h"
#include "algorithms/Homology.h"			/* for function Homology(...)*/
#include "complexes/Adaptive_Complex.h"
#include "algorithms/Morse_Theory.h"

/* CMPD includes */
#include "data_structures/Directed_Graph.h"

/* BOOST includes */
/* Interval */
#define __USE_ISOC99
#include <boost/numeric/interval.hpp>
/* Graph */
#include <boost/property_map/property_map.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/concept_check.hpp>
#include <boost/graph/create_condensation_graph.hpp>

using namespace Adaptive_Cubical;

/* Forward Declarations */

void subdivide_toplex ( Toplex & my_toplex );
void inspect_toplex ( const Toplex & my_toplex );
void draw_ascii_subset ( const Toplex & my_toplex, const Toplex::Subset & my_subset );
Toplex::Subset get_nontrivial_attractor ( const DirectedGraph < Toplex > & my_directed_graph );

/* The Map */

struct myMap {
  Geometric_Description operator () ( const Geometric_Description & rectangle ) const {
    Geometric_Description return_value ( 2 );
    return_value . lower_bounds [ 0 ] = - rectangle . upper_bounds [ 0 ];
    return_value . upper_bounds [ 0 ] = - rectangle . lower_bounds [ 0 ];
    return_value . lower_bounds [ 1 ] =   rectangle . lower_bounds [ 1 ];
    return_value . upper_bounds [ 1 ] =   rectangle . upper_bounds [ 1 ];
    return rectangle; //return_value;
  }
};
 
struct LeslieMap {

  //using namespace boost;
  //using namespace numeric;
  typedef boost::numeric::interval<double, boost::numeric::interval_lib::
  policies<
  boost::numeric::interval_lib::save_state<
  boost::numeric::interval_lib::rounded_transc_opp<double> >,
  boost::numeric::interval_lib::checking_base<double> > > interval;
  
  interval parameter1, parameter2;
  LeslieMap ( int b1, int b2 ) {
    parameter1 = interval (8.0 + ( 37.0 - 8.0 ) / 50.0 * (double) b1,
                           8.0 + ( 37.0 - 8.0 ) / 50.0 * (double) ( b1 + 1 ) );
    parameter2 = interval (3.0 + ( 50.0 - 3.0 ) / 50.0 * (double) b2,
                           3.0 + ( 50.0 - 3.0 ) / 50.0 * (double) ( b2 + 1 ) ); 
    std::cout << "P1 = " << parameter1 . lower () << ", " << parameter1 . upper () << 
    "\n and P2 = " << parameter2 . lower () << ", " << parameter2 . upper () << 
    "\n";
  }
  
  Geometric_Description operator () ( const Geometric_Description & rectangle ) const {    
    /* Read input */
    interval x0 = interval (rectangle . lower_bounds [ 0 ], 
                           rectangle . upper_bounds [ 0 ]);
    interval x1 = interval (rectangle . lower_bounds [ 1 ], 
                            rectangle . upper_bounds [ 1 ]);
    
    /* Perform map computation */
    interval y0 = (parameter1 * x0 + parameter2 * x1 ) * 
                  exp ( (double) -0.1 * (x0 + x1) );
    interval y1 = (double) 0.7 * x0;
    
    /* Write output */
    Geometric_Description return_value ( 2 );
    return_value . lower_bounds [ 0 ] = y0 . lower ();
    return_value . upper_bounds [ 0 ] = y0 . upper ();
    return_value . lower_bounds [ 1 ] = y1 . lower ();
    return_value . upper_bounds [ 1 ] = y1 . upper ();
    return return_value;
  } /* MapPopModel::compute */

};

/* * * * * * * * *
 * Main Program  *
 * * * * * * * * */


int main ( int argc, char * argv [] ) {
  using namespace Adaptive_Cubical;
  using namespace boost;
  
  /* Create Toplex */
  std::cout << "Creating toplex.\n";
  Geometric_Description bounds ( 2, Real ( 0 ), Real ( 1.0 ) );
  bounds . upper_bounds [ 0 ] = 320;
  bounds . upper_bounds [ 1 ] = 224;
  clock_t start, stop;
  Toplex my_toplex ( bounds );
  for ( int i = 0; i < 5; ++ i ) subdivide_toplex ( my_toplex );
  
  /* Produce a directed graph */
  std::cout << "Creating directed graph.\n";
  LeslieMap f( 40, 8 );
  
  for ( int i = 0; i < 4; ++ i ) {
    std::cout << "toplex size = " << my_toplex . size () << "\n";
    
    if ( i % 7 == 6 ) {
      std::cout << "Coarsening. \n";
      my_toplex . coarsen ();
      std::cout << "coarsened size = " << my_toplex . size () << "\n";
    }
    
    std::cout << "subdividing...\n";
    subdivide_toplex ( my_toplex );
    std::cout << "subdivided size = " << my_toplex . size () << "\n";

    std::cout << "Computing directed graph...\n";
    start = clock ();
    DirectedGraph<Toplex> my_directed_graph = compute_directed_graph ( my_toplex, f );
    stop = clock ();
    std::cout << "elasped time: " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";

    /* Obtain an attractor */
    std::cout << "Aquiring attractor...\n";
    Toplex::Subset attractor = get_nontrivial_attractor ( my_directed_graph );
  
    std::cout << "attractor size = " << attractor . size ();
    
    /* Examine attractor */
    for (Toplex::Subset::const_iterator it = attractor . begin (); 
         it != attractor . end (); ++ it ) {
      //std::cout << my_toplex . geometry ( my_toplex . find ( * it ) ) << "\n";
    }

    draw_ascii_subset ( my_toplex, attractor );
  
    /* Erase top cells not in attractor sets */
    std::cout << "Erasing cells that aren't in attractor sets...\n";
    start = clock ();
    Toplex::const_iterator it = my_toplex . begin ();
    while ( it != my_toplex . end () ) {
      Toplex::const_iterator next = it;
      ++ next;
      if ( attractor . find ( *it ) == attractor . end () ) 
        my_toplex . erase ( it );
      //std::cout << "size = " << my_toplex . size ();
      it = next;
    } /* while */
    stop = clock ();
    std::cout << "elasped time: " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";

  }
  
  std::cout << "Size of toplex = " << my_toplex . size () << "\n";
  std::cout << "Coarsening. \n";
  my_toplex . coarsen ();
  subdivide_toplex ( my_toplex );
  std::cout << "Size of toplex = " << my_toplex . size () << "\n";
  /* Compute map homology on attractor */
  std::cout << "Computing map homology.\n";
  Map_Homology ( my_toplex, my_toplex, myMap () );
  
  return 0;
} /* main */

/* * * * * * * * * * * * * * * * * * 
 * Class and Function Definitions  *
 * * * * * * * * * * * * * * * * * */

void subdivide_toplex ( Toplex & my_toplex ) {
  /* subdivide every top cell */ 
  Toplex::const_iterator it = my_toplex . begin ();
  while ( it != my_toplex . end () ) {
    Toplex::const_iterator next = it;
    ++ next;
    my_toplex . subdivide ( it );
    it = next;
  } /* while */
  
} /* subdivide_toplex */

void inspect_toplex ( const Toplex & my_toplex ) {
  std::cout << "Inspecting.\n";
  for ( Toplex::const_iterator it = my_toplex . begin (); it != my_toplex . end (); ++ it )
    std::cout << "top cell " << * it << " has geometry " << my_toplex . geometry ( it ) << "\n";
} /* inspect_toplex */

void draw_ascii_subset ( const Toplex & my_toplex, const Toplex::Subset & my_subset ) {
  int Width = 128;
  int Height = 128;
  int i_max = 0;
  int j_max = 0;
  Geometric_Description bounds = my_toplex . bounds ();
  std::vector < std::vector < bool > > data ( Height + 1 );
  for ( int j = 0; j <= Height; ++ j ) data [ j ] . resize ( Width + 1, false );
  for (Toplex::Subset::const_iterator it = my_subset . begin (); 
       it != my_subset . end (); ++ it ) {
    Geometric_Description box = my_toplex . geometry ( my_toplex . find ( *it ) );
    int i_left = ( box . lower_bounds [ 0 ] - bounds . lower_bounds [ 0 ] ) /
    ( bounds . upper_bounds [ 0 ] - bounds . lower_bounds [ 0 ] ) * (float) Width;
    int j_left = ( box . lower_bounds [ 1 ] - bounds . lower_bounds [ 1 ] ) /
    ( bounds . upper_bounds [ 1 ] - bounds . lower_bounds [ 1 ] ) * (float) Height;
    int i_right = ( box . upper_bounds [ 0 ] - bounds . lower_bounds [ 0 ] ) /
    ( bounds . upper_bounds [ 0 ] - bounds . lower_bounds [ 0 ] ) * (float) Width;
    int j_right = ( box . upper_bounds [ 1 ] - bounds . lower_bounds [ 1 ] ) /
    ( bounds . upper_bounds [ 1 ] - bounds . lower_bounds [ 1 ] ) * (float) Height;
    
    i_max = std::max ( i_right, i_max );
    j_max = std::max ( j_right, j_max );
    
    for ( int i = i_left; i < i_right; ++ i )
      for ( int j = j_left; j < j_right; ++ j ) 
        data [ j ] [ i ] = true;
  }
  /*
  for ( int j = Height; j >= 0; -- j ) {
    for ( int i = 0; i <= Width; ++ i ) {
      if ( data [ j ] [ i ] ) std::cout << "#";
      else std::cout << ".";
    }
    std::cout << "\n";
  } 
  */
  j_max += 5;
  i_max += 5;
  data . clear ();
  data . resize ( Height + 1);
  for ( int j = 0; j <= Height; ++ j ) data [ j ] . resize ( Width + 1, false );
  for (Toplex::Subset::const_iterator it = my_subset . begin (); 
       it != my_subset . end (); ++ it ) {
    Geometric_Description box = my_toplex . geometry ( my_toplex . find ( *it ) );
    int i_left = ( box . lower_bounds [ 0 ] - bounds . lower_bounds [ 0 ] ) /
    ( bounds . upper_bounds [ 0 ] - bounds . lower_bounds [ 0 ] ) * (float) Width
    * ((float) Width / (float) i_max );
    int j_left = ( box . lower_bounds [ 1 ] - bounds . lower_bounds [ 1 ] ) /
    ( bounds . upper_bounds [ 1 ] - bounds . lower_bounds [ 1 ] ) * (float) Height
    * ( (float) Height / (float) j_max );
    int i_right = ( box . upper_bounds [ 0 ] - bounds . lower_bounds [ 0 ] ) /
    ( bounds . upper_bounds [ 0 ] - bounds . lower_bounds [ 0 ] ) * (float) Width
    * ((float) Width / (float) i_max );
    int j_right = ( box . upper_bounds [ 1 ] - bounds . lower_bounds [ 1 ] ) /
    ( bounds . upper_bounds [ 1 ] - bounds . lower_bounds [ 1 ] ) * (float) Height
    * ( (float) Height / (float) j_max );
    
    for ( int i = i_left; i < i_right; ++ i )
      for ( int j = j_left; j < j_right; ++ j ) { 
        if ( j > Height || i > Width ) {
          std::cout << " i = " << i << " and j = " << j << "\n";
        } else {
          data [ j ] [ i ] = true;
        }
      }
  }
  for ( int j = Height; j >= 0; -- j ) {
    for ( int i = 0; i <= Width; ++ i ) {
      if ( data [ j ] [ i ] ) std::cout << "#";
      else std::cout << ".";
    } /* for */
    std::cout << "\n";
  } /* for */
} /* draw_ascii_subset */

Toplex::Subset get_nontrivial_attractor ( const DirectedGraph < Toplex > & G ) {
  using namespace boost;
  std::cout << "The directed graph has " << num_vertices (G) << "nodes.\n"; 
  typedef DirectedGraph<Toplex> Graph;
  typedef graph_traits<Graph>::vertex_descriptor Vertex;
  /* scope */ 
  {
  graph_traits<Graph>::vertex_iterator a, b;
  for ( tie ( a, b ) = vertices (G); a != b; ++ a ) {
    //std::cout << * a << " -> ";
    graph_traits<Graph>::out_edge_iterator c, d;
    for ( tie ( c, d ) = out_edges ( *a, G ); c != d; ++ c ) {
      //std::cout << (*c).second << " ";
    } /* for */
    //std::cout << "\n";
  } /* for */
  } /* scope */
  /* Pair Associative Container -- Vertex to Vertex or Integer or Color */
  typedef std::map< Vertex, Vertex > PAC_VV_t; 
  typedef std::map< Vertex, int > PAC_VI_t; 
  typedef std::map< Vertex, default_color_type > PAC_VC_t; 
  /* Associative Property Map -- Vertex to Integer or Color */
  typedef boost::associative_property_map< PAC_VV_t > APM_VV_t;
  typedef boost::associative_property_map< PAC_VI_t > APM_VI_t;
  typedef boost::associative_property_map< PAC_VC_t > APM_VC_t;
  
  clock_t start, stop;
  
  PAC_VI_t pac_component;
  PAC_VI_t pac_discover_time;
  PAC_VV_t pac_root;
  PAC_VC_t pac_color;
  
  APM_VI_t component_number ( pac_component );
  APM_VI_t discover_time ( pac_discover_time );
  APM_VV_t root ( pac_root );
  APM_VC_t color ( pac_color );
  
  std::cout << "Computing strong_components...\n";
  start = clock ();
  int num_scc = strong_components(G, component_number, 
                              root_map(root).
                              color_map(color).
                              discover_time_map(discover_time));
  stop = clock ();
  std::cout << "elasped time: " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";
  
  std::cout << "Total number of components: " << num_scc << std::endl;
  
  /*
  graph_traits<Graph>::vertex_iterator a, b;
  for ( tie ( a, b ) = vertices (G); a != b; ++ a ) {
    std::cout << "Vertex " << * a
    << " is in component " << component_number[*a] << std::endl;
  }
  */
  

  std::vector < std::vector < Vertex > > components;
  std::cout << "Building component list...\n";

  start = clock ();
  build_component_lists(G, num_scc, component_number, components);
  stop = clock ();
  std::cout << "elasped time: " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";

  typedef adjacency_list<vecS, vecS, directedS> CG_t;
  CG_t cg;
  
  std::cout << "Creating condensation graph...\n";
  start = clock ();
  create_condensation_graph(G,
                            components,
                            component_number,
                            cg);
  stop = clock ();
  std::cout << "elasped time: " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";

  //print_graph ( cg );

  Toplex::Subset subset;

  std::cout << "Collecting attractor cells...\n";
  start = clock ();
  graph_traits<CG_t>::vertex_iterator a, b;
  for ( tie ( a, b ) = 
       vertices ( cg ); a != b; ++ a ) {
    //std::cout << "checking " << *a << "\n";
    
    if ( out_degree ( *a, cg ) == 0 ) {
      std::cout << "attractor " << *a << ".\n";
      for (unsigned int v_index = 0; 
           v_index < components [ *a ] . size ();
           ++ v_index ) {
        subset . insert ( components [ *a ] [ v_index ] );
        //std::cout << "inserting Top_Cell " <<  components [ *a ] [ v_index ] << "\n";
        
      } /* for */
    }
  }
  stop = clock ();
  std::cout << "elasped time: " << (float) ( stop - start ) / (float) CLOCKS_PER_SEC << "\n";

  return subset;
} /* get_nontrivial_attractor */
