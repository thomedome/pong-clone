//
// Created by win11 on 02/10/2026.
//

#ifndef PONG_BALL_H
#define PONG_BALL_H
#include <SFML/Graphics.hpp>
#include "config.h"
#include "enums.h"

class Paddle;

class Ball {
public:
    sf::Vector2f position = {Config::screenWidth/ 2.f, Config::screenHeight / 2.f};
    sf::CircleShape objectOnScreen{Config::ballRadius};
    sf::Vector2f ballVelocity{0, 0};
    sf::Vector2f newPos{};
    ballDirection BallDir = Right;

    bool conceded = false;

    Ball() { // Constructor
        objectOnScreen.setOrigin({Config::ballRadius, Config::ballRadius});
        objectOnScreen.setFillColor(sf::Color::White);

        objectOnScreen.setPosition(position);
        ballVelocity = sf::Vector2f(static_cast<float>(random::dis(random::gen)), static_cast<float>(random::dis(random::gen)));
    }

    void draw(sf::RenderWindow& Window) {
        objectOnScreen.setPosition(position);
        Window.draw(objectOnScreen);
    }

    void updateDir(ballDirection dir);

    bool update(float deltaTime, const Paddle& playerPaddle, const Paddle& CPUPaddle); // Forward Declaration
};


#endif //PONG_BALL_H
