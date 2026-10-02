//
// Created by win11 on 02/10/2026.
//

#include "paddle.h"
#include <algorithm>
#include "ball.h"

Paddle::Paddle(const bool isPlayer) : playerOwned(isPlayer) {
    objectOnScreen.setOrigin({Config::paddleSize.x / 2, Config::paddleSize.y / 2});
    objectOnScreen.setFillColor(sf::Color::White);

    if (isPlayer) {
        position = {(10 + Config::paddleSize.x), Config::screenHeight / 2};
    } else {
        position = {Config::screenWidth - (10 + Config::paddleSize.x), Config::screenHeight / 2};
    }
}

void Paddle::update(const sf::RenderWindow& window, const Ball& ball, const float dt) {
    if (playerOwned) {
        const sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        const auto floatedYPos = static_cast<float>(mousePos.y);

        // Clamping Y Position
        if (floatedYPos < (0 + (Config::paddleSize.y / 2))) { // If collided with roof
            position.y = 0 + (Config::paddleSize.y / 2);
        } else if (floatedYPos > (Config::screenHeight - (Config::paddleSize.y / 2))) { // If collided with floor
            position.y = Config::screenHeight - (Config::paddleSize.y / 2);
        } else { // Non Y-Axis Collision
            position.y = static_cast<float>(mousePos.y);
        }
    } else if (!playerOwned) {
        const float diff = position.y - ball.position.y;

        if (diff >= 20) {
            position.y = position.y - (Config::cpuPaddleMoveSpeed * dt);
        } else if ((diff <= -20)) {
            position.y = position.y + (Config::cpuPaddleMoveSpeed * dt);
        }

        position.y = std::clamp(position.y, Config::paddleSize.y / 2, Config::screenHeight - (Config::paddleSize.y / 2));
    }
}

void Paddle::draw(sf::RenderWindow& window) {
    objectOnScreen.setPosition(position);
    window.draw(objectOnScreen);
}