/**
 * File: tempfs.h
 * Description: Ram-based filesystem for Xinu
 * Author: Owen Royall-Kahin
 */

#ifndef _TEMPFS_H_
#define _TEMPFS_H_

#include <stdint.h>

typedef uint32_t size_t;

typedef struct fs_file {
    uint8_t *data;
    uint32_t dt_index;
    size_t size;
} fs_file_t;

int _fs_init( void );

/**
 * Get data with associated file name.
 * File is read from the FS, then saved in memory as a
 * block of data. Be sure to close this block when done.
 */

fs_file_t * openFile( char * );

/**
 * Create a new file. Same as touch, but meant to be used from
 * an application. Returns fs_file_t of new file. Note that
 * data block is unitialized after calling this function, you must
 * allocate the data in the owning application.
 */

fs_file_t * newFile( char * );

/**
 * Close the target file and save changes, otherwise it will 
 * linger in memory.
 */

void closeFile( fs_file_t * );

/***** Filesystem functions *****/

/**
 * touch: create a file
 */

void touch( char* );

/**
 * rm: delete a file
 */

void rm( char* );


/**
 * ls: Print the contents of the current working directory
 */

void ls( void );

/**
 * cat: display the contents of the file
 */

void cat( char* );

/**
 * fileappend: append string to an existing file, for testing
 */

void fileappend( char*, char* );

/**
 * df: get filesystem statistics
 */

void df( void );

/**
 * stat: get file information
 */

void stat( char * filename );

#endif
