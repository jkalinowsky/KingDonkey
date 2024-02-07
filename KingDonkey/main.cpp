#include "Game.h"
#include "TextRenderer.h"
#include "HighScores.h"

void drawGUI(Game* game, TextRenderer* textRenderer, GameState gameState) {
    char livesText[20];
    char pointsText[20];
    char timeText[20];
    snprintf(pointsText, sizeof(pointsText), "Points: %d", game->getPlayerPoints());
    snprintf(timeText, sizeof(timeText), "Time: %.1f s", game->getGameTime());

    textRenderer->drawGameUI(timeText, pointsText);

    if (gameState == PLAYER_DEAD) {
        textRenderer->drawPlayerDied();
    }
}

void menuHandling(Game* game, TextRenderer* textRenderer, MenuState menuState, GameState gameState, int* selectedLevel,
    bool* restartGame, HighScore*& highScores, int& numHighScores, int totalPages, int scoresPerPage, int currentPage,
    const char* nickname, int cursorPosition) {
    if (menuState == MAIN_MENU) {
        textRenderer->drawMenu();
    }
    else if (menuState == HIGHSCORES) {
        delete[] highScores;

        numHighScores = 0;
        highScores = loadHighScores("./highscores.txt", &numHighScores);
        bubbleSort(highScores, numHighScores);
        textRenderer->drawHighscores(highScores, numHighScores, totalPages, scoresPerPage, currentPage);
        textRenderer->drawBack();
    }
    else if (menuState == LEVEL_SELECT) {
        textRenderer->drawLevelSelect();
        if (*selectedLevel == 1) {
            game->setGameLevel(1);
            *restartGame = true;
        }
        else if (*selectedLevel == 2) {
            game->setGameLevel(2);
            *restartGame = true;
        }
        else if (*selectedLevel == 3) {
            game->setGameLevel(3);
            *restartGame = true;
        }
        *selectedLevel = 0;
        textRenderer->drawBack();
    }
    else if (menuState == STORY) {
        textRenderer->drawStory();
        textRenderer->drawBack();
    }
    else if (menuState == GAME_END) {
        textRenderer->drawGameEnd(game->getPlayerPoints(), game->getGameTime(), gameState);
        textRenderer->drawNickname(nickname, cursorPosition);
    }
    else if (menuState == GAME_OVER_MENU) {
		textRenderer->drawGameOver();
	}
}


void playingGame(Game* game, SDL_Renderer* renderer, MenuState* menuState, GameState* gameState, bool* inMenu, int previousLives) {
    game->update(renderer);

    if (game->getPlayerLives() < previousLives) {
        previousLives = game->getPlayerLives();
        *gameState = PLAYER_DEAD;
    }

    if (game->getPlayerLives() == 0) {
        *gameState = GAME_OVER;
        *menuState = GAME_OVER_MENU;
        *inMenu = true;
    }
}

void restartingGame(Game* game, SDL_Renderer* renderer, MenuState& menuState, GameState& gameState, bool* restartGame, bool* inMenu) {
    if (gameState != PLAYER_DEAD && gameState != LEVEL_COMPLETED) {
        game->restartPlayerLives();
        game->restartPlayerPoints();
        game->restartGameTime();
    } 
    if (gameState != PLAYER_DEAD && gameState != LEVEL_COMPLETED && menuState != LEVEL_SELECT) {
        game->restartLevel();
    }
    game->restart(renderer);
    gameState = PLAYING;
    menuState = MAIN_MENU;
    *restartGame = false;
    *inMenu = false;
}

void handleMainMenuKeys(Game* game, SDL_Keycode sym, MenuState* menuState, GameState* gameState, bool* quitProgram, bool* inMenu,
    bool* restartGame, int* selectedLevel) {
    if (sym == SDLK_ESCAPE) {
        *quitProgram = true;
    }
    else if (sym == SDLK_n && *menuState != GAME_OVER_MENU) {
        *restartGame = true;
        *gameState = PLAYING;
    }
    else if (sym == SDLK_SPACE && *gameState == PLAYER_DEAD) {
        *restartGame = true;
    }
    else if (sym == SDLK_SPACE && game->playerCompletedLevel()) {
        game->increaseLevel();
        *restartGame = true;
    }
    else if (sym == SDLK_l && inMenu) {
        *menuState = LEVEL_SELECT;
    }
    else if (sym == SDLK_h && inMenu) {
        *menuState = HIGHSCORES;
    }
    else if (sym == SDLK_s && inMenu) {
        *menuState = STORY;
    }
    else if (*menuState == LEVEL_SELECT) {
        if (sym == SDLK_1) {
            *selectedLevel = 1;
        }
        else if (sym == SDLK_2) {
            *selectedLevel = 2;
        }
        else if (sym == SDLK_3) {
            *selectedLevel = 3;
        }
        else if (sym == SDLK_x) {
            *menuState = MAIN_MENU;
        }
    }
    else if (sym == SDLK_x && *menuState != MAIN_MENU) {
        *menuState = MAIN_MENU;
    }
    else if (*menuState == GAME_OVER_MENU) {
        if (sym == SDLK_y) {
            *menuState = GAME_END;
        }
        else if (sym == SDLK_n) {
            *menuState = MAIN_MENU;
            *gameState = PLAYING;
        }
    }
}

void handleHighScoresKeys(SDL_Keycode sym, MenuState& menuState, bool& quitProgram, int& currentPage, int totalPages) {
    if (sym == SDLK_LEFT && currentPage > 1) {
        currentPage--;
    }
    else if (sym == SDLK_RIGHT && currentPage < totalPages) {
        currentPage++;
    }
    else if (sym == SDLK_x) {
        menuState = MAIN_MENU;
    }
    else if (sym == SDLK_ESCAPE) {
        quitProgram = true;
    }
}

void handleEvents(Game* game, SDL_Event& event, MenuState* menuState, GameState* gameState, HighScore* highScores, bool* quitProgram, 
    bool* restartGame, bool* inMenu, int* selectedLevel, char* nickname, int& cursorPosition, int* currentPage, int* totalPages,
    int* numHighScores) {
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_KEYDOWN) {
            SDL_Keycode sym = event.key.keysym.sym;

            if (*menuState != GAME_END && *menuState != HIGHSCORES) {
                handleMainMenuKeys(game, sym, menuState, gameState, quitProgram, inMenu, restartGame, selectedLevel);
            }
            else if (*menuState == GAME_END) {
                if (sym == SDLK_RETURN) {
                    savePlayerHighscore(nickname, game->getPlayerPoints(), game->getGameTime());
                    *nickname = '\0';
                    *inMenu = true;
                    *gameState = PLAYING;
                    *menuState = MAIN_MENU;
                }
                else if (sym == SDLK_BACKSPACE) {
                    handleBackspace(nickname, cursorPosition);
                }
                else if (sym == SDLK_ESCAPE) {
					*quitProgram = true;
                }
            }
            else if (*menuState == HIGHSCORES) {
                handleHighScoresKeys(sym, *menuState, *quitProgram, *currentPage, *totalPages);
            }
        }
        else if (event.type == SDL_TEXTINPUT && *menuState == GAME_END) {
            if (calculateStringLength(nickname) + calculateStringLength(event.text.text) < 20) {
                appendString(nickname, event.text.text);
                cursorPosition += calculateStringLength(event.text.text);
            }
        }
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

    // loading from file funcionality
    int numPlatforms = loadPlatformsNumber("./lvl1.txt");
    int numLadders = loadLaddersNumber("./lvl1.txt");
    int numTrophies = loadTrophiesNumber("./lvl1.txt");

    Coordinates* platformCoordinates = new Coordinates[numPlatforms];
    Coordinates* ladderCoordinates = new Coordinates[numLadders];
    Coordinates* trophiesCoordinates = new Coordinates[numTrophies];

    loadPlatformCoordinates("./lvl1.txt", platformCoordinates, numPlatforms);
    loadLadderCoordinates("./lvl1.txt", ladderCoordinates, numLadders);
    loadTrophyCoordinates("./lvl1.txt", trophiesCoordinates, numTrophies);

    Game game(renderer, 100, 600, platformCoordinates, numPlatforms, ladderCoordinates, numLadders, trophiesCoordinates, numTrophies);
    TextRenderer textRenderer(renderer);

    int numHighScores;
    HighScore* highScores = loadHighScores("./highscores.txt", &numHighScores);
    bubbleSort(highScores, numHighScores);

    // main loop and variables
    SDL_Event event;
    bool quitProgram = false;
    bool restartGame = false;
    bool inMenu = true;
    int selectedLevel = 0;

    GameState gameState = PLAYING;
    MenuState menuState = MAIN_MENU;

    char nickname[20 + 1] = "";
    int cursorPosition = 0;

    int currentPage = 1;
    int scoresPerPage = 5;
    int totalPages = 0;

    while (!quitProgram) {
        // event handling
        handleEvents(&game, event, &menuState, &gameState, highScores, &quitProgram, &restartGame, &inMenu, 
            &selectedLevel, nickname, cursorPosition, &currentPage, &totalPages, &numHighScores);
        int previousLives = game.getPlayerLives();
        totalPages = (numHighScores + scoresPerPage - 1) / scoresPerPage;

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
            game.setGameLevel(1);
        }

        if (!inMenu && gameState == PLAYING) {
            playingGame(&game, renderer, &menuState, &gameState, &inMenu, previousLives);
        }

        if (gameState == GAME_COMPLETED) {
            inMenu = true;
            menuState = GAME_END;
        }

        if (restartGame) {
            restartingGame(&game, renderer, menuState, gameState, &restartGame, &inMenu);
        }

        if (inMenu) {
            menuHandling(&game, &textRenderer, menuState, gameState, &selectedLevel, &restartGame,
                highScores, numHighScores, totalPages, scoresPerPage, currentPage, nickname, cursorPosition);
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