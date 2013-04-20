/**
 * Implementation for display.h
 * Author: Owen Royall-Kahin
 */

#include <display.h>
#include <stdio.h>
#include <memory.h>
#include "../system/platforms/raspberry-pi/mailbox.h"
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;


typedef struct pixel {
    uint8_t r, g, b;
} pixel_t;

uint16_t getColor( pixel_t p ) {
    return (p.r << 11) | (p.g << 5) | p.b;
}

pixel_t *buffer;
int buffersize;
struct fb_info *fb;


/************** Critical Output Methods ***************/

void clear_buffer() {
    int i;
    for (i = 0; i != buffersize; ++i) {
        pixel_t p = {0, 0, 0};
        buffer[i] = p;
    }
}

void output_buffer() {
    int i;
    uint16_t *addr;
    
    addr = fb->fb_ptr;
    for (i = 0; i != buffersize; ++i) {
        *addr++ = getColor( buffer[i] );
    }
}

void setPixel(int x, int y, pixel_t pix) {
    buffer[(x * (fb->width)) + y] = pix;
}

/************** end output methods *******************/

/******** Testing methods *********/

void display_test(int test) {
    fb = fb_init();
    if (!fb) {
        printf("fb null\n");
        return;
    }
    if (!buffer) {
        buffersize = (fb->height) * (fb->width);
        buffer = memget( buffersize );
    }
    
    switch (test) {
        case 0:
            fbtest();
            break;
        case 1:
            display_test1();
            break;
        case 2:
            display_test2();
            break;
    }
    
    
}

void next_color(u8 *r, u8 *g, u8 *b)
{
    /*
     * http://www.controlbooth.com/attachments/lighting-electrics/8328d1355455375-color-mixing-seven-colors-make-white-rgb-color-wheel-lg.jpg
     * */
    /* red to yellow; increasing green */
    if (*r == 31 && *b == 0 && *g < 31) {
        (*g)++;
        /* yellow to green; decreasing red */
    } else if (*g == 31 && *b == 0 && *r > 0) {
        (*r)--;
        /* green to cyan; increasing blue */
    } else if (*r == 0 && *g == 31 && *b < 31) {
        (*b)++;
        /* cyan to blue; decreasing green */
    } else if (*r == 0 && *b == 31 && *g > 0) {
        (*g)--;
        /* blue to violet; increasing red */
    } else if (*g == 0 && *b == 31 && *r < 31) {
        (*r)++;
        /* violet to red; decreasing blue */
    } else if (*r == 31 && *g == 0 && *b > 0) {
        (*b)--;
    }
}

void fbtest() {
    
    u32 i, j;
    u16 color;
    u16* addr;
    
    u8 r0, g0, b0;
    u8 r, g, b;
    r0 = 31; g0 = 0; b0 = 0; /* begin with red */
    addr = (u16 *)fb->fb_ptr;
    for (j = 0; j < fb->height; j++) {
        
        r = r0; g = g0; b = b0;
        for (i = 0; i < fb->width; i++) {
            color = (r << 11) | (g << 5) | b;
            //printf("%d, %d, %x", j, i, addr);
            *addr++ = color;
            next_color(&r, &g, &b);
        }
        next_color(&r0, &g0, &b0);
    }
}

void display_test1() {
    clear_buffer();
    int x, y;
    pixel_t pix = { 255, 255, 255 };
    for (y = 100; y != 200; ++y) {
        for (x = 300; x != 600; ++x) {
            setPixel(x, y, pix);
        }
    }
    output_buffer();
}

void display_test2() {
    
    
    clear_buffer();
    
    drawLine( 100, 100, 100, 150 );  /* Vertical */
    drawLine( 100, 100, 100, 50 );   /* Vertical */
    
    drawLine( 100, 100, 50, 100 );   /* Horizontal */
    drawLine( 100, 100, 150, 100 );  /* Horizontal */
    
    drawLine( 100, 100, 150, 150 );  /* + diagonal */
    drawLine( 100, 100, 50, 50 );    /* + diagonal */
    
    drawLine( 100, 100, 50, 150 );   /* - diagonal */
    drawLine( 100, 100, 150, 50 );   /* - diagonal */
    
    drawLine( 100, 100, 150, 125 );  /* shallow + slope */
    drawLine( 100, 100, 50, 75 );    /* shallow + slope */
    
    drawLine( 100, 100, 150, 75 );   /* shallow - slope */
    drawLine( 100, 100, 50, 125 );   /* shallow - slope */
    
    drawLine( 100, 100, 125, 150 );  /* steep + slope */
    drawLine( 100, 100, 75, 50 );    /* steep + slope */
    
    drawLine( 100, 100, 125, 50 );   /* steep - slope */
    drawLine( 100, 100, 75, 150 );   /* steep - slope */
    
    output_buffer();
}

void drawLine (int x0, int y0, int x1, int y1)
{
    int dE, dNE, x, y, d;
    
    int steep = abs(y1-y0) > abs(x1-x0) ? 1 : 0;
    if (steep == 1) {
        //printf("line is steep %d\n", y1-y0);
        swap2(&x0, &y0);
        swap2(&x1, &y1);
    }
    
    pixel_t pix = { 255, 255, 255 };
    
    int dy = abs(y1 - y0);
    int dx = abs(x1 - x0);
    
    dE = 2 * dy;
    dNE = 2 * ( dy - dx );
    d = dE - dx; /* 2*dy – dx */
    
    
    int xinc = x0 < x1 ? 1 : -1;
    int yinc = y0 < y1 ? 1 : -1;
    
    //printf("loop about to start %d %d %d\n", xinc, yinc, steep);
    
    for(x = x0, y = y0; x != x1+xinc; x += xinc) {
        if (steep == 1) {
            setPixel( y, x, pix );
            //printf("x=%d y=%d d=%d dE=%d dNE=%d\n", y,x,d,dE,steep);
        }
        else {
            setPixel( x, y, pix );
            //printf("x=%d y=%d d=%d dE=%d dNE=%d\n", x,y,d,dE,steep);
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

void swap2(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}