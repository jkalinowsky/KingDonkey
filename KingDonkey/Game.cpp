#include "Game.h"

Game::Game(SDL_Renderer* renderer, int playerX, int playerY, Coordinates platformCoordinates[], int numPlatforms, Coordinates ladderCoordinates[], int numLadders, Coordinates barrelSpawnPoint)
    : player(renderer, playerX, playerY), platforms_number(numPlatforms), ladders_number(numLadders), barrels_number(0), targetFrameTime(1000 / FPS),
    lastBarrelSpawnTime(SDL_GetTicks()), barrelSpawnInterval(8000), barrelSpawnPoint(barrelSpawnPoint) {

    platforms = new Sprite * [numPlatforms];
    ladders = new Sprite * [numLadders];
    barrels = new Barrel * [MAX_BARRELS];

    for (int i = 0; i < numPlatforms; ++i) {
        platforms[i] = new Sprite(renderer, "./graphics/platform1.bmp", platformCoordinates[i].x, platformCoordinates[i].y);
    }

    for (int i = 0; i < numLadders; ++i) {
        ladders[i] = new Sprite(renderer, "./graphics/ladder85.bmp", ladderCoordinates[i].x, ladderCoordinates[i].y);
    }

    lastFrameTime = SDL_GetTicks();
}

Game::~Game() {
    for (int i = 0; i < platforms_number; ++i) {
        delete platforms[i];
    }
    delete[] platforms;

    for (int i = 0; i < ladders_number; ++i) {
        delete ladders[i];
    }
    delete[] ladders;

    for (int i = 0; i < barrels_number; ++i) {
        delete barrels[i];
    }
    delete[] barrels;
}

void Game::update(SDL_Renderer* renderer) {
    Uint32 currentFrameTime = SDL_GetTicks();
    Uint32 deltaTime = currentFrameTime - lastFrameTime;

    float deltaTimeSeconds = static_cast<float>(deltaTime) / 1000.0f;

    player.handleInput();
    player.updateAnimations(deltaTimeSeconds);

    for (int i = 0; i < barrels_number; ++i) {
        barrels[i]->update(deltaTimeSeconds);
        if (barrels[i]->isOffScreen(SCREEN_HEIGHT)) {
            delete barrels[i];
            barrels[i] = barrels[--barrels_number];
        }
    }

    handleAllCollisions();

    lastFrameTime = currentFrameTime;

    if (barrels_number < MAX_BARRELS && SDL_GetTicks() - lastBarrelSpawnTime >= barrelSpawnInterval) {
        spawnBarrel(renderer, barrelSpawnPoint.x, barrelSpawnPoint.y, 100.0f);
        lastBarrelSpawnTime = SDL_GetTicks();
    }

    Uint32 elapsedTime = SDL_GetTicks() - currentFrameTime;
    Uint32 remainingTime = (elapsedTime < targetFrameTime) ? (targetFrameTime - elapsedTime) : 0;

    if (remainingTime > 0) {
        SDL_Delay(remainingTime);
    }
}

void Game::render(SDL_Renderer* renderer) {
    for (int i = 0; i < ladders_number; ++i) {
        ladders[i]->render(renderer);
    }
    for (int i = 0; i < platforms_number; ++i) {
        platforms[i]->render(renderer);
    }
    for (int i = 0; i < barrels_number; ++i) {
        barrels[i]->render(renderer);
    }
    player.render(renderer);
}

void Game::handleAllCollisions() {
    bool onLadder = false;

    for (int i = 0; i < ladders_number; ++i) {
        player.handleLaddersCollision(ladders[i]);
        if (player.isOnLadder()) {
            onLadder = true;
            break;
        }
    }

    if (!onLadder) {
        for (int i = 0; i < platforms_number; ++i) {
            if (player.isColliding(platforms[i]->rect)) {
                player.handlePlatformsCollision(platforms[i]->rect);
            }
        }
    }

    for (int i = 0; i < barrels_number; i++) {
        for (int j = 0; j < platforms_number; ++j) {
            if (barrels[i]->isColliding(platforms[j]->rect)) {
                barrels[i]->handlePlatformsCollision(platforms[j]->rect);
            }
        }
    }
}

void Game::spawnBarrel(SDL_Renderer* renderer, int x, int y, float velocityX) {
    if (barrels_number < MAX_BARRELS) {
        barrels[barrels_number++] = new Barrel(renderer, "./graphics/barrel.bmp", x, y, velocityX);
    }
}
