//
// Created by win11 on 02/10/2026.
//

#ifndef PONG_PADDLE_H
#define PONG_PADDLE_H

#include <SFML/Graphics.hpp>
#include "config.h"

class Ball;

class Paddle {
public:
    sf::Vector2f position;
    sf::RectangleShape objectOnScreen {Config::paddleSize};
    bool playerOwned {};

    explicit Paddle(bool isPlayer); // Constructor

    void update(const sf::RenderWindow& window, const Ball& ball, float dt);

    void draw(sf::RenderWindow& window);
};


#endif //PONG_PADDLE_H
