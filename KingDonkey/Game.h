#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "player.h"
#include "barrel.h"
#include "sprite.h"
#include "loading.h"
#include "Donkey.h"
#include "TextRenderer.h" 

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
    Coordinates barrelSpawnPoint;
    Coordinates livesSpritesCoordinates[3];
    int lives;
    int level;
    int points;
    int trophies_number;
    int platforms_number;
    int ladders_number;
    int barrels_number;
    float totalGameTime;
    bool renderPointsMessage;
    bool showPointsMessage;
    Uint32 deltaTime; 
    Uint32 lastFrameTime;
    Uint32 pointsMessageStartTime;
    Uint32 lastBarrelSpawnTime;
    const Uint32 targetFrameTime;
    const Uint32 barrelSpawnInterval;
public:
    char gameTimeText[128];

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
