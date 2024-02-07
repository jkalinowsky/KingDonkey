#ifndef PLAYER_H
#define PLAYER_H

#include "defines.h"
#include "sprite.h"

class Player : public Sprite {
private:
    SDL_Texture* walkLeftTextures[3];
    SDL_Texture* walkRightTextures[3];
    SDL_Texture* jumpTextures[2];
    SDL_Texture* climbingTextures[4];
    Sprite* currentLadder;
    int currentFrame;
    int frameCounter;
    static const int FRAME_INTERVAL = FPS / 4;
    float velocityX;
    float velocityY;
    bool onGround;
    bool isClimbing;
    bool isFacingLeft;
    bool onLadder;
public:
    static const float MOVE_SPEED;
    static const float JUMP_FORCE;
    static const float GRAVITY;

    Player(SDL_Renderer* renderer, int x, int y);
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* imagePath);

    void updateAnimations(float deltaTime);
    void handleInput();
    void applyGravity(float deltaTime);
    void jump();
    void moveOnLadder(float deltaY);
    void handlePlatformsCollision(const SDL_Rect& otherRect);
    void handleLaddersCollision(Sprite* ladder);
    void resetState();
    void setPos(int x, int y);
    int getPosX();
    int getPosY();
    bool isOffScreen();
    bool isColliding(const SDL_Rect& otherRect) const;
    bool isCollidingY(const SDL_Rect& otherRect) const;
    bool isOnLadder();
    bool isOnGround();
};

#endif // !PLAYER_H