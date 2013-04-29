/**
 * File: tempfs.h
 * Description: Ram-based filesystem for Xinu
 * Author: Owen Royall-Kahin
 */


#include <stdint.h>

typedef uint32_t size_t;

typedef struct file {
    uint8_t *data;
} file_t;

int _fs_init();

/**
 * Get data with associated file name.
 * File is read from the FS, then saved in memory as a
 * block of data. Be sure to close this block when done.
 */

file_t openFile( char * );

/**
 * Close the target file, otherwise it will linger in memory.
 */

void closeFile( file_t );


