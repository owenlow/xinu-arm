/**
 * flashtest.c
 * Implementation of flash device operation
 * Author: Owen Royall-Kahin
 */

#include <flashtest.h>
#include <flash.h>
#include <stdio.h>
#include <string.h>

struct dt_entry {
    char name[112];
    uint index;
    uint size;
    uint type;
    uint creation;
};

struct flash *flash;


void open_device(ushort dev) {
#ifdef NFLASH
    flash = &flashtab[dev];
    
    // Read first few bytes: if our filesystem is defined, call open_fs()
    uchar buffer[flash->log_size];
    
    if ( flashInit( flash->device ) == OK ) {
        
        flashOpen( flash->device, 0 );
        
        // Read the first block
        
        flashRead( flash->device, buffer, 0);
        
    }
    
    
    
    // Else, call initialize_fs()
#endif
}

void initialize_fs(device d) {
#ifdef NFLASH
    // Initialize a filesystem on the target device
    
    // Write boot record ( cluster size, fs size, dt idx, fat idx )
    
    
#endif
}

void open_fs() {
#ifdef NFLASH
    // Read boot record from opened device
    
#endif
}