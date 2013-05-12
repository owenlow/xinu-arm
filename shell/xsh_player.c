/**
 * xsh_player.c
 * Author: Owen Royall-Kahin
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <player.h>

/**
 * Shell command (player).
 * @param nargs  number of arguments in args array
 * @param args   array of arguments
 * @return OK for success, SYSERR for syntax error
 */
shellcmd xsh_player(int nargs, char *args[])
{
    
    /* Output help, if '--help' argument was supplied */
    if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
    {
        printf("Usage: %s\n\n", args[0]);
        printf("Description:\n");
        printf("\tAudio player application\n");
        printf("Options:\n");
        printf("\t--help\tdisplay this help and exit\n");
        return OK;
    }
    
    /* Check for correct number of arguments */
    if (nargs > 2)
    {
        fprintf(stderr, "%s: too many arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n",
                args[0]);
        return SYSERR;
    }
    if (nargs < 2)
    {
        fprintf(stderr, "%s: too few arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n",
                args[0]);
        return SYSERR;
    }
    
    if (strnlen(args[1], 1000) == 1) {
        audio_test(args[1][0]-'0');
    }
    else {
        audio_play(args[1]);
    }
    
    return OK;
}
