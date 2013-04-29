/**
 * File: tempfs.c
 * Description: Ram-based filesystem for Xinu
 * Author: Owen Royall-Kahin
 */

#include <tempfs.h>
#include <memory.h>
#include <string.h>

/***** Private Definitions *****/



// Cluster size in bytes (should be multiple of 1024)
#define CL_SIZE 8192
#define CL_COUNT 6000

// FS size in bytes
#define FS_SIZE ( CL_SIZE * CL_COUNT )

// DT entry size is 128 bytes
typedef struct dt_entry {
    char name[112];
    uint32_t index; // index of the first cluster for the file
    uint32_t size; // size of the file
    uint32_t type; // 0x0000 = file, 0xFFFF = directory
    uint32_t creation; // creation date (unix epoch format)
} dt_entry_t;

/***** Globals *****/
uint32_t fs_index;
uint8_t *boot_record;
dt_entry_t *dir_table;
uint16_t *fat;
uint8_t *fs;

/****** Navigating the Filesystem *****/

int tempread( void*, size_t, size_t );
int tempwrite( const void*, size_t, size_t );
void tempseek( uint32_t );

/***** Begin Init *****/

int _fs_init() {
    fs_index = 0;
    
    if (!fs) {
        // data is the 'disk'
        fs = memget( FS_SIZE );
        
        boot_record = memget(4*sizeof(int));
        // configure boot record
        // [0] = cluster size
        // [1] = fs size
        // [2] = dt index
        // [3] = fat index
        boot_record[0] = CL_SIZE;
        boot_record[1] = FS_SIZE;
        boot_record[2] = 1;
        boot_record[3] = 2;
        
        printf("fs initialized\nsize: %d\ncluster size:%d\ncluster count:%d\n",
               FS_SIZE, CL_SIZE, CL_COUNT);
        
        fat = memget(CL_COUNT);
        // Clear FAT
        memset(fat, 0x0000, CL_COUNT);
        
        // First 3 clusters occupied (BR, DT, FAT)
        memset(fat, 0xFFFF, 3);
        
        // Write BR
        // Write DT
        // Write FAT
        
        
    }
    return 0;
}



int tempread( void* buffer, size_t size, size_t n ) {
    int i;
    
    for (i = 0; i < (n*size); ++i) {
        ((uint8_t*)buffer)[i] = fs[fs_index+i];
    }
    
    fs_index += i;
    return 0;
}

int tempwrite( const void* buffer, size_t size, size_t n ) {
    int i;
    
    for (i = 0; i < (n*size); ++i) {
        fs[fs_index+i] = ((uint8_t*)buffer)[i];
    }
    
    fs_index += i;
    return 0;
}

void tempseek( uint32_t offset ) {
    fs_index = offset;
}


