/**
 * lights.c
 * Code for testing the light functionality
 * Author: Owen Royall-Kahin
 */

#include <lights.h>

#include <thread.h>
#include "../system/platforms/raspberry-pi/gpio.h"


void light_set(int state) {
    switch (state) {
        case ON:
            printf("light is on\n");
            GPIOCLR(16);
            break;
        case OFF:
            printf("light is off\n");
            GPIOSET(16);
            break;
    }
}

