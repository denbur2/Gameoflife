/**
 * @file gameoflife.c
 * 
 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_mixer.h>
#include <SDL_ttf.h>


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "game_state.h"
#include "pixel_font.h"
#include "core.h"
#include "cmdargs.h"


/**
 * these variables are editable
 * 
 */
#define WINDOW_WIDTH 1000           //dimension of window
#define WINDOW_HEIGHT 500
#define max_width 23                //number of squares
#define max_height 23
#define simulations_tiefe 100
//#define state->einstellungen[2] 23     //size of square

/** The entire sdl render context */
struct game_sdl_context
{
  SDL_Renderer *renderer;
  TTF_Font *font;
};
struct Muzik{
    Mix_Music *music1;
    Mix_Music *music2;
    Mix_Music *music3;
};

void game_draw_menu(struct game_sdl_context *ctx, int e);
void game_draw(struct game_sdl_context *ctx, const struct game_state *state, int farbe);
void game(struct game_sdl_context *ctx, struct Muzik *muzik, struct cmdargs *cmdargs);

/**
 * The program's main entry point.
 * 
 * @param argc number of args
 * @param argv argument vector
 * @return int 
 */

int main (int argc, char** argv)
{
    struct game_sdl_context ctx = {0};
    struct Muzik muzik = {0};
    struct cmdargs cmdargs;

    if (!cmdargs_parse(&cmdargs, argc, argv))
    {
        return EXIT_FAILURE;
    }

    /** Do some basic build up*/
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Failed to init SDL2\n!");
        return 1;
    }


    if (SDL_Init(SDL_INIT_AUDIO) < 0) 
    {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        //return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) 
    {
        fprintf(stderr, "Mix_OpenAudio failed: %s\n", Mix_GetError());
        return 1;
    }

    /**list of music*/ 
    muzik.music1 = Mix_LoadMUS("L's theme A.mp3");
    muzik.music2= Mix_LoadMUS("Elevator Music.mp3");
    muzik.music3= Mix_LoadMUS("m.mp3");   
    if (!muzik.music1 || !muzik.music2 || !muzik.music3)
    {
        fprintf(stderr, "Mix_LoadMUS failed: %s\n", Mix_GetError());
        //return 1;
    }

    if(cmdargs.noMusic)
    {
    }else
    {
        if (Mix_PlayMusic(muzik.music1, 0) == -1) 
        {
            fprintf(stderr, "Mix_PlayMusic failed: %s\n", Mix_GetError());
        }
    }
    //Mix_HaltMusic();
    //Mix_FreeMusic(music); // Musik freigeben
    
    TTF_Init();

    SDL_Window *window = NULL;
    if (!(window = SDL_CreateWindow
    (
        "Game Of Life", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    )))

    {
        fprintf(stderr, "Failed to create window\n!");
        return 1;
    }

    if (!(ctx.renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)))
    {
        fprintf(stderr, "Failed to create renderer\n!");
        return 1;
    }

    if (!(ctx.font = TTF_OpenFontRW(SDL_RWFromMem(pixel_font, pixel_font_len), 1, 16)))
    {
        fprintf(stderr, "Failed to open internal font: %s\n!", SDL_GetError());
        return 1;
    }

    game(&ctx, &muzik, &cmdargs);

    TTF_CloseFont(ctx.font);
    SDL_DestroyWindow(window);
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}


/**
 * Runs the game on the given renderer.
 *
 * @param renderer specifies where the graphics is drawn to.
 */

void game(struct game_sdl_context *ctx, struct Muzik *muzik, struct cmdargs *cmdargs)
{
    struct game_state state;

    game_state_init(&state, 0);
    if(cmdargs->noMusic)
    {

    }else {Mix_PlayMusic(muzik->music2, 0);}

    int delayy = 700;
    int e = 0;
    char inputChar[399] = "";
    int eingabe;

    if(cmdargs->delayCounter)
    {
        delayy = cmdargs->delay;
    }
    
    if(cmdargs->cubeSizeCounter || cmdargs->windowSizeCounter) 
    {
        if(cmdargs->windowSizeCounter)
        {
            state.einstellungen[0] = cmdargs->x+(simulations_tiefe*2);      //x
            state.einstellungen[1] = cmdargs->y+(simulations_tiefe*2);      //y
        }else 
        {
            state.einstellungen[0] = 230;      //x
            state.einstellungen[1] = 230;     //y
        }  
        if(cmdargs->cubeSizeCounter)
        {                      
            state.einstellungen[2] = cmdargs->cubeSize;      //cube-size
        }else 
        {
            state.einstellungen[2] = 30;      
        }

    }else
    {
        while (e<4)
        {
            SDL_Event menu; 
            game_draw_menu(ctx, e);
            while( SDL_PollEvent( &menu ) )
            {

                switch( menu.type )
                {
                    case SDL_QUIT:
                        goto quit;
                    case SDL_KEYDOWN:
                        if (menu.key.keysym.sym >= SDLK_SPACE && menu.key.keysym.sym <= SDLK_z)
                        {
                            // Ein ASCII-zeichen wurde eingegeben
                            char eingabe = (char)menu.key.keysym.sym;
                            strncat(inputChar, &eingabe, 1);
                            //inputChar = (char)menu.key.keysym.sym;
                            printf("Eingegebenes Zeichen: %s %c\n", inputChar, eingabe);
                        }   
                    if (menu.key.keysym.sym == SDLK_RETURN) 
                    {
                        // Enter-Taste wurde gedrückt
                        SDL_Log("Enter-Taste wurde gedrückt");
                        eingabe = strtol(inputChar,NULL,10)+simulations_tiefe*2;//hier könnte man noch fehler überprüfung machen mit &endpointer statt NULL und so...
                        SDL_Log("%d", eingabe);
                        if(e!=2)
                        {
                            if (eingabe-(simulations_tiefe*2)==0)
                            {
                                state.einstellungen[e]=30+(simulations_tiefe*2);
                            }else
                            {
                                state.einstellungen[e]=eingabe;
                            }
                        }else
                        {
                            if (eingabe-(simulations_tiefe*2)==0)
                            {
                                state.einstellungen[e]=30;
                            }else
                            {
                                state.einstellungen[e]=eingabe-(simulations_tiefe*2);
                            }
                        }
                        inputChar[0]='\0';
                        e++;
                        
                    }
                    default:
                        break;
                }
            }
        }     
    }
    endmenu:;
    
    if(cmdargs->noMusic)
    {
    }else 
    {
        Mix_HaltMusic();
        Mix_PlayMusic(muzik->music1, 0);
    }
    /**
     * here begins the drawing phase
    */
    int automate=0;
    while (1)
    {                                              
        SDL_Event plant; 
        game_draw(ctx, &state, 100);
        while( SDL_PollEvent( &plant ) )
        {
            int x;
            int y;
            int kx;
            int ky;
            if(plant.type == SDL_MOUSEBUTTONDOWN)
            {
                switch(plant.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        SDL_GetMouseState(&x, &y);

                        kx = (x+(state.einstellungen[2]*simulations_tiefe)) / state.einstellungen[2];
                        ky = (y-50+(state.einstellungen[2]*simulations_tiefe)) / state.einstellungen[2];

                        SDL_Log("clicked\n%d %d",x, y);
                        game_state_plant(&state, kx, ky);
                        //here are some explanations for the buttons
                        //not everybutton is usable in this phase

                        if(y<50 && x <50)  //green button: this button starts the third phase
                        {
                            goto ENDPLANT;
                        }
                        if(x>250 && x<300 && y<50)  //pink button: switches between automatic and manual simulation
                        {
                            automate = !automate;
                        }
                        if(x>100 && x<150 && y<50)  //blue button: resets the board
                        {
                            game_state_init(&state, 1);
                            game_draw(ctx, &state, 100);
                        }
                        if(x>150 && x<200 && y<50)  //grey button1: saves the gameplay
                        {
                            game_save(&state);
                        }
                        if(x>200 && x<250 && y<50)  //grey button2: loads the saved gameplay
                        {
                            game_load(&state);
                        }
                        break;
                }
            }
            switch( plant.type )
            {
                case SDL_QUIT:
                    goto quit;
                default:
                    break;
            }
        }
    }                                  
    ENDPLANT:;
    if(cmdargs->noMusic)
    {
    }else
    {
        Mix_HaltMusic();
        Mix_PlayMusic(muzik->music3, 0);
    }
    game_draw(ctx, &state, 0);
       
    while (1) 
    {
        SDL_Event event;

        if(automate)
        {
            game_state_update(&state);
            game_draw(ctx, &state, 0);
            SDL_Delay(delayy);
        }
        while (SDL_PollEvent(&event)) //active phase
        {                 
        int x, y;
        if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            switch (event.button.button)
            {
                case SDL_BUTTON_LEFT:
                    SDL_GetMouseState(&x, &y);

                    
                    if(x>250 && x<300 && y<50)//pink button: switches between automatic and manual simulation
                    {
                        automate = !automate;
                    }
                    
                    if(x>50 && x<100 && y<50) //red button: goes back to drawing phase
                    {
                        goto endmenu;
                    }
                  
                    if(x>100 && x<150 && y<50)  //blue button: resets the board
                    {
                        game_state_init(&state, 1);
                        game_draw(ctx, &state, 0);
                    }
                   
                    if(x>150 && x<200 && y<50) //grey button1: saves the gameplay
                    {
                        game_save(&state);
                    }
                    
                    if(x>200 && x<250 && y<50) //grey button2: loads the saved gameplay
                    {
                        game_load(&state);
                    }
                    
                    if(y>50)
                    {
                        game_state_update(&state); //manual progression per click
                        game_draw(ctx, &state, 0);
                        SDL_Log("test");
                    }
                    default:
                        break;
                }
            }
            switch (event.type)
            {
                case SDL_QUIT:
                    goto quit;
                default:
                    break;
            }
        }
    }
quit:;
}


/**
 * Render the state using the given SDL renderer.
 *
 * @param ctx specifies the sdl context
 * @param state the state that shall be  rendered
 */

void game_draw_menu(struct game_sdl_context *ctx, int e)
{
    /* Clear background with black color */
    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
    SDL_RenderClear(ctx->renderer);
 
    SDL_Color black = {255, 255, 0};
   
    const char *texts[] = {
    "Wie breit soll die Simulation sein? Bestaetige mit Enter.",
    "Wie hoch soll die Simulation sein? Bestaetige mit Enter.",
    "Wie gross soll eine Zelle sein? Bestaetige mit Enter.",
    "Zum Starten mit Enter!",
    "Achtung: Das Spiel startet in manueller Entwicklung!(siehe Anleitung)"
    };
    
    SDL_Texture *text_texture[20];

    for(int i = 0; i <= 4; i++)
    {
        if(e>i)
        {
            black = (SDL_Color){0, 255, 0};
        }
        else
        {
            black = (SDL_Color){255, 255, 0};
        }
        SDL_Surface *text_surface = TTF_RenderText_Blended(ctx->font, texts[i], black); 
        text_texture[i] = SDL_CreateTextureFromSurface(ctx->renderer, text_surface);
        SDL_RenderCopy(ctx->renderer, text_texture[i], NULL, &(struct SDL_Rect){.x=100,.y=i*60+100,.w=text_surface->w+10,.h=text_surface->h+10});
        SDL_FreeSurface(text_surface);  
        SDL_DestroyTexture(text_texture[i]);
    }
    /* Present */
    SDL_RenderPresent(ctx->renderer);
}

void game_draw(struct game_sdl_context *ctx, const struct game_state *state, int farbe)
{
    /* Clear background with red color */
    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 0);
    SDL_RenderClear(ctx->renderer);

    /**
     * here is how we made the buttons and their symbols
    */

    SDL_Texture *text_texture[20];

    SDL_Rect start = {.x=0,.y=0,.w=42,.h=42};
    SDL_SetRenderDrawColor(ctx->renderer, 0, 255, 0, 0);
    SDL_RenderFillRect(ctx->renderer, &start);
    SDL_Surface *text_start = TTF_RenderText_Blended(ctx->font, " I>", (SDL_Color){0, 0, 0}); 
    text_texture[0] = SDL_CreateTextureFromSurface(ctx->renderer, text_start);
    SDL_RenderCopy(ctx->renderer, text_texture[0], NULL, &start);

    SDL_Rect stop = {.x=50,.y=0,.w=42,.h=42};
    SDL_SetRenderDrawColor(ctx->renderer, 255, 0, 0, 0);
    SDL_RenderFillRect(ctx->renderer, &stop);
    SDL_Surface *text_stop = TTF_RenderText_Blended(ctx->font, " II", (SDL_Color){0, 0, 0}); 
    text_texture[1] = SDL_CreateTextureFromSurface(ctx->renderer, text_stop);
    SDL_RenderCopy(ctx->renderer, text_texture[1], NULL, &stop);

    SDL_Rect reset = {.x=100,.y=0,.w=42,.h=42};
    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 255, 0);
    SDL_RenderFillRect(ctx->renderer, &reset);
    SDL_Surface *text_reset = TTF_RenderText_Blended(ctx->font, " 0 ", (SDL_Color){0, 0, 0}); 
    text_texture[2] = SDL_CreateTextureFromSurface(ctx->renderer, text_reset);
    SDL_RenderCopy(ctx->renderer, text_texture[2], NULL, &reset);


    SDL_Rect save = {.x=150,.y=0,.w=42,.h=42};
    SDL_SetRenderDrawColor(ctx->renderer, 100, 100, 100, 0);
    SDL_RenderFillRect(ctx->renderer, &save);
    SDL_Surface *text_save = TTF_RenderText_Blended(ctx->font, "  \\/ ", (SDL_Color){0, 0, 0}); 
    text_texture[3] = SDL_CreateTextureFromSurface(ctx->renderer, text_save);
    SDL_Surface *text_save2 = TTF_RenderText_Blended(ctx->font, " _ ", (SDL_Color){0, 0, 0}); 
    text_texture[6] = SDL_CreateTextureFromSurface(ctx->renderer, text_save2);
    SDL_RenderCopy(ctx->renderer, text_texture[3], NULL, &save);
    SDL_RenderCopy(ctx->renderer, text_texture[6], NULL, &save);

    SDL_Rect load = {.x=200,.y=0,.w=42,.h=42};
    SDL_SetRenderDrawColor(ctx->renderer, 100, 100, 100, 0);
    SDL_RenderFillRect(ctx->renderer, &load);
    SDL_Surface *text_load = TTF_RenderText_Blended(ctx->font, " ^ ", (SDL_Color){0, 0, 0}); 
    SDL_Surface *text_load2 = TTF_RenderText_Blended(ctx->font, " _ ", (SDL_Color){0, 0, 0}); 
    text_texture[4] = SDL_CreateTextureFromSurface(ctx->renderer, text_load);
    text_texture[5] = SDL_CreateTextureFromSurface(ctx->renderer, text_load2);
    SDL_RenderCopy(ctx->renderer, text_texture[4], NULL, &load);
    SDL_RenderCopy(ctx->renderer, text_texture[5], NULL, &load);
    
    SDL_Rect automateb = {.x=250,.y=0,.w=42,.h=42};
    SDL_SetRenderDrawColor(ctx->renderer, 255, 100, 255, 0);
    SDL_RenderFillRect(ctx->renderer, &automateb);
    SDL_Surface *text_automateb = TTF_RenderText_Blended(ctx->font, " >I ", (SDL_Color){0, 0, 0}); 
    text_texture[5] = SDL_CreateTextureFromSurface(ctx->renderer, text_automateb);
    SDL_RenderCopy(ctx->renderer, text_texture[5], NULL, &automateb);


    //black squares getting rendered
    //for this purpose, the play dates are taken from the koordinate[x][y] array

    for(int x=simulations_tiefe;x <= state->einstellungen[0]-simulations_tiefe;x++)
    {
        for(int y=simulations_tiefe; y<= state->einstellungen[1]-simulations_tiefe; y++)
        {
            if(state->koordinate[x][y]==1)
            {
                //this calculation allows for a buffer edge of 3 times the size of a cell and moves the playing field accordingly
                SDL_Rect r = {.x=(x*state->einstellungen[2])-state->einstellungen[2]*simulations_tiefe,.y=(y*state->einstellungen[2])+50-(state->einstellungen[2]*simulations_tiefe),.w=state->einstellungen[2]-1,.h=state->einstellungen[2]-1};
                SDL_SetRenderDrawColor(ctx->renderer, farbe, farbe, farbe, farbe);
                SDL_RenderFillRect(ctx->renderer, &r);
            }
            if(state->koordinate[x][y]==0)
            {
                // same for the white squares
    
                SDL_Rect r = {.x=(x*state->einstellungen[2])-state->einstellungen[2]*simulations_tiefe,.y=(y*state->einstellungen[2])+50-(state->einstellungen[2]*simulations_tiefe),.w=state->einstellungen[2]-1,.h=state->einstellungen[2]-1};
                SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(ctx->renderer, &r);
            }
        }
    }
    /* Present */
    SDL_RenderPresent(ctx->renderer);
}
