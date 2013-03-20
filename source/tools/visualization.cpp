/*
 *  visualization.cpp
 *  
 *
 *  Created by Shaun Harker on 5/5/11.
 *  Copyright 2011. All rights reserved.
 *
 */

#include "tools/visualization.h"

GraphicsWindow::GraphicsWindow( const char * title ) {  
  unsigned long black,white;
  dis=XOpenDisplay(NULL);
  screen=DefaultScreen(dis);
  black=BlackPixel(dis,screen),
  white=WhitePixel(dis, screen);
  win=XCreateSimpleWindow(dis,DefaultRootWindow(dis),0,0, 
                          512, 512, 5,black, white);
  XSetStandardProperties(dis,win, title,"Hi",None,NULL,0,NULL);
  XSelectInput(dis, win, KeyPressMask);
  // get Graphics Context
  gc=XCreateGC(dis, win, 0,0);        
  XSetBackground(dis,gc,white);
  XSetForeground(dis,gc,black);
  XClearWindow(dis, win);
  XMapRaised(dis, win);
  // SRH BEGIN
  /* first, find the default visual for our screen. */
  Visual* default_visual = DefaultVisual(dis, DefaultScreen(dis));
  /* this creates a new color map. the number of color entries in this map */
  /* is determined by the number of colors supported on the given screen.  */
  Colormap my_colormap = XCreateColormap(dis,
                                         win,
                                         default_visual,
                                         AllocNone);
  palette . resize ( 256 );
  for ( int i = 0; i <  128 ; ++ i ) {
    XColor rgb_color;
    
    rgb_color.red = 0; 
    rgb_color.green = 0;
    rgb_color.blue = i * 512;
    //rgb_color.red = rand () % 65536; 
    //rgb_color.green = rand () % 65536; 
    //rgb_color.blue = rand () % 65536;
     /*Status rc = */XAllocColor(dis,
                            my_colormap,
                            &rgb_color);
    palette [ i ] = rgb_color . pixel;
  }
  
  for ( int i = 0; i < 128; ++ i ) {
    XColor rgb_color;
    rgb_color.red = i * 512; 
    rgb_color.green = 0;
    rgb_color.blue = 0;
    XAllocColor(dis, my_colormap, &rgb_color);
    palette [ i + 128 ] = rgb_color . pixel;
  } 
   
}

GraphicsWindow::~GraphicsWindow ( void ) {
  XFreeGC(dis, gc);
  XDestroyWindow(dis,win);
  XCloseDisplay(dis);     
}

void GraphicsWindow::clear ( void ) {
  XClearWindow(dis, win);
}
char GraphicsWindow::wait ( void ) {
  XFlush ( dis );
  XEvent event;
  while(1){
		XNextEvent(dis, &event);
    //std::cout << "event occurred.\n";
		switch(event.type){
      case KeyPress:
			
				char string[25];
				KeySym keysym;
				XLookupString(&event.xkey, string, 25, &keysym, NULL);
				return string [ 0 ];
        break;
      
		}
	}
}
void GraphicsWindow::line (int color, int x0, int y0, int x1, int y1) {
  XSetForeground(dis, gc, palette[color]);
  XDrawLine(dis, win, gc, x0, y0, x1, y1);
}

void GraphicsWindow::rect (int color, int x, int y, int w, int h) {
  XSetForeground(dis, gc, palette[color]);
  //std::cout << x << " " << y << " " << w << " " << h << "\n";
  XFillRectangle(dis, win, gc, x - 1, y - 1, w + 2, h + 2);
}
