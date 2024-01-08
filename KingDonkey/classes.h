#ifndef CLASSES_H
#define CLASSES_H

#include "loading.h"
#include "includes.h"

class Sprite {
public:
    SDL_Rect rect;
    SDL_Texture* texture;

    Sprite() : texture(nullptr) {}

    Sprite(SDL_Renderer* renderer, const char* imagePath, float x, float y) {
        this->rect.x = x;
        this->rect.y = y;

        SDL_Surface* surface = SDL_LoadBMP(imagePath);
        if (!surface) {
            printf("Error loading texture: %s\n", SDL_GetError());
            return;
        }

        this->texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!this->texture) {
            printf("Error creating texture: %s\n", SDL_GetError());
            SDL_FreeSurface(surface);
            return;
        }
        SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
        this->rect.w = surface->w;
        this->rect.h = surface->h;
        SDL_FreeSurface(surface);
    }


    ~Sprite() {
        SDL_DestroyTexture(texture);
    }

    void render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
};

class Player : public Sprite {
private:
    float velocityX;
    float velocityY;
    bool isOnGround;

    int currentFrame;
    int frameCounter;
    static const int FRAME_INTERVAL = FPS / 4;
    SDL_Texture* walkLeftTextures[3];
    SDL_Texture* walkRightTextures[3];
    SDL_Texture* jumpTextures[2];
    bool isFacingLeft;

    bool onLadder;
    Sprite* currentLadder;

public:
    static const float MOVE_SPEED;
    static const float JUMP_FORCE;
    static const float GRAVITY;

    Player(SDL_Renderer* renderer, int x, int y) : Sprite(renderer, "./graphics/player_walk_left1.bmp", x, y),
        velocityX(0), velocityY(0), isOnGround(false), currentFrame(0), onLadder(false), currentLadder(nullptr)
    {
        for (int i = 0; i < 3; ++i) {
            char walkLeftPath[50];
            char walkRightPath[50];
            sprintf(walkLeftPath, "./graphics/player_walk_left%d.bmp", i + 1);
            sprintf(walkRightPath, "./graphics/player_walk_right%d.bmp", i + 1);

            walkLeftTextures[i] = loadTexture(renderer, walkLeftPath);
            walkRightTextures[i] = loadTexture(renderer, walkRightPath);
        }
        jumpTextures[0] = loadTexture(renderer, "./graphics/player_jump_left.bmp");
        jumpTextures[1] = loadTexture(renderer, "./graphics/player_jump_right.bmp");
        texture = walkRightTextures[1];
        isFacingLeft = false;
    }

    SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* imagePath) {
        SDL_Surface* surface = SDL_LoadBMP(imagePath);
        if (!surface) {
            printf("Error loading texture: %s\n", SDL_GetError());
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            printf("Error creating texture: %s\n", SDL_GetError());
            SDL_FreeSurface(surface);
            return nullptr;
        }

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(surface);

        return texture;
    }

    void applyGravity(float deltaTime) {
        if (!onLadder) {
            velocityY += GRAVITY * deltaTime;
        }
    }

    bool isColliding(const SDL_Rect& otherRect) const {
        return SDL_HasIntersection(&rect, &otherRect);
    }

    bool isCollidingY(const SDL_Rect& otherRect) const {
        return rect.y + rect.h > otherRect.y && rect.y < otherRect.y + otherRect.h;
    }

    void jump() {
        if (isOnGround && !onLadder) {
            velocityY = -JUMP_FORCE;
            isOnGround = false;
        }
    }

    void updateAnimations(float deltaTime) {
        rect.x += velocityX * deltaTime;
        rect.y += velocityY * deltaTime;

        applyGravity(deltaTime);

        frameCounter = (frameCounter + 1) % FRAME_INTERVAL;

        if (isOnGround) {
            if (velocityX > 0) {
                texture = walkRightTextures[currentFrame];
                isFacingLeft = false;
            }
            else if (velocityX < 0) {
                texture = walkLeftTextures[currentFrame];
                isFacingLeft = true;
            }
            else {
                if (isFacingLeft) {
                    texture = walkLeftTextures[1];
                }
                else {
                    texture = walkRightTextures[1];
                }
            }
        }
        else {
            if (isFacingLeft) {
                texture = jumpTextures[0];
            }
            else {
                texture = jumpTextures[1];
            }
        }

        if (frameCounter == 0) {
            currentFrame = (currentFrame + 1) % 3;
        }
    }

    void handleInput() {
        const Uint8* state = SDL_GetKeyboardState(nullptr);

        if (state[SDL_SCANCODE_LEFT]) {
            velocityX = -MOVE_SPEED;
        }
        else if (state[SDL_SCANCODE_RIGHT]) {
            velocityX = MOVE_SPEED;
        }
        if (state[SDL_SCANCODE_SPACE]) {
            jump();
        }
        if (!state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]) {
            velocityX = 0.0f;
        }
    }

    bool isOnLadder() {
        return onLadder;
    }

    void moveOnLadder(float deltaY) {
        if (currentLadder) {
            if (deltaY > 0 && rect.y + rect.h < currentLadder->rect.y + currentLadder->rect.h) {
                rect.y += deltaY;
                rect.x = currentLadder->rect.x + (currentLadder->rect.w - rect.w) / 2;
            }
            else if (deltaY < 0 && rect.y > currentLadder->rect.y) {
                rect.y += deltaY;
                rect.x = currentLadder->rect.x + (currentLadder->rect.w - rect.w) / 2;
            }

            if (deltaY < 0 && rect.y <= currentLadder->rect.y) {
                rect.y = currentLadder->rect.y - rect.h;
                isOnGround = true;
            }
        }
    }

    void handlePlatformsCollision(const SDL_Rect& otherRect) {
        if (isCollidingY(otherRect) && onLadder == false) {
            int overlapTop = otherRect.y + otherRect.h - rect.y;
            int overlapBottom = rect.y + rect.h - otherRect.y;

            if (overlapTop > overlapBottom) {
                // Colliding from the top
                rect.y = otherRect.y - rect.h;
                velocityY = 0.0f;
                isOnGround = true;
            }
            else {
                // Colliding from the bottom
                rect.y = otherRect.y + otherRect.h;
                velocityY = 0.0f;
            }
        }
    }

    void handleLaddersCollision(Sprite* ladder) {
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        SDL_Rect ladderRect = ladder->rect;
        int playerMiddleX = rect.x + rect.w / 2;
        int ladderMiddleX = ladderRect.x + ladderRect.w / 2;
        int xRange = 10;


        if (isColliding(ladderRect) && playerMiddleX >= ladderMiddleX - xRange && playerMiddleX <= ladderMiddleX + xRange) {
            currentLadder = ladder;
            onLadder = true;
            isOnGround = false;

            velocityY = 0.0f;

            if (state[SDL_SCANCODE_UP]) {
                moveOnLadder(-7.0f);
            }
            else if (state[SDL_SCANCODE_DOWN]) {
                moveOnLadder(7.0f);
            }
        }
        else {
            currentLadder = nullptr;
            onLadder = false;
        }
    }
};

const float Player::MOVE_SPEED = 200.0f;
const float Player::JUMP_FORCE = 200.0f;
const float Player::GRAVITY = 800.0f;

class Barrel : public Sprite {
private:
    float velocityX;
    float velocityY;
    bool isFalling;
    bool hasFallen;
    float rotationAngle;

public:
    Barrel(SDL_Renderer* renderer, const char* imagePath, float x, float y, float velocityX)
        : Sprite(renderer, imagePath, x, y), velocityX(velocityX), velocityY(0.0f), isFalling(false), rotationAngle(0.0f) {}

    void update(float deltaTime, SDL_Rect& playerRect) {
        if (!isFalling) {
        	rect.x += velocityX * deltaTime;
        }
        else {
            velocityY += Player::GRAVITY * deltaTime;
            rect.y += velocityY * deltaTime;
        }
        rotationAngle += 180.0f * deltaTime;
        if (rotationAngle >= 360.0f) {
            rotationAngle -= 360.0f;
        }
    }

    void render(SDL_Renderer* renderer) {
        SDL_RenderCopyEx(renderer, texture, NULL, &rect, rotationAngle, NULL, SDL_FLIP_NONE);
    }

private:
    bool isOffScreen(int screenHeight) {
		return rect.y > screenHeight;
	}
};

class Game {
private:
    Player player;
    Sprite** platforms;
    Sprite** ladders;
    Barrel** barrels;

    int platforms_number;
    int ladders_number;
    int barrels_number;

    Uint32 lastFrameTime;
    const Uint32 targetFrameTime;

public:
    Game(SDL_Renderer* renderer, int playerX, int playerY, Coordinates platformCoordinates[], int numPlatforms, Coordinates ladderCoordinates[], int numLadders, Coordinates barrelSpawnPoint)
        : player(renderer, playerX, playerY), platforms_number(numPlatforms), ladders_number(numLadders), barrels_number(0), targetFrameTime(1000 / FPS)
    {

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
    };

    ~Game() {
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

    void update() {
        Uint32 currentFrameTime = SDL_GetTicks();
        Uint32 deltaTime = currentFrameTime - lastFrameTime;

        float deltaTimeSeconds = static_cast<float>(deltaTime) / 1000.0f;

        player.handleInput();
        player.updateAnimations(deltaTimeSeconds);

        handleAllCollisions();

        lastFrameTime = currentFrameTime;

        Uint32 elapsedTime = SDL_GetTicks() - currentFrameTime;
        Uint32 remainingTime = (elapsedTime < targetFrameTime) ? (targetFrameTime - elapsedTime) : 0;

        if (remainingTime > 0) {
            SDL_Delay(remainingTime);
        }
    }

    void render(SDL_Renderer* renderer) {
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

private:
    void handleAllCollisions() {
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
    }
};

#endif // CLASSES_H