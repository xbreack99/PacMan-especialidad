#pragma once

#include "../FrameworkCore/Utilitys.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include <iostream>

using namespace std;

class TileMapComponent;

class TileMap
{
public:
	using MapGrid = std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>;

	TileMap();

	~TileMap();
	
	void Initialize(const MapGrid& mapSketch);

	void Draw(sf::RenderWindow& window);

	std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> ParseMapToGame(const std::array<std::string, MAP_HEIGHT>& i_map_sketch);

	void BuildMesh();

	void UpdateTile(int x, int y);

	TileMapComponent* mTileMapComponent;
private:
};