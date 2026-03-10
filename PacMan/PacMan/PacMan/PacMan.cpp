// PacMan.cpp : Defines the entry point for the application.
//

#include "PacMan.h"
#include "../FrameworkCore/Node.h"
#include "../FrameworkCore/IComponent.h"
#include "../FrameworkCore/GraphicsComponent.h"
#include "../FrameworkCore/TransformComponent.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

PacMan::PacMan() : mDirection(0)
{
}

PacMan::~PacMan()
{
}


