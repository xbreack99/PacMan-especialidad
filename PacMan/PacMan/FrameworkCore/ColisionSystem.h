#pragma once

#include "ISystem.h"

#include <vector>

class Node;

class ColisionSystem : public ISystem
{
public:
	ColisionSystem();

	~ColisionSystem();

	void Update(float deltaTime , std::vector<Node*>& entities);

private:

	void OnCollision(Node* entityA, Node* entityB);
};