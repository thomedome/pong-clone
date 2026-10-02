//
// Created by win11 on 02/10/2026.
//

#include "ball.h"
#include "paddle.h"

#include <iostream>

#include "config.h"
#include "enums.h"

void Ball::updateDir(const ballDirection dir) {
    BallDir = dir;

    if (BallDir == Left) {
        ballVelocity.x = -std::abs(ballVelocity.x);
    } else {
        ballVelocity.x = std::abs(ballVelocity.x);
    }

    ballVelocity = sf::Vector2f(ballVelocity.x * Config::ballSpeedIncrease, ballVelocity.y * Config::ballSpeedIncrease);
}

bool Ball::update(const float deltaTime, const Paddle& playerPaddle, const Paddle& CPUPaddle) {
    newPos = position + (ballVelocity * deltaTime);

    if (newPos.x >= (Config::screenWidth - Config::ballRadius) || newPos.x <= Config::ballRadius) { // Checking if the ball has collided with the side of the screen (Non-Paddle.)
        conceded = true;
        ballVelocity = {0, 0};
        return true;
    }

    if (newPos.y <= Config::ballRadius || newPos.y >= (Config::screenHeight - Config::ballRadius)) { // Checking if the ball has collided with the roof / floor of the screen.
        ballVelocity.y = -ballVelocity.y; // Inverse Y Velocity
    }

    if (BallDir == Right) {
        if (objectOnScreen.getGlobalBounds().findIntersection(CPUPaddle.objectOnScreen.getGlobalBounds())) { // CPU Paddle Collision
            Ball::updateDir(Left);
        }
    } else if (BallDir == Left) {
        if (objectOnScreen.getGlobalBounds().findIntersection(playerPaddle.objectOnScreen.getGlobalBounds())) { // Player Paddle Collision
            updateDir(Right);
        }
    } else {
        std::cout << "[BALL] Object missing Direction Enum" << std::endl;
    }

    position = newPos;

    return false;
}
