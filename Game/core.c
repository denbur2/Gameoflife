#include <stdio.h>
#include "core.h"
#include "game_state.h"
#include "cmdargs.h"
/**
 * 0|0|0
 * 1|0|1
 * 1|1|1
 * 
 * die mittlere stelle soll bei diesem beispiel 5 nachbarn haben...
*/
int anzahl_nachbarn(struct game_state *state, int x, int y){
  int counter = 0;
  if(state->koordinate[x][y+1]==1){counter++;}
  if(state->koordinate[x][y-1]==1){counter++;}
  if(state->koordinate[x+1][y+1]==1){counter++;}
  if(state->koordinate[x+1][y-1]==1){counter++;}
  if(state->koordinate[x-1][y+1]==1){counter++;}
  if(state->koordinate[x-1][y-1]==1){counter++;}
  if(state->koordinate[x+1][y]==1){counter++;}
  if(state->koordinate[x-1][y]==1){counter++;}
  return counter;
}
