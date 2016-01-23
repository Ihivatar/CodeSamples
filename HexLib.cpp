#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <time.h>

#include "Hex.h"

#define DEBUG

const int WIDTH = 1600;
const int HEIGHT = 900;
const Point ORIGIN = Point(WIDTH / 2.0f, HEIGHT / 2.0f);
const int RADIUS = 7;

enum OrientationType {FLAT, POINTY};

bool ValidHex(Hex a);

enum TEXTURES {DIRT, GRASS, SNOW, WATER};

namespace std
{
	template <> struct hash < Hex >
	{
		size_t operator () (const Hex& h) const
		{
			hash<int> int_hash;
			size_t hq = int_hash(h.q);
			size_t hr = int_hash(h.r);

			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
		}
	};
}

bool operator == (const Hex a, const Hex b)
{
	if (a.q == b.q && a.r == b.r && a.s == b.s)
		return true;
	return false;
}

struct Shape
{
	sf::ConvexShape hex;
	Hex coords;
	Layout layout;
	Hex neighbors[6];
	int id = -1;

	Shape()
	{
		for (int i = 0; i < 6; ++i)
			neighbors[i] = Hex(0, 0, 0);
	}
};

void ClearBoard(std::unordered_map<Hex, Shape>& map, sf::Color color)
{
	for (auto i = map.begin(); i != map.end(); ++i)
		(*i).second.hex.setFillColor(color);
}

void DrawHexes(std::unordered_map<Hex, Shape>& hexes, sf::RenderWindow& window)
{
	for (auto i = hexes.begin(); i != hexes.end(); ++i)
	{
		window.draw((*i).second.hex);
	}
}

void CreateShapeHex(OrientationType type, Point size, Point origin, Shape& shape)
{
	if (type == FLAT)
		shape.layout.orientation = shape.layout.flat;
	else if (type == POINTY)
		shape.layout.orientation = shape.layout.pointy;
	shape.layout.size = size;
	shape.layout.origin = origin;
	shape.layout.origin = HexToPixel(shape.layout, shape.coords);
	std::vector<Point> corners = PolygonCorners(shape.layout, shape.coords);
	shape.hex.setPointCount(6);
	for (int i = 0; i < 6; ++i)
	{
		shape.hex.setPoint(i, sf::Vector2f(corners[i].x, corners[i].y));
	}
}

void FindNeighbors(std::unordered_map<Hex, Shape>& map, Hex coords)
{
	for (int i = 0; i < 6; ++i)
	{
		map[coords].neighbors[i] = map[Neighbor(coords, i)].coords;
		auto nb = map[coords].neighbors[i];
	}
}

void InitNeighbors(std::unordered_map<Hex, Shape>& map)
{
	for (int q = -RADIUS; q <= RADIUS; ++q)
	{
		int r1 = std::max(-RADIUS, -q - RADIUS);
		int r2 = std::min(RADIUS, -q + RADIUS);
		for (int r = r1; r <= r2; ++r)
		{
			FindNeighbors(map, Hex(q, r));
		}
	}
}

void ShowNeighbors(std::unordered_map<Hex, Shape>& map, Hex coords, sf::Color color)
{
	for (int i = 0; i < 6; ++i)
	{
		auto neighbor = map[coords].neighbors[i];
		map[neighbor].hex.setFillColor(color);
	}
}

void CreateMapHex(int map_radius, std::unordered_map<Hex, Shape>& map, OrientationType type, Point size)
{
	for (int q = -map_radius; q <= map_radius; ++q)
	{
		int r1 = std::max(-map_radius, -q - map_radius);
		int r2 = std::min(map_radius, -q + map_radius);
		for (int r = r1; r <= r2; ++r)
		{
			Shape temp;
			temp.coords = Hex(q, r, -q - r);
			CreateShapeHex(type, size, ORIGIN, temp);
			temp.hex.setOutlineThickness(1);
			temp.hex.setOutlineColor(sf::Color::Black);
			int num = rand() % 4;
			temp.id = num;
			map[temp.coords] = temp;
		}
	}
	InitNeighbors(map);
}

void DrawLine(std::unordered_map<Hex, Shape>& map, Hex a, Hex b, sf::Color color)
{
	std::vector<Hex> line = HexLineDraw(map[a].coords, map[b].coords);

	for (int i = 0; i < line.size(); ++i)
		map[line[i]].hex.setFillColor(color);
}

void MovePlayer(std::unordered_map<Hex, Shape>& map, Shape& player, Hex dest, sf::Texture* textures)
{
	player = map[dest];
	player.id = 4;
	player.hex.setTexture(&textures[player.id]);
}

bool ValidHex(Hex a)
{
	if (a.q >= -RADIUS && a.q <= RADIUS &&
		a.r >= -RADIUS && a.r <= RADIUS &&
		a.s >= -RADIUS && a.s <= RADIUS)
		return true;
	return false;
}

void ApplyTextures(std::unordered_map<Hex, Shape>& map, sf::Texture* textures)
{
	for (int q = -RADIUS; q <= RADIUS; ++q)
	{
		int r1 = std::max(-RADIUS, -q - RADIUS);
		int r2 = std::min(RADIUS, -q + RADIUS);
		for (int r = r1; r <= r2; ++r)
		{
			if (map[Hex(q, r)].id >= 0)
				map[Hex(q, r)].hex.setTexture(&textures[map[Hex(q, r)].id]);
		}
	}
}

int main()
{
	sf::Texture* textures = new sf::Texture[6];
	textures[0].loadFromFile("HexTileset/Terrain/Dirt.png");
	textures[1].loadFromFile("HexTileset/Terrain/Grass.png");
	textures[2].loadFromFile("HexTileset/Terrain/Snow.png");
	textures[3].loadFromFile("HexTileset/Terrain/Water.png");
	textures[4].loadFromFile("HexTileset/Units/Air.png");
	textures[5].loadFromFile("HexTileset/Roads/Junction A.png");
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Hex Grid Fun");

	std::unordered_map<Hex, Shape> hex_map;
	CreateMapHex(RADIUS, hex_map, FLAT, Point(32.0f, 32.0f));

	Shape Player;
	Player.id = 4;
	Player.coords = Hex(0, 0);
	CreateShapeHex(FLAT, Point(32.0f, 32.0f), ORIGIN, Player);
	Player.hex.setTexture(&textures[Player.id]);
	ApplyTextures(hex_map, textures);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					window.close();
					break;
				}
			case sf::Event::MouseButtonPressed:
				sf::Vector2i mousePos(0, 0);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					mousePos = sf::Mouse::getPosition(window);
					FractionalHex fract = PixelToHex(hex_map[Hex(0, 0, 0)].layout, Point(mousePos.x, mousePos.y));
					Hex temp = HexRound(fract);
					if (ValidHex(temp))
					{
						auto found = hex_map.find(temp)->second;
						ClearBoard(hex_map, sf::Color::White);
#ifdef DEBUG
						DrawLine(hex_map, Player.coords, temp, sf::Color::Red);
						ShowNeighbors(hex_map, temp, sf::Color::Green);
#endif
						MovePlayer(hex_map, Player, temp, textures);
					}
				}
			}
		}

		window.clear(sf::Color::Black);
		DrawHexes(hex_map, window);
		window.draw(Player.hex);
		window.display();
	}

	return 0;
}