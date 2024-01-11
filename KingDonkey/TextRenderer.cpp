#include "TextRenderer.h"

SDL_Color whiteColor = { 255, 255, 255 };
SDL_Color blueColor = { 0, 0, 255 };
SDL_Color greenColor = { 0, 255, 0 };
SDL_Color redColor = { 255, 0, 0 };

TextRenderer::TextRenderer(SDL_Renderer* renderer)
    : renderer(renderer), charsetTexture(nullptr), charWidth(8), charHeight(8) {
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
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        SDL_FreeSurface(charset);
    }
    else {
        printf("Failed to load charset texture!\n");
    }
}

void TextRenderer::drawString(int x, int y, float scale, const char* text, SDL_Color color) {
    int c;
    SDL_Rect src = { 0, 0, charWidth, charHeight };
    SDL_Rect dest = { x, y, charWidth * scale, charHeight * scale };

    SDL_SetTextureColorMod(charsetTexture, color.r, color.g, color.b);

    for (int i = 0; text[i] != '\0'; i++) {
        c = text[i] & 255;
        src.x = (c % 16) * charWidth;
        src.y = (c / 16) * charHeight;
        dest.x = x + i * charWidth * scale;

        SDL_RenderCopy(renderer, charsetTexture, &src, &dest);
    }
    SDL_SetTextureColorMod(charsetTexture, 255, 255, 255);
}

void TextRenderer::drawMenu() {
    drawString(160, 120, 4.0, "Press 'N' to start a new game.", greenColor);
    drawString(128, 240, 4.0, "Press 'H' to see the highscores.", whiteColor);
    drawString(192, 360, 4.0, "Press 'L' to select a level.", whiteColor);
    drawString(256, 480, 4.0, "Press 'S' to read story.", whiteColor);
    drawString(320, 600, 4.0, "Press 'ESC' to quit.", redColor);
}

void TextRenderer::drawGameOver() {
    drawString(412, 50, 3.0, "You died. Try again", redColor);
}

void TextRenderer::drawPlayerDied() {
    drawString(336, 30, 2.0, "If you want to continue, press 'SPACE'", redColor);
}

void TextRenderer::drawLevelCompleted() {
    drawString(296, 30, 2.0, "Level completed! press 'SPACE' to continue.", blueColor);
}

void TextRenderer::drawLevelSelect() {
    drawString(432, 100, 4.0, "Select level:", blueColor);
    drawString(304, 300, 3.0, "Press '1' to select level 1.", whiteColor);
	drawString(304, 410, 3.0, "Press '2' to select level 2.", whiteColor);
	drawString(304, 520, 3.0, "Press '3' to select level 3.", whiteColor);
}

void TextRenderer::drawBack() {
    drawString(10, 700, 2.0, "Press 'X' to go back.", greenColor);
}

void TextRenderer::drawStory() {
    drawString(124, 100, 1.5, "In one of a million different realities, Mario became a villain, constantly tormenting", whiteColor);
    drawString(148, 140, 1.5, "Donkey Kong. However, on a particular day, Mario was in an exceptionally bad mood,", whiteColor);
    drawString(172, 180, 1.5, "and it happened to be Donkey's birthday. As Donkey Kong peacefully planned his", whiteColor);
    drawString(124, 220, 1.5, "birthday party, enjoying the birthday cake with guests and about to open presents, the", whiteColor);
    drawString(214, 260, 1.5, "malicious Mario showed up, determined to ruin the birthday celebration.", whiteColor);
    drawString(344, 450, 2.0, "You are the one embodying this Mario!", redColor);
}

void TextRenderer::drawGameEnd(int points, float time) {
    char pointsText[10], timeText[6];
    sprintf(pointsText, "%d", points);
    sprintf(timeText, "%.2f", time);
    drawString(225, 50, 4.0, "You won! Congratulations!", greenColor);
    drawString(448, 100, 2.0, "Your points:", whiteColor);
    drawString(460, 120, 2.0, pointsText, whiteColor);
    drawString(768, 100, 2.0, "Your time:", whiteColor);
    drawString(780, 120, 2.0, timeText, whiteColor);
}

void TextRenderer::drawGameUI(const char* gameTimeText, const char* pointsText) {
    drawString(10, 10, 1, gameTimeText, whiteColor);
    drawString(1120, 60, 1, pointsText, whiteColor);
}

void TextRenderer::drawPoints(int x, int y, int points) {
	char pointsText[10];
	sprintf(pointsText, "%d", points);
	drawString(x, y, 1, pointsText, greenColor);
}