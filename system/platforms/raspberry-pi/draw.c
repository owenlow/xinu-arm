/**
 * File: draw.c
 * Functions for writing to the display. Pretty poor performing.
 * enhancements.
 * Author: Owen Royall-Kahin
 */

#include "draw.h"
#include "mailbox.h"
#include <stdio.h>
#include <stdlib.h>


uint16_t getColor( uint8_t r, uint8_t g, uint8_t b ) {
    return (r << 11) | (g << 5) | b;
}

struct fb_info *fb;

void clearBuffer() {
    clearBuffer3( BLACK );
}

void clearBuffer2( uint8_t r, uint8_t g, uint8_t b ) {
    clearBuffer3( getColor(r, g, b) );
}

void clearBuffer3( uint16_t color ) {
    int i;
    uint16_t *addr = fb->fb_ptr;
    
    for (i = 0; i < (fb->height)*(fb->width); ++i) {
        if (addr[i] != color) {
            addr[i] = color;
        }
    }
}

uint32_t getFbHeight( void ) {
    return fb->height;
}

uint32_t getFbWidth( void ) {
    return fb->width;
}

void setPixel(int x, int y ) {
    setPixel3( x, y, WHITE );
}

void setPixel2(int x, int y, uint8_t r, uint8_t g, uint8_t b ) {
    setPixel3( x, y, getColor(r, g, b) );
}

void setPixel3( int x, int y, uint16_t color ) {
    ((uint16_t*)fb->fb_ptr)[x + ((fb->width) * y)] = color;
}

void drawLine( int x0, int y0, int x1, int y1 ) {
    drawLine3( x0, y0, x1, y1, WHITE );
}

void drawLine2( int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b ) {
    drawLine3( x0, y0, x1, y1, getColor(r, g, b) );
}

void drawLine3( int x0, int y0, int x1, int y1, uint16_t color ) {
    int dE, dNE, x, y, d;
    
    int steep = abs(y1-y0) > abs(x1-x0) ? 1 : 0;
    if (steep == 1) {
        swap(&x0, &y0);
        swap(&x1, &y1);
    }
    
    int dy = abs(y1 - y0);
    int dx = abs(x1 - x0);
    
    dE = 2 * dy;
    dNE = 2 * ( dy - dx );
    d = dE - dx; /* 2*dy – dx */
    
    int xinc = x0 < x1 ? 1 : -1;
    int yinc = y0 < y1 ? 1 : -1;
    
    for(x = x0, y = y0; x != x1+xinc; x += xinc) {
        if (steep == 1) {
            setPixel3( y, x, color );
        }
        else {
            setPixel3( x, y, color );
        }
        
        if( d <= 0 ) { /* choose E */
            d += dE;
        }
        else { /* choose NE */
            y += yinc;
            d += dNE;
        }
    }
}



static int initialized = 0;

void _display_init() {
    if (initialized != 1) {
        fb = fb_init();
        if (!fb) {
            printf("Error creating fb\n");
        }
        else {
            initialized = 1;
        }
    }
}

