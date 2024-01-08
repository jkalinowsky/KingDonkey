#include "Player.h"

const float Player::MOVE_SPEED = 200.0f;
const float Player::JUMP_FORCE = 200.0f;
const float Player::GRAVITY = 800.0f;

Player::Player(SDL_Renderer* renderer, int x, int y)
    : Sprite(renderer, "./graphics/player_walk_left1.bmp", x, y),
    velocityX(0), velocityY(0), isOnGround(false), currentFrame(0), onLadder(false), currentLadder(nullptr) {
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

SDL_Texture* Player::loadTexture(SDL_Renderer* renderer, const char* imagePath) {
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

void Player::applyGravity(float deltaTime) {
    if (!onLadder) {
        velocityY += GRAVITY * deltaTime;
    }
}

bool Player::isColliding(const SDL_Rect& otherRect) const {
    return SDL_HasIntersection(&rect, &otherRect);
}

bool Player::isCollidingY(const SDL_Rect& otherRect) const {
    return rect.y + rect.h > otherRect.y && rect.y < otherRect.y + otherRect.h;
}

void Player::jump() {
    if (isOnGround && !onLadder) {
        velocityY = -JUMP_FORCE;
        isOnGround = false;
    }
}

void Player::updateAnimations(float deltaTime) {
    rect.x += velocityX * deltaTime;
    rect.y += velocityY * deltaTime;

    applyGravity(deltaTime);

    frameCounter = (frameCounter + 1) % FRAME_INTERVAL;

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

    if (!isOnGround) {
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

void Player::handleInput() {
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

bool Player::isOnLadder() {
    return onLadder;
}

void Player::moveOnLadder(float deltaY) {
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

void Player::handlePlatformsCollision(const SDL_Rect& otherRect) {
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

void Player::handleLaddersCollision(Sprite* ladder) {
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
            moveOnLadder(-3.0f);
        }
        else if (state[SDL_SCANCODE_DOWN]) {
            moveOnLadder(3.0f);
        }
    }
    else {
        currentLadder = nullptr;
        onLadder = false;
    }
}

void Player::resetState() {
	velocityX = 0.0f;
	velocityY = 0.0f;
	isOnGround = false;
	onLadder = false;
	currentLadder = nullptr;
}

void Player::setPos(int x, int y) {
	rect.x = x;
	rect.y = y;
}
