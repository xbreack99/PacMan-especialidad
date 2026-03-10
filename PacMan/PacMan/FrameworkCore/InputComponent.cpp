#include "InputComponent.h"

InputComponent::InputComponent()
{
}

InputComponent::~InputComponent()
{
	mKeyMap.clear();
}

ComponentType InputComponent::GetType()
{
	return ComponentType::INPUT;
}

bool InputComponent::isKeyPressed(sf::Keyboard::Key key)
{
	auto it = mKeyMap.find(key);
	if (it != mKeyMap.end())
	{
		return it->second;
	}
	return false;
}

void InputComponent::setKey(sf::Keyboard::Key key, bool isPressed)
{
	mKeyMap[key] = isPressed;
}
