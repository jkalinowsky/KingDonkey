#include "Game.h"

Game::Game(SDL_Renderer* renderer, int playerX, int playerY, Coordinates platformCoordinates[], int numPlatforms, 
    Coordinates ladderCoordinates[], int numLadders)
    : player(renderer, playerX, playerY), donkey(renderer, 160, 55), platforms_number(numPlatforms), ladders_number(numLadders), 
    barrels_number(0), targetFrameTime(1000 / FPS), lastBarrelSpawnTime(SDL_GetTicks()),
    barrelSpawnInterval(8000), barrelSpawnPoint({280, 145}), totalGameTime(0.0) {

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

    player.handleInput();
    player.updateAnimations((float)(deltaTime) / 1000.0f);
    donkey.updateAnimations((float)(deltaTime) / 1000.0f);

    handleAllCollisions(renderer);

    for (int i = 0; i < barrels_number; ++i) {
        barrels[i]->update((float)(deltaTime) / 1000.0f);
        if (barrels[i]->isOffScreen(SCREEN_HEIGHT)) {
            delete barrels[i];
            barrels[i] = barrels[--barrels_number];
        }
    }

    if (barrels_number < MAX_BARRELS && SDL_GetTicks() - lastBarrelSpawnTime >= barrelSpawnInterval - 2000) {
        donkey.isThrowing = true;
    }

    if (barrels_number < MAX_BARRELS && SDL_GetTicks() - lastBarrelSpawnTime >= barrelSpawnInterval) {
        spawnBarrel(renderer, barrelSpawnPoint.x, barrelSpawnPoint.y, 100.0f);
        lastBarrelSpawnTime = SDL_GetTicks();
    }

    totalGameTime += (float)deltaTime / 1000.0f;
    snprintf(gameTimeText, sizeof(gameTimeText), "Time: %.1f s", totalGameTime);

    lastFrameTime = currentFrameTime;
    Uint32 remainingTime = (deltaTime < targetFrameTime) ? (targetFrameTime - deltaTime) : 0;

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
    donkey.render(renderer);
    player.render(renderer);
}

void Game::handleAllCollisions(SDL_Renderer* renderer) {
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
        if (barrels[i]->isColliding(player.rect)) {
            player.die();
            restart(renderer);
		}
	}

    for (int i = 0; i < barrels_number; i++) {
        for (int j = 0; j < platforms_number; ++j) {
            if (barrels[i]->isColliding(platforms[j]->rect)) {
                barrels[i]->handlePlatformsCollision(platforms[j]->rect);
                break;
            }
        }
    }
}

void Game::spawnBarrel(SDL_Renderer* renderer, int x, int y, float velocityX) {
    if (barrels_number < MAX_BARRELS) {
        barrels[barrels_number++] = new Barrel(renderer, "./graphics/barrel.bmp", x, y, velocityX);
    }
}

void Game::restart(SDL_Renderer* renderer) {
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

    int numPlatforms = loadPlatformsNumber("./map.txt");
    int numLadders = loadLaddersNumber("./map.txt");

    Coordinates* platformCoordinates = new Coordinates[numPlatforms];
    Coordinates* ladderCoordinates = new Coordinates[numLadders];

    loadPlatformCoordinates("./map.txt", platformCoordinates, numPlatforms);
    loadLadderCoordinates("./map.txt", ladderCoordinates, numLadders);

    platforms_number = numPlatforms;
    ladders_number = numLadders;
    barrels_number = 0;
    lastBarrelSpawnTime = SDL_GetTicks();

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

    player.setPos(200, 500);
    player.resetState();

    delete[] platformCoordinates;
    delete[] ladderCoordinates;
}


