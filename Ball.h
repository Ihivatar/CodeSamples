#pragma once

#include <SFML/Graphics.hpp>

class Ball : public sf::CircleShape
{
public:
	Ball();

	void setSpeed(sf::Vector2f speed);
	sf::Vector2f getSpeed();

	bool exists;

private:
	sf::Vector2f Speed;
};