#ifndef DONKEY_H
#define DONKEY_H

#include "includes.h"
#include "Sprite.h"
class Donkey : public Sprite
{
private:
	SDL_Texture* standingTexture[4];
	SDL_Texture* throwingTexture[3];

	float animationTimer;
	int standingAnimationIndex;
	int throwingAnimationIndex;

public:
	bool isThrowing;

	Donkey(SDL_Renderer* renderer, int x, int y);

	SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* imagePath);

	void updateAnimations(float deltaTime);

};

#endif // !DONKEY_H