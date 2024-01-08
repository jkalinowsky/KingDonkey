#include "Barrel.h"

Barrel::Barrel(SDL_Renderer* renderer, const char* imagePath, float x, float y, float velocityX)
    : Sprite(renderer, imagePath, x, y), velocityX(velocityX), velocityY(0.0f), rotationAngle(0.0f) {}

void Barrel::update(float deltaTime) {
    applyGravity(deltaTime);

    rect.x += velocityX * deltaTime;
    rect.y += velocityY * deltaTime;

    rotationAngle += 180.0f * deltaTime;
    if (rotationAngle >= 360.0f) {
        rotationAngle -= 360.0f;
    }
}

void Barrel::render(SDL_Renderer* renderer) {
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, rotationAngle, NULL, SDL_FLIP_NONE);
}

bool Barrel::isColliding(const SDL_Rect& otherRect) const {
    return SDL_HasIntersection(&rect, &otherRect);
}

void Barrel::applyGravity(float deltaTime) {
    velocityY += Player::GRAVITY * deltaTime;
}

void Barrel::handlePlatformsCollision(const SDL_Rect& otherRect) {
    if (isColliding(otherRect)) {
        rect.y = otherRect.y - rect.h;
        velocityY = 0.0f;
    }
}

bool Barrel::isOffScreen(int screenHeight) const {
    return rect.y > screenHeight;
}
