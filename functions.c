#include "header.h" //aqui esta a constante vel
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h> //I know that this is native in C++
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void runWindow() {
  Uint32 interval = 1000;
  Uint32 lastTime = SDL_GetTicks();
  Pipe pipes[MAX_PIPES];
  Pipe hitBoxPipeTop[MAX_PIPES];
  Pipe hitBoxPipeBottom[MAX_PIPES]; // I can't name better these var
  int pipeCount = 0;
  // seed for the pipe random position
  srand(time(NULL));

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *pwindow =
      SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *prender =
      SDL_CreateRenderer(pwindow, -1, SDL_RENDERER_ACCELERATED);

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    printf("Erro ao inicializar SDL2_image: %s\n", IMG_GetError());
  }
  SDL_Texture *ptxtPipe = IMG_LoadTexture(prender, "sprites/pipe.png");
  SDL_Texture *ptxtFlappy = IMG_LoadTexture(prender, "sprites/flappy.png");
  SDL_Texture *ptxtSky = IMG_LoadTexture(prender, "sprites/sky.jpg");
  SDL_Rect sky = {0, 0, WIDTH, HEIGHT};
  SDL_Rect flappy = {100, 50, 60, 50};
  SDL_Rect test = {500, 0, 110, 82};

  bool gameRunning = true;
  while (gameRunning) {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      // keyevents
      switch (event.type) {
      case SDL_QUIT:
        gameRunning = false;
        break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_SPACE) {
          flappy.y -= 90;
        }
      }
    }
    Uint32 now = SDL_GetTicks();

    // rule to add new pipes
    if (now - lastTime >= interval) {
      addPipe(pipes, hitBoxPipeTop, hitBoxPipeBottom, &pipeCount);
      lastTime = now;
    }

    // rule to gravity
    flappy.y += FLAPPY_VEL;

    // clean the screen
    SDL_RenderClear(prender);

    // render sky
    SDL_RenderCopy(prender, ptxtSky, NULL, &sky);

    // render flappy
    SDL_RenderCopy(prender, ptxtFlappy, NULL, &flappy);

    // to make the pipe hitbox invisible
    SDL_SetRenderDrawBlendMode(prender, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(prender, 0, 0, 0, 0);

    // this is temporary, i'll do it better in the next commit
    gameRunning = updateAndDrawPipe(pipes, hitBoxPipeTop, hitBoxPipeBottom,
                                    &pipeCount, prender, ptxtPipe, &flappy);
    // render
    SDL_RenderPresent(prender);

    // frames per second
    SDL_Delay(16);
  }
  SDL_DestroyTexture(ptxtFlappy);
  SDL_DestroyTexture(ptxtPipe);
  IMG_Quit();
  SDL_DestroyRenderer(prender);
  SDL_DestroyWindow(pwindow);
  SDL_Quit();
}

int genPipePosition() {
  int position = (rand() % 201 + 100) * -1; // -100 to -300 pixels
  return position;
}

void addPipe(Pipe pipes[], Pipe hitBoxPipeTop[], Pipe hitBoxPipeBottom[],
             int *pipeCount) {
  if (*pipeCount < MAX_PIPES) {
    Pipe newPipe;
    int pipePos = genPipePosition();
    int hitTopHeight = 82 + (pipePos - (-300)); // the rule to hitbox size
    // 82 is the minimum hitbox height pixels when pipePos is -300 (i tested)

    int hitBottomY = hitTopHeight + PIPE_GAP;
    int hitBottomHeight = WIDTH - hitBottomY;
    newPipe.rect.x = WIDTH;
    newPipe.rect.y = pipePos;
    newPipe.rect.w = PIPE_WIDTH;
    newPipe.rect.h = PIPE_HEIGHT;
    newPipe.activate = true;
    pipes[*pipeCount] = newPipe;

    Pipe hitTop;
    hitTop.rect.x = WIDTH;
    hitTop.rect.y = 0;
    hitTop.rect.w = PIPE_WIDTH;
    hitTop.rect.h = hitTopHeight;
    hitTop.activate = true;
    hitBoxPipeTop[*pipeCount] = hitTop;

    Pipe hitBottom;
    hitBottom.rect.x = WIDTH;
    hitBottom.rect.y = hitBottomY;
    hitBottom.rect.w = PIPE_WIDTH;
    hitBottom.rect.h = hitBottomHeight;
    hitBottom.activate = true;
    hitBoxPipeBottom[*pipeCount] = hitBottom;
    (*pipeCount)++;
  }
}

bool updateAndDrawPipe(Pipe pipes[], Pipe hitBoxPipeTop[],
                       Pipe hitBoxPipeBottom[], int *pipeCount,
                       SDL_Renderer *prender, SDL_Texture *ptxtPipe,
                       SDL_Rect *flappy) {
  for (int i = 0; i < *pipeCount; i++) {
    if (pipes[i].activate && hitBoxPipeTop[i].activate) {
      pipes[i].rect.x -= PIPE_VEL;
      hitBoxPipeTop[i].rect.x -= PIPE_VEL;
      hitBoxPipeBottom[i].rect.x -= PIPE_VEL;
      SDL_RenderCopy(prender, ptxtPipe, NULL, &pipes[i].rect);
      SDL_RenderDrawRect(prender, &hitBoxPipeTop[i].rect);
      SDL_RenderDrawRect(prender, &hitBoxPipeBottom[i].rect);
      if (pipes[i].rect.x + pipes[i].rect.w < 0) {
        pipes[i].activate = false;
        hitBoxPipeTop[i].activate = false;
      }
      if (itCollides(*flappy, hitBoxPipeTop, i) ||
          itCollides(*flappy, hitBoxPipeBottom, i)) {
        SDL_Quit();
        return false;
        // printf("Collides\n");
      }
    }
  }
  return true;
}

int itCollides(SDL_Rect flappy, Pipe hitBox[], int i) {
  // it see if it's on left of each other;
  if (flappy.x + flappy.w <= hitBox[i].rect.x)
    return false;
  if (hitBox[i].rect.x + hitBox[i].rect.w <= flappy.x)
    return false;

  // see if it's above
  if (flappy.y + flappy.h <= hitBox[i].rect.y)
    return false;
  if (hitBox[i].rect.y + hitBox[i].rect.h <= flappy.y)
    return false;

  // else, it collides
  return true;
}
