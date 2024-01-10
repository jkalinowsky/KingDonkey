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

    int points;

    int trophies_number;
    int platforms_number;
    int ladders_number;
    int barrels_number;

    Uint32 deltaTime; 
    Uint32 lastFrameTime;
    const Uint32 targetFrameTime;
    float totalGameTime;

    bool renderPointsMessage;
    bool showPointsMessage;
    Uint32 pointsMessageStartTime;

    Uint32 lastBarrelSpawnTime;
    const Uint32 barrelSpawnInterval;

    int lives;
    int level;
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

    int getPlayerLives();

    void restartPlayerLives();

    int getPlayerPoints();

    void restartPlayerPoints();

    bool playerCompletedLevel();

    void increaseLevel();

    void restartLevel();

    void restartGameTime();

private:
    void handleAllCollisions(SDL_Renderer* renderer);

    void spawnBarrel(SDL_Renderer* renderer, int x, int y, float velocityX);
};

#endif // GAME_H
