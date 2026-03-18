#pragma once

#include <iostream>
#include "../FrameworkCore/Utilitys.h"
#include <SFML/Graphics.hpp>
#include <memory>

class GraphicsComponent;
class TransformComponent;
class ColliderComponent;
class InputComponent;
class MovementComponent;
class Node;

class PacMan
{
public:
	
	PacMan();
	
	~PacMan();

	void Update();

	void InputEvent(sf::Keyboard::Key key, bool isPressed);

	std::shared_ptr<Node> GetNode() const { return mPacMan; }

	sf::Vector2f GetPosition() const;

	sf::FloatRect GetBounds() const;

	void AddScore(int points) { score += points; }

private:
	
	std::shared_ptr<Node> mPacMan;
	
	int score = 0;

	GraphicsComponent* mGraphicsComponent;
	TransformComponent* mTransformComponent;
	ColliderComponent* mColliderComponent;
	InputComponent* mInputComponent;
	MovementComponent* mMovementComponent;
	
};