#ifndef _CORE_H
#define _CORE_H

struct game_state;


/**
*This function counts the number of neighbors of a coordinate [x|y]
*
*/

int anzahl_nachbarn(struct game_state *state, int x, int y);

#endif
