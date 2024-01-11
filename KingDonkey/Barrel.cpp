#include "Barrel.h"

Barrel::Barrel(SDL_Renderer* renderer, const char* imagePath, float x, float y, float velocityX)
    : Sprite(renderer, imagePath, x, y), velocityX(velocityX), velocityY(0.0f), rotationAngle(0.0f), isFalling(false), fallingTime(0), FALLING_DELAY(200), rotationDirection(1) {}

void Barrel::update(float deltaTime) {
    applyGravity(deltaTime);

    if (velocityY > 150.0f) {
        if (!isFalling) {
            fallingTime = SDL_GetTicks();
        }
        isFalling = true;
    }
    else {
        isFalling = false;
    }

    if (isFalling) {
        Uint32 currentTime = SDL_GetTicks();

        if (currentTime - fallingTime >= FALLING_DELAY) {
            velocityX = -velocityX;
            isFalling = false;
            rotationDirection = -rotationDirection;
        }
    }

    if (velocityX > 0.0f) {
        velocityX = 200.0f;
    }
    else {
        velocityX = -100.0f;
    }

    rect.x += velocityX * deltaTime;
    rect.y += velocityY * deltaTime;

    rotationAngle += rotationDirection * 180.0f * deltaTime;
    if (rotationAngle >= 360.0f) {
        rotationAngle -= 360.0f;
    }
    else if (rotationAngle < 0.0f) {
        rotationAngle += 360.0f;
    }
}

void Barrel::renderWithAngle(SDL_Renderer* renderer) {
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, rotationAngle, NULL, SDL_FLIP_NONE);
}

void Barrel::handlePlatformsCollision(const SDL_Rect& otherRect) {
    isFalling = false;
    rect.y = otherRect.y - rect.h;
    velocityY = 0.0f;
}

void Barrel::applyGravity(float deltaTime) {
    velocityY += Player::GRAVITY * deltaTime;
}

bool Barrel::isColliding(const SDL_Rect& otherRect) const {
    return SDL_HasIntersection(&rect, &otherRect);
}

bool Barrel::isOffScreen(int screenHeight) const {
    return rect.y > screenHeight;
}

bool Barrel::isPlayerJumpedOver(const SDL_Rect& playerRect) {
    if (velocityX > 0) {
        return ((playerRect.y < rect.y && playerRect.y > rect.y - 80) && (playerRect.x >= rect.x && playerRect.x <= rect.x + 10));
    }
    else {
		return ((playerRect.y < rect.y && playerRect.y > rect.y - 80) && (playerRect.x >= rect.x + rect.w - 10 && playerRect.x <= rect.x + rect.w));
	}
}