#include "Game.h"

void DrawString(SDL_Renderer* renderer, int x, int y, const char* text,
    SDL_Texture* charsetTexture) {
    int px, py, c;
    SDL_Rect s, d;
    s.w = 8;
    s.h = 8;
    d.w = 8;
    d.h = 8;

    while (*text) {
        c = *text & 255;
        px = (c % 16) * 8;
        py = (c / 16) * 8;
        s.x = px;
        s.y = py;
        d.x = x;
        d.y = y;

        SDL_Rect destRect = { x, y, 8, 8 };
        SDL_RenderCopy(renderer, charsetTexture, &s, &destRect);

        x += 8;
        text++;
    }
}


int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    // window and renderer creation and settings
    SDL_Window* window;
    SDL_Renderer* renderer;
    int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    if (rc != 0) {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetWindowTitle(window, "King Donkey");

    SDL_Surface* charset = SDL_LoadBMP("./graphics/cs8x8.bmp");
    
    SDL_Texture* charsetTexture = SDL_CreateTextureFromSurface(renderer, charset);
    SDL_FreeSurface(charset);

    // loading from file funcionality
    int numPlatforms = loadPlatformsNumber("./map.txt");
    int numLadders = loadLaddersNumber("./map.txt");

    Coordinates* platformCoordinates = new Coordinates[numPlatforms];
    Coordinates* ladderCoordinates = new Coordinates[numLadders];

    loadPlatformCoordinates("./map.txt", platformCoordinates, numPlatforms);
    loadLadderCoordinates("./map.txt", ladderCoordinates, numLadders);

    Game game(renderer, 200, 500, platformCoordinates, numPlatforms, ladderCoordinates, numLadders);


    // main loop and variables
    SDL_Event event;
    bool quitProgram = false;
    bool restartGame = false;

    while (!quitProgram) {
        // event handling
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quitProgram = true;
                }
                else if (event.key.keysym.sym == SDLK_n) {
                    restartGame = true;
                }
            }
        }

        if (restartGame) {
            game.restart(renderer);
            restartGame = false;
        }

        game.update(renderer);

        // rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        DrawString(renderer, 10, 10, game.gameTimeText, charsetTexture);

        game.render(renderer);

        SDL_RenderPresent(renderer);
    }

    // cleaning up
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}