/* 
 * 
 * Copyright 2014 ESAT. All rights reserved.
 * Author: Jose L. Hidalgo <jlhidalgo@esat.es>
 * 
 * Drawing commands
 * 
 * Engine updated from 2025 forward by:
 *   Ivan Sancho <isancho@esat.es>
 * 
 */

#ifndef __ESAT_DRAW_H__
#define __ESAT_DRAW_H__

namespace esat {

//Call this before start sending drawing commands:
void DrawBegin();

//Call this after all drawing is done:
void DrawEnd();

void DrawSetStrokeColor(const unsigned char R,
                        const unsigned char G, 
                        const unsigned char B,
                        const unsigned char Alpha = 255);

void DrawSetFillColor(const unsigned char R,
                      const unsigned char G,
                      const unsigned char B,
                      const unsigned char Alpha = 255);

void DrawClear(const unsigned char R,
               const unsigned char G,
               const unsigned char B,
               const unsigned char Alpha = 255);

void DrawLine(const float x1, const float y1,
              const float x2, const float y2);

//Draw a polygone without filling it. The user must repeat the
//first point at the end:
void DrawPath(float *pairs_of_points, int num_points);

//Draw a polygone and fill it. The user must not repeat the
//first point at the end:
void DrawSolidPath(float *pairs_of_points, int num_points, 
                   bool stroke = true);

void DrawSetTextFont(const char *name);
void DrawSetTextSize(float size);
void DrawSetTextBlur(float blur_radius);
void WriteText(float x, float y, const char *text);

} //esat

#endif //__ESAT_DRAW_H__
