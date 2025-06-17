/**
 * @file game_state.c
 *
 * Implements the program's logic.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "game_state.h"
#define max_width 100
#define max_height 100
#define simulations_tiefe 100
#include "cmdargs.h"



void game_state_init(struct game_state *state, int b)
{

  for (int i = 0; i < 1000; i++){
    for (int j = 0; j < 1000; j++){
        state->koordinate[i][j] = 0;
    }
  }
  if(b==0){
    for (int i = 0; i<5 ; i++){
      state->einstellungen[i]=0;
    }
  }
}

void game_state_plant(struct game_state *state, int x, int y){
  state->koordinate[x][y]=!state->koordinate[x][y];
}


void game_state_update(struct game_state *state)
{
  int ablage[1000][1000]={0};
  int nachbarn = 0;
  for(int x=0; x<(state->einstellungen[0]); x++){
    for(int y=0; y<(state->einstellungen[1]); y++){
      nachbarn = anzahl_nachbarn(state, x, y);


      if(state->koordinate[x][y]==1){
        if(nachbarn<=1){
          ablage[x][y]=0;
        }
        else if(nachbarn==2 || nachbarn==3){
          ablage[x][y]=1;
        }
        else if(nachbarn>3){
          ablage[x][y]=0;
        }
      }

      if(state->koordinate[x][y]==0){
        if(nachbarn==3){
          ablage[x][y]=1;
        }
        else{
          ablage[x][y]=0;
        }
      }

    }
  }
  for(int x=0; x<(state->einstellungen[0]); x++){
    for(int y=0; y<(state->einstellungen[1]); y++){
      state->koordinate[x][y]=ablage[x][y];
    }

  }
}

void game_save(struct game_state *state){

    FILE *out = fopen("game_save","w");
  
    if (out == NULL) {
      fprintf(stderr, "Datei konnte nicht geöffnet werden.\n");
      exit(1);
    }
    for(int x=simulations_tiefe; x<(state->einstellungen[0]-simulations_tiefe); x++){
        for(int y=simulations_tiefe; y<(state->einstellungen[1]-simulations_tiefe); y++){
          fprintf(out, "%d", state->koordinate[x][y]);
        }
        fprintf(out, "\n");
    }
    fclose(out);
}


void game_load(struct game_state *state){

    FILE *in = fopen("game_save","r");

    if (in == NULL) {
        perror("Fehler beim Öffnen der Datei");
        exit(1);
    }

    char line[1000]; //buffer


    for(int x=simulations_tiefe; x<(state->einstellungen[0]-simulations_tiefe); x++){
      fgets(line, sizeof(line), in);
      for(int y=simulations_tiefe; y<(state->einstellungen[1]-simulations_tiefe); y++){
          if (line[y-simulations_tiefe] >= '0' && line[y-simulations_tiefe] <= '9') {
            int num = line[y-simulations_tiefe] - '0';
            printf("%d", num);
            state->koordinate[x][y] = num;
          }
        }
          printf("|\n");
    }
    fprintf(stderr, "checkpoint");


    fclose(in);
}
