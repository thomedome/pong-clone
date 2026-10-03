//
// Created by win11 on 02/10/2026.
//

#ifndef PONG_CONFIG_H
#define PONG_CONFIG_H

#endif //PONG_CONFIG_H

#pragma once
#include <random>
#include <SFML/Graphics.hpp>

namespace Config {
    const sf::VideoMode screenResolution({700, 700});
    constexpr float ballRadius = 10.f;
    constexpr sf::Vector2f paddleSize {25, 200};
    constexpr float cpuPaddleMoveSpeed = 170.f;

    constexpr float ballSpeedIncrease = 1.01f;
    constexpr int LBRandomSpeed = 150;
    constexpr int UBRandomSpeed = 250;

    constexpr int timeBetweenRounds = 3;

    const unsigned int scoreToWin = 3;

    const float screenWidth {static_cast<float>(screenResolution.size.x)};
    const float screenHeight {static_cast<float>(screenResolution.size.y)};
}

namespace Fonts {
    const sf::Font font ("assets/fonts/LiberationSans-Regular.ttf");
}

namespace random {
    static std::random_device rd; // Seed the random device
    static std::mt19937 gen(rd()); // Using Mersenne Twister engine - better randomness than rand() and srand()
    static std::uniform_real_distribution<> dis(Config::LBRandomSpeed, Config::UBRandomSpeed); // distribution between 150 and 250
}
