#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Paddle.h"
#include "Ball.h"
#include "GamePhysics.h"

class Game
{
public:
	Game(int WIDTH, int HEIGHT);
	void Run();

private:	// member functions
	void processEvents();
	void Update(sf::Time elapsedTime);
	void Render();
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void updateText();

private:	// member variables
	enum states { menu, playing, victory, pause };
	int gameState;

	bool p1MovingDown;
	bool p1MovingUp;
	bool p2MovingDown;
	bool p2MovingUp;

	bool spBoxActive;
	bool tpBoxActive;
	bool hmBoxActive;
	bool shmBoxActive;

	bool invisBallLaunched;

	bool reverseActive;

	static const float PlayerSpeed;
	static const sf::Time TimePerFrame;

	int width;
	int height;
	int padding;

	int p1Score;
	int p2Score;
	int ballHits;

private:	// graphics and sounds
	sf::RenderWindow window;

	sf::View view;

	sf::Clock powerUpTimer;
	sf::Time countdown;
	sf::Time Dummy;

	sf::RectangleShape leftSide;
	sf::RectangleShape rightSide;

	sf::RectangleShape spBox;
	sf::RectangleShape tpBox;
	sf::RectangleShape hmBox;
	sf::RectangleShape shmBox;

	sf::Font font;

	sf::Text p1ScoreDisplay;
	sf::Text p2ScoreDisplay;
	sf::Text p1Win;
	sf::Text p2Win;
	sf::Text playText;
	sf::Text ballSpeedYDisplay;
	sf::Text ballSpeedXDisplay;
	sf::Text ballHitsDisplay;
	sf::Text paused;

	sf::Text singlePlayer;
	sf::Text twoPlayer;
	sf::Text hardMode;
	sf::Text singleHardMode;

	std::string p1ScoreText;
	std::string p2ScoreText;
	std::string ballSpeedYText;
	std::string ballSpeedXText;
	std::string ballHitsText;

	sf::SoundBuffer padBuffer1;
	sf::SoundBuffer padBuffer2;

	sf::Sound padSound1;
	sf::Sound padSound2;

	sf::Music theme;

	Paddle player;
	Paddle player2;
	Ball ball;

	Ball invisBall;

	Physics phys;
};
