#pragma once

#include "ISystem.h"

#include <vector>

class Node;

class ColisionSystem : public ISystem
{
public:
	ColisionSystem();

	~ColisionSystem();

	virtual void Update(float deltaTime , std::vector<Node*>& entities) override;

private:

	virtual void OnCollision(Node* entityA, Node* entityB);
};