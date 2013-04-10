/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <printtos.h>
#include <thread.h>

int myprogA();
void myfuncA(int count);


/**
 * printtos
 * @param none
 * @return none
 */
void printtos() {
    char *name = "printtos";
    tid_typ tid = create(myprogA, 1024, 20, name, 0);
    resume(tid);
}

int myprogA() {
    myfuncA(5);
    return 0;
}

void myfuncA(int count) {
    //asm("mov r0, sp");
    // Print <count> stack elements, up to 5
    // gdb: x/5wx $sp
    
    int i;
    for ( i = 0; i != count; i++ ) {
        asm("mov r0, sp");
        register int *sp asm("r0");
        
        int j;
        for ( j = 0; j != i; j++) {
            asm("add r0, #4");
        }
        printf("%x = %d\n", sp, *sp);
         
    }
    
}
