#pragma once

#include <iostream>

// Enum with all of type of components
enum class ComponentType
{
    NONE = 0,
    TRANSFORM,
    GRAPHICS,
    SOUND,
    INPUT,
    //TO BE DEFINE
};

class IComponent
{
 public:

    virtual ~IComponent() {}

	virtual ComponentType GetType() = 0;
};