#pragma once

#include <sfml/Graphics.hpp>

class SceneManager;

enum class SCENE_TYPE
{
	NONE = 0,
	MENU,
	GAME,
	PAUSE,
	GAMEOVER,
};

class IScene
{
public:
	IScene(SceneManager& sceneManager, sf::RenderWindow& window)
		: mSceneManager(sceneManager), mWindow(window)
	{
	};

	virtual ~IScene() = default;

	virtual void OnEnter() = 0;

	virtual void OnExit() = 0;

	virtual void HandleEvent(const sf::Event& event) = 0;

	virtual void Update(float deltaTime) = 0;

	virtual void Render() = 0;

protected:
	SceneManager& mSceneManager;
	sf::RenderWindow& mWindow;
};