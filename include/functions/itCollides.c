#include "header/header.h"

bool itCollides(SDL_Rect *flappy, SDL_Rect *intersection, Pipe hitBox[],
                int i) {
  // it see if it's on left of each other;
  return SDL_IntersectRect(flappy, &hitBox[i].rect, intersection);
}
