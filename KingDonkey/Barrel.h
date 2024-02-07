#ifndef BARREL_H
#define BARREL_H

#include "defines.h"
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
    void renderWithAngle(SDL_Renderer* renderer);
    void handlePlatformsCollision(const SDL_Rect& otherRect);
    void applyGravity(float deltaTime);
    bool isColliding(const SDL_Rect& otherRect) const;
    bool isOffScreen(int screenHeight) const;
    bool isPlayerJumpedOver(const SDL_Rect& playerRect);
};

#endif // !BARREL_H