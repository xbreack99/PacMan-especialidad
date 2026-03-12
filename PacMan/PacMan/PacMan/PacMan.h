#pragma once

#include <iostream>
#include "../FrameworkCore/Utilitys.h"
#include <SFML/Graphics.hpp>
class GraphicsComponent;
class TransformComponent;
class ColliderComponent;
class Node;

class PacMan
{
public:
	
	PacMan();
	
	~PacMan();

	void Update();

	void Draw();

	void MovePacman(
		sf::Vector2f& position,
		sf::Vector2f velocity,
		const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& map);

private:
	
	Node* mPacMan;
	
	GraphicsComponent* mGraphicsComponent;
	TransformComponent* mTransformComponent;
	ColliderComponent* mColliderComponent;
};