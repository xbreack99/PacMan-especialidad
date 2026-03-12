#include "TileMapComponent.h"
#include <SFML/Graphics.hpp>

TileMapComponent::TileMapComponent()
{
	mTexture = new sf::Texture("Map.png");
}

TileMapComponent::~TileMapComponent()
{
}

bool TileMapComponent::IsInsideMap(int x, int y) const
{
	return x >= 0 && x < static_cast<int>(MAP_WIDTH) &&
		y >= 0 && y < static_cast<int>(MAP_HEIGHT);;
}

bool TileMapComponent::IsBlockingCell(int x, int y) const
{
	if (!IsInsideMap(x, y))
	{
		return false;
	}

	const Cell cell = mMap[x][y];
	return cell == Cell::Wall || cell == Cell::Door;
}

bool TileMapComponent::MoveAndCollide(sf::Vector2f& position, const sf::Vector2f& size, sf::Vector2f& velocity) const
{
	bool collided = false;
	sf::FloatRect bounds(position, size);

	// -------------------- X --------------------
	bounds.position.x += velocity.x;

	int leftTile = ToTile(bounds.position.x);
	int rightTile = ToTile(bounds.position.x + bounds.size.x - 0.001f);
	int topTile = ToTile(bounds.position.y);
	int bottomTile = ToTile(bounds.position.y + bounds.size.y - 0.001f);

	if (velocity.x != 0.f)
	{
		const int xToCheck = (velocity.x > 0.f) ? rightTile : leftTile;

		for (int y = topTile; y <= bottomTile; ++y)
		{
			if (!IsBlockingCell(xToCheck, y))
			{
				continue;
			}

			const sf::FloatRect tileBounds(
				sf::Vector2f(static_cast<float>(xToCheck * TILE_SIZE), static_cast<float>(y * TILE_SIZE)),
				sf::Vector2f(static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE))
			);

			if (bounds.findIntersection(tileBounds).has_value())
			{
				collided = true;

				if (velocity.x > 0.f)
				{
					bounds.position.x = tileBounds.position.x - bounds.size.x;
				}
				else
				{
					bounds.position.x = tileBounds.position.x + tileBounds.size.x;
				}

				velocity.x = 0.f;
				break;
			}
		}
	}

	// -------------------- Y --------------------
	bounds.position.y += velocity.y;

	leftTile = ToTile(bounds.position.x);
	rightTile = ToTile(bounds.position.x + bounds.size.x - 0.001f);
	topTile = ToTile(bounds.position.y);
	bottomTile = ToTile(bounds.position.y + bounds.size.y - 0.001f);

	if (velocity.y != 0.f)
	{
		const int yToCheck = (velocity.y > 0.f) ? bottomTile : topTile;

		for (int x = leftTile; x <= rightTile; ++x)
		{
			if (!IsBlockingCell(x, yToCheck))
			{
				continue;
			}

			const sf::FloatRect tileBounds(
				sf::Vector2f(static_cast<float>(x * TILE_SIZE), static_cast<float>(yToCheck * TILE_SIZE)),
				sf::Vector2f(static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE))
			);

			if (bounds.findIntersection(tileBounds).has_value())
			{
				collided = true;

				if (velocity.y > 0.f)
				{
					bounds.position.y = tileBounds.position.y - bounds.size.y;
				}
				else
				{
					bounds.position.y = tileBounds.position.y + tileBounds.size.y;
				}

				velocity.y = 0.f;
				break;
			}
		}
	}

	position = bounds.position;
	return collided;
}

ComponentType TileMapComponent::GetType()
{
	return ComponentType::TILEMAP;
}

sf::IntRect TileMapComponent::GetTextureRectForCell(int x, int y)
{
    switch (mMap[x][y])
    {
    case Cell::Door:
    {
        return sf::IntRect(sf::Vector2i(2 * TILE_SIZE, TILE_SIZE), sf::Vector2i( TILE_SIZE, TILE_SIZE));
    }

    case Cell::Pellet:
    {
        return sf::IntRect(sf::Vector2i(0, TILE_SIZE), sf::Vector2i(TILE_SIZE, TILE_SIZE));
    }

    case Cell::Energizer:
    {
        return sf::IntRect(sf::Vector2i(TILE_SIZE, TILE_SIZE), sf::Vector2i(TILE_SIZE, TILE_SIZE));
    }

    case Cell::Wall:
    {
        bool down = false;
        bool left = false;
        bool right = false;
        bool up = false;

        if (y < MAP_HEIGHT - 1 && mMap[x][y + 1] == Cell::Wall)
        {
            down = true;
        }

        if (x > 0)
        {
            if (mMap[x - 1][y] == Cell::Wall)
            {
                left = true;
            }
        }
        else
        {
            left = true; // warp tunnel visual
        }

        if (x < MAP_WIDTH - 1)
        {
            if (mMap[x + 1][y] == Cell::Wall)
            {
                right = true;
            }
        }
        else
        {
            right = true; // warp tunnel visual
        }

        if (y > 0 && mMap[x][y - 1] == Cell::Wall)
        {
            up = true;
        }

        const int tileIndex = down + 2 * (left + 2 * (right + 2 * up));
        return sf::IntRect(sf::Vector2i(tileIndex * TILE_SIZE, 0), sf::Vector2i(TILE_SIZE, TILE_SIZE));
    }

    default:
    {
        return sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(0, 0)); // Empty
    }

    }
}