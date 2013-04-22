/**
 * File: display.c
 * Functions for writing to the display. Should be updated for performance
 * enhancements.
 * Author: Owen Royall-Kahin
 */

#include "display.h"
#include "mailbox.h"

uint16_t getColor( pixel_t p ) {
    return (p.r << 11) | (p.g << 5) | p.b;
}

uint16_t getColor2( uint8_t r, uint8_t g, uint8_t b ) {
    return (r << 11) | (g << 5) | b;
}

struct fb_info *fb;

#define BLACK 0

void clear_buffer() {
    int i;
    uint16_t *addr = fb->fb_ptr;
    
    for (i = 0; i != (fb->height)*(fb->width); ++i) {
        if (addr[i] != BLACK) {
            addr[i] = BLACK;
        }
    }
}


void setPixel(int x, int y, pixel_t pix ) {
    ((uint16_t*)fb->fb_ptr)[(x * (fb->width)) + y] = getColor(pix);
}


void setPixel2(int x, int y, uint8_t r, uint8_t g, uint8_t b ) {
    ((uint16_t*)fb->fb_ptr)[(x * (fb->width)) + y] = getColor2(r, g, b);
}

void display_init() {
    fb = fb_init();
    if (!fb) {
        printf("Error creating fb\n");
    }
}

