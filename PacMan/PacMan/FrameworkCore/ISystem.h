#pragma once
#include <memory>
#include <vector>
#include "Node.h"

class ISystem
{
public:
	virtual ~ISystem() = 0;

	virtual void Update(float deltaTime) = 0;

	bool AddNode(std::shared_ptr<Node> node);

protected:
	std::vector<std::shared_ptr<Node>> mNodes;
};

inline ISystem::~ISystem() {}

inline bool ISystem::AddNode(std::shared_ptr<Node> node)
{
	if(node == nullptr)
		return false;

	mNodes.push_back(node);
	return true;
}