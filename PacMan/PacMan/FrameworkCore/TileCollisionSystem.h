#pragma once

#include "ISystem.h"

class TileCollisionSystem : public ISystem
{
public:

	TileCollisionSystem();

	~TileCollisionSystem();
	
	void Update(float deltaTime) override;
};