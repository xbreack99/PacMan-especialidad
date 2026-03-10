#pragma once

#include <iostream>

class IComponent;
class Node;

class PacMan
{
public:
	
	PacMan();
	
	~PacMan();

	void Update();

	void Draw();

private:
	unsigned int mDirection;
	
	Node* mPacMan;
	
	IComponent* mGraphicsComponent;
	IComponent* mTransformComponent;
};