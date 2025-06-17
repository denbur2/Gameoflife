/**
 * @file cmdargs.c
 */

#include "cmdargs.h"
//#include "game_state.h"
//touc#include "core.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



int testnum(char input[])
{
    char *endptr = input;
    
    while(*endptr)
    {
        if(isdigit(*endptr++) == 0)
        {
            fprintf(stderr,"'%s' is not a number or a command!\n", input);            
            return 1;    
        }
    }
    return 0;
}


int cmdargs_parse(struct cmdargs *cmdargs, int argc, char *argv[])
{
    *cmdargs = (struct cmdargs){0};

    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i],"--delay") == 0)
        {
            cmdargs->delay = strtol(argv[i+1], NULL, 10);
            cmdargs->delayCounter = 1;
            if(testnum(argv[i + 1]))
            {
                return 0;
            }
            i++;
        } else if(strcmp(argv[i],"--no-music") == 0)
        {
            cmdargs->noMusic = 1;

        } else if(strcmp(argv[i],"--cube-size") == 0)
        {
            cmdargs->cubeSize = strtol(argv[i+1], NULL, 10);
            cmdargs->cubeSizeCounter = 1;
            if(testnum(argv[i + 1]))
            {
                return 0;
            }
            i++;
        } else if(strcmp(argv[i],"--window-size") == 0)
        {
            cmdargs->x = strtol(argv[i+1], NULL, 10);
            cmdargs->y = strtol(argv[i+2], NULL, 10);
            cmdargs->windowSizeCounter = 1;
            if(testnum(argv[i + 1]))
            {
                return 0;
            }
            else if(testnum(argv[i + 2]))
            {
                return 0;
            }
            i++;
            i++;
            
        }  else if (!strncmp(argv[i], "--", 2))
		{
			fprintf(stderr, "Unknown flag %s\n", argv[i]);
			return 0;
		} else 
        {
           fprintf(stderr,"Invalid command, useable commands are --no-Music, --delay x , --cube-size x , --window-size x y \n(x and y are variables)");
           return 0;
	    }
    }    
    return 1;
}
