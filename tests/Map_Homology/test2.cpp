/* Test program for Map_Homology
 * Shaun Harker 
 */

/* STD includes */
#include <iostream>

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
#include <boost/property_map/property_map.hpp>
#include <boost/graph/strong_components.hpp>
//#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
//#include <boost/graph/graph_archetypes.hpp>
//concept checking
#include <boost/graph/graph_concepts.hpp>
#include <boost/concept_check.hpp>


using namespace Adaptive_Cubical;

/* Forward Declarations */

void subdivide_toplex ( Toplex & my_toplex );
void inspect_toplex ( const Toplex & my_toplex );
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

/* * * * * * * * *
 * Main Program  *
 * * * * * * * * */


int main ( int argc, char * argv [] ) {
  using namespace Adaptive_Cubical;
  using namespace boost;
  
  /* Create Toplex */
  std::cout << "Creating toplex.\n";
  Geometric_Description bounds ( 2, Real ( -1.0 ), Real ( 1.0 ) );
  Toplex my_toplex ( bounds );
  for ( int i = 0; i < 3; ++ i ) subdivide_toplex ( my_toplex );
  
  /* Produce a directed graph */
  std::cout << "Creating directed graph.\n";
  DirectedGraph<Toplex> my_directed_graph = compute_directed_graph ( my_toplex, myMap () );
  
  /* Obtain an attractor */
  std::cout << "Aquiring attractor.\n";
  Toplex::Subset attractor = get_nontrivial_attractor ( my_directed_graph );
  
  /* Compute map homology on attractor */
  std::cout << "Computing map homology.\n";
  Map_Homology ( my_toplex, my_toplex, myMap () );
  
  return 0;
} /* main */

/* * * * * * * * * * * * * * * * * * 
 * Class and Function Definitions  *
 * * * * * * * * * * * * * * * * * */

void subdivide_toplex ( Toplex & my_toplex ) {
  /* subdivide every top cell */ /* only for [-1,1]^2 */
  Geometric_Description bounds ( 2, Real ( -1.0 ), Real ( 1.0 ) );
  Toplex_Subset my_subset = my_toplex . cover ( bounds );
  for ( Toplex_Subset::const_iterator it = my_subset . begin (); it != my_subset . end (); ++ it )
    my_toplex . subdivide ( my_toplex . find ( *it ) );
} /* subdivide_toplex */

void inspect_toplex ( const Toplex & my_toplex ) {
  std::cout << "Inspecting.\n";
  for ( Toplex::const_iterator it = my_toplex . begin (); it != my_toplex . end (); ++ it )
    std::cout << "top cell " << * it << " has geometry " << my_toplex . geometry ( it ) << "\n";
} /* inspect_toplex */

Toplex::Subset get_nontrivial_attractor ( const DirectedGraph < Toplex > & G ) {
  using namespace boost;
  std::cout << "The directed graph has " << num_vertices (G) << "nodes.\n"; 
  typedef DirectedGraph<Toplex> Graph;
  typedef graph_traits<Graph>::vertex_descriptor Vertex;
  /* scope */ 
  {
  graph_traits<Graph>::vertex_iterator a, b;
  for ( tie ( a, b ) = vertices (G); a != b; ++ a ) {
    std::cout << * a << " -> ";
    graph_traits<Graph>::out_edge_iterator c, d;
    for ( tie ( c, d ) = out_edges ( *a, G ); c != d; ++ c ) {
      std::cout << (*c).second << " ";
    } /* for */
    std::cout << "\n";
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
  
  PAC_VI_t pac_component;
  PAC_VI_t pac_discover_time;
  PAC_VV_t pac_root;
  PAC_VC_t pac_color;
  
  APM_VI_t component ( pac_component );
  APM_VI_t discover_time ( pac_discover_time );
  APM_VV_t root ( pac_root );
  APM_VC_t color ( pac_color );
  
  std::cout << "Computing strong_components.\n";
  int num = strong_components(G, component, 
                              root_map(root).
                              color_map(color).
                              discover_time_map(discover_time));
    
  std::cout << "Total number of components: " << num << std::endl;
  graph_traits<Graph>::vertex_iterator a, b;
  for ( tie ( a, b ) = vertices (G); 
       a != b; ++ a ) {
    std::cout << "Vertex " << * a
    << " is in component " << component[*a] << std::endl;
  }
  return Toplex::Subset ();
} /* get_nontrivial_attractor */
