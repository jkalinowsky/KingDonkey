#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "player.h"
#include "barrel.h"
#include "sprite.h"
#include "loading.h"
#include "Donkey.h"

class Game {
private:
    Player* player;
    Donkey donkey;
    Sprite** platforms;
    Sprite** ladders;
    Sprite** trophies;
    Barrel** barrels;

    Coordinates barrelSpawnPoint;

    int points;

    int platforms_number;
    int ladders_number;
    int barrels_number;

    Uint32 deltaTime; 
    Uint32 lastFrameTime;
    const Uint32 targetFrameTime;
    float totalGameTime;

    Uint32 lastBarrelSpawnTime;
    const Uint32 barrelSpawnInterval;

    int lives;

public:
    char gameTimeText[128];

    Game(SDL_Renderer* renderer, int playerX, int playerY,
        Coordinates platformCoordinates[], int numPlatforms,
        Coordinates ladderCoordinates[], int numLadders);

    ~Game();

    void update(SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);

    void restart(SDL_Renderer* renderer);

    void playerDie();

    int getPlayerLives();

    void restartPlayerLives();

    int getPlayerPoints();

private:
    void handleAllCollisions(SDL_Renderer* renderer);

    void spawnBarrel(SDL_Renderer* renderer, int x, int y, float velocityX);
};

#endif // GAME_H
