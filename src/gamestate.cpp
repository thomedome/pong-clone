//
// Created by win11 on 02/10/2026.
//

#include "gamestate.h"

GameState::GameState() {
    scoreText.setPosition(sf::Vector2f(Config::screenWidth / 2, 30.f));
    scoreText.setOrigin(scoreText.getLocalBounds().getCenter());
    scoreText.setString(std::to_string(playerScore) + " | " + std::to_string(cpuScore));
}