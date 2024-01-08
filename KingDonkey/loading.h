#ifndef LOADING_H
#define LOADING_H

#include "includes.h"

int loadPlatformsNumber(const char* filePath);
void loadPlatformCoordinates(const char* filePath, Coordinates platformCoordinates[], int numPlatforms);
int loadLaddersNumber(const char* filePath);
void loadLadderCoordinates(const char* filePath, Coordinates ladderCoordinates[], int numLadders);

#endif // LOADING_H
