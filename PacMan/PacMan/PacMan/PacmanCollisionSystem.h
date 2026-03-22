#pragma once

#include "../FrameworkCore/ColisionSystem.h"
#include "GhostAISystem.h"
#include <functional>

class Node;

class PacmanCollisionSystem : public ColisionSystem
{
public:
	PacmanCollisionSystem();

	void SetGhostAISystem(GhostAISystem* system) { mGhostAISystem = system; };

	//create callbacks 
	std::function<void()> OnPacmanKilled;
	std::function<void(Node*)> OnGhostEaten;

protected:
	void OnCollision(Node* entityA, Node* entityB) override;

private:

	GhostAISystem* mGhostAISystem = nullptr;
};