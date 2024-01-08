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

public:
    Barrel(SDL_Renderer* renderer, const char* imagePath, float x, float y, float velocityX);

    void update(float deltaTime);

    void render(SDL_Renderer* renderer);

    bool isColliding(const SDL_Rect& otherRect) const;

    void applyGravity(float deltaTime);

    void handlePlatformsCollision(const SDL_Rect& otherRect);

    bool isOffScreen(int screenHeight) const;

    void setOnPlatform(bool value);
};

#endif // !BARREL_H