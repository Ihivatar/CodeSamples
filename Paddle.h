#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Paddle : public sf::RectangleShape
{
public:
	Paddle();

	void setIsPlayer(bool isPlayer);
	bool getIsPlayer();
	void setSpeed(sf::Vector2f speed);
	sf::Vector2f getSpeed();
	void setHits();
	int getHits();

	int reverse;
	bool eligible;

private:
	sf::Vector2f Speed;

	int hits;
	bool isPlayer;
};