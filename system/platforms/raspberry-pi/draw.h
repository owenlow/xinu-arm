/**
 * Header for draw.c
 * Author: Owen Royall-Kahin
 */

#ifndef _DRAW_H_
#define _DRAW_H_

#include <stdio.h>

#define BLACK   0x00
#define WHITE   0xFF

/**
 * Initializes the display module. Must be done before anything else,
 * or things will break.
 */
void _display_init();

/**
 * clearBuffer/clearBuffer2/clearBuffer3
 * Resets the display to black, clearBuffer2 takes 3 color parameters,
 * clearBuffer3 takes a single 16-bit color parameter
 */
void clearBuffer( void );
void clearBuffer2( uint8_t, uint8_t, uint8_t );
void clearBuffer3( uint16_t );

/**
 * getFbHeight/getFbWidth
 * Returns the framebuffer height and width, respectively.
 */
uint32_t getFbHeight();
uint32_t getFbWidth();

/**
 * setPixel family, the first sets white pixels, the second
 * takes 3 color parameters, and the third takes a single 16-bit
 * color parameter.
 */
void setPixel( int, int );
void setPixel2( int, int, uint8_t, uint8_t, uint8_t );
void setPixel3( int, int, uint16_t );

/**
 * drawLine family, takes start and end coordinates, and a color.
 * Default is white.
 */
void drawLine( int, int, int, int );
void drawLine2( int, int, int, int, uint8_t, uint8_t, uint8_t );
void drawLine3( int, int, int, int, uint16_t );

#endif

