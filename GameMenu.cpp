#include "Game.h"    // Includes the Game class declaration.
#include <iostream>  // Needed for std::cerr.


void Game::updateMenuColors() {
    easyButton_.setFillColor(sf::Color(180, 180, 180));   // Set default color for easy button.
    mediumButton_.setFillColor(sf::Color(180, 180, 180)); // Set default color for medium button.
    hardButton_.setFillColor(sf::Color(180, 180, 180));   // Set default color for hard button.

    if (selectedDifficulty_ == Difficulty::Easy) { // Highlight easy if it is selected.
        easyButton_.setFillColor(sf::Color(100, 220, 100));
    }
    else if (selectedDifficulty_ == Difficulty::Medium) { // Highlight medium if it is selected.
        mediumButton_.setFillColor(sf::Color(255, 220, 100));
    }
    else if (selectedDifficulty_ == Difficulty::Hard) { // Highlight hard if it is selected.
        hardButton_.setFillColor(sf::Color(220, 100, 100));
    }
}


void Game::handleMenuClick(sf::Vector2f mousePos) {
    if (easyButton_.getGlobalBounds().contains(mousePos)) { // Check if easy button was clicked.
        selectedDifficulty_ = Difficulty::Easy;             // Select easy difficulty.
        updateMenuColors();                                 // Refresh button colors.
    }
    else if (mediumButton_.getGlobalBounds().contains(mousePos)) { // Check if medium button was clicked.
        selectedDifficulty_ = Difficulty::Medium;                  // Select medium difficulty.
        updateMenuColors();                                        // Refresh colors.
    }
    else if (hardButton_.getGlobalBounds().contains(mousePos)) { // Check if hard button was clicked.
        selectedDifficulty_ = Difficulty::Hard;                  // Select hard difficulty.
        updateMenuColors();                                      // Refresh colors.
    }
    else if (startButton_.getGlobalBounds().contains(mousePos)) { // Check if start button was clicked.
        applyDifficultySettings(); // Apply settings based on chosen difficulty.
        startLevel(1);             // Start the first level.
        state_ = GameState::Playing; // Change game state to playing.
        deltaClock_.restart();       // Reset frame timer.
    }
}


void Game::applyDifficultySettings() {
    if (selectedDifficulty_ == Difficulty::Easy) { // Settings for easy mode.
        gold_ = 180;         // More starting gold.
        spawnInterval_ = 1.2f; // Slower enemy spawning.
        castleMaxHP_ = 10;   // More castle health.
        castleHP_ = 10;      // Reset current castle health.
    }
    else if (selectedDifficulty_ == Difficulty::Medium) { // Settings for medium mode.
        gold_ = 110;
        spawnInterval_ = 0.9f;
        castleMaxHP_ = 8;
        castleHP_ = 8;
    }
    else if (selectedDifficulty_ == Difficulty::Hard) { // Settings for hard mode.
        gold_ = 80;          // Less starting gold.
        spawnInterval_ = 0.7f; // Faster enemy spawning.
        castleMaxHP_ = 6;    // Lower castle health.
        castleHP_ = 6;       // Reset current castle health.
    }
}


void Game::drawMenu() {
    window_.draw(*menuTitle_);   // Draw menu title.
    window_.draw(easyButton_);   // Draw easy button.
    window_.draw(mediumButton_); // Draw medium button.
    window_.draw(hardButton_);   // Draw hard button.
    window_.draw(startButton_);  // Draw start button.
    window_.draw(*easyText_);    // Draw easy label.
    window_.draw(*mediumText_);  // Draw medium label.
    window_.draw(*hardText_);    // Draw hard label.
    window_.draw(*startText_);   // Draw start label.
}

void Game::drawMainMenu() {
    window_.draw(*mainTitle_);      // Draw main menu title.
    window_.draw(mainStartButton_); // Draw start button.
    window_.draw(exitButton_);      // Draw exit button.
    window_.draw(*mainStartText_);  // Draw start text.
    window_.draw(*exitText_);       // Draw exit text.
}


void Game::handleMainMenuClick(sf::Vector2f mousePos) {
    if (mainStartButton_.getGlobalBounds().contains(mousePos)) { // Check if start button was clicked.
        state_ = GameState::DifficultyMenu; // Open difficulty selection menu.
    }
    else if (exitButton_.getGlobalBounds().contains(mousePos)) { // Check if exit button was clicked.
        window_.close(); // Close the game window.
    }
}


void Game::setupMainMenu() {
    const float centerX = static_cast<float>(window_.getSize().x) / 2.f; // Horizontal center of the window.

    mainTitle_ = std::make_unique<sf::Text>(font_, "Tower Defense", 56); // Create main title text.
    mainStartText_ = std::make_unique<sf::Text>(font_, "Start", 30);     // Create start button text.
    exitText_ = std::make_unique<sf::Text>(font_, "Exit", 30);           // Create exit button text.

    mainStartButton_.setSize({220.f, 70.f}); // Set start button size.
    exitButton_.setSize({220.f, 70.f});      // Set exit button size.

    mainStartButton_.setPosition({centerX - 110.f, 280.f}); // Place start button in the center.
    exitButton_.setPosition({centerX - 110.f, 390.f});      // Place exit button below it.

    mainStartButton_.setFillColor(sf::Color(70, 130, 70)); // Set start button color.
    exitButton_.setFillColor(sf::Color(140, 60, 60));      // Set exit button color.

    mainTitle_->setFillColor(sf::Color::White);     // Set title text color.
    mainStartText_->setFillColor(sf::Color::White); // Set start text color.
    exitText_->setFillColor(sf::Color::White);      // Set exit text color.

    sf::FloatRect titleBounds = mainTitle_->getLocalBounds(); // Get local bounds of title text.
    mainTitle_->setOrigin({
        titleBounds.position.x + titleBounds.size.x / 2.f, // Center title origin in X.
        titleBounds.position.y + titleBounds.size.y / 2.f  // Center title origin in Y.
    });
    mainTitle_->setPosition({centerX, 170.f}); // Position title near the top center.

    auto centerTextInButton = [](sf::Text& text, const sf::RectangleShape& button) { // Helper lambda to center text inside a button.
        sf::FloatRect textBounds = text.getLocalBounds();   // Get text bounds.
        sf::FloatRect buttonBounds = button.getGlobalBounds(); // Get button bounds.

        text.setOrigin({
            textBounds.position.x + textBounds.size.x / 2.f, // Center text origin in X.
            textBounds.position.y + textBounds.size.y / 2.f  // Center text origin in Y.
        });

        text.setPosition({
            buttonBounds.position.x + buttonBounds.size.x / 2.f, // Move text to horizontal center of button.
            buttonBounds.position.y + buttonBounds.size.y / 2.f  // Move text to vertical center of button.
        });
    };

    centerTextInButton(*mainStartText_, mainStartButton_); // Center start text in start button.
    centerTextInButton(*exitText_, exitButton_);           // Center exit text in exit button.
}


void Game::setupMenu() {
    if (!font_.openFromFile("photos/Inter_18pt-BlackItalic.ttf")) { // Load font for menu text.
        std::cerr << "Could not load Inter_18pt-BlackItalic.ttf\n"; // Print error if loading fails.
    }

    const float windowWidth = static_cast<float>(window_.getSize().x); // Store window width.
    const float centerX = windowWidth / 2.f; // Calculate center X position.

    const sf::Vector2f diffButtonSize(180.f, 60.f); // Size of each difficulty button.
    const float gap = 30.f; // Gap between difficulty buttons.

    const float totalWidth = diffButtonSize.x * 3.f + gap * 2.f; // Total width of all difficulty buttons plus gaps.
    const float startX = centerX - totalWidth / 2.f; // Starting X so buttons are centered.
    const float buttonsY = 280.f; // Y position for difficulty buttons.

    menuTitle_ = std::make_unique<sf::Text>(font_, "Choose difficulty level", 40); // Create menu title.
    {
        sf::FloatRect bounds = menuTitle_->getLocalBounds(); // Get title bounds.
        menuTitle_->setOrigin({
            bounds.position.x + bounds.size.x / 2.f, // Center origin in X.
            bounds.position.y + bounds.size.y / 2.f  // Center origin in Y.
        });
    }
    menuTitle_->setFillColor(sf::Color::White); // Set title color.
    menuTitle_->setPosition({centerX, 170.f});  // Position title near top center.

    easyButton_.setSize(diffButtonSize);     // Set easy button size.
    easyButton_.setPosition({startX, buttonsY}); // Position easy button.

    mediumButton_.setSize(diffButtonSize); // Set medium button size.
    mediumButton_.setPosition({startX + diffButtonSize.x + gap, buttonsY}); // Position medium button.

    hardButton_.setSize(diffButtonSize); // Set hard button size.
    hardButton_.setPosition({startX + 2.f * (diffButtonSize.x + gap), buttonsY}); // Position hard button.

    startButton_.setSize({220.f, 70.f});           // Set start button size.
    startButton_.setPosition({centerX - 110.f, 420.f}); // Center start button.
    startButton_.setFillColor(sf::Color(70, 130, 70));  // Set start button color.

    easyText_ = std::make_unique<sf::Text>(font_, "Easy", 28);     // Create easy label.
    mediumText_ = std::make_unique<sf::Text>(font_, "Medium", 28); // Create medium label.
    hardText_ = std::make_unique<sf::Text>(font_, "Hard", 28);     // Create hard label.
    startText_ = std::make_unique<sf::Text>(font_, "Start", 30);   // Create start label.

    easyText_->setFillColor(sf::Color::Black);   // Set easy text color.
    mediumText_->setFillColor(sf::Color::Black); // Set medium text color.
    hardText_->setFillColor(sf::Color::Black);   // Set hard text color.
    startText_->setFillColor(sf::Color::White);  // Set start text color.

    auto centerTextInButton = [](sf::Text& text, const sf::RectangleShape& button) { // Helper lambda for centering text.
        sf::FloatRect textBounds = text.getLocalBounds();    // Get text bounds.
        sf::FloatRect buttonBounds = button.getGlobalBounds(); // Get button bounds.

        text.setOrigin({
            textBounds.position.x + textBounds.size.x / 2.f, // Center origin in X.
            textBounds.position.y + textBounds.size.y / 2.f  // Center origin in Y.
        });

        text.setPosition({
            buttonBounds.position.x + buttonBounds.size.x / 2.f, // Center text horizontally.
            buttonBounds.position.y + buttonBounds.size.y / 2.f  // Center text vertically.
        });
    };
    
    centerTextInButton(*easyText_, easyButton_);     // Center easy label.
    centerTextInButton(*mediumText_, mediumButton_); // Center medium label.
    centerTextInButton(*hardText_, hardButton_);     // Center hard label.
    centerTextInButton(*startText_, startButton_);   // Center start label.

    updateMenuColors(); // Apply initial difficulty button colors.
}