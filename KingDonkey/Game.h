#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "player.h"
#include "barrel.h"
#include "sprite.h"

class Game {
private:
    Player player;
    Sprite** platforms;
    Sprite** ladders;
    Barrel** barrels;

    Coordinates barrelSpawnPoint;

    int platforms_number;
    int ladders_number;
    int barrels_number;

    Uint32 lastFrameTime;
    const Uint32 targetFrameTime;

    Uint32 lastBarrelSpawnTime;
    const Uint32 barrelSpawnInterval;

public:
    Game(SDL_Renderer* renderer, int playerX, int playerY, Coordinates platformCoordinates[], int numPlatforms, Coordinates ladderCoordinates[], int numLadders, Coordinates barrelSpawnPoint);

    ~Game();

    void update(SDL_Renderer* renderer);

    void render(SDL_Renderer* renderer);

private:
    void handleAllCollisions();

    void spawnBarrel(SDL_Renderer* renderer, int x, int y, float velocityX);
};

#endif // GAME_H
