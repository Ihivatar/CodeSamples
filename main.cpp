#define _CRT_SECURE_NO_WARNINGS

#include <SFML/Graphics.hpp>

#include <vector>

#include "MassObject.h"

const int num_objects = 5;
int max_size = 0;
int min_size = 0;
const int width = 1600;
const int height = 900;

void Simulation1()
{
	sf::RenderWindow window(sf::VideoMode(width, height, 32), "Physics");

	MassObject objects[num_objects] = {MassObject(250.0f, 250.0f, 50.0f), MassObject(800.0f, 800.0f, 25.0f), MassObject(1000.0f, 250.0f, 40.0f), MassObject(1200.0f, 650.0f, 70.0f), MassObject(70.0f, 700.0f, 35.0f)};
	objects[1].obj.setFillColor(sf::Color::Blue);
	objects[2].obj.setFillColor(sf::Color::Green);
	objects[3].obj.setFillColor(sf::Color::Yellow);
	objects[4].obj.setFillColor(sf::Color::Magenta);

	sf::Clock timer;
	sf::Time prev = timer.restart();

	while (window.isOpen())
	{
		sf::Time delta = timer.restart() - prev;
		prev = delta;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
			}
		}

		for (int i = 0; i < num_objects; ++i)
		{
			sf::Vector2f sum(0.0f, 0.0f);		// used to store all forces affecting object[i]
			for (int j = 0; j < num_objects; ++j)
			{
				if (i != j)
				{
					if (!objects[i].IsColliding(objects[j]))
					{
						objects[i].CalculateDirectionalForce(objects[j]);
						sum += Normalize(objects[i].dir);
					}
					else
					{
						objects[i].ResolveCollision(objects[j]);
					}
				}
			}
			objects[i].AddForce(sum.x, sum.y, delta.asSeconds());		// apply final force to object[i]
		}

		window.clear(sf::Color::Black);
		for (auto i = 0; i < num_objects; ++i)
			window.draw(objects[i].obj);
		window.display();
	}
}

void CreatePlanet(std::vector<MassObject>& objects, sf::Vector2i mouse_pos)
{
	float mass = 0.0f;
	int color = -1;
	sf::Color fillColor = sf::Color::Black;

	mass = rand()%max_size;
	if (mass < min_size)
		mass = min_size;
	color = rand()%6;

	if (color == 0)
		fillColor = sf::Color::White;
	else if (color == 1)
		fillColor = sf::Color::Red;
	else if (color == 2)
		fillColor = sf::Color::Blue;
	else if (color == 3)
		fillColor = sf::Color::Green;
	else if (color == 4)
		fillColor = sf::Color::Yellow;
	else if (color == 5)
		fillColor = sf::Color::Magenta;

	MassObject planet(mouse_pos.x, mouse_pos.y, mass);
	planet.obj.setFillColor(fillColor);
	planet.obj.setOrigin(planet.obj.getLocalBounds().width / 2.0f, planet.obj.getLocalBounds().height / 2.0f);

	objects.push_back(planet);
}

void Simulation2()
{
	sf::RenderWindow window(sf::VideoMode(width, height, 32), "Physics");

	std::vector<MassObject> objects;

	srand(time(NULL));

	sf::Clock timer;
	sf::Time prev = timer.restart();

	while (window.isOpen())
	{
		sf::Time delta = timer.restart() - prev;
		prev = delta;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
					CreatePlanet(objects, mouse_pos);
					printf("Objects: %i\n", objects.size());
				}
			}
		}

		for (auto i = 0; i < objects.size(); ++i)
		{
			sf::Vector2f sum(0.0f, 0.0f);	// used to store all forces affecting object[i]
			for (auto j = 0; j < objects.size(); ++j)
			{
				if (i != j)
				{
					if (!objects[i].IsColliding(objects[j]))
					{
						objects[i].CalculateDirectionalForce(objects[j]);
						sum += Normalize(objects[i].dir);
					}
					else
					{
						objects[i].ResolveCollision(objects[j]);
					}
				}
			}
			objects[i].AddForce(sum.x, sum.y, delta.asSeconds());	// apply final force to object[i]
		}

		window.clear(sf::Color::Black);
		for (auto i = 0; i < objects.size(); ++i)
			window.draw(objects[i].obj);
		window.display();
	}
}

int main()
{
	printf("1: Pre-placed planets.\n");
	printf("2: Create planets with click.\n");
	int choice = 0;
	scanf("%i", &choice);

	if (choice == 1)
		Simulation1();
	else
	{
		printf("Set max mass: ");
		scanf("%i", &max_size);
		printf("Set min mass: ");
		scanf("%i", &min_size);
		Simulation2();
	}

	return 0;
}
