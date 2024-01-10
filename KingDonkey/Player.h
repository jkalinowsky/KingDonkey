#ifndef PLAYER_H
#define PLAYER_H

#include "includes.h"
#include "sprite.h"

class Player : public Sprite {
private:
    float velocityX;
    float velocityY;
    bool onGround;

    int currentFrame;
    int frameCounter;
    bool isClimbing;
    static const int FRAME_INTERVAL = FPS / 4;
    SDL_Texture* walkLeftTextures[3];
    SDL_Texture* walkRightTextures[3];
    SDL_Texture* jumpTextures[2];
    SDL_Texture* climbingTextures[4];
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

    bool isOnGround();

    void moveOnLadder(float deltaY);

    void handlePlatformsCollision(const SDL_Rect& otherRect);

    void handleLaddersCollision(Sprite* ladder);

    void resetState();

    void setPos(int x, int y);

    int getPosX();

    int getPosY();
};

#endif // !PLAYER_H