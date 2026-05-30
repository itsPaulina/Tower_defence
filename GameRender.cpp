#include "Game.h" // Includes the Game class declaration.


void Game::render() {
    float mapWidth = static_cast<float>(window_.getSize().x) - menuWidth_; // Width of the playable map area.
    float mapHeight = static_cast<float>(window_.getSize().y);              // Height of the playable map area.

    if (grassTexture_.getSize().x > 0 && grassTexture_.getSize().y > 0) { // Check if grass texture was loaded correctly.
        sf::Sprite grassSprite(grassTexture_); // Create sprite for grass background.
        grassSprite.setTextureRect(sf::IntRect(
            {0, 0}, // Start at top-left corner of the texture.
            {static_cast<int>(mapWidth), static_cast<int>(mapHeight)} // Stretch/repeat to cover the whole map area.
        ));
        grassSprite.setColor(sf::Color(170, 170, 170)); // Tint the grass texture slightly darker.
        grassSprite.setPosition({0.f, 0.f}); // Place background at the top-left corner.
        window_.draw(grassSprite); // Draw grass background.
    } else {
        sf::RectangleShape fallback({mapWidth, mapHeight}); // Fallback rectangle if grass texture is missing.
        fallback.setPosition({0.f, 0.f}); // Place fallback background at top-left.
        fallback.setFillColor(sf::Color(60, 120, 60)); // Give fallback a green color.
        window_.draw(fallback); // Draw fallback background.
    }

    for (int r = 0; r < rows_; ++r) { // Loop through all rows of the map.
        for (int c = 0; c < cols_; ++c) { // Loop through all columns of the map.
            if (!isPathTile(c, r)) { // Skip tiles that are not part of the path.
                continue;
            }

            sf::Vector2f pos{
                c * tileSize_, // Top-left X position of the tile.
                r * tileSize_  // Top-left Y position of the tile.
            };

            if (dirtTexture_.getSize().x > 0 && dirtTexture_.getSize().y > 0) { // Check if dirt texture is loaded.
                sf::Sprite dirtSprite(dirtTexture_); // Create sprite for one path tile.
                dirtSprite.setTextureRect(sf::IntRect(
                    {0, 0}, // Start at top-left of the texture.
                    {static_cast<int>(tileSize_), static_cast<int>(tileSize_)} // Use one tile-sized part.
                ));
                dirtSprite.setPosition(pos); // Place the path tile.
                window_.draw(dirtSprite);    // Draw path tile.
            } else {
                sf::RectangleShape fallback({tileSize_, tileSize_}); // Fallback path tile.
                fallback.setPosition(pos); // Place fallback tile.
                fallback.setFillColor(sf::Color(160, 110, 60)); // Give fallback a dirt-like brown color.
                window_.draw(fallback); // Draw fallback tile.
            }
        }
    }

    if (castleSprite_) { // Check if castle sprite exists.
        window_.draw(*castleSprite_); // Draw the castle.
    }

    for (const auto& obj : objects_) { // Draw all active game objects.
        if (obj->isActive()) {
            obj->draw(window_);
        }
    }

    if (castleSprite_) { // Draw castle HP only if castle exists.
        drawCastleHP();
    }

    towerMenu_.draw(window_); // Draw tower menu.
    drawUI();                 // Draw UI texts.

    if (gameOver_) { // Show game over overlay if player lost.
        sf::RectangleShape overlay({
            static_cast<float>(window_.getSize().x), // Full window width.
            static_cast<float>(window_.getSize().y)  // Full window height.
        });
        overlay.setFillColor(sf::Color(0, 0, 0, 160)); // Semi-transparent dark overlay.
        window_.draw(overlay); // Draw overlay.

        sf::Text loseText(font_, "Game Over", 68); // Create lose text.
        loseText.setFillColor(sf::Color::Red);     // Set text color to red.

        sf::FloatRect bounds = loseText.getLocalBounds(); // Get text bounds for centering.
        loseText.setOrigin({
            bounds.position.x + bounds.size.x / 2.f, // Center origin in X.
            bounds.position.y + bounds.size.y / 2.f  // Center origin in Y.
        });

        loseText.setPosition({
            static_cast<float>(window_.getSize().x) / 2.f, // Place text at horizontal center.
            static_cast<float>(window_.getSize().y) / 2.f  // Place text at vertical center.
        });

        window_.draw(loseText); // Draw game over text.
    }

    if (victory_) { // Show victory overlay if player won.
        sf::RectangleShape overlay({
            static_cast<float>(window_.getSize().x), // Full window width.
            static_cast<float>(window_.getSize().y)  // Full window height.
        });
        overlay.setFillColor(sf::Color(0, 0, 0, 160)); // Semi-transparent dark overlay.
        window_.draw(overlay); // Draw overlay.

        sf::Text winText(font_, "You win!", 68); // Create win text.
        winText.setFillColor(sf::Color::Yellow); // Set text color to yellow.

        sf::FloatRect bounds = winText.getLocalBounds(); // Get text bounds for centering.
        winText.setOrigin({
            bounds.position.x + bounds.size.x / 2.f, // Center origin in X.
            bounds.position.y + bounds.size.y / 2.f  // Center origin in Y.
        });

        winText.setPosition({
            static_cast<float>(window_.getSize().x) / 2.f, // Horizontal center.
            static_cast<float>(window_.getSize().y) / 2.f  // Vertical center.
        });

        window_.draw(winText); // Draw victory text.
    }
}


void Game::drawUI() {
    float panelX = static_cast<float>(window_.getSize().x) - menuWidth_; // X position where side panel begins.

    sf::Text goldText(font_, "Gold: " + std::to_string(gold_), 26); // Create gold display text.
    goldText.setFillColor(sf::Color::Yellow); // Set gold text color.
    goldText.setPosition({panelX + 20.f, 20.f}); // Place gold text in side panel.
    window_.draw(goldText); // Draw gold text.

    sf::Text levelText(font_, "LEVEL " + std::to_string(currentLevel_), 42); // Create level display text.
    levelText.setFillColor(sf::Color::White); // Set level text color.

    sf::FloatRect bounds = levelText.getLocalBounds(); // Get level text bounds.
    levelText.setOrigin({
        bounds.position.x + bounds.size.x / 2.f, // Center origin in X.
        bounds.position.y + bounds.size.y / 2.f  // Center origin in Y.
    });

    float mapWidth = static_cast<float>(window_.getSize().x) - menuWidth_; // Width of map area.
    levelText.setPosition({mapWidth / 2.f, 40.f}); // Place level text at top center of map.

    window_.draw(levelText); // Draw level text.
}


void Game::drawCastleHP() {
    if (!castleSprite_) { // Stop if castle sprite does not exist.
        return;
    }

    float barWidth = 180.f;  // Width of castle HP bar.
    float barHeight = 14.f;  // Height of castle HP bar.

    float ratio = 0.f; // Current HP percentage.
    if (castleMaxHP_ > 0) {
        ratio = static_cast<float>(castleHP_) / static_cast<float>(castleMaxHP_); // Compute health ratio.
    }
    if (ratio < 0.f) ratio = 0.f; // Prevent negative ratio.

    sf::Vector2f pos = castleSprite_->getPosition(); // Get castle position.

    sf::RectangleShape back({barWidth, barHeight}); // Background of HP bar.
    back.setFillColor(sf::Color(40, 40, 40)); // Dark color for background.
    back.setPosition({pos.x - barWidth / 2.f, pos.y - 120.f}); // Place HP bar above castle.

    sf::RectangleShape front({barWidth * ratio, barHeight}); // Foreground HP bar based on current health.
    front.setFillColor(sf::Color::Red); // Red color for HP.
    front.setPosition({pos.x - barWidth / 2.f, pos.y - 120.f}); // Same position as background.

    window_.draw(back);  // Draw HP bar background.
    window_.draw(front); // Draw current HP amount.
}