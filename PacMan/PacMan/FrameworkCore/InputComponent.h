#pragma once

#include "IComponent.h"
#include <unordered_map>
#include <SFML/Window/Keyboard.hpp>

class InputComponent : public IComponent
{
 public:
	
	InputComponent();
	
	~InputComponent();

	ComponentType GetType() override;

	bool isKeyPressed(sf::Keyboard::Key key);

	void setKey(sf::Keyboard::Key key, bool isPressed);

public:
	std::unordered_map<sf::Keyboard::Key, bool> mKeyMap;

};
