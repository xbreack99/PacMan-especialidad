#pragma once

#include "IComponent.h"
#include "Utilitys.h"
#include <array>
#include <SFML/Graphics.hpp>

class TileMapComponent : public IComponent
{
public:
	
	TileMapComponent();

	~TileMapComponent();
	
	ComponentType GetType() override;

	sf::IntRect GetTextureRectForCell(int x, int y);

	bool MoveAndCollide(sf::Vector2f& position, const sf::Vector2f& size, sf::Vector2f& velocity) const;

	static int ToTile(float coordinate)
	{
		return static_cast<int>(coordinate) / TILE_SIZE;
	};

	bool IsInsideMap(int x, int y) const;

	bool IsBlockingCell(int x, int y) const;

	std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> mMap;
	sf::Texture* mTexture;
	sf::VertexArray mVertexArray;
};