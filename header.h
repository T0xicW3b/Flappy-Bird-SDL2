#ifndef HEADER
#define HEADER
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>

// const area
#define WIDTH 900
#define HEIGHT 600
#define FLAPPY_VEL 4
#define PIPE_VEL 7
#define MAX_PIPES 100
#define PIPE_HEIGHT 1000
#define PIPE_WIDTH 110
#define PIPE_GAP 231
// holly f***, u dont know the workaround i just did to find that gap value LOL

typedef struct {
  SDL_Rect rect;
  bool activate;
} Pipe;

// functions header area
int genPipePosition();
void genPipe(SDL_Texture *ptxtPipe, SDL_Renderer *prender, Uint32 intervalo,
             Uint32 *ultimoTempo, SDL_Rect *pipe);
void addPipe(Pipe pipes[], Pipe hitBoxPipeTop[], Pipe hitBoxPipeBottom[],
             int *pipeCount);
void updateAndDrawPipe(Pipe pipes[], Pipe hitBoxPipeTop[],
                       Pipe hitBoxPipeBottom[], int *pipeCount,
                       SDL_Renderer *prender, SDL_Texture *ptxtPipe,
                       SDL_Rect *flappy, SDL_Rect *intersection);
bool itCollides(SDL_Rect *player, SDL_Rect *intersection, Pipe hitBox[],
                int index);
void runApp();

#endif
