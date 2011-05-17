/*
 *  visualization.h
 *  
 *
 *  Created by Shaun Harker on 5/5/11.
 *  Copyright 2011. All rights reserved.
 *
 */

#ifndef VISUALIZATION_H
#define VISUALIZATION_H 
#include <iostream>
#include <vector>
/* include the X library headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
// X11 has some foolish macros that conflict with us
#undef index
#undef Complex

#include "complexes/Subcomplex.h"



class GraphicsWindow {
public:
  GraphicsWindow( const char * title );
  ~GraphicsWindow ( void );

  void clear ( void );
  char wait ( void ); // return a keystroke
  void line ( int color, int x0, int y0, int x1, int y1 );
  void rect (int color, int x, int y, int w, int h);
  std::vector < int > palette;

protected:
  Display *dis;
  int screen;
  Window win;
  GC gc;
};
  
/// ComplexVisualization. A class for visualizing 2D complexes.
template < class Cell_Complex >
class ComplexVisualization : public GraphicsWindow {
public:
  ComplexVisualization ( const char * title );
  void drawNow ( void );
  void drawQuick ( void );

  void drawCell ( const Cell_Complex & complex, const typename Cell_Complex::Cell & cell, int color );
  void drawSpecialCell ( const Cell_Complex & complex, const typename Cell_Complex::Cell & cell, int color );
  void drawChain ( const Cell_Complex & complex, const typename Cell_Complex::Chain & chain, int color );
  void drawComplex ( const Cell_Complex & complex, int color );
  void drawRelativeComplex ( const Cell_Complex & complex, const Subcomplex<Cell_Complex> & subcomplex,
                             int color, int color2 );
  void explore ( void );
private:
  bool rescale_cell ( std::vector<float> & bounds );
  std::vector < std::pair < std::vector < float >, int > > cells_;
  std::vector < std::pair < std::vector < float >, int > > special_cells_;

  std::vector < float > bounds_;
};


/* ComplexVisualization */
template < class Cell_Complex >
ComplexVisualization<Cell_Complex>
::ComplexVisualization ( const char * title ) : GraphicsWindow ( title ) {
  bounds_ . resize ( 4 );
  bounds_ [ 0 ] = 0.0f;
  bounds_ [ 1 ] = 0.0f;
  bounds_ [ 2 ] = 1.0f;
  bounds_ [ 3 ] = 1.0f;
}


template < class Cell_Complex >
void ComplexVisualization<Cell_Complex>::drawNow ( void ) {
  float scale = 480.0f;
  float border = 16.0f;
  typedef std::pair < std::vector < float >, int > drawobject;
  clear ();
  BOOST_FOREACH ( drawobject object, cells_ ) {
    std::vector<float> bounds = object . first;
    if ( not rescale_cell ( bounds ) ) continue; // bounds is affected 
    GraphicsWindow::rect ( object . second, 
                          border + scale * ( bounds [ 0 ] - bounds [ 2 ] / 2.0f ),  
                          512.0f - border - scale * ( bounds [ 1 ] + bounds [ 3 ] / 2.0f ), 
                          scale * bounds [ 2 ], 
                          scale * bounds [ 3 ]);
  }
  BOOST_FOREACH ( drawobject object, special_cells_ ) {
    std::vector<float> bounds = object . first;
    if ( not rescale_cell ( bounds ) ) continue; // bounds is affected 
    GraphicsWindow::rect ( object . second, 
                          border + scale * ( bounds [ 0 ] - bounds [ 2 ] / 2.0f ),  
                          512.0f - border - scale * ( bounds [ 1 ] + bounds [ 3 ] / 2.0f ), 
                          scale * bounds [ 2 ], 
                          scale * bounds [ 3 ]);
  }
  XFlush ( dis );
}

template < class Cell_Complex >
void ComplexVisualization<Cell_Complex>::drawQuick ( void ) {
  float scale = 480.0f;
  float border = 16.0f;
  typedef std::pair < std::vector < float >, int > drawobject;
  clear ();
  int count = 0;
  for ( int i = 0; i < 10000; ++ i ) {
    ++ count;
    int j = rand () % cells_ . size ();
    drawobject object =  cells_ [j];
    std::vector<float> & bounds = object . first;
    if ( not rescale_cell ( bounds ) ) {
      -- i;
      if ( count == 100000 ) break;
      continue; // bounds is affected 
    }
    GraphicsWindow::rect ( object . second, 
                          border + scale * ( bounds [ 0 ] - bounds [ 2 ] / 2.0f ),  
                          512.0f - border - scale * ( bounds [ 1 ] + bounds [ 3 ] / 2.0f ), 
                          scale * bounds [ 2 ], 
                          scale * bounds [ 3 ]);
  }
  BOOST_FOREACH ( drawobject object, special_cells_ ) {
    std::vector<float> bounds = object . first;
    if ( not rescale_cell ( bounds ) ) continue; // bounds is affected 
    bounds [ 2 ] *= 2;
    bounds [ 3 ] *= 2;
    
    GraphicsWindow::rect ( object . second, 
                          border + scale * ( bounds [ 0 ] - bounds [ 2 ] / 2.0f ),  
                          512.0f - border - scale * ( bounds [ 1 ] + bounds [ 3 ] / 2.0f ), 
                          scale * bounds [ 2 ], 
                          scale * bounds [ 3 ]);
  }
  XFlush ( dis );
}

template < class Cell_Complex >
bool ComplexVisualization<Cell_Complex>::rescale_cell ( std::vector<float> & bounds ) {
  bounds [ 0 ] = ( bounds [ 0 ] - bounds_ [ 0 ] ) / ( bounds_ [ 2 ] - bounds_ [ 0 ] );
  bounds [ 1 ] = ( bounds [ 1 ] - bounds_ [ 1 ] ) / ( bounds_ [ 3 ] - bounds_ [ 1 ] );
  bounds [ 2 ] = ( bounds [ 2 ] ) / ( bounds_ [ 2 ] - bounds_ [ 0 ] );
  bounds [ 3 ] = ( bounds [ 3 ] ) / ( bounds_ [ 3 ] - bounds_ [ 1 ] );
  for ( int i = 0; i < 2; ++ i ) {
    if ( bounds [ i ] < 0.0f ) return false;
    if ( bounds [ i ] > 1.0f ) return false;
  }
  return true;
}

template < class Cell_Complex >
void ComplexVisualization<Cell_Complex>::drawCell ( const Cell_Complex & complex, const typename Cell_Complex::Cell & cell, int color ) {
  std::vector<float> bounds = complex . geometry ( cell );
  cells_ . push_back ( std::make_pair ( bounds, color ) );
}

template < class Cell_Complex >
void ComplexVisualization<Cell_Complex>::drawSpecialCell ( const Cell_Complex & complex, const typename Cell_Complex::Cell & cell, int color ) {
  std::vector<float> bounds = complex . geometry ( cell );
  special_cells_ . push_back ( std::make_pair ( bounds, color ) );
}

template < class Cell_Complex >
void ComplexVisualization<Cell_Complex>::drawChain ( const Cell_Complex & complex, const typename Cell_Complex::Chain & chain, int color ) {
  typedef const typename Cell_Complex::Chain::value_type Term;
  BOOST_FOREACH ( const Term & term, chain ) {
    drawSpecialCell ( complex, * term . first, color );
  }
}

template < class Cell_Complex >
void ComplexVisualization<Cell_Complex>::drawComplex ( const Cell_Complex & complex, int color ) {
  for ( typename Cell_Complex::const_iterator it = complex . begin (); it != complex . end (); ++ it ) {
    drawCell ( complex, *it, color );
  }
}

template < class Cell_Complex >
void ComplexVisualization<Cell_Complex>::drawRelativeComplex ( const Cell_Complex & complex, const Subcomplex<Cell_Complex> & subcomplex, int color, int color2 ) {
  for ( typename Cell_Complex::const_iterator it = complex . begin (); it != complex . end (); ++ it ) {
    drawCell ( complex, *it, color );
  }
  for ( typename Subcomplex<Cell_Complex>::const_iterator it = subcomplex . begin (); it != subcomplex . end (); ++ it ) {
    drawCell ( complex, * subcomplex . include ( it ), color2 );
  }
}

template < class Cell_Complex >
void ComplexVisualization<Cell_Complex>::explore ( void ) {
  typedef std::pair < std::vector < float >, int > drawobject;

  if ( not special_cells_ . empty () ) {
    bounds_ [ 0 ] = 1.0f;
    bounds_ [ 1 ] = 1.0f;
    bounds_ [ 2 ] = 0.0f;
    bounds_ [ 3 ] = 0.0f;
    BOOST_FOREACH ( drawobject object, special_cells_ ) {
      std::vector<float> bounds = object . first;
      bounds_ [ 0 ] = std::min ( bounds [ 0 ] - 2 * bounds [ 2 ], bounds_ [ 0 ] );
      bounds_ [ 1 ] = std::min ( bounds [ 1 ] - 2 * bounds [ 3 ], bounds_ [ 1 ] );
      bounds_ [ 2 ] = std::max ( bounds [ 0 ] + 2 * bounds [ 2 ], bounds_ [ 2 ] );
      bounds_ [ 3 ] = std::max ( bounds [ 1 ] + 2 * bounds [ 3 ], bounds_ [ 3 ] );
    }
    float scale = std::max ( bounds_ [ 2 ] - bounds_ [ 0 ],
                            bounds_ [ 3 ] - bounds_ [ 1 ] );
    float mid_x = (bounds_ [ 0 ] + bounds_ [ 2 ])/2.0f;
    float mid_y = (bounds_ [ 1 ] + bounds_ [ 3 ])/2.0f;
    bounds_ [ 0 ] = mid_x - scale;
    bounds_ [ 1 ] = mid_y - scale;
    bounds_ [ 2 ] = mid_x + scale;
    bounds_ [ 3 ] = mid_y + scale;
  }
  
  drawQuick ();
  while ( 1 ) {
    char c = wait ();
    float scale = bounds_ [ 2 ] - bounds_ [ 0 ];
    
    if ( c == 'q' ) return;
    if ( c == 'z' ) drawNow ();

    if ( c == '=' ) {
      bounds_ [ 0 ] -= scale / 3.0f;
      bounds_ [ 1 ] -= scale / 3.0f;
      bounds_ [ 2 ] += scale / 3.0f;
      bounds_ [ 3 ] += scale / 3.0f;
      drawQuick ();
    }
    if ( c == '-' ) {
      bounds_ [ 0 ] += scale / 3.0f;
      bounds_ [ 1 ] += scale / 3.0f;
      bounds_ [ 2 ] -= scale / 3.0f;
      bounds_ [ 3 ] -= scale / 3.0f;
      drawQuick ();
    }
    if ( c == 'w' ) {
      bounds_ [ 1 ] += scale / 3.0f;
      bounds_ [ 3 ] += scale / 3.0f;
      drawQuick ();
    }
    if ( c == 'a' ) {
      bounds_ [ 0 ] -= scale / 3.0f;
      bounds_ [ 2 ] -= scale / 3.0f;
      drawQuick ();
    }
    if ( c == 's' ) {
      bounds_ [ 1 ] -= scale / 3.0f;
      bounds_ [ 3 ] -= scale / 3.0f;
      drawQuick ();
    }   
    if ( c == 'd' ) {
      bounds_ [ 0 ] += scale / 3.0f;
      bounds_ [ 2 ] += scale / 3.0f;
      drawQuick ();
    }

  }
}
#endif
