#pragma once // Ensures this header file is included only once.

#include <SFML/Graphics.hpp> // Provides SFML graphics classes.
#include <memory>            // Provides smart pointers like std::unique_ptr.
#include <vector>            // Provides std::vector.
#include "GameObject.h"      // Base class for game objects.
#include "TowerMenu.h"       // Tower menu class.
#include <SFML/Audio.hpp>    // Provides SFML audio classes like sf::Music.



enum class Difficulty { // Defines available game difficulty levels.
    Easy,
    Medium,
    Hard
};


enum class GameState { // Defines the current state of the game.
    MainMenu,
    DifficultyMenu,
    Playing,
    GameOver,
    Victory
};


class Game {
public:
    Game();    // Constructor that initializes the game.
    void run(); // Starts the main game loop.


private:
    struct WaveData { // Stores data for one enemy wave.
        int slimes;      // Number of slimes in the wave.
        int goblins;     // Number of goblins in the wave.
        int wolves;      // Number of wolves in the wave.
        int towerLimit;  // Maximum number of towers allowed for this wave.
    };

    sf::Music backgroundMusic_; // Background music played during the game.
    GameState state_ = GameState::MainMenu; // Current game state, later used to switch between menus and gameplay.
    Difficulty selectedDifficulty_ = Difficulty::Medium; // Currently selected difficulty.

    sf::RenderWindow window_; // Main game window.
    sf::Font font_;           // Font used for text rendering.

    sf::RectangleShape easyButton_;   // Button for easy difficulty.
    sf::RectangleShape mediumButton_; // Button for medium difficulty.
    sf::RectangleShape hardButton_;   // Button for hard difficulty.
    sf::RectangleShape startButton_;  // Button for starting the game from menu.

    std::unique_ptr<sf::Text> menuTitle_;  // Title text in the difficulty menu.
    std::unique_ptr<sf::Text> easyText_;   // Label for easy button, smart pointer for automatic memory management.
    std::unique_ptr<sf::Text> mediumText_; // Label for medium button.
    std::unique_ptr<sf::Text> hardText_;   // Label for hard button.
    std::unique_ptr<sf::Text> startText_;  // Label for start button.

    sf::RectangleShape mainStartButton_; // Start button in the main menu.
    sf::RectangleShape exitButton_;      // Exit button in the main menu.

    std::unique_ptr<sf::Text> mainTitle_;     // Main menu title.
    std::unique_ptr<sf::Text> mainStartText_; // Main menu start text.
    std::unique_ptr<sf::Text> exitText_;      // Main menu exit text.

    int castleHP_ = 10;     // Current castle health points.
    int castleMaxHP_ = 10;  // Maximum castle health points.

    bool betweenLevels_ = false;     // True if the game is in a pause between levels.
    float levelPauseTimer_ = 0.f;    // Timer counting pause time.
    float levelPauseDuration_ = 2.5f; // How long the pause between levels lasts.

    sf::Texture castleTexture_;               // Texture for the castle sprite.
    std::unique_ptr<sf::Sprite> castleSprite_; // Castle sprite.

    int gold_ = 0;            // Current amount of player gold.
    int currentLevel_ = 1;    // Current level number.
    int maxLevels_ = 7;       // Maximum number of levels in the game.
    int towersPlaced_ = 0;    // Number of towers currently placed.
    int towerLimit_ = 2;      // Maximum towers allowed.
    int enemiesToSpawn_ = 0;  // Total number of enemies left to spawn.

    int slimesLeftToSpawn_ = 0;  // Number of slimes still waiting to spawn.
    int goblinsLeftToSpawn_ = 0; // Number of goblins still waiting to spawn.
    int wolvesLeftToSpawn_ = 0;  // Number of wolves still waiting to spawn.

    float spawnTimer_ = 0.f;     // Timer for enemy spawning.
    float spawnInterval_ = 1.f;  // Time between enemy spawns.

    bool levelInProgress_ = false; // True while a level is currently active.
    bool gameOver_ = false;        // True if the player lost.
    bool victory_ = false;         // True if the player won.

    float tileSize_;         // Size of one map tile.
    int cols_;               // Number of map columns.
    int rows_;               // Number of map rows.
    float menuWidth_ = 180.f; // Width of the side menu.

    sf::Clock deltaClock_; // Clock used to measure frame time.

    std::vector<std::unique_ptr<GameObject>> objects_; // All game objects stored dynamically.
    std::vector<sf::Vector2i> path_;                   // Enemy movement path.
    std::vector<sf::Vector2i> occupiedTiles_;         // Tiles already occupied by towers.
    std::vector<WaveData> waves_;                     // All wave configurations.

    sf::Texture grassTexture_;    // Texture for grass tiles.
    sf::Texture dirtTexture_;     // Texture for path tiles.
    sf::Texture catapultTexture_; // Texture for catapult tower.
    sf::Texture cannonTexture_;   // Texture for cannon tower.
    sf::Texture tankTexture_;     // Texture for tank tower.

    TowerMenu towerMenu_; // Tower selection and placement menu.

    void setupMenu();            // Creates and initializes the difficulty menu.
    void updateMenuColors();     // Updates button colors depending on selection.
    void handleMenuClick(sf::Vector2f mousePos); // Handles clicks in the difficulty menu.
    void applyDifficultySettings(); // Applies selected difficulty settings.

    void updateMenu(); // Updates menu logic.
    void drawMenu();   // Draws the difficulty menu.

    void handleEvents(); // Handles all window and input events.
    void handleMousePressed(const sf::Event::MouseButtonPressed& mb);   // Handles mouse press events.
    void handleMouseMoved(const sf::Event::MouseMoved& mm);             // Handles mouse movement.
    void handleMouseReleased(const sf::Event::MouseButtonReleased& mr); // Handles mouse release events.

    void update(float dt); // Updates the whole game.
    void render();         // Draws the whole game scene.
    void drawUI();         // Draws user interface elements.

    void setupWaves();        // Creates all enemy waves.
    void startLevel(int level); // Starts a selected level.
    void spawnNextEnemy();    // Spawns the next enemy.
    void checkLevelFinished(); // Checks whether the current level has ended.

    bool isPathTile(int col, int row) const;     // Checks if a tile belongs to the path.
    bool isTileOccupied(int col, int row) const; // Checks if a tile already has a tower.
    sf::Vector2f tileCenter(int col, int row) const; // Returns the center position of a tile.
    bool canPlaceTower(int col, int row) const;  // Checks if tower placement is allowed on a tile.
    void placeDraggedTower(int col, int row);    // Places the currently dragged tower.
    void drawCastleHP();                         // Draws castle health bar or text.

    void setupMainMenu();                    // Initializes the main menu.
    void drawMainMenu();                     // Draws the main menu.
    void handleMainMenuClick(sf::Vector2f mousePos); // Handles clicks in the main menu.
};