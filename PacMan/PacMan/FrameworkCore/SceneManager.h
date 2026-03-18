#pragma once

#include "IScene.h"
#include <memory>
#include <map>
#include <functional>
#include <stack>

class SceneManager
{
public:
	explicit SceneManager(sf::RenderWindow& window);
	~SceneManager();

	void RegisterScene(SCENE_TYPE type, std::unique_ptr<IScene> scene);

	void ChangeScene(SCENE_TYPE type);
	void PushScene(SCENE_TYPE type);
	void PopScene();
	void Quit();

	void HandleEvent(const sf::Event& event);
	void Update(float deltaTime);
	void Render();

	bool IsRunning() const { return !mScenes.empty(); }

	IScene* GetScene(SCENE_TYPE type) const;
private:
	IScene* TopScene() const;

	void ApplyPendingCommand();

	sf::RenderWindow& mWindow;
	std::map<SCENE_TYPE, std::unique_ptr<IScene>> mScenes;
	
	std::stack<SCENE_TYPE> mSceneStack;
	
	enum class Command
	{
		None,
		ChangeScene,
		PushScene,
		PopScene,
		Quit
	};

	struct PendingCommand
	{
		Command command = Command::None;
		SCENE_TYPE sceneType = SCENE_TYPE::NONE;
	};

	PendingCommand mPendingCommand;
};
