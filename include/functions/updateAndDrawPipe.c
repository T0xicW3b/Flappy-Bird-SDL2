#include "SDL_gamecontroller.h"
#include "SDL_stdinc.h"
#include "header/header.h"
// #include <stdio.h>

// i'll move this collision function outside of
// this function in the future
bool updateAndDrawPipe(Pipe pipes[], Pipe hitBoxPipeTop[],
                       Pipe hitBoxPipeBottom[], Pipe hitBoxPipePoints[],
                       int *pipeCount, int *points, SDL_Renderer *prender,
                       SDL_Texture *ptxtPipe, SDL_Rect *flappy,
                       SDL_Rect *intersection, SDL_GameController *gamepad,
                       TTF_Font *font, bool isFlappyAlive, bool scored[]) {
  for (int i = 0; i < *pipeCount; i++) {
    if (pipes[i].activate && hitBoxPipeTop[i].activate) {
      pipes[i].rect.x -= PIPE_VEL;
      hitBoxPipeTop[i].rect.x -= PIPE_VEL;
      hitBoxPipeBottom[i].rect.x -= PIPE_VEL;
      hitBoxPipePoints[i].rect.x -= PIPE_VEL;
      SDL_RenderCopy(prender, ptxtPipe, NULL, &pipes[i].rect);
      SDL_RenderDrawRect(prender, &hitBoxPipeTop[i].rect);
      SDL_RenderDrawRect(prender, &hitBoxPipeBottom[i].rect);
      SDL_RenderDrawRect(prender, &hitBoxPipePoints[i].rect);
      if (pipes[i].rect.x + pipes[i].rect.w < 0) {
        pipes[i].activate = false;
        hitBoxPipeTop[i].activate = false;
        // hitBoxPipeBottom[i].rumbled = false;
      }
      if (!hitBoxPipeBottom[i].rumbled &&
          hitBoxPipeBottom[i].rect.y - flappy->y < 100 &&
          hitBoxPipeBottom[i].rect.x <= (flappy->x + flappy->w) &&
          hitBoxPipeBottom[i].rect.x + hitBoxPipeBottom[i].rect.w >=
              flappy->x) {
        Uint16 low_freq = 0xFFFF;
        Uint16 high_freq = 0xFFFF;
        Uint32 duration = 10;
        SDL_GameControllerRumble(gamepad, low_freq, high_freq, duration);
        hitBoxPipeBottom[i].rumbled = true;
      } else {
        hitBoxPipeBottom[i].rumbled = false;
      }

      if (itCollides(flappy, intersection, hitBoxPipePoints, i)) {
        if (!scored[i] && intersection->w < intersection->h) {
          (*points)++;
          scored[i] = true;
          // printf("Score: %d\n", *points);
        }
      }
      if (itCollides(flappy, intersection, hitBoxPipeTop, i) ||
          itCollides(flappy, intersection, hitBoxPipeBottom, i)) {
        if (intersection->w < intersection->h) {
          if (flappy->x < hitBoxPipeTop[i].rect.x ||
              flappy->x < hitBoxPipeBottom[i].rect.x) {
            // if collides on the left just push to the left
            // we don't need to check the right
            return isFlappyAlive = false;
            // flappy->x -= intersection->w;
          }
        } else {
          if (flappy->y <= hitBoxPipeTop[i].rect.y + hitBoxPipeTop[i].rect.h) {
            return isFlappyAlive = false;
            // flappy->y += intersection->h;
          }
          if (flappy->y + flappy->h >= hitBoxPipeBottom[i].rect.h) {
            return isFlappyAlive = false;
            // flappy->y -= intersection->h;
          }
        }
      }
    }
  }
  return isFlappyAlive = true;
}
