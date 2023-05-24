#ifndef GAME_STATE
#define GAME_STATE

#include "stateManager.h"
#include <stdbool.h>

void GameStart(GSGLOBAL* gsGlobal);
void GameUpdate(GSGLOBAL* gsGlobal);
void GameDraw(GSGLOBAL* gsGlobal, u64 colour);
void GameEnd(GSGLOBAL* gsGlobal);

void GeneratePlayField();
int MaxMin(int* X, int* Y);
int MaxMax(int* X, int* Y);
int MinMin(int* X, int* Y);
int MinMax(int* X, int* Y);
float Gurvic(int* X, int* Y, int* value);

void GamePlaying();

extern StateManager GameState;

#endif
