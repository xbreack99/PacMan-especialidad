#pragma once

#include "../FrameworkCore/Utilitys.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include <iostream>
#include <memory>

using namespace std;

class TileMapComponent;
class Node;

class TileMap
{
public:
	using MapGrid = std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>;

	TileMap();
	
	TileMap(const std::string& path);


	~TileMap();
	
	void Initialize(const MapGrid& mapSketch);

	void Draw(sf::RenderWindow& window);

	std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> ParseMapToGame(const std::array<std::string, MAP_HEIGHT>& i_map_sketch);

	void BuildMesh();

	void UpdateTile(int x, int y);

	bool TryConsumePickup(const sf::Vector2f& worldCenter, Cell& consumedCell);

	bool TryTunnel(sf::Vector2f& position) const;

	bool LoadMapFromFile(const std::string& path, MapGrid& outGrid);

	std::shared_ptr<Node> GetNode() const { return mTileNode; }

	TileMapComponent* mTileMapComponent;

	sf::Vector2i mPlayerStartposition;

private:

	std::shared_ptr<Node> mTileNode;

	std::vector<int> mTunnelRows;

	void DetectTunnelRows();
};