#ifndef SPRITE_H
#define SPRITE_H

#include "defines.h"

class Sprite {
protected:
    SDL_Rect rect;
    SDL_Texture* texture;
public:
    Sprite(SDL_Renderer* renderer, const char* imagePath, float x, float y);
    ~Sprite();

    SDL_Rect getHitbox();
    void render(SDL_Renderer* renderer);
};

#endif // !SPRITE_H