#include "TextRenderer.h"

TextRenderer::TextRenderer(SDL_Renderer* renderer)
    : renderer(renderer), charsetTexture(nullptr) {
    loadCharset();
}

TextRenderer::~TextRenderer() {
    if (charsetTexture != nullptr) {
        SDL_DestroyTexture(charsetTexture);
    }
}

void TextRenderer::loadCharset() {
    SDL_Surface* charset = SDL_LoadBMP("./graphics/cs8x8.bmp");

    if (charset != nullptr) {
        charsetTexture = SDL_CreateTextureFromSurface(renderer, charset);
        SDL_FreeSurface(charset);
    }
    else {
        printf("Failed to load charset texture!\n");
    }
}

void TextRenderer::drawString(int x, int y, int font, const char* text) {
    int px, py, c;
    SDL_Rect src = { 0, 0, 8, 8 };
    SDL_Rect dest = { x, y, 8 * font, 8 * font };

    for (int i = 0; text[i] != '\0'; i++) {
        c = text[i] & 255;
        src.x = (c % 16) * 8;
        src.y = (c / 16) * 8;
        dest.x = x + i * 8 * font;

        SDL_RenderCopy(renderer, charsetTexture, &src, &dest);
    }
}

void TextRenderer::drawMenu() {
    drawString(200, 200, 4, "Press 'N' to start a new game");
}

void TextRenderer::drawGameOver() {
    drawString(150, 100, 1, "You died. Try again");
}

void TextRenderer::drawPlayerDied() {
    drawString(508, 30, 1, "If you want to continue, press 'SPACE'");
}

void TextRenderer::drawLevelCompleted() {
    drawString(468, 30, 1, "Level completed! press 'SPACE' to continue.");
}

void TextRenderer::drawGameUI(const char* gameTimeText, const char* pointsText) {
    drawString(10, 10, 1, gameTimeText);
    drawString(1120, 60, 1, pointsText);
}

void TextRenderer::drawPoints(int x, int y, int points) {
	char pointsText[10];
	sprintf(pointsText, "%d", points);
	drawString(x, y, 1, pointsText);
}