#include "header/header.h"

// i'll move this collision function outside of
// this function in the future
bool updateAndDrawPipe(Pipe pipes[], Pipe hitBoxPipeTop[],
                       Pipe hitBoxPipeBottom[], Pipe hitBoxPipePoints[],
                       int *pipeCount, int *points, SDL_Renderer *prender,
                       SDL_Texture *ptxtPipe, SDL_Rect *flappy,
                       SDL_Rect *intersection, bool isFlappyAlive,
                       bool scored[]) {
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
      }

      if (itCollides(flappy, intersection, hitBoxPipePoints, i)) {
        if (!scored[i] && intersection->w < intersection->h) {
          (*points)++;
          scored[i] = true;
          printf("Score: %d\n", *points);
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
