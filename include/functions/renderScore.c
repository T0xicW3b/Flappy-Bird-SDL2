#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"
#include "header/header.h"
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

void renderScore(SDL_Renderer *prender, TTF_Font *font, const char *text, int x,
                 int y, int *score) {

  SDL_Color red = {207, 63, 41, 255};
  SDL_Color white = {255, 255, 255, 255};
  SDL_Surface *tempSurface = TTF_RenderText_Blended(font, text, red);
  if (!tempSurface) {
    printf("Error initializing TTF: %s\n", TTF_GetError());
    return;
  }
  SDL_Texture *ptxtScore = SDL_CreateTextureFromSurface(prender, tempSurface);

  if (ptxtScore) {
    SDL_Rect rectDest = {x, y, tempSurface->w, tempSurface->h};
    SDL_RenderCopy(prender, ptxtScore, NULL, &rectDest);
    SDL_DestroyTexture(ptxtScore);
  }
  // free surface from RAM
  SDL_FreeSurface(tempSurface);
}
