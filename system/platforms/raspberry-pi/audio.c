/**
 * audio.c
 * Author: Owen Royall-Kahin
 * Implementation for audio driver
 */

#include <stdio.h>
#include "gpio.h"

#define PCM_BASE    0x7E203000

#define CS_A        (PCM_BASE + 0x0)
#define FIFO_A      (PCM_BASE + 0x4)
#define MODE_A      (PCM_BASE + 0x8)
#define RXC_A       (PCM_BASE + 0xC)
#define TXC_A       (PCM_BASE + 0x10)
#define DREQ_A      (PCM_BASE + 0x14)
#define INTEN_A     (PCM_BASE + 0x18)
#define INTSTC_A    (PCM_BASE + 0x1c)
#define GRAY        (PCM_BASE + 0x20)

#define CS_A_STBY   0x2000000
#define CS_A_SYNC   0x1000000
#define CS_A_RXSEX  0x800000
#define CS_A_RXF    0x400000
#define CS_A_TXE    0x200000
#define CS_A_RXD    0x100000
#define CS_A_TXD    0x80000
#define CS_A_RXR    0x40000
#define CS_A_TXW    0x20000
#define CS_A_RXERR  0x10000
#define CS_A_TXERR  0x8000
#define CS_A_RXSYNC 0x4000
#define CS_A_TXSYNC 0x2000
#define CS_A_DMAEN  0x200
#define CS_A_RXTHR  0x180
#define CS_A_TXTHR  0x60
#define CS_A_RXCLR  0x10
#define CS_A_TXCLR  0x8
#define CS_A_TXON   0x4
#define CS_A_RXON   0x2
#define CS_A_EN     0x1



void _audio_init( void ) {
    
}