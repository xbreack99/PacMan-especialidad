#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>
#include "../FrameworkCore/Node.h"
#include "../FrameworkCore/TransformComponent.h"
#include "../FrameworkCore/GraphicsComponent.h"
#include "../FrameworkCore/InputComponent.h"
#include "../FrameworkCore/RenderSystem.h"
#include "../FrameworkCore/InputSystem.h"

using namespace std;

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML window");
	
    shared_ptr<Node> Pacman = make_shared<Node>();

	Pacman->AddComponent(new TransformComponent());
    Pacman->AddComponent(new GraphicsComponent());
    RenderSystem renderSystem;

    InputSystem inputSystem;
    
    auto inputComponent = std::make_shared<InputComponent>();
    inputComponent->setKey(sf::Keyboard::Key::Up, false);
    inputComponent->setKey(sf::Keyboard::Key::Down, false);
    inputComponent->setKey(sf::Keyboard::Key::Left, false);
    inputComponent->setKey(sf::Keyboard::Key::Right, false);

	Pacman->AddComponent(inputComponent.get());

	renderSystem.AddNode(Pacman);
	inputSystem.AddNode(Pacman);

    const float timePerFrame = 1.0f / 60.0f;
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    // Start the game loop
    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        timeSinceLastUpdate += deltaTime;
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (inputComponent)
                {
                    inputComponent->setKey(keyPressed->code, true);
                }
            }
            else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
            {
                if (inputComponent)
                {
                    inputComponent->setKey(keyReleased->code, false);
                }
            }
        }

        while (timeSinceLastUpdate.asSeconds() > timePerFrame)
        {
            timeSinceLastUpdate -= sf::seconds(timePerFrame);
            inputSystem.Update(timePerFrame);
        }

        // Clear screen
        window.clear();
		renderSystem.Update(0.f);
		renderSystem.Render(window);
        // Update the window
        window.display();
    }
}
