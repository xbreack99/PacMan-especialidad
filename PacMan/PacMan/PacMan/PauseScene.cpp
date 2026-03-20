#include "PauseScene.h"
#include "../FrameworkCore/SceneManager.h"

PauseScene::PauseScene(SceneManager& sceneManager, sf::RenderWindow& window)
	: IScene(sceneManager, window)
	, mResumeText(mFont)
	, mPauseText(mFont)
	, mMenuText(mFont)
{
}

void PauseScene::OnEnter()
{
	mFont.openFromFile("ARCADE_N.ttf");

    const sf::Vector2u winSize = mWindow.getSize();
    const float cx = static_cast<float>(winSize.x) / 2.f;
    const float cy = static_cast<float>(winSize.y) / 2.f;

    // Semi-transparent overlay
    mOverlay.setSize(sf::Vector2f(static_cast<float>(winSize.x),
        static_cast<float>(winSize.y)));
    mOverlay.setFillColor(sf::Color(0, 0, 0, 160));

    mPauseText.setFont(mFont);
    mPauseText.setString("PAUSED");
    mPauseText.setCharacterSize(52);
    mPauseText.setFillColor(sf::Color::Yellow);
    mPauseText.setOrigin(mPauseText.getLocalBounds().size / 2.f);
    mPauseText.setPosition({ cx, cy - 80.f });

    mResumeText.setFont(mFont);
    mResumeText.setString("RESUME");
    mResumeText.setCharacterSize(30);
    mResumeText.setOrigin(mResumeText.getLocalBounds().size / 2.f);
    mResumeText.setPosition({ cx, cy + 20.f });

    mMenuText.setFont(mFont);
    mMenuText.setString("MAIN MENU");
    mMenuText.setCharacterSize(30);
    mMenuText.setOrigin(mMenuText.getLocalBounds().size / 2.f);
    mMenuText.setPosition({ cx, cy + 70.f });

    mSelectedIndex = 0;
    UpdateSelection();

}

void PauseScene::OnExit()
{
}

void PauseScene::HandleEvent(const sf::Event& event)
{
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>())
    {
        switch (keyEvent->code)
        {
        case sf::Keyboard::Key::Escape:
            mSceneManager.PopScene();
            break;

        case sf::Keyboard::Key::Up:
        case sf::Keyboard::Key::W:
            mSelectedIndex = (mSelectedIndex - 1 + 2) % 2;
            UpdateSelection();
            break;

        case sf::Keyboard::Key::Down:
        case sf::Keyboard::Key::S:
            mSelectedIndex = (mSelectedIndex + 1) % 2;
            UpdateSelection();
            break;

        case sf::Keyboard::Key::Enter:
        case sf::Keyboard::Key::Space:
            if (mSelectedIndex == 0)
                mSceneManager.PopScene();
            else
                mSceneManager.ChangeScene(SCENE_TYPE::MENU);
            break;

        default: break;
        }
    }
}

void PauseScene::Update(float deltaTime)
{
    mBlinkTimer += deltaTime;
    if (mBlinkTimer >= 0.5f)
    {
        mBlinkTimer = 0.f;
        mShowCursor = !mShowCursor;
        UpdateSelection();
    }
}

void PauseScene::Render()
{
    mWindow.draw(mOverlay);
    mWindow.draw(mPauseText);
    mWindow.draw(mResumeText);
    mWindow.draw(mMenuText);
    mWindow.display();
}

void PauseScene::UpdateSelection()
{
    auto highlight = [&](sf::Text& text, bool selected)
        {
            if (selected)
            {
                text.setFillColor(mShowCursor ? sf::Color::Yellow : sf::Color(180, 180, 0));
            }
            else
            {
                text.setFillColor(sf::Color::White);
            }
        };

    highlight(mResumeText, mSelectedIndex == 0);
    highlight(mMenuText, mSelectedIndex == 1);
}
