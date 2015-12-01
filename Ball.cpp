#include "Ball.h"

Ball::Ball()
{
	this->Speed = sf::Vector2f(0, 0);
	this->exists = false;
}

void Ball::setSpeed(sf::Vector2f speed)
{
	this->Speed = speed;
}

sf::Vector2f Ball::getSpeed()
{
	return this->Speed;
}
