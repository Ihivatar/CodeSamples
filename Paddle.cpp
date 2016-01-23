#include "Paddle.h"

Paddle::Paddle()
{
	this->Speed = sf::Vector2f(0, 0);
	this->isPlayer = false;
	this->hits = 0;
	this->reverse = 0;
	this->eligible = false;
}

void Paddle::setIsPlayer(bool isPlayer)
{
	this->isPlayer = isPlayer;
}

bool Paddle::getIsPlayer()
{
	return this->isPlayer;
}

void Paddle::setSpeed(sf::Vector2f speed)
{
	this->Speed = speed;
}

sf::Vector2f Paddle::getSpeed()
{
	return this->Speed;
}

void Paddle::setHits()
{
	++hits;
}

int Paddle::getHits()
{
	return this->hits;
}