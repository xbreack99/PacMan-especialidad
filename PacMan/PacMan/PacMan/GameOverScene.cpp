#include "GameOverScene.h"
#include "../FrameworkCore/SceneManager.h"
#include <string>

GameOverScene::GameOverScene(SceneManager& sceneManager, sf::RenderWindow& window)
	: IScene(sceneManager, window)
	, mResultText(mFont)
	, mScoreText(mFont)
	, mRestartText(mFont)
	, mMenuText(mFont)
{
	mFont.openFromFile("");
	RebuildTexts();
	mSelectedIndex = 0;
	UpdateSelection();
}

void GameOverScene::OnEnter()
{
}

void GameOverScene::OnExit()
{
}

void GameOverScene::HandleEvent(const sf::Event& event)
{
}

void GameOverScene::Update(float deltaTime)
{
}

void GameOverScene::Render()
{
}

void GameOverScene::RebuildTexts()
{
}

void GameOverScene::UpdateSelection()
{
}
