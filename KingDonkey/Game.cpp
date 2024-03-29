#include "Game.h"

Game::Game(SDL_Renderer* renderer, int playerX, int playerY, Coordinates platformCoordinates[], int numPlatforms,
    Coordinates ladderCoordinates[], int numLadders, Coordinates trophiesCoordinates[], int numTrophies)
    : player(new Player(renderer, playerX, playerY)), donkey(new Donkey(renderer, 160, 55)), textRenderer(renderer),
    platforms_number(numPlatforms), ladders_number(numLadders), trophies_number(numTrophies), 
    barrels_number(0), targetFrameTime(1000 / FPS), lastBarrelSpawnTime(SDL_GetTicks()), barrelSpawnInterval(6000), barrelSpawnPoint({ 280, 145 }), 
    totalGameTime(0.0), lives(3), level(1), points(0), showPointsMessage(false), pointsMessageStartTime(0), renderPointsMessage(false),
    livesSpritesCoordinates{ {1121, 10}, {1174, 10}, {1227, 10} } {

    platforms = new Sprite * [numPlatforms];
    ladders = new Sprite * [numLadders];
    trophies = new Sprite * [numTrophies];
    barrels = new Barrel * [MAX_BARRELS];
    livesSprites = new Sprite * [3];

    for (int i = 0; i < numPlatforms; ++i) {
        platforms[i] = new Sprite(renderer, "./graphics/platform.bmp", platformCoordinates[i].x, platformCoordinates[i].y);
    }

    for (int i = 0; i < numLadders; ++i) {
        ladders[i] = new Sprite(renderer, "./graphics/ladder85.bmp", ladderCoordinates[i].x, ladderCoordinates[i].y);
    }

    for (int i = 0; i < numTrophies; ++i) {
        trophies[i] = new Sprite(renderer, "./graphics/trophie.bmp", trophiesCoordinates[i].x, trophiesCoordinates[i].y);
    }

    for (int i = 0; i < 3; ++i) {
        livesSprites[i] = new Sprite(renderer, "./graphics/life.bmp", livesSpritesCoordinates[i].x, livesSpritesCoordinates[i].y);
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

    for (int i = 0; i < 3; ++i) {
        delete livesSprites[i];
    }
    delete[] livesSprites;
}

void Game::update(SDL_Renderer* renderer) {
    Uint32 currentFrameTime = SDL_GetTicks();
    deltaTime = currentFrameTime - lastFrameTime;

    player->handleInput();
    player->updateAnimations((float)(deltaTime) / SECOND);
    donkey->updateAnimations((float)(deltaTime) / SECOND);

    handleAllCollisions(renderer);

    if (showPointsMessage) {
        Uint32 elapsedTime = SDL_GetTicks() - pointsMessageStartTime;
        if (elapsedTime < SECOND) {
            renderPointsMessage = true;
        }
        else {
            renderPointsMessage = false;
			showPointsMessage = false;
		}
    }

    for (int i = 0; i < barrels_number; ++i) {
        barrels[i]->update((float)(deltaTime) / SECOND);
        if (!showPointsMessage && player->isOnGround() == false && barrels[i]->isPlayerJumpedOver(player->getHitbox())) {
            points += 100;

            showPointsMessage = true;
            pointsMessageStartTime = SDL_GetTicks();
        }
        if (barrels[i]->isOffScreen(SCREEN_HEIGHT)) {
            delete barrels[i];
            barrels[i] = barrels[--barrels_number];
        }
    }

    if (barrels_number < MAX_BARRELS && SDL_GetTicks() - lastBarrelSpawnTime >= barrelSpawnInterval - (2 * SECOND)) {
        donkey->setIsThrowing(true);
    }

    if (barrels_number < MAX_BARRELS && SDL_GetTicks() - lastBarrelSpawnTime >= barrelSpawnInterval) {
        spawnBarrel(renderer, barrelSpawnPoint.x, barrelSpawnPoint.y, BARREL_SPEED);
        lastBarrelSpawnTime = SDL_GetTicks();
        donkey->setIsThrowing(false);
    }

    totalGameTime += ((float)(deltaTime) / SECOND);

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
        barrels[i]->renderWithAngle(renderer);
    }
    for (int i = 0; i < trophies_number; ++i) {
		trophies[i]->render(renderer);
	}
    for (int i = 0; i < lives; i++) {
        livesSprites[i]->render(renderer);
    }
    donkey->render(renderer);
    player->render(renderer);
    if (renderPointsMessage == true) 
        textRenderer.drawPoints(player->getPosX() + 9, player->getPosY() - 30, 100);
}

void Game::restart(SDL_Renderer* renderer) {
    for (int i = 0; i < platforms_number; ++i) {
        delete platforms[i];
    }
    delete[] platforms;
    platforms = nullptr;

    for (int i = 0; i < ladders_number; ++i) {
        delete ladders[i];
    }
    delete[] ladders;
    ladders = nullptr;

    for (int i = 0; i < barrels_number; ++i) {
        delete barrels[i];
    }
    delete[] barrels;
    barrels = nullptr;
    barrels_number = 0;

    for (int i = 0; i < trophies_number; ++i) {
        delete trophies[i];
    }
    delete[] trophies;

    for (int i = 0; i < 3; ++i) {
        delete livesSprites[i];
    }
    delete[] livesSprites;
    livesSprites = nullptr;

    char levelFilePath[50];
    sprintf(levelFilePath, "./lvl%d.txt", level);
    int numPlatforms = loadPlatformsNumber(levelFilePath);
    int numLadders = loadLaddersNumber(levelFilePath);
    int numTrophies = loadTrophiesNumber(levelFilePath);

    Coordinates* platformCoordinates = new Coordinates[numPlatforms];
    Coordinates* ladderCoordinates = new Coordinates[numLadders];
    Coordinates* trophiesCoordinates = new Coordinates[numTrophies];

    loadPlatformCoordinates(levelFilePath, platformCoordinates, numPlatforms);
    loadLadderCoordinates(levelFilePath, ladderCoordinates, numLadders);
    loadTrophyCoordinates(levelFilePath, trophiesCoordinates, numTrophies);

    livesSpritesCoordinates[0] = { 1121, 10 };
    livesSpritesCoordinates[1] = { 1174, 10 };
    livesSpritesCoordinates[2] = { 1227, 10 };

    platforms_number = numPlatforms;
    ladders_number = numLadders;
    trophies_number = numTrophies;
    lastBarrelSpawnTime = SDL_GetTicks();

    platforms = new Sprite * [numPlatforms];
    ladders = new Sprite * [numLadders];
    trophies = new Sprite * [numTrophies];
    barrels = new Barrel * [MAX_BARRELS];
    livesSprites = new Sprite * [3];

    for (int i = 0; i < numPlatforms; ++i) {
        platforms[i] = new Sprite(renderer, "./graphics/platform.bmp", platformCoordinates[i].x, platformCoordinates[i].y);
    }

    for (int i = 0; i < numLadders; ++i) {
        ladders[i] = new Sprite(renderer, "./graphics/ladder85.bmp", ladderCoordinates[i].x, ladderCoordinates[i].y);
    }

    for (int i = 0; i < numTrophies; ++i) {
        trophies[i] = new Sprite(renderer, "./graphics/trophie.bmp", trophiesCoordinates[i].x, trophiesCoordinates[i].y);
    }

    for (int i = 0; i < 3; ++i) {
        livesSprites[i] = new Sprite(renderer, "./graphics/life.bmp", livesSpritesCoordinates[i].x, livesSpritesCoordinates[i].y);
    }

    lastFrameTime = SDL_GetTicks();

    player->setPos(200, 600);
    player->resetState();

    delete[] platformCoordinates;
    delete[] ladderCoordinates;
    delete[] trophiesCoordinates;
}

void Game::playerDie() {
    if (lives > 0) {
        delete livesSprites[lives - 1];
        livesSprites[lives - 1] = nullptr;
        lives--;
    }
}

void Game::restartPlayerLives() {
    lives = 3;
}

void Game::restartPlayerPoints() {
    points = 0;
}

void Game::increaseLevel() {
    level += 1;
    points += 1000;
}

void Game::restartLevel() {
    level = 1;
}

void Game::restartGameTime() {
    totalGameTime = 0.0;
}

void Game::setGameLevel(int level) {
	this->level = level;
}

bool Game::playerCompletedLevel() {
    if (player->getPosY() < WINNING_Y) {
        return true;
    }
    return false;
}

int Game::getPlayerLives() {
    return lives;
}

int Game::getPlayerPoints() {
    return points;
}

int Game::getGameLevel() {
	return level;
}

float Game::getGameTime() {
    return totalGameTime;
}

void Game::handleAllCollisions(SDL_Renderer* renderer) {
    bool onLadder = false;

    for (int i = 0; i < ladders_number; ++i) {
        player->handleLaddersCollision(ladders[i]);
        if (player->isOnLadder()) {
            onLadder = true;
            break;
        }
    }

    if (!onLadder) {
        for (int i = 0; i < platforms_number; ++i) {
            if (player->isColliding(platforms[i]->getHitbox())) {
                player->handlePlatformsCollision(platforms[i]->getHitbox());
            }
        }
    }
    for (int i = 0; i < barrels_number; i++) {
        if (barrels[i]->isColliding(player->getHitbox())) {
            playerDie();
            restart(renderer);
        }
    }

    for (int i = 0; i < barrels_number; i++) {
        for (int j = 0; j < platforms_number; ++j) {
            if (barrels[i]->isColliding(platforms[j]->getHitbox())) {
                barrels[i]->handlePlatformsCollision(platforms[j]->getHitbox());
                break;
            }
        }
    }

    for (int i = 0; i < trophies_number; i++) {
        if (player->isColliding(trophies[i]->getHitbox())) {
            delete trophies[i];
            trophies[i] = trophies[--trophies_number];
            points += 100;

            showPointsMessage = true;
            pointsMessageStartTime = SDL_GetTicks();
        }
    }

    if (player->isOffScreen()) {
        playerDie();
        restart(renderer);
    }
}

void Game::spawnBarrel(SDL_Renderer* renderer, int x, int y, float velocityX) {
    if (barrels_number < MAX_BARRELS) {
        barrels[barrels_number++] = new Barrel(renderer, "./graphics/barrel.bmp", x, y, velocityX);
    }
}