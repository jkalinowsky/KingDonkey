#ifndef LOADING_H
#define LOADING_H

#include "defines.h"

int loadPlatformsNumber(const char* filePath);
void loadPlatformCoordinates(const char* filePath, Coordinates platformCoordinates[], int numPlatforms);
int loadLaddersNumber(const char* filePath);
void loadLadderCoordinates(const char* filePath, Coordinates ladderCoordinates[], int numLadders);
int loadTrophiesNumber(const char* filePath);
void loadTrophyCoordinates(const char* filePath, Coordinates trophyCoordinates[], int numTrophies);

#endif // LOADING_H
