#pragma once

#include "ISystem.h"

class TileMapComponent;

class TileCollisionSystem : public ISystem
{
public:

	TileCollisionSystem();

	~TileCollisionSystem();
	
	void Update(float deltaTime) override;
	void SetTileMap(TileMapComponent* tileMap) { mTileMap = tileMap; };
private:
	TileMapComponent* mTileMap = nullptr;
};