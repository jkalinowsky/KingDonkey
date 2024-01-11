#include "Game.h"
#include "TextRenderer.h"

void drawGUI(Game* game, TextRenderer* textRenderer,GameState gameState) {
    char livesText[20];
    char pointsText[20];
    snprintf(pointsText, sizeof(pointsText), "Points: %d", game->getPlayerPoints());

    textRenderer->drawGameUI(game->gameTimeText, pointsText);

    if (gameState == PLAYER_DEAD) {
        textRenderer->drawPlayerDied();
    }
}

void restartGameVariables(Game* game, MenuState menuState) {
    game->restartPlayerLives();
    game->restartPlayerPoints();
    if (menuState == MAIN_MENU) {
        game->restartLevel();
	}
    game->restartGameTime();
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

    // loading from file funcionality
    int numPlatforms = loadPlatformsNumber("./map.txt");
    int numLadders = loadLaddersNumber("./map.txt");
    int numTrophies = loadTrophiesNumber("./map.txt");

    Coordinates* platformCoordinates = new Coordinates[numPlatforms];
    Coordinates* ladderCoordinates = new Coordinates[numLadders];
    Coordinates* trophiesCoordinates = new Coordinates[numTrophies];

    loadPlatformCoordinates("./map.txt", platformCoordinates, numPlatforms);
    loadLadderCoordinates("./map.txt", ladderCoordinates, numLadders);
    loadTrophyCoordinates("./map.txt", trophiesCoordinates, numTrophies);

    Game game(renderer, 200, 600, platformCoordinates, numPlatforms, ladderCoordinates, numLadders, trophiesCoordinates, numTrophies);
    TextRenderer textRenderer(renderer);

    // main loop and variables
    SDL_Event event;
    bool quitProgram = false;
    bool restartGame = false;
    bool inMenu = true;
    int selectedLevel = 0;

    GameState gameState = PLAYING;
    MenuState menuState = MAIN_MENU;

    while (!quitProgram) {
        // event handling
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quitProgram = true;
                }
                else if (event.key.keysym.sym == SDLK_n) {
                    restartGame = true;
                    gameState = PLAYING;
                }
                else if (event.key.keysym.sym == SDLK_SPACE && gameState == PLAYER_DEAD) {
                    restartGame = true;
                } 
                else if (event.key.keysym.sym == SDLK_SPACE && game.playerCompletedLevel()) {
                    game.increaseLevel();
                    restartGame = true;
                }
                else if (event.key.keysym.sym == SDLK_l && inMenu) {
                    menuState = LEVEL_SELECT;
                }
                else if (event.key.keysym.sym == SDLK_h && inMenu) {
                    menuState = HIGHSCORES;
                }
                else if (event.key.keysym.sym == SDLK_s && inMenu) {
                    menuState = STORY;
                }
                else if (event.key.keysym.sym == SDLK_1 && menuState == LEVEL_SELECT) {
                    selectedLevel = 1;
                }
                else if (event.key.keysym.sym == SDLK_2 && menuState == LEVEL_SELECT) {
                    selectedLevel = 2;
                }
                else if (event.key.keysym.sym == SDLK_3 && menuState == LEVEL_SELECT) {
                    selectedLevel = 3;
                }
                else if (event.key.keysym.sym == SDLK_x && menuState != MAIN_MENU) {
					menuState = MAIN_MENU;
				}
            }
        }
        int previousLives = game.getPlayerLives();

        // rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        if (!inMenu && game.playerCompletedLevel()) {
            gameState = LEVEL_COMPLETED;
            textRenderer.drawLevelCompleted();
        }
        else if (!inMenu && game.getGameLevel() == 4) {
            gameState = GAME_COMPLETED;
        }

        if (!inMenu && gameState == PLAYING) {
            game.update(renderer);

            if (game.getPlayerLives() < previousLives) {
                previousLives = game.getPlayerLives();
                gameState = PLAYER_DEAD;
            }

            if (game.getPlayerLives() == 0) {
                gameState = GAME_OVER;
                inMenu = true;
            }
        }

        if (gameState == GAME_COMPLETED) {
            inMenu = true;
            menuState = GAME_END;
        }

        if (restartGame) {
            if (gameState != PLAYER_DEAD && gameState != LEVEL_COMPLETED) {
                restartGameVariables(&game, menuState);
            }
            game.restart(renderer);
            menuState = MAIN_MENU;
            restartGame = false;
            inMenu = false;
            gameState = PLAYING;
        }

        if (inMenu) {
            if (menuState == MAIN_MENU) {
                textRenderer.drawMenu();

                if (gameState == GAME_OVER) {
                    textRenderer.drawGameOver();
                }
            }
            else if (menuState == HIGHSCORES) {
            
            }
            else if (menuState == LEVEL_SELECT) {
				textRenderer.drawLevelSelect();
                if (selectedLevel == 1) {
                    game.setGameLevel(1);
                    restartGame = true;
                }
                else if (selectedLevel == 2) {
					game.setGameLevel(2);
					restartGame = true;
				}
                else if (selectedLevel == 3) {
					game.setGameLevel(3);
					restartGame = true;
				}
                textRenderer.drawBack();
			}
            else if (menuState == STORY) {
                textRenderer.drawStory();
                textRenderer.drawBack();
            }
            else if (menuState == GAME_END) {
                textRenderer.drawGameEnd(game.getPlayerPoints(), game.getGameTime());
            }
        }
        else {
            drawGUI(&game, &textRenderer, gameState);
            game.render(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    // cleaning up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}