#ifndef BARREL_H
#define BARREL_H

#include "includes.h"
#include "player.h"
#include "Sprite.h"

class Barrel : public Sprite {
private:
    float velocityX;
    float velocityY;
    float rotationAngle;

    int rotationDirection;
    bool isFalling;
    Uint32 fallingTime;
    const Uint32 FALLING_DELAY = 200;

public:
    Barrel(SDL_Renderer* renderer, const char* imagePath, float x, float y, float velocityX);

    void update(float deltaTime);

    void render(SDL_Renderer* renderer);

    bool isColliding(const SDL_Rect& otherRect) const;

    void applyGravity(float deltaTime);

    void handlePlatformsCollision(const SDL_Rect& otherRect);

    bool isOffScreen(int screenHeight) const;

    void setIsFalling(bool value);

    void setHasFallen(bool value);

    void setOnPlatform(bool value);
};

#endif // !BARREL_H