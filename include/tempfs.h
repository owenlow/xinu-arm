/**
 * File: tempfs.h
 * Description: Ram-based filesystem for Xinu
 * Author: Owen Royall-Kahin
 */


#include <stdint.h>

typedef uint32_t size_t;

typedef struct fs_file {
    uint8_t *data;
    size_t size;
} fs_file_t;

int _fs_init();

/**
 * Get data with associated file name.
 * File is read from the FS, then saved in memory as a
 * block of data. Be sure to close this block when done.
 */

fs_file_t openFile( char * );

/**
 * Close the target file, otherwise it will linger in memory.
 */

void closeFile( fs_file_t );

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

void ls();

/**
 * cat: display the contents of the file
 */

void cat(char*);

/**
 * fileappend: append content to an existing file, for testing
 */

void fileappend(char*, char**, int);

/**
 * df: get filesystem statistics
 */

void df();

