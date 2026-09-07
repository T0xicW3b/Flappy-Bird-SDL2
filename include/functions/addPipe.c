#include "header/header.h"

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
