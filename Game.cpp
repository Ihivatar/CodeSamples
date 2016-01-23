#include "Game.h"

#include <time.h>
#include <fstream>
#include <iostream>

// uncomment to view ball speed and if playing Singple Player Hard mode, the "ghost ball" the cpu uses to predict the ball movement
//#define DEBUG

const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

Game::Game(int WIDTH, int HEIGHT)
	: window(sf::VideoMode(WIDTH, HEIGHT), "Ultra Pong")
{
	p1MovingDown = false;
	p1MovingUp = false;
	p2MovingDown = false;
	p2MovingUp = false;
	reverseActive = false;
	if (!font.loadFromFile("Arial.ttf"))
		std::cout << "Could not load \"Arial.ttf\"" << std::endl;
	width = WIDTH;
	height = HEIGHT;
	padding = 5;
	p1Score = 0;
	p2Score = 0;
	ballHits = 0;
	gameState = menu;
	powerUpTimer.restart();
	countdown = sf::Time::Zero;
	if (!padBuffer1.loadFromFile("jump_01.wav"))
		std::cout << "Could not load \"jump_01.wav\"" << std::endl;
	if (!padBuffer2.loadFromFile("jump_07.wav"))
		std::cout << "Could not load \"jump_07.wav\"" << std::endl;
	padSound1.setBuffer(padBuffer1);
	padSound2.setBuffer(padBuffer2);
	if (!theme.openFromFile("afterburner.ogg"))
		std::cout << "Could not load \"afterburner.ogg\"" << std::endl;
	theme.setLoop(true);
	spBoxActive = true;
	tpBoxActive = false;
	hmBoxActive = false;
	shmBoxActive = false;
	invisBallLaunched = false;
}

void Game::Run()
{
	leftSide.setSize(sf::Vector2f(width / 2, height));
	leftSide.setFillColor(sf::Color::Black);
	leftSide.setPosition(0, 0);

	rightSide.setSize(sf::Vector2f(width / 2, height));
	rightSide.setFillColor(sf::Color::White);
	rightSide.setPosition(width / 2, 0);

	player.setSize(sf::Vector2f(10, 80));
	player.setSpeed(sf::Vector2f(0, 200));
	player.setFillColor(sf::Color::Transparent);
	player.setOutlineColor(sf::Color::White);
	player.setOutlineThickness(1);
	player.setPosition(padding, height / 2 - player.getSize().y);
	player.setIsPlayer(true);

	player2.setSize(sf::Vector2f(10, 80));
	player2.setSpeed(sf::Vector2f(0, 200));
	player2.setFillColor(sf::Color::Transparent);
	player2.setOutlineColor(sf::Color::Black);
	player2.setOutlineThickness(1);
	player2.setPosition(width - padding - player2.getSize().x, height / 2 - player2.getSize().y);

	ball.exists = true;
	ball.setRadius(10);
	ball.setOrigin(sf::Vector2f(10, 10));
	ball.setSpeed(sf::Vector2f(-120, -80));
	ball.setFillColor(sf::Color::White);
	ball.setPosition(sf::Vector2f(width / 2, height / 2));

	invisBall.setSpeed(sf::Vector2f(0, 0));

	p1ScoreText = std::to_string(p1Score);
	p2ScoreText = std::to_string(p2Score);
	ballSpeedYText = std::to_string(ball.getSpeed().y);
	ballSpeedXText = std::to_string(ball.getSpeed().x);
	ballHitsText = std::to_string(ballHits);

	p1ScoreDisplay.setFont(font);
	p1ScoreDisplay.setCharacterSize(36);
	p1ScoreDisplay.setColor(sf::Color::White);
	p1ScoreDisplay.setPosition(75, padding * 4);
	p1ScoreDisplay.setString("P1 Score: " + p1ScoreText);

	p2ScoreDisplay.setFont(font);
	p2ScoreDisplay.setCharacterSize(36);
	p2ScoreDisplay.setColor(sf::Color::Black);
	p2ScoreDisplay.setPosition(550, padding * 4);
	p2ScoreDisplay.setString("P2 Score: " + p2ScoreText);

	ballHitsDisplay.setFont(font);
	ballHitsDisplay.setCharacterSize(24);
	ballHitsDisplay.setColor(sf::Color::Black);
	ballHitsDisplay.setPosition(width / 2 + 25, height - 50);
	ballHitsDisplay.setString("Ball hits: " + ballHitsText);

	ballSpeedYDisplay.setFont(font);
	ballSpeedYDisplay.setCharacterSize(24);
	ballSpeedYDisplay.setColor(sf::Color::White);
	ballSpeedYDisplay.setPosition(150, 100);
	ballSpeedYDisplay.setString("Ball Y: " + ballSpeedYText);

	ballSpeedXDisplay.setFont(font);
	ballSpeedXDisplay.setCharacterSize(24);
	ballSpeedXDisplay.setColor(sf::Color::Black);
	ballSpeedXDisplay.setPosition(450, 100);
	ballSpeedXDisplay.setString("Ball X: " + ballSpeedXText);

	playText.setFont(font);
	playText.setCharacterSize(36);
	playText.setColor(sf::Color::Red);
	playText.setString("Press Space to select");
	playText.setPosition(width/2 - playText.getGlobalBounds().width/2, 450);

	p1Win.setFont(font);
	p1Win.setCharacterSize(36);
	p1Win.setColor(sf::Color::Yellow);
	p1Win.setPosition(300, height / 2);
	p1Win.setString("P1 Wins!");

	p2Win.setFont(font);
	p2Win.setCharacterSize(36);
	p2Win.setColor(sf::Color::Yellow);
	p2Win.setPosition(300, height / 2);
	p2Win.setString("P2 Wins!");

	paused.setFont(font);
	paused.setCharacterSize(36);
	paused.setColor(sf::Color::Red);
	paused.setPosition(200, 350);
	paused.setString("Press space to resume");

	singlePlayer.setFont(font);
	singlePlayer.setCharacterSize(36);
	singlePlayer.setColor(sf::Color::Green);
	singlePlayer.setString("Single Player");
	singlePlayer.setPosition(width / 2 - singlePlayer.getGlobalBounds().width / 2, 100);

	twoPlayer.setFont(font);
	twoPlayer.setCharacterSize(36);
	twoPlayer.setColor(sf::Color::Green);
	twoPlayer.setString("Two Player");
	twoPlayer.setPosition(width/2 - twoPlayer.getGlobalBounds().width/2, 175);

	hardMode.setFont(font);
	hardMode.setCharacterSize(36);
	hardMode.setColor(sf::Color::Green);
	hardMode.setString("Hard Mode");
	hardMode.setPosition(width/2 - hardMode.getGlobalBounds().width/2, 250);

	singleHardMode.setFont(font);
	singleHardMode.setCharacterSize(36);
	singleHardMode.setColor(sf::Color::Green);
	singleHardMode.setString("Single Player Hard Mode");
	singleHardMode.setPosition(width/2 - singleHardMode.getGlobalBounds().width/2, 325);

	spBox.setSize(sf::Vector2f(singlePlayer.getGlobalBounds().width + 20, singlePlayer.getGlobalBounds().height + 20));
	spBox.setFillColor(sf::Color::Black);
	spBox.setOutlineColor(sf::Color::White);
	spBox.setOutlineThickness(3);
	spBox.setPosition(singlePlayer.getPosition().x - 10, singlePlayer.getPosition().y);

	tpBox.setSize(sf::Vector2f(twoPlayer.getGlobalBounds().width + 20, twoPlayer.getGlobalBounds().height + 20));
	tpBox.setFillColor(sf::Color::Black);
	tpBox.setOutlineColor(sf::Color::White);
	tpBox.setOutlineThickness(3);
	tpBox.setPosition(twoPlayer.getPosition().x - 10, twoPlayer.getPosition().y);

	hmBox.setSize(sf::Vector2f(hardMode.getGlobalBounds().width + 20, hardMode.getGlobalBounds().height + 20));
	hmBox.setFillColor(sf::Color::Black);
	hmBox.setOutlineColor(sf::Color::White);
	hmBox.setOutlineThickness(3);
	hmBox.setPosition(hardMode.getPosition().x - 10, hardMode.getPosition().y);

	shmBox.setSize(sf::Vector2f(singleHardMode.getGlobalBounds().width + 20, singleHardMode.getGlobalBounds().height + 20));
	shmBox.setFillColor(sf::Color::Black);
	shmBox.setOutlineColor(sf::Color::White);
	shmBox.setOutlineThickness(3);
	shmBox.setPosition(singleHardMode.getPosition().x - 10, singleHardMode.getPosition().y);

	theme.play();
	theme.setVolume(50);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	clock_t t1;
	clock_t t2;
	t1 = std::clock();
	while (window.isOpen())
	{
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			Update(TimePerFrame);
		}
		Render();
		t2 = std::clock();
		double diff = t2 - t1;
		window.setTitle("Ultra Pong - " + std::to_string(diff) + " ms frame time");
		t1 = t2;
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			window.close();
			break;
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
		{
			if (gameState == pause)
			{
				gameState = playing;
			}
			else if (gameState == menu)
			{
				gameState = playing;
			}
			else if (gameState == playing)
			{
				gameState = pause;
			}
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F)
		{
			if (player.reverse > 0)
			{
				reverseActive = true;
			}
		}
		if (gameState == menu)
		{
			if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) && spBoxActive == true)
			{
				spBoxActive = false;
				tpBoxActive = true;
			}
			else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) && tpBoxActive == true)
			{
				tpBoxActive = false;
				hmBoxActive = true;
			}
			else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) && hmBoxActive == true)
			{
				hmBoxActive = false;
				shmBoxActive = true;
			}
			else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) && shmBoxActive == true)
			{
				shmBoxActive = false;
				hmBoxActive = true;
			}
			else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) && hmBoxActive == true)
			{
				hmBoxActive = false;
				tpBoxActive = true;
			}
			else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) && tpBoxActive == true)
			{
				tpBoxActive = false;
				spBoxActive = true;
			}
		}
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W)
		p1MovingUp = isPressed;
	else if (key == sf::Keyboard::S)
		p1MovingDown = isPressed;
	else if (key == sf::Keyboard::Up && (tpBoxActive == true || hmBoxActive == true))
	{
		if (reverseActive == true)
			p2MovingDown = isPressed;
		else
			p2MovingUp = isPressed;
	}
	else if (key == sf::Keyboard::Down && (tpBoxActive == true || hmBoxActive == true))
	{
		if (reverseActive == true)
			p2MovingUp = isPressed;
		else
			p2MovingDown = isPressed;
	}
	else if (key == sf::Keyboard::Escape)
		window.close();
}

void Game::Update(sf::Time elapsedTime)
{
	if (shmBoxActive == true)
		player2.setSpeed(sf::Vector2f(0, 300));

	if (reverseActive == false)
	{
		Dummy = powerUpTimer.restart();
	}

	//
	// Game state logic section
	//

	sf::Vector2f p1Movement(0.f, 0.f);
	sf::Vector2f p2Movement(0.f, 0.f);
	if (p1MovingUp)
		p1Movement.y -= player.getSpeed().y;
	if (p1MovingDown)
		p1Movement.y += player.getSpeed().y;
	if (p2MovingUp)
		p2Movement.y -= player2.getSpeed().y;
	if (p2MovingDown)
		p2Movement.y += player2.getSpeed().y;

	if (gameState == playing)
	{
		player.move(p1Movement * elapsedTime.asSeconds());
		player2.move(p2Movement * elapsedTime.asSeconds());
		ball.move(ball.getSpeed() * elapsedTime.asSeconds());
		if (invisBall.exists == true)
			invisBall.move(invisBall.getSpeed() * elapsedTime.asSeconds());
	}

	// Player has powerup
	if (player.eligible == true)
	{
		player.reverse++;
		player.eligible = false;
	}

	if (p1Score == 3 || p2Score == 3)
		gameState = victory;


	//
	// Positional logic section
	//

	// Powerup used
	if (reverseActive == true)
	{
		if (countdown.asSeconds() == 0)
		{
			countdown += powerUpTimer.restart();
			player.reverse--;
		}
		else if (countdown.asSeconds() > 0 && countdown.asSeconds() < 2)
		{
			countdown += powerUpTimer.restart();
		}
		else if (countdown.asSeconds() >= 2)
		{
			reverseActive = false;
			countdown = sf::Time::Zero;
		}
	}

	// Keep player one paddle inside the screen bounds
	if (player.getPosition().y < padding)
		player.setPosition(sf::Vector2f(padding, padding));
	if (player.getPosition().y > height - padding - player.getSize().y)
		player.setPosition(sf::Vector2f(padding, height - padding - player.getSize().y));

	// Keep player two paddle inside the screen bounds
	if (player2.getPosition().y < padding)
		player2.setPosition(sf::Vector2f(width - padding - player2.getSize().x, padding));
	if (player2.getPosition().y > height - padding - player2.getSize().y)
		player2.setPosition(sf::Vector2f(width - padding - player2.getSize().x, height - padding - player2.getSize().y));

	// Handle ball collision physics with wall
	if (ball.getPosition().y < padding + ball.getRadius() || ball.getPosition().y > height - padding - ball.getRadius())
	{
		ball.setSpeed(sf::Vector2f(ball.getSpeed().x, ball.getSpeed().y * -1));
		if (hmBoxActive == true)
			view.rotate(180);
	}

	// Change ball color depending which side it's on
	if (ball.getPosition().x < width / 2)
		ball.setFillColor(sf::Color::White);
	if (ball.getPosition().x > width / 2)
		ball.setFillColor(sf::Color::Black);

	// Handle ball collision with player one paddle
	if (phys.Collides(player, ball))
	{
		ball.setSpeed(sf::Vector2f(ball.getSpeed().x * -1.1, ball.getSpeed().y * 1.05));
		++ballHits;
		player.setHits();
		updateText();
		padSound1.play();
		if (hmBoxActive == true)
			view.rotate(180);
		if (player.getHits() % 5 == 0)
			player.eligible = true;
	}
	// Handle ball collision with player two paddle
	if (phys.Collides(player2, ball))
	{
		ball.setSpeed(sf::Vector2f(ball.getSpeed().x * -1.1, ball.getSpeed().y * 1.05));
		++ballHits;
		player2.setHits();
		updateText();
		padSound2.play();
		if (hmBoxActive == true)
			view.rotate(180);
	}

	// Player 2 AI
	if (spBoxActive == true)
	{
		if (ball.getPosition().y - ball.getRadius() < player2.getPosition().y)
		{
			p2MovingDown = false;
			p2MovingUp = true;
		}
		else if (ball.getPosition().y + ball.getRadius() > player2.getPosition().y + player2.getSize().y)
		{
			p2MovingUp = false;
			p2MovingDown = true;
		}
		else if (ball.getPosition().y - ball.getRadius() > player2.getPosition().y + 20 || ball.getPosition().y + ball.getRadius() < player2.getPosition().y + player2.getSize().y - 20)
		{
			p2MovingDown = false;
			p2MovingUp = false;
		}
	}
	if (shmBoxActive == true && invisBall.getSpeed().x == 0)
	{
		if (invisBall.getPosition().y - invisBall.getRadius() < player2.getPosition().y)
		{
			p2MovingDown = false;
			p2MovingUp = true;
		}
		else if (invisBall.getPosition().y + invisBall.getRadius() > player2.getPosition().y + player2.getSize().y)
		{
			p2MovingUp = false;
			p2MovingDown = true;
		}
		else if (invisBall.getPosition().y - invisBall.getRadius() > player2.getPosition().y + 35 || invisBall.getPosition().y + invisBall.getRadius() < player2.getPosition().y + player2.getSize().y - 35)
		{
			p2MovingDown = false;
			p2MovingUp = false;
		}
	}

	// Single Player Hardmode InvisBall Logic
	if ((shmBoxActive == true && invisBallLaunched == false) && ball.getSpeed().x > 0)
	{
		invisBallLaunched = true;
		invisBall.exists = true;
		invisBall.setRadius(10);
		invisBall.setOrigin(sf::Vector2f(10, 10));
		invisBall.setSpeed(sf::Vector2f(ball.getSpeed().x * 10, ball.getSpeed().y * 10));
		invisBall.setFillColor(sf::Color::Transparent);
#ifdef DEBUG
		invisBall.setOutlineColor(sf::Color::Yellow);
		invisBall.setOutlineThickness(2);
#endif
		invisBall.setPosition(sf::Vector2f(ball.getPosition().x, ball.getPosition().y));
	}
	if (shmBoxActive == true && ball.getSpeed().x < 0)
	{
		invisBall.exists = false;
		invisBallLaunched = false;
	}
	if (invisBall.exists == true && invisBallLaunched == true)
	{
		if (invisBall.getPosition().y < padding + invisBall.getRadius() || invisBall.getPosition().y > height - padding - invisBall.getRadius())
			invisBall.setSpeed(sf::Vector2f(invisBall.getSpeed().x, invisBall.getSpeed().y * -1));
		if (invisBall.getPosition().x > width - padding - invisBall.getRadius())
		{
			invisBall.setSpeed(sf::Vector2f(0, 0));
			invisBall.setPosition(sf::Vector2f(width - padding - invisBall.getRadius(), invisBall.getPosition().y));
		}
	}

	// Player two scores
	if (ball.getPosition().x < padding + ball.getRadius())
	{
		++p2Score;
		ballHits = 0;
		p2ScoreText = std::to_string(p2Score);
		p2ScoreDisplay.setString("P2 Score: " + p2ScoreText);
		ball.setSpeed(sf::Vector2f(-120, -80));
		ball.setPosition(width / 2, height / 2);
		gameState = pause;
		updateText();
		view.reset(sf::FloatRect(0, 0, 1, 1));
	}
	// Player one scores
	if (ball.getPosition().x > width - padding - ball.getRadius())
	{
		++p1Score;
		ballHits = 0;
		p1ScoreText = std::to_string(p1Score);
		p1ScoreDisplay.setString("P1 Score: " + p1ScoreText);
		ball.setSpeed(sf::Vector2f(120, -80));
		ball.setPosition(width / 2, height / 2);
		gameState = pause;
		updateText();
		view.reset(sf::FloatRect(0, 0, 1, 1));
		if (shmBoxActive == true)
			invisBallLaunched = false;
	}
}

void Game::Render()
{
	window.clear(sf::Color::Black);
	if (gameState == menu)
	{
		window.draw(playText);
		if (spBoxActive == true)
			window.draw(spBox);
		if (tpBoxActive == true)
			window.draw(tpBox);
		if (hmBoxActive == true)
			window.draw(hmBox);
		if (shmBoxActive == true)
			window.draw(shmBox);
		window.draw(singlePlayer);
		window.draw(twoPlayer);
		window.draw(hardMode);
		window.draw(singleHardMode);
	}
	else if (gameState == playing)
	{
		window.setView(view);
		view.setCenter(sf::Vector2f(width / 2, height / 2));
		view.setSize(sf::Vector2f(width, height));
		window.draw(leftSide);
		window.draw(rightSide);
		window.draw(player);
		window.draw(player2);
		window.draw(p1ScoreDisplay);
		window.draw(p2ScoreDisplay);
		window.draw(ballHitsDisplay);
#ifdef DEBUG
		window.draw(ballSpeedYDisplay);
		window.draw(ballSpeedXDisplay);
#endif
		window.draw(ball);
		if (invisBall.exists == true)
			window.draw(invisBall);
	}
	else if (gameState == pause)
	{
		window.draw(leftSide);
		window.draw(rightSide);
		window.draw(player);
		window.draw(player2);
		window.draw(p1ScoreDisplay);
		window.draw(p2ScoreDisplay);
		window.draw(ballHitsDisplay);
#ifdef DEBUG
		window.draw(ballSpeedYDisplay);
		window.draw(ballSpeedXDisplay);
#endif
		window.draw(ball);
		window.draw(paused);
	}
	else if (gameState == victory)
	{
		if (p1Score == 3)
			window.draw(p1Win);
		else if (p2Score == 3)
			window.draw(p2Win);
	}
	window.display();
}

void Game::updateText()
{
	ballHitsText = std::to_string(ballHits);
	ballSpeedYText = std::to_string(ball.getSpeed().y);
	ballSpeedXText = std::to_string(ball.getSpeed().x);
	ballHitsDisplay.setString("Ball hits: " + ballHitsText);
	ballSpeedYDisplay.setString("Ball Y: " + ballSpeedYText);
	ballSpeedXDisplay.setString("Ball X: " + ballSpeedXText);
}