/* 
 * 
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * 
 * Window API (main graphics display)
 * 
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 * 
 */

#ifndef __ESAT_WINDOW_H__
#define __ESAT_WINDOW_H__


namespace esat {

// Initializes the main graphical window
void WindowInit(unsigned int width, unsigned int height);

// This function must be called and the end of the frame, all graphical
// commands will be executed
void WindowFrame();

// Destroys the graphical window
void WindowDestroy();

// returns true if the window is opened, false if the window was closed
bool WindowIsOpened();

unsigned int WindowHeight();
unsigned int WindowWidth();

// must be declared by the user in order to use this library
int main(int, char **);

void WindowSetMouseVisibility(bool visible);

} //esat

#endif //__ESAT_WINDOW_H__
