#ifndef INCLUDES_H
#define INCLUDES_H

#define _CRT_SECURE_NO_WARNINGS 1

#include <SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   720
#define FPS 60
#define MAX_BARRELS 10


typedef struct {
    int x;
    int y;
} Coordinates;

#endif // INCLUDES_H