//
// Created by win11 on 02/10/2026.
//

#ifndef PONG_GAMESTATE_H
#define PONG_GAMESTATE_H

#include <SFML/Graphics.hpp>
#include "enums.h"
#include "config.h"

class GameState {
public:
    unsigned int playerScore {0};
    unsigned int cpuScore {0};

    // Score Text
    sf::Text scoreText{Fonts::font, "0 | 0", 30};

    GameState();

    const unsigned int scoreToWin = Config::scoreToWin;
    E_player roundWinner {};
};

#endif //PONG_GAMESTATE_H
