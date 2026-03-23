#pragma once
#include <SFML/Graphics.hpp>
#include "GhostAIComponent.h"
#include <memory>

class Node;
class MovementComponent;
class TransformComponent;
class GraphicsComponent;
class ColliderComponent;

class Ghost
{
public:
	Ghost();

	Ghost(GhostType type);

	~Ghost();

	void Initialize(GhostType type);

	std::shared_ptr<Node> GetNode() const { return mGhost; }

private:
	std::shared_ptr<Node> mGhost;

	MovementComponent* mMovementComponent;
	GraphicsComponent* mGraphicsComponent;
	TransformComponent* mTransformComponent;
	GhostAIComponent* mGhostAIComponent;
	ColliderComponent* mColiderComponent;
};