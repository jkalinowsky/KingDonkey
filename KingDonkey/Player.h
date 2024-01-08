#ifndef PLAYER_H
#define PLAYER_H

#include "includes.h"
#include "sprite.h"

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

    Player(SDL_Renderer* renderer, int x, int y);

    SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* imagePath);

    void applyGravity(float deltaTime);

    bool isColliding(const SDL_Rect& otherRect) const;

    bool isCollidingY(const SDL_Rect& otherRect) const;

    void jump();

    void updateAnimations(float deltaTime);

    void handleInput();

    bool isOnLadder();

    void moveOnLadder(float deltaY);

    void handlePlatformsCollision(const SDL_Rect& otherRect);

    void handleLaddersCollision(Sprite* ladder);

    void resetState();

    void setPos(int x, int y);
};

#endif // !PLAYER_H