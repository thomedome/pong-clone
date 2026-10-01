#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>

const sf::VideoMode screenResolution({700, 700});
constexpr float ballRadius = 10.f;
constexpr sf::Vector2f paddleSize {50, 200};

const float screenWidth {static_cast<float>(screenResolution.size.x)};
const float screenHeight {static_cast<float>(screenResolution.size.y)};

class Paddle {
public:
	sf::Vector2f position;
	sf::RectangleShape objectOnScreen {paddleSize};

	bool playerOwned;

	explicit Paddle(const bool isPlayer) { // Constructor
		objectOnScreen.setOrigin({paddleSize.x / 2, paddleSize.y / 2});
		objectOnScreen.setFillColor(sf::Color::White);

		if (isPlayer) {
			playerOwned = true;
			position = {(10 + paddleSize.x), screenHeight / 2};
		} else {
			playerOwned = false;
			position = {screenWidth - 10, screenHeight / 2};
		}
	}

	void update(sf::RenderWindow& window) {
		if (playerOwned) {
			const sf::Vector2i mousePos = sf::Mouse::getPosition(window);

			const auto floatedYPos = static_cast<float>(mousePos.y);

			std::cout << floatedYPos << std::endl;

			if (floatedYPos < (0 + (paddleSize.y / 2))) { // If collided with roof
				position.y = 0 + (paddleSize.y / 2);
			} else if (floatedYPos > (screenHeight - (paddleSize.y / 2))) { // If collided with floor
				position.y = screenHeight - (paddleSize.y / 2);
			} else { // Non Y-Axis Collision
				position.y = static_cast<float>(mousePos.y);
			}
		}

		draw(window);
	}

	void draw(sf::RenderWindow& window) {
		objectOnScreen.setPosition(position);
		window.draw(objectOnScreen);
	}
};

class Ball {
	public:
		sf::Vector2f position = {static_cast<float>(screenResolution.size.x)/ 2.f, static_cast<float>(screenResolution.size.y) / 2.f};
		sf::CircleShape objectOnScreen{ballRadius};
		sf::Vector2f ballVelocity{5.f, 50.f};

		bool conceded = false;

		Ball() { // Constructor
			objectOnScreen.setOrigin({ballRadius, ballRadius});
			objectOnScreen.setFillColor(sf::Color::White);
		}

		void draw(sf::RenderWindow& Window) {
			objectOnScreen.setPosition(position);
			Window.draw(objectOnScreen);
		}

		void update(const float deltaTime, sf::RenderWindow& window) {
			const sf::Vector2f newPos = position + (ballVelocity * deltaTime);

			if (newPos.x >= (screenWidth - ballRadius) || newPos.x <= ballRadius) { // Checking if the ball has collided with the side of the screen (Non-Paddle.)
				conceded = true;
				ballVelocity = {0, 0};
			}

			if (newPos.y <= ballRadius || newPos.y >= (screenHeight - ballRadius)) { // Checking if the ball has collided with the roof / floor of the screen.
				ballVelocity.y = -ballVelocity.y; // Inverse Y Velocity
			}

			position = newPos;

			draw(window);
		}
};


int main()
{
	Ball ballObject; // Initialise Ball Obj

	Paddle playerPaddle(true);

	sf::RenderWindow window(screenResolution, "Pong by thomedome", sf::Style::Titlebar | sf::Style::Close); // Initialise Window

	window.setVerticalSyncEnabled(true);

	sf::Clock deltaClock; // Delta Clock - Used to update deltaTime for frames

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
		}

		float dt = deltaClock.restart().asSeconds(); // DeltaTime between frames

		// To prevent game breaking when user grabs window & Windows locks the thread, force dt to 1/60th of a second

		if (dt > 0.01666f) {
			dt = 0.01666f;
		}

		const unsigned int fps {static_cast<unsigned int>(std::ceil(1 / dt))};
		// std::cout << fps << "FPS" << std::endl;

		window.clear();

		ballObject.update(dt, window); // update ball - calculates new position and draws on screen
		playerPaddle.update(window);

		window.display();
	}
}
