#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <thread>
#include <chrono>

using namespace std;

const sf::VideoMode screenResolution({700, 700});
constexpr float ballRadius = 10.f;
constexpr sf::Vector2f paddleSize {25, 200};
constexpr float cpuPaddleMoveSpeed = 160.f;

constexpr float ballSpeedIncrease = 1.01f;
constexpr int LBRandomSpeed = 150;
constexpr int UBRandomSpeed = 250;

const float screenWidth {static_cast<float>(screenResolution.size.x)};
const float screenHeight {static_cast<float>(screenResolution.size.y)};

static std::random_device rd; // Seed the random device
static std::mt19937 gen(rd()); // Using Mersenne Twister engine - better randomness than rand() and srand()
static std::uniform_real_distribution<> dis(LBRandomSpeed, UBRandomSpeed); // distribution between 150 and 250


class Paddle;

enum ballDirection {
	Left,
	Right,
};

enum E_player {
	Player,
	CPU
};

class GameState {
public:
	unsigned int playerScore {0};
	unsigned int cpuScore {0};

	const unsigned int scoreToWin = 3;
	E_player roundWinner {};
};

class Ball {
	public:
		sf::Vector2f position = {static_cast<float>(screenResolution.size.x)/ 2.f, static_cast<float>(screenResolution.size.y) / 2.f};
		sf::CircleShape objectOnScreen{ballRadius};
		sf::Vector2f ballVelocity{0, 0};
		sf::Vector2f newPos{};
		ballDirection BallDir = Right;

		bool conceded = false;

		Ball() { // Constructor
			objectOnScreen.setOrigin({ballRadius, ballRadius});
			objectOnScreen.setFillColor(sf::Color::White);

			objectOnScreen.setPosition(position);
			ballVelocity = sf::Vector2f(static_cast<float>(dis(gen)), static_cast<float>(dis(gen)));
		}

		void updateDir(const ballDirection dir) {
			BallDir = dir;

			if (BallDir == Left) {
				ballVelocity.x = -std::abs(ballVelocity.x);
			} else {
				ballVelocity.x = std::abs(ballVelocity.x);
			}

			ballVelocity = sf::Vector2f(ballVelocity.x * ballSpeedIncrease, ballVelocity.y * ballSpeedIncrease);
		}

		void draw(sf::RenderWindow& Window) {
			objectOnScreen.setPosition(position);
			Window.draw(objectOnScreen);
		}

		bool update(float deltaTime, const Paddle& playerPaddle, const Paddle& CPUPaddle); // Forward Declaration
};

class Paddle {
public:
	sf::Vector2f position;
	sf::RectangleShape objectOnScreen {paddleSize};

	bool playerOwned;

	explicit Paddle(const bool isPlayer) : playerOwned(isPlayer) { // Constructor
		objectOnScreen.setOrigin({paddleSize.x / 2, paddleSize.y / 2});
		objectOnScreen.setFillColor(sf::Color::White);

		if (isPlayer) {
			position = {(10 + paddleSize.x), screenHeight / 2};
		} else {
			position = {screenWidth - (10 + paddleSize.x), screenHeight / 2};
		}
	}

	void update(const sf::RenderWindow& window, const Ball& ball, const float dt) {
		if (playerOwned) {
			const sf::Vector2i mousePos = sf::Mouse::getPosition(window);

			const auto floatedYPos = static_cast<float>(mousePos.y);

			// cout << floatedYPos << std::endl;

			// Clamping Y Position
			if (floatedYPos < (0 + (paddleSize.y / 2))) { // If collided with roof
				position.y = 0 + (paddleSize.y / 2);
			} else if (floatedYPos > (screenHeight - (paddleSize.y / 2))) { // If collided with floor
				position.y = screenHeight - (paddleSize.y / 2);
			} else { // Non Y-Axis Collision
				position.y = static_cast<float>(mousePos.y);
			}
		} else if (!playerOwned) {
			const float diff = position.y - ball.position.y;

			if (diff >= 20) {
				position.y = position.y - (cpuPaddleMoveSpeed * dt);
			} else if ((diff <= -20)) {
				position.y = position.y + (cpuPaddleMoveSpeed * dt);
			}

			position.y = std::clamp(position.y, paddleSize.y / 2, screenHeight - (paddleSize.y / 2));
		}
	}

	void draw(sf::RenderWindow& window) {
		objectOnScreen.setPosition(position);
		window.draw(objectOnScreen);
	}
};

bool Ball::update(const float deltaTime, const Paddle& playerPaddle, const Paddle& CPUPaddle) {
	newPos = position + (ballVelocity * deltaTime);

	if (newPos.x >= (screenWidth - ballRadius) || newPos.x <= ballRadius) { // Checking if the ball has collided with the side of the screen (Non-Paddle.)
		conceded = true;
		ballVelocity = {0, 0};
		return true;
	}

	if (newPos.y <= ballRadius || newPos.y >= (screenHeight - ballRadius)) { // Checking if the ball has collided with the roof / floor of the screen.
		ballVelocity.y = -ballVelocity.y; // Inverse Y Velocity
	}

	if (BallDir == Right) {
		if (objectOnScreen.getGlobalBounds().findIntersection(CPUPaddle.objectOnScreen.getGlobalBounds())) { // CPU Paddle Collision
			updateDir(Left);
		}
	} else if (BallDir == Left) {
		if (objectOnScreen.getGlobalBounds().findIntersection(playerPaddle.objectOnScreen.getGlobalBounds())) { // Player Paddle Collision
			updateDir(Right);
		}
	} else {
		cout << "[BALL] Object missing Direction Enum" << endl;
	}

	position = newPos;

	return false;
}

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
			if ( event->is<sf::Event::Closed>() )
				window.close();
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

		window.display();

		if (conceded) {
			window.clear();
			window.display();

			if (ballObject.position.x < 350) {
				gc.cpuScore += 1;
				gc.roundWinner = E_player::CPU;
			} else {
				gc.playerScore += 1;
				gc.roundWinner = E_player::Player;
			}

			return;
		}
	}
}

int main()
{
	sf::RenderWindow window(screenResolution, "Pong by thomedome", sf::Style::Titlebar | sf::Style::Close); // Initialise Window
	window.setVerticalSyncEnabled(true); // Prevent GPU Burn

	GameState gameState;

	while (true) {
		newRound(gameState, window);
		cout << gameState.playerScore << " | " << gameState.cpuScore << endl;

		if (gameState.cpuScore == gameState.scoreToWin) {
			gameState.roundWinner = E_player::CPU;
			break;
		} if (gameState.playerScore == gameState.scoreToWin) {
			gameState.roundWinner = E_player::Player;
			break;
		}

		this_thread::sleep_for(3s);

	}

	string text {};

	if (gameState.roundWinner == E_player::CPU) {
		text = "CPU";
	} else if (gameState.roundWinner == E_player::Player) {
		text = "Player";
	}

	cout << text << " won the game!" << endl;

	window.close();

	return 0;

}
