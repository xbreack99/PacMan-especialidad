#include "MenuScene.h"
#include  "../FrameworkCore/SceneManager.h"

MenuScene::MenuScene(SceneManager& sceneManager, sf::RenderWindow& window)
	: IScene(sceneManager, window)
	, mTitle(mFont)
	, mStartText(mFont)
	, mQuitText(mFont)
{
}

void MenuScene::OnEnter()
{
    if (!mFont.openFromFile("ARCADE_N.ttf"))
    {
    }

    const sf::Vector2u winSize = mWindow.getSize();
    const float cx = static_cast<float>(winSize.x) / 2.f;

    mTitle.setFont(mFont);
    mTitle.setString("PAC-MAN");
    mTitle.setCharacterSize(40);
    mTitle.setFillColor(sf::Color::Yellow);
    mTitle.setOrigin(mTitle.getLocalBounds().size / 2.f);
    mTitle.setPosition({ cx, static_cast<float>(winSize.y) * 0.25f });

    mStartText.setFont(mFont);
    mStartText.setString("START GAME");
    mStartText.setCharacterSize(32);
    mStartText.setOrigin(mStartText.getLocalBounds().size / 2.f);
    mStartText.setPosition({ cx, static_cast<float>(winSize.y) * 0.55f });

    mQuitText.setFont(mFont);
    mQuitText.setString("QUIT");
    mQuitText.setCharacterSize(32);
    mQuitText.setOrigin(mQuitText.getLocalBounds().size / 2.f);
    mQuitText.setPosition({ cx, static_cast<float>(winSize.y) * 0.65f });

    mSelectedIndex = 0;
    UpdateSelection();
}

void MenuScene::OnExit()
{
}

void MenuScene::HandleEvent(const sf::Event& event)
{
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>())
    {
        switch (keyEvent->code)
        {
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
                mSceneManager.ChangeScene(SCENE_TYPE::GAME);
            else
                mSceneManager.Quit();
            break;

        default: break;
        }
    }
}

void MenuScene::Update(float deltaTime)
{
    mBlinkTimer += deltaTime;
    if (mBlinkTimer >= 0.5f)
    {
        mBlinkTimer = 0.f;
        mShowCursor = !mShowCursor;
        UpdateSelection();
    }
}

void MenuScene::Render()
{
    mWindow.clear(sf::Color::Black);
    mWindow.draw(mTitle);
    mWindow.draw(mStartText);
    mWindow.draw(mQuitText);
    mWindow.display();
}

void MenuScene::UpdateSelection()
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

    highlight(mStartText, mSelectedIndex == 0);
    highlight(mQuitText, mSelectedIndex == 1);
}
