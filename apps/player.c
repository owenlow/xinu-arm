/**
 * player.c
 * Implementation for audio player application
 * Author: Owen Royall-Kahin
 */

#include <player.h>
#include "../system/platforms/raspberry-pi/audio.h"

void audio_test( int num ) {
    _audio_init();
    
    switch (num) {
        case 0:
            // test 0
            break;
            
        default:
            break;
    }
}

void audio_play( char * filename ) {
    _audio_init();
    
    // open, then play a file
}