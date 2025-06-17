/**
 * @file core_test.c
 *
 * Test for functions in core.c
 */

#include <assert.h>
#include <stdio.h>


/* Include the interface of the module under testing */

#include"core.h"
#include"game_state.h"

/******************************************************/




static void valid_test2(void)
{

struct game_state state = {0};
state.koordinate[10][16] = 0;
state.koordinate[10][14] = 1;
state.koordinate[11][16] = 0;
state.koordinate[11][14] = 1;
state.koordinate[9][16] = 1;
state.koordinate[9][14] = 1;
state.koordinate[11][15] = 0;
state.koordinate[9][15] = 1;

assert(5 == anzahl_nachbarn(&state,10,15));

}



int main(int argc, char **argv)
{
    valid_test2();
    return 0;
}
