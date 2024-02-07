#ifndef INCLUDES_H
#define INCLUDES_H

#define _CRT_SECURE_NO_WARNINGS 1

#include <SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   720
#define FPS 60
#define MAX_BARRELS 10
#define BARREL_SPEED 100.0f
#define BARREL_FALLING_SPEED 150.0f
#define DONKEY_ANIMATION_SPEED1 0.7F
#define DONKEY_ANIMATION_SPEED2 0.66F
#define SECOND 1000
#define WINNING_Y 50


typedef struct {
    int x;
    int y;
} Coordinates;

typedef struct {
    char nickname[20];
    int points;
    float time;
} HighScore;

enum GameState {
    PLAYING,
    PLAYER_DEAD,
    GAME_OVER,
    LEVEL_COMPLETED,
    GAME_COMPLETED
};

enum MenuState {
    MAIN_MENU,
	HIGHSCORES,
    STORY,
	LEVEL_SELECT,
    GAME_END,
    GAME_OVER_MENU,
};

#endif // INCLUDES_H