/**
 * @file cmdargs.h
 */

#ifndef _CMDARGS_H_
#define _CMDARGS_H_

/** Holds all the possible command line arguments */
struct cmdargs
{
    int x;
    int y;
    int delayCounter;
    int delay;
    int windowSizeCounter;
    int windowSize;
    int cubeSizeCounter;
    int cubeSize;
    int noMusic;
};

/**
 * @brief Checks if the input string is a valid number.
 *
 * @param input is the input string to check
 * @return 0 if the input is a valid number, 1 otherwise
 */
int testnum(char input[]);


/**
 * @brief parses the command-line arguments and populates the cmdargs struct.
 *
 * @param cmdargs pointer to the cmdargs struct to populate.
 * @param argc number of arguments given by the user
 * @param argv argv the argument vector
 * @return 1 if parsing is successful, 0 otherwise
 */
int cmdargs_parse(struct cmdargs *cmdargs, int argc, char *argv[]);

#endif
