#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include "defines.h"

HighScore* loadHighScores(const char* fileName, int* numHighScores);
void bubbleSort(HighScore* highScores, int numHighScores);
void savePlayerHighscore(const char* nickname, int points, float time);

#endif // HIGHSCORES_H
