#include "header/header.h" //aqui esta a constante vel
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_joystick.h>
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

void runApp() {
  Uint32 interval = 1000;
  Uint32 lastTime = SDL_GetTicks();
  Pipe pipes[MAX_PIPES];
  Pipe hitBoxPipeTop[MAX_PIPES];
  Pipe hitBoxPipeBottom[MAX_PIPES]; // I can't name better these var
  Pipe hitBoxPipePoints[MAX_PIPES]; // even these LOL
  int pipeCount = 0, points = 0;
  bool scored[MAX_PIPES] = {false};
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
  SDL_Texture *ptxtPipe =
      IMG_LoadTexture(prender, "../assets/sprites/pipe.png");
  SDL_Texture *ptxtFlappy =
      IMG_LoadTexture(prender, "../assets/sprites/flappy.png");
  SDL_Texture *ptxtSky = IMG_LoadTexture(prender, "../assets/sprites/sky.jpg");
  int boxPositionX = (WIDTH / 2) - (BOX_WIDTH / 2);
  int boxPositionY = (HEIGHT / 2) - (BOX_HEIGHT / 2);

  SDL_Rect box = {boxPositionX, 50, BOX_WIDTH, BOX_HEIGHT};
  SDL_Rect box2 = {boxPositionX, 50, BOX_WIDTH, BOX_HEIGHT};
  SDL_Rect sky = {0, 0, WIDTH, HEIGHT};
  SDL_Rect flappy = {100, 50, 60, 50};
  SDL_Rect intersection;

  bool isFlappyAlive = true;
  bool appRunning = true;
  bool isGameStarted = true;
  while (appRunning) {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      // keyevents
      switch (event.type) {
      case SDL_QUIT:
        appRunning = false;
        break;
      case SDL_KEYDOWN:
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
          flappy.y -= 90;
          // printf("space pressed\n");
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_R) {
          flappy.x = 100; // for debbug
          printf("x postion reseted\n");
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_E) {
          flappy.y = 50;
          printf("y position reseted\n");
        }
      }
    }
    // clean the screen to the game
    SDL_RenderClear(prender);

    if (isFlappyAlive) {
      Uint32 now = SDL_GetTicks();

      // rule to add new pipes
      if (now - lastTime >= interval) {
        addPipe(pipes, hitBoxPipeTop, hitBoxPipeBottom, hitBoxPipePoints,
                &pipeCount);

        lastTime = now;
      }

      // rule to gravity
      flappy.y += FLAPPY_VEL;

      // this collide system needs to be out of the collision function
      // because of the the looping we need to check add the hitbox and pipes
      // i'll fix it later
      if (flappy.y + flappy.h >= HEIGHT) {
        flappy.y = HEIGHT - flappy.h;
      }
      if (flappy.y <= 0) {
        flappy.y = 0;
      }

      // render sky
      SDL_RenderCopy(prender, ptxtSky, NULL, &sky);

      // render flappy
      SDL_RenderCopy(prender, ptxtFlappy, NULL, &flappy);

      // to make the pipe hitbox invisible
      SDL_SetRenderDrawBlendMode(prender, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor(prender, 0, 0, 0, 0);

      // this is temporary, i'll do it better in the next commit
      isFlappyAlive = updateAndDrawPipe(pipes, hitBoxPipeTop, hitBoxPipeBottom,
                                        hitBoxPipePoints, &pipeCount, &points,
                                        prender, ptxtPipe, &flappy,
                                        &intersection, isFlappyAlive, scored);
    } else {
      SDL_RenderCopy(prender, ptxtSky, NULL, &sky);
      SDL_SetRenderDrawColor(prender, 237, 207, 71, 255);
      SDL_RenderFillRect(prender, &box);
    }

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
             Pipe hitBoxPipePoints[], int *pipeCount) {
  if (*pipeCount < MAX_PIPES) {
    Pipe newPipe;
    int pipePos = genPipePosition();
    int hitTopHeight = 82 + (pipePos - (-300)); // the rule to hitbox size
    // 82 is the minimum hitbox height pixels when pipePos is -300 (i tested.)

    int hitBottomY = hitTopHeight + PIPE_GAP;
    int hitBottomHeight = HEIGHT - hitBottomY;

    int hitPointsY = hitTopHeight;

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

    Pipe hitPoints;
    hitPoints.rect.x = WIDTH;
    hitPoints.rect.y = hitPointsY;
    hitPoints.rect.w = PIPE_WIDTH;
    hitPoints.rect.h = PIPE_GAP;
    hitPoints.activate = true;
    hitBoxPipePoints[*pipeCount] = hitPoints;

    (*pipeCount)++;
  }
}

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

bool itCollides(SDL_Rect *flappy, SDL_Rect *intersection, Pipe hitBox[],
                int i) {
  // it see if it's on left of each other;
  return SDL_IntersectRect(flappy, &hitBox[i].rect, intersection);
}
