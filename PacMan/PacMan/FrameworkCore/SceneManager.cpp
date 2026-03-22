#include "SceneManager.h"
#include <SFML/Graphics.hpp>

SceneManager::SceneManager(sf::RenderWindow& window) : mWindow(window)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::RegisterScene(SCENE_TYPE type, std::unique_ptr<IScene> scene)
{
	mScenes[type] = std::move(scene);
}

void SceneManager::ChangeScene(SCENE_TYPE type)
{
	mPendingCommand = { Command::ChangeScene, type };
}

void SceneManager::PushScene(SCENE_TYPE type)
{
	mPendingCommand = { Command::PushScene, type };
}

void SceneManager::PopScene()
{
	mPendingCommand = { Command::PopScene, SCENE_TYPE::NONE };
}

void SceneManager::Quit()
{
	mPendingCommand = { Command::Quit, SCENE_TYPE::NONE };
}

void SceneManager::HandleEvent(const sf::Event& event)
{
	if (IScene* scene = TopScene())
	{
		scene->HandleEvent(event);
	}
}

void SceneManager::Update(float deltaTime)
{
	if(IScene* scene = TopScene())
	{
		scene->Update(deltaTime);
	}
	ApplyPendingCommand();
}

void SceneManager::Render()
{
	if (mSceneStack.empty() >= 2)
	{
		std::stack<SCENE_TYPE> tempStack = mSceneStack;
		tempStack.pop();
		auto scene = mScenes.find(tempStack.top());
		if (scene != mScenes.end())
		{
			scene->second->Render();
		}
	}
	else
	{
		if (IScene* scene = TopScene())
		{
			scene->Render();
			return;
		}
	}

	if (IScene* scene = TopScene())
	{
		scene->Render();
	}
}

IScene* SceneManager::GetScene(SCENE_TYPE type) const
{
	auto scene = mScenes.find(type);
	return (scene != mScenes.end()) ? scene->second.get() : nullptr;
}

IScene* SceneManager::TopScene() const
{
	if (mSceneStack.empty()) return nullptr;
	auto it = mScenes.find(mSceneStack.top());
	return (it != mScenes.end()) ? it->second.get() : nullptr;
}

void SceneManager::ApplyPendingCommand()
{
	if (mPendingCommand.command == Command::None)
		return;

	const Command command = mPendingCommand.command;	
	const SCENE_TYPE sceneType = mPendingCommand.sceneType;
	mPendingCommand = { Command::None, SCENE_TYPE::NONE };

	switch (command)
	{
		case Command::ChangeScene:
		{
			if (!mSceneStack.empty())
			{
				if (IScene* old = TopScene()) old->OnExit();
				mSceneStack.pop();
			}
			mSceneStack.push(sceneType);
			if (IScene* next = TopScene()) next->OnEnter();
			break;
		}
		case Command::PushScene:
		{
			mSceneStack.push(sceneType);
			if (IScene* next = TopScene()) next->OnEnter();
			break;
		}
		case Command::PopScene:
		{
			if (!mSceneStack.empty())
			{
				if (IScene* old = TopScene()) old->OnExit();
				mSceneStack.pop();
			}
			break;

		}
		case Command::Quit:
		{
			while (!mSceneStack.empty())
			{
				if (IScene* old = TopScene()) old->OnExit();
				mSceneStack.pop();
			}
			mWindow.close();
			break;
		}
	default:
		break;
	}
}
