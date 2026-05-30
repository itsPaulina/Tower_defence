#include "Game.h"   // Includes the Game class declaration.
#include <cstdlib>  // Needed for std::srand.
#include <ctime>    // Needed for std::time.
#include <iostream> // Needed for std::cerr.



Game::Game()
    : window_(sf::VideoMode({1280, 720}), "Tower Defense"), // Create the main game window.
      gold_(60),                                            // Starting gold for the player.
      tileSize_(64.f),                                      // Size of one tile in pixels.
      cols_(1280 / 64),                                     // Number of columns in the map.
      rows_(720 / 64) {                                     // Number of rows in the map.
    std::srand(static_cast<unsigned>(std::time(nullptr)));  // Seed random generator using current time.

    path_ = {
        {0,3},{1,3},{2,3},{3,3},{4,3},{5,3},{6,3},{7,3},{8,3}, // First horizontal part of the path.
        {8,4},{8,5},{8,6},                                      // Move downward.
        {7,6},{6,6},{5,6},{4,6},{3,6},                         // Move left.
        {3,7},{3,8},{3,9},                                     // Move downward again.
        {4,9},{5,9},{6,9},{7,9},{8,9},{9,9},{10,9},{11,9},{12,9},{13,9},{14,9}, // Long horizontal section.
        {14,8},{14,7},{14,6},{14,5},{14,4}                     // Final upward part.
    };

    if (!grassTexture_.loadFromFile("photos/grass.png")) { // Try to load grass texture.
        std::cerr << "Could not load grass.png\n";         // Print error if loading fails.
    } else {
        grassTexture_.setRepeated(true);                   // Allow grass texture to repeat.
    }

    if (!castleTexture_.loadFromFile("photos/tower.png")) { // Try to load castle texture.
        std::cerr << "Could not load tower.png\n";          // Print error if loading fails.
    }
    else {
        castleSprite_ = std::make_unique<sf::Sprite>(castleTexture_); // Create castle sprite from texture.

        sf::Vector2f scale(
            180.f / static_cast<float>(castleTexture_.getSize().x), // Scale width to about 180 pixels.
            180.f / static_cast<float>(castleTexture_.getSize().y)  // Scale height to about 180 pixels.
        );
        castleSprite_->setScale(scale); // Apply scale to the castle sprite.

        sf::Vector2f origin(
            static_cast<float>(castleTexture_.getSize().x) * 0.5f, // Center origin in X.
            static_cast<float>(castleTexture_.getSize().y) * 0.5f  // Center origin in Y.
        );
        castleSprite_->setOrigin(origin); // Set sprite origin to its center.

        sf::Vector2i lastTile = path_.back();                  // Get the last tile of the enemy path.
        sf::Vector2f castlePos = tileCenter(lastTile.x, lastTile.y - 1); // Place castle near the end of the path.
        castleSprite_->setPosition(castlePos);                 // Set castle position.
    }

    if (!dirtTexture_.loadFromFile("photos/dirt4.png.preview.jpg")) { // Try to load dirt texture.
        std::cerr << "Could not load dirt texture\n";                 // Print error if loading fails.
    } else {
        dirtTexture_.setRepeated(true);                               // Allow dirt texture repeating.
    }

    if (!catapultTexture_.loadFromFile("photos/catapult.png")) { // Load catapult texture.
        std::cerr << "Could not load catapult.png\n";            // Print error if loading fails.
    }

    if (!cannonTexture_.loadFromFile("photos/cannon.png")) { // Load cannon texture.
        std::cerr << "Could not load cannon.png\n";           // Print error if loading fails.
    }

    if (!tankTexture_.loadFromFile("photos/tank.png")) { // Load tank texture.
        std::cerr << "Could not load tank.png\n";        // Print error if loading fails.
    }

    towerMenu_.setup(window_, catapultTexture_, cannonTexture_, tankTexture_); // Prepare tower selection menu.

    setupMenu();      // Set up difficulty menu.
    setupMainMenu();  // Set up main menu.
    setupWaves();     // Set up enemy waves.

    if (!backgroundMusic_.openFromFile("photos/music.ogg")) { // Try to load background music.
        std::cerr << "Could not load background.ogg\n";       // Print error if loading fails.
    } else {
        backgroundMusic_.setLooping(true); // Make music repeat continuously.
        backgroundMusic_.setVolume(50.f);  // Set music volume.
        backgroundMusic_.play();           // Start playing music.
    }
}



void Game::run() {
    while (window_.isOpen()) { // Main game loop runs while window is open.
        handleEvents();        // Process user input and window events.

        float dt = deltaClock_.restart().asSeconds(); // Get elapsed time since last frame.

        if (state_ == GameState::MainMenu) {      // If game is in main menu state,
            window_.clear(sf::Color(40, 40, 60)); // clear the window with a dark color.
            drawMainMenu();                       // Draw main menu.
            window_.display();                    // Show frame on screen.
            continue;                            // Skip the rest of the loop.
        }

        if (state_ == GameState::DifficultyMenu) { // If game is in difficulty menu state,
            window_.clear(sf::Color(40, 40, 60));  // clear the window.
            drawMenu();                            // Draw difficulty menu.
            window_.display();                     // Show frame.
            continue;                             // Skip update/render of gameplay.
        }

        update(dt); // Update game logic using delta time.

        window_.clear(); // Clear previous frame.
        render();        // Draw all game objects.
        window_.display(); // Present the final frame on screen.
    }
}