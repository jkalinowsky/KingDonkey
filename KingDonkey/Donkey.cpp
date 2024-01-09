#include "Donkey.h"

Donkey::Donkey(SDL_Renderer* renderer, int x, int y)
    : Sprite(renderer, "./graphics/donkey1.bmp", x, y), isThrowing(false),
    animationTimer(0.0f), standingAnimationIndex(0), throwingAnimationIndex(0)
{
    for (int i = 0; i < 4; i++) {
        char path[50];
        sprintf(path, "./graphics/donkey%d.bmp", i+1);

        standingTexture[i] = loadTexture(renderer, path);
    }
    for (int i = 0; i < 3; i++) {
        char path[50];
        sprintf(path, "./graphics/donkey%d.bmp", i+5);

        throwingTexture[i] = loadTexture(renderer, path);
    }
    texture = standingTexture[0];
}

SDL_Texture* Donkey::loadTexture(SDL_Renderer* renderer, const char* imagePath) {
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

void Donkey::updateAnimations(float deltaTime) {
    if (!isThrowing) {
        animationTimer += deltaTime;
        if (animationTimer >= 0.7f) {
            standingAnimationIndex = (standingAnimationIndex + 1) % 4;
            texture = standingTexture[standingAnimationIndex];
            animationTimer = 0.0f;
        }
    }
    else {
        animationTimer += deltaTime;
        if (animationTimer >= 0.66f) {
            texture = throwingTexture[throwingAnimationIndex];
            throwingAnimationIndex += 1;
            animationTimer = 0.0f;

            if (throwingAnimationIndex == 3) {
                isThrowing = false;
                throwingAnimationIndex = 0;
            }
        }
    }
}

