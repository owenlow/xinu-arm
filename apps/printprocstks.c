#include <printprocstks.h>
#include <thread.h>
#include <stddef.h>

#include <printtos.h>

#define NUMPROCS 3

void printstacksegment(int*);

void printprocstks() {
    // printprocstks
    
    char *name;
    
    int i;
    
    tid_typ myprocs[NUMPROCS];
    
    for (i = 0; i != NUMPROCS; i++) {
        sprintf(name, "sub process %d", i);
        myprocs[i] = create(myprogA, 1024, 20, name, 0);
    }
    
    for (i = 0; i != NTHREAD; i++) {
        struct thrent *thrptr = &thrtab[i];
        if (thrptr->state == THRFREE) {
            continue;
        }
        
        printf("Proc [%s]. Pid = %d\n", thrptr->name, i);
        printf("       Stack: Base  = %x\n", thrptr->stkbase);
        printf("              Len   = %d\n", thrptr->stklen);
        printf("              Limit = %d\n", thrptr->stkbase-4092);
        printf("              StkPtr = %x\n", (thrptr->stkptr));
            
    }
    
    // Resume the paused procs
    for(i = 0; i != NUMPROCS; i++) {
        resume(myprocs[i]);
    }
}



/*
void printsegaddress() {
    
}

long host2netl_asm(long param) {
    
}

*/
