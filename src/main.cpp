#include <SFML/Graphics.hpp>
#include <random>

#include "classes/ball.h"
#include "classes/paddle.h"
#include "classes/enums.h"
#include "classes/config.h"
#include "classes/gamestate.h"

void newRound(GameState& gc, sf::RenderWindow& window) {
	// Seed Random Device
	Ball ballObject; // Initialise Ball Obj
	Paddle playerPaddle(true); // Initialise Player Paddle Obj
	Paddle CPUPaddle(false); // Init Computer paddle

	sf::Clock deltaClock; // Delta Clock - Used to update deltaTime for frames

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() ) {
				window.close();
				exit(0);
			}
		}

		float dt = deltaClock.restart().asSeconds(); // DeltaTime between frames

		// To prevent game breaking when user grabs window & Windows locks the thread, cap dt to 1/60th of a second

		if (dt > 0.01666f) {
			dt = 0.01666f;
		}

		// const unsigned int fps {static_cast<unsigned int>(std::ceil(1 / dt))};

		window.clear();

		playerPaddle.update(window, ballObject, dt); // update paddle - moves paddles position according to relative to window mouse pos
		playerPaddle.draw(window); // draw player paddle

		CPUPaddle.update(window, ballObject, dt); // update paddle - moves according to ball pos
		CPUPaddle.draw(window);

		const bool conceded = ballObject.update(dt, playerPaddle, CPUPaddle); // update ball - calculates new position
		ballObject.draw(window); // draw ball

		window.draw(gc.scoreText); // Draw score line

		window.display();

		if (conceded) {
			window.clear();
			window.display();

			if (ballObject.position.x < 350) {
				gc.cpuScore += 1;
				gc.roundWinner = CPU;

			} else {
				gc.playerScore += 1;
				gc.roundWinner = Player;
			}

			gc.scoreText.setString(std::to_string(gc.playerScore) + " | " + std::to_string(gc.cpuScore));

			return;
		}
	}
}

int main() {
	sf::RenderWindow window(Config::screenResolution, "Pong by thomedome", sf::Style::Titlebar | sf::Style::Close); // Initialise Window
	window.setVerticalSyncEnabled(true); // Prevent GPU Burn

	GameState gameState;

	while (window.isOpen()) {

		while ( const std::optional event = window.pollEvent() ) {
			if ( event->is<sf::Event::Closed>() ) {
				window.close();
				exit(0);
			}
		}

		while (true) {
			newRound(gameState, window);

			if (gameState.cpuScore == gameState.scoreToWin) {
				gameState.roundWinner = CPU;
				break;
			} if (gameState.playerScore == gameState.scoreToWin) {
				gameState.roundWinner = Player;
				break;
			}

			sf::Clock roundClock = sf::Clock();

			// Text Between Rounds
			sf::Text textObj {Fonts::font, "3", 100};
			textObj.setOrigin(textObj.getLocalBounds().getCenter());
			textObj.setPosition(sf::Vector2f(Config::screenWidth / 2, Config::screenHeight / 2));

			while (roundClock.getElapsedTime().asSeconds() < 3.f) {
				const float elapsed = roundClock.getElapsedTime().asSeconds();

				if (elapsed < 1.f) {
					textObj.setString("3");
				} if (elapsed > 1.f && elapsed < 2.f) {
					textObj.setString("2");
				} if (elapsed > 2.f && elapsed < 3.f) {
					textObj.setString("1");
				} if (elapsed > 3.f) {
					window.clear();
					window.display();
				}

				window.clear();
				window.draw(textObj);
				window.display();
			}
		}

		std::string text {};

		if (gameState.roundWinner == E_player::CPU) {
			text = "CPU";
		} else if (gameState.roundWinner == E_player::Player) {
			text = "Player";
		}

		text += " won!";

		sf::Clock endClock = sf::Clock();
		sf::Text textObj1 {Fonts::font, text, 50};
		textObj1.setOrigin(textObj1.getLocalBounds().getCenter());
		textObj1.setPosition(sf::Vector2f(Config::screenWidth / 2, Config::screenHeight / 2));

		while (window.isOpen()) {
			while (const std::optional event = window.pollEvent() ) {
				if ( event->is<sf::Event::Closed>() ) {
					window.close();
					exit(0);
				}
			}
			const float elapsed = endClock.getElapsedTime().asSeconds();
			if (elapsed < 5) {
				window.clear();
				window.draw(textObj1);
				window.display();
			}

			if (elapsed > 5) {

				window.close(); // Exit Game
				return 0;
			}
		}
	}
}
