#ifndef GAME_PHYSICS_H
#define GAME_PHYSICS_H

#include <SFML/Graphics.hpp>

class Physics
{
public:
	bool Collides(sf::RectangleShape rect1, sf::RectangleShape rect2);
	bool Collides(sf::RectangleShape rect1, sf::CircleShape circle1);
};

#endif