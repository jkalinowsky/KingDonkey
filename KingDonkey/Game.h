#ifndef GAME_H
#define GAME_H

#include "defines.h"
#include "player.h"
#include "barrel.h"
#include "sprite.h"
#include "Loading.h"
#include "Donkey.h"
#include "TextRenderer.h" 
#include "StringUtils.h"

class Game {
private:
    Player* player;
    Donkey* donkey;
    Sprite** platforms;
    Sprite** ladders;
    Sprite** trophies;
    Sprite** livesSprites;
    Barrel** barrels;
    TextRenderer textRenderer;
    Coordinates barrelSpawnPoint, livesSpritesCoordinates[3];
    int lives, level, points;
    int trophies_number, platforms_number, ladders_number, barrels_number;
    float totalGameTime;
    bool renderPointsMessage;
    bool showPointsMessage;
    Uint32 deltaTime, lastFrameTime;
    Uint32 pointsMessageStartTime, lastBarrelSpawnTime;
    const Uint32 targetFrameTime;
    const Uint32 barrelSpawnInterval;
public:
    Game(SDL_Renderer* renderer, int playerX, int playerY,
        Coordinates platformCoordinates[], int numPlatforms,
        Coordinates ladderCoordinates[], int numLadders,
        Coordinates trophiesCoordinates[], int numTrophies);
    ~Game();

    void update(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    void restart(SDL_Renderer* renderer);
    void playerDie();
    void restartPlayerLives();
    void restartPlayerPoints();
    void increaseLevel();
    void restartLevel();
    void restartGameTime();
    void setGameLevel(int level);
    bool playerCompletedLevel();
    int getPlayerLives();
    int getPlayerPoints();
    int getGameLevel();
    float getGameTime();

private:
    void handleAllCollisions(SDL_Renderer* renderer);
    void spawnBarrel(SDL_Renderer* renderer, int x, int y, float velocityX);
};

#endif // GAME_H
