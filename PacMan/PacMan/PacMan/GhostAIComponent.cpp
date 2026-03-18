#include "GhostAIComponent.h"

static constexpr sf::Vector2i SCATTER_BLINKY = { 25, 0 };
static constexpr sf::Vector2i SCATTER_PINKY = { 2,  0 };
static constexpr sf::Vector2i SCATTER_INKY = { 27, 30 };
static constexpr sf::Vector2i SCATTER_CLYDE = { 0,  30 };

static constexpr sf::Vector2i SPAWN_BLINKY = { 13, 11 };
static constexpr sf::Vector2i SPAWN_PINKY = { 13, 14 };
static constexpr sf::Vector2i SPAWN_INKY = { 11, 14 };
static constexpr sf::Vector2i SPAWN_CLYDE = { 15, 14 };

GhostAIComponent::GhostAIComponent(GhostType type) : mGhostType(type)
{
	switch (type)
	{
	case GhostType::Blinky:
		mScatterTargetTile = SCATTER_BLINKY;
		mSpawnTile = SPAWN_BLINKY;
		break;
	case GhostType::Pinky:
		mScatterTargetTile = SCATTER_PINKY;
		mSpawnTile = SPAWN_PINKY;
		break;
	case GhostType::Inky:
		mScatterTargetTile = SCATTER_INKY;
		mSpawnTile = SPAWN_INKY;
		break;
	case GhostType::Clyde:
		mScatterTargetTile = SCATTER_CLYDE;
		mSpawnTile = SPAWN_CLYDE;
		break;
	}

	mCurrentTile = mSpawnTile;
	mTargetTile = mScatterTargetTile;
	mPosition = {
		static_cast<float>(mSpawnTile.x * TILE_SIZE),
		static_cast<float>(mSpawnTile.y * TILE_SIZE)
	};
}
