#include "TileMap.h"
#include "../FrameworkCore/ColliderComponent.h"
#include "../FrameworkCore/GraphicsComponent.h"

TileMap::TileMap()
{
}

TileMap::~TileMap()
{
}

std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> TileMap::ParseMapToGame(const std::array<std::string, MAP_HEIGHT>& map_sketch)
{
	std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> output_map{};

	for (unsigned char x = 0; x < MAP_WIDTH; x++)
	{
		for (unsigned char y = 0; y < MAP_HEIGHT; y++)
		{
			switch (map_sketch[y][x])
			{
			case '#':
				output_map[x][y] = Cell::Wall;
				break;
			case '.':
				output_map[x][y] = Cell::Pellet;
				break;
			case 'o':
				output_map[x][y] = Cell::Energizer;
				break;
			case '=':
				output_map[x][y] = Cell::Door;
				break;
			default:
				output_map[x][y] = Cell::Empty;
				break;
			}
		}
	}
	return output_map;
}

void TileMap::PrintMapToConsole(const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, sf::RenderWindow& i_window)
{
	Node node;
	node.AddComponent(new GraphicsComponent());
	node.GetComponent<GraphicsComponent>()->mShape.setFillColor(sf::Color::Red);
	
	sf::Texture texture;
	texture.loadFromFile("Map.png");
	
	sf::Sprite sprite(texture);

	for (unsigned char x = 0; x < MAP_WIDTH; x++)
	{
		for (unsigned char y = 0; y < MAP_HEIGHT; y++)
		{
			sprite.setPosition(sf::Vector2f((16 * x),(16 * y)));

			switch (i_map[x][y])
			{
				case Cell::Door:
				{
					sprite.setTextureRect(sf::IntRect(sf::Vector2i(2 * 16, 16), sf::Vector2i(16, 16)));

					i_window.draw(sprite);

					break;
				}
				case Cell::Pellet:
				{
					sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 16), sf::Vector2i(16, 16)));

					i_window.draw(sprite);

					break;
				}
				case Cell::Energizer:
				{
					sprite.setTextureRect(sf::IntRect(sf::Vector2i(16, 16), sf::Vector2i(16, 16)));

					i_window.draw(sprite);

					break;
				}
				case Cell::Wall:
				{
					bool down = 0;
					bool left = 0;
					bool right = 0;
					bool up = 0;

					if (y < MAP_HEIGHT - 1)
					{
						if (Cell::Wall == i_map[x][1 + y])
						{
							down = 1;
						}
					}

					//Since we have warp tunnels, we need to draw them as if they're connected.
					if (0 < x)
					{
						if (Cell::Wall == i_map[x - 1][y])
						{
							left = 1;
						}
					}
					else
					{
						left = 1;
					}

					if (x < MAP_WIDTH - 1)
					{
						if (Cell::Wall == i_map[1 + x][y])
						{
							right = 1;
						}
					}
					else
					{
						right = 1;
					}

					if (0 < y)
					{
						if (Cell::Wall == i_map[x][y - 1])
						{
							up = 1;
						}
					}

					//--------------------------------------------<         DISTRIBUTIVE PROPERTY!         >----------------------------
					sprite.setTextureRect(sf::IntRect(sf::Vector2i(16 * (down + 2 * (left + 2 * (right + 2 * up))), 0),sf::Vector2i( 16, 16)));

					i_window.draw(sprite);
					break;
				}

			}
		}
	}
}
