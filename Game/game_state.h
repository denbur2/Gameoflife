#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

/** This is the context that contains all of the logical game state */
struct game_state
{
    int koordinate[1000][1000];
    int einstellungen[5];
};

/**
 * Initialize the given game state with default values.
 * 
 * @param state the game state that shall be initialized
 */
void game_state_init(struct game_state *state, int b);

void game_state_plant(struct game_state *state, int x, int y);

/**
 * Execute one time step to update the games state.
 *
 * @param state defines the state of being updated.
 */
void game_state_update(struct game_state *state);

void game_save(struct game_state *state);
void game_load(struct game_state *state);

#endif