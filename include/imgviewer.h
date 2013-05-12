/**
 * imgviewer.h
 * Author: Owen Royall-Kahin
 */

#ifndef _IMGVIEWER_H_
#define _IMGVIEWER_H_

/**
 * If the given filename exists, attempts to display a bitmap
 * from the file data.
 */
void display_image(char *);

/**
 * Display test driver function, options are:
 * 0: display white rectangle on black background
 * 1: display color test
 * 2: display orientation test
 * 3: display toucan
 */
void test_display(int );

#endif