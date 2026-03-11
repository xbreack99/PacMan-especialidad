#pragma once

#include "../FrameworkCore/Node.h"
#include <SFML/Graphics.hpp>

#include <array>
#include <string>
#include <iostream>

constexpr unsigned char MAP_HEIGHT = 21;
constexpr unsigned char MAP_WIDTH = 21;

using namespace std;

enum class Cell
{
	Door,
	Empty,
	Energizer,
	Pellet,
	Wall
};

class TileMap
{
public:
	
	TileMap();

	~TileMap();
	
	std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> ParseMapToGame(const std::array<std::string, MAP_HEIGHT>& i_map_sketch);

	void PrintMapToConsole(const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, sf::RenderWindow& i_window);




private:
};