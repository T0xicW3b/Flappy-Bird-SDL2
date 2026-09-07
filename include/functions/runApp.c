#include "SDL_gamecontroller.h"
#include "functions/addPipe.c"
#include "functions/genPipePosition.c"
#include "functions/itCollides.c"
#include "functions/updateAndDrawPipe.c"
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
#include <stddef.h>
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

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER)) {
    fprintf(stderr, "Error initializing SDL2: %s\n", SDL_GetError());
  }
  SDL_Window *pwindow =
      SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *prender =
      SDL_CreateRenderer(pwindow, -1, SDL_RENDERER_ACCELERATED);
  SDL_GameController *gamepad = NULL;

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
          flappy.y -= FLAPPY_JUMP;
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
        break;
      case SDL_CONTROLLERDEVICEADDED:
        if (!gamepad) {
          gamepad = SDL_GameControllerOpen(event.cdevice.which);
          if (gamepad) {
            printf("Controller connected\n");
          }
        }
        break;
      case SDL_CONTROLLERDEVICEREMOVED:
        if (gamepad) {
          SDL_Joystick *joy = SDL_GameControllerGetJoystick(gamepad);
          SDL_JoystickID id = SDL_JoystickInstanceID(joy);

          if (event.cdevice.which == id) {
            SDL_GameControllerClose(gamepad);
            gamepad = NULL;
            printf("Controller Disconnected\n");
          }
          break;
        case SDL_CONTROLLERBUTTONDOWN:
          if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
            flappy.y -= FLAPPY_JUMP;
          }
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
