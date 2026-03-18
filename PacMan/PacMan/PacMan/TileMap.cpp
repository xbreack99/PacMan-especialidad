#include "TileMap.h"
#include "../FrameworkCore/ColliderComponent.h"
#include "../FrameworkCore/GraphicsComponent.h"
#include "../FrameworkCore/TileMapComponent.h"
#include "../FrameworkCore/Node.h"

TileMap::TileMap()
{
}

TileMap::~TileMap()
{

}

//do some checkings 
void TileMap::Initialize(const MapGrid& mapSketch)
{
	mTileNode = std::make_shared<Node>();
	
	mTileMapComponent = new TileMapComponent();
	mTileMapComponent->mMap = mapSketch;
	BuildMesh();

	mTileNode->AddComponent(mTileMapComponent);
}

void TileMap::Draw(sf::RenderWindow& window)
{
	window.draw(mTileMapComponent->mVertexArray, mTileMapComponent->mTexture);
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

bool TileMap::TryConsumePickup(const sf::Vector2f& worldCenter, Cell& consumedCell)
{
	const int tileX = mTileMapComponent->ToTile(worldCenter.x);
	const int tileY = mTileMapComponent->ToTile(worldCenter.y);

	if (!mTileMapComponent->IsInsideMap(tileX, tileY))
	{
		return false;
	}

	Cell& cell = mTileMapComponent->mMap[tileX][tileY];

	if (cell == Cell::Pellet || cell == Cell::Energizer)
	{
		consumedCell = cell;
		cell = Cell::Empty;
		UpdateTile(static_cast<unsigned int>(tileX), static_cast<unsigned int>(tileY));
		return true;
	}

	return false;
}
void TileMap::BuildMesh()
{
	mTileMapComponent->mVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
	mTileMapComponent->mVertexArray.resize(MAP_WIDTH * MAP_HEIGHT * 6);

	for (unsigned int x = 0; x < MAP_WIDTH; ++x)
	{
		for (unsigned int y = 0; y < MAP_HEIGHT; ++y)
		{
			UpdateTile(x, y);
		}
	}
}

void TileMap::UpdateTile(int x, int y)
{
	const sf::IntRect textureRect = mTileMapComponent->GetTextureRectForCell(x, y);

	const float worldX = static_cast<float>(x * TILE_SIZE);
	const float worldY = static_cast<float>(y * TILE_SIZE);

	const float texLeft = static_cast<float>(textureRect.position.x);
	const float texTop = static_cast<float>(textureRect.position.y);
	const float texRight = static_cast<float>(textureRect.position.x + textureRect.size.x);
	const float texBottom = static_cast<float>(textureRect.position.y + textureRect.size.y);

	const std::size_t baseIndex = (x + MAP_WIDTH * y) * 6;
	sf::Vertex* triangle = &mTileMapComponent->mVertexArray[baseIndex];

	if (mTileMapComponent->mMap[x][y] == Cell::Empty)
	{
		for (unsigned int i = 0; i < 6; ++i)
		{
			triangle[i].position = { worldX, worldY };
			triangle[i].texCoords = { 0.f, 0.f };
			triangle[i].color = sf::Color::Transparent;
		}
		return;
	}

	// Triángulo 1
	triangle[0].position = { worldX, worldY };
	triangle[1].position = { worldX + TILE_SIZE, worldY };
	triangle[2].position = { worldX + TILE_SIZE, worldY + TILE_SIZE };

	triangle[0].texCoords = { texLeft, texTop };
	triangle[1].texCoords = { texRight, texTop };
	triangle[2].texCoords = { texRight, texBottom };

	// Triángulo 2
	triangle[3].position = { worldX, worldY };
	triangle[4].position = { worldX + TILE_SIZE, worldY + TILE_SIZE };
	triangle[5].position = { worldX, worldY + TILE_SIZE };

	triangle[3].texCoords = { texLeft, texTop };
	triangle[4].texCoords = { texRight, texBottom };
	triangle[5].texCoords = { texLeft, texBottom };

	for (unsigned int i = 0; i < 6; ++i)
	{
		triangle[i].color = sf::Color::White;
	}
}
