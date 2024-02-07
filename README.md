# Donkey Kong Game with SDL Library in C/C++

This project is a recreation of the classic Nintendo game, Donkey Kong, implemented using the SDL (Simple DirectMedia Layer) library in C/C++. It was created as a part of the basics of programming project for computer science studies, with the objective of understanding fundamental programming concepts and game development principles.

## Game Features:

- **Complete Game Mechanics:** Includes player movement, barrels, Donkey Kong throwing barrels, scoring system, and multiple levels.
  
- **Custom Animations and Graphics:** All animations and graphics are handcrafted for this project to closely resemble the original Donkey Kong game.

- **Simple Level Design System:** The game employs a simple level design system where level layouts are read from a text file, allowing for easy modification and creation of new levels.

- **Custom Text Rendering System:** Developed a custom text rendering system to display game information, scores, and messages.

- **High Score Tracking:** The game tracks and saves high scores, displaying them in a hall of fame with a paging system for easy navigation. High scores are stored and retrieved from a file.

## How to Play:

1. **Controls:**
   - Use the arrow keys to move the player character (on platforms and ladders).
   - Press 'Space' to jump'
   - Use menu controls to navigate in menu.

2. **Objective:**
   - Navigate the player character through each level, avoiding barrels and collecting extra points.
   - Reach the top to rescue the princess while accumulating points along the way.

3. **Scoring:**
   - Points are awarded for various actions such as jumping over barrel, reaching the top or picking trophies, which are placed in various places.
   - The game keeps track of the highest scores achieved, you can save it with your nickname at the end of the game.

4. **Levels:**
   - Each level presents new challenges and obstacles.
   - Level layouts are defined in text files for easy customization and extension.

## Installation:

1. **Prerequisites:**
   - Ensure SDL library is installed on your system. Instructions for installation can be found at https://www.libsdl.org/download-2.0.php.
   - Compiler with C/C++ support (e.g., GCC, Clang).

2. **Clone the Repository:**
   ```
   git clone https://github.com/your_username/donkey_kong.git
   cd donkey_kong
   ```

3. **Compile and Run:**
   ```
   gcc -o donkey_kong main.c -lSDL2 -lSDL2_ttf -lSDL2_image
   ./donkey_kong
   ```
   
## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments
- This game was made by me for studies. It was inspired by original Nintendo game.
- Special thanks to the creators of the SDL library for providing a robust framework for game development.

# Enjoy the game and happy jumping! 🍄👲🏻🏰
