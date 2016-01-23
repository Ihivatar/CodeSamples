#include "GamePhysics.h"

bool Physics::Collides(sf::RectangleShape rect1, sf::RectangleShape rect2)
{
	if (rect1.getGlobalBounds().intersects(rect2.getGlobalBounds()))
		return true;
	return false;
}

bool Physics::Collides(sf::RectangleShape rect1, sf::CircleShape circle1)
{
	if (rect1.getGlobalBounds().intersects(circle1.getGlobalBounds()))
		return true;
	return false;
}